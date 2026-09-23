#include <stdio.h>
#include <string.h>
#include "battle.h"

static uint32_t random_next(Battle *b)
{
    uint32_t x=b->random;
    x^=x<<13; x^=x>>17; x^=x<<5;
    return b->random=x;
}
void battler_restore(Battler *b)
{
    b->hp=b->max_hp;
    for(int i=0;i<BATTLE_MOVES;++i) b->uses[i]=attack_get(b->moves[i])->uses;
}
static void create(Battler *b,const char *name,Element type,int level)
{
    *b=(Battler){0};
    b->name=name; b->element=type;
    b->level=level<1?1:level>100?100:level;
    b->max_hp=24+b->level*5;
    b->attack=10+b->level*3; b->defense=10+b->level*2;
    b->speed=8+b->level*2;
    b->moves[0]=MOVE_NUDGE;
    b->moves[1]=type==ELEMENT_EMBER?MOVE_CINDER:type==ELEMENT_GROVE?MOVE_LEAF:
                type==ELEMENT_STONE?MOVE_PEBBLE:MOVE_GUST;
    b->moves[2]=MOVE_LUNGE; b->moves[3]=MOVE_NUDGE;
    battler_restore(b);
}
void battler_starter(Battler *b)
{
    create(b,"CINDLET",ELEMENT_EMBER,5);
    b->moves[3]=MOVE_GUST; battler_restore(b);
}
void battle_begin(Battle *b,const Battler *ally,const char *name,int level,uint32_t seed)
{
    *b=(Battle){0};
    b->ally=*ally; b->random=seed?seed:0x3291u;
    Element type=ELEMENT_GROVE;
    if(!strcmp(name,"FLINTLING") || !strcmp(name,"ECHOCRAG")) type=ELEMENT_STONE;
    if(!strcmp(name,"GLOWMOTH") || !strcmp(name,"DUSKWISP")) type=ELEMENT_WIND;
    create(&b->enemy,name,type,level);
    /* Three distinct wild moves, with a fourth reserve basic attack slot. */
    b->phase=BATTLE_MESSAGE; b->after=AFTER_MENU;
    snprintf(b->message,sizeof(b->message),"A WILD %s APPEARS.\nCINDLET IS READY.\nX CONTINUE",name);
}
int battle_damage(const Battler *a,const Battler *d,const Attack *move,int variation)
{
    int defense=d->defense>0?d->defense:1;
    if(variation<90) variation=90;
    if(variation>100) variation=100;
    int base=((2*a->level/5+2)*move->power*a->attack/defense)/20+2;
    int damage=base*attack_effectiveness(move->element,d->element)*variation/200;
    return damage<1?1:damage;
}
static void message(Battle *b,const char *text,BattleAfter after)
{
    snprintf(b->message,sizeof(b->message),"%s",text);
    b->phase=BATTLE_MESSAGE; b->after=after;
}
static int choose_enemy(Battle *b)
{
    int slots[4],count=0;
    for(int i=0;i<4;++i) if(b->enemy.uses[i]>0) slots[count++]=i;
    return count?slots[random_next(b)%(unsigned int)count]:-1;
}
static void next_action(Battle *b)
{
    if(b->turn_index>=2) { b->phase=BATTLE_MENU; return; }
    int side=b->order[b->turn_index++];
    Battler *a=side==0?&b->ally:&b->enemy;
    Battler *d=side==0?&b->enemy:&b->ally;
    int slot=b->choices[side];
    /* PRESS ON is an unlimited weak fallback, only when every move is spent. */
    const Attack fallback={"PRESS ON",15,100,ELEMENT_PLAIN,0};
    const Attack *move=slot<0?&fallback:attack_get(a->moves[slot]);
    if(slot>=0) --a->uses[slot];
    int hit=(int)(random_next(b)%100)<move->accuracy;
    if(!hit) {
        snprintf(b->message,sizeof(b->message),"%s USED %s.\nTHE ATTACK MISSED.",a->name,move->name);
    } else {
        int damage=battle_damage(a,d,move,90+(int)(random_next(b)%11));
        if(damage>d->hp) damage=d->hp;
        d->hp-=damage;
        int effect=attack_effectiveness(move->element,d->element);
        snprintf(b->message,sizeof(b->message),"%s USED %s.\n%d DAMAGE. %s",a->name,move->name,damage,
                 effect==4?"STRONG MATCH.":effect==1?"RESISTED.":"");
    }
    b->phase=BATTLE_MESSAGE; b->after=AFTER_TURN;
    if(d->hp==0) {
        b->result=side==0?BATTLE_WIN:BATTLE_LOSS;
        /* Damage is shown first; the next confirmation shows the outcome. */
    }
}
static void begin_turn(Battle *b,int slot)
{
    b->choices[0]=slot; b->choices[1]=choose_enemy(b);
    int enemy_first=b->enemy.speed>b->ally.speed;
    if(b->enemy.speed==b->ally.speed) enemy_first=(int)(random_next(b)%2);
    b->order[0]=enemy_first; b->order[1]=1-enemy_first; b->turn_index=0;
    next_action(b);
}
static int navigation(Battle *b,const Input *input)
{
    int direction=input->vertical?input->vertical:input->horizontal;
    int edge=direction && direction!=b->previous_direction;
    b->previous_direction=direction;
    return edge?direction:0;
}
void battle_update(Battle *b,const Input *input)
{
    int nav=navigation(b,input);
    if(b->phase==BATTLE_DONE) return;
    if(b->phase==BATTLE_MESSAGE) {
        if(!input->confirm) return; /* Results cannot be accidentally canceled. */
        if(b->after==AFTER_DONE) { b->phase=BATTLE_DONE; return; }
        if(b->after==AFTER_MENU) { b->phase=BATTLE_MENU; return; }
        if(b->result==BATTLE_WIN) {
            message(b,"VICTORY.\nTHE WILD VEYLING RETREATS.\nPARTNER RESTORED AFTER BATTLE.",AFTER_DONE);
        } else if(b->result==BATTLE_LOSS) {
            message(b,"CINDLET NEEDS A REST.\nRETURNING TO HEARTH CLEARING.\nPARTNER RESTORED AFTER BATTLE.",AFTER_DONE);
        } else next_action(b);
        return;
    }
    if(b->phase==BATTLE_ATTACKS) {
        if(input->cancel) { b->phase=BATTLE_MENU; return; }
        if(nav) b->move_cursor=(b->move_cursor+nav+4)%4;
        if(!input->confirm) return;
        int available=0;
        for(int i=0;i<4;++i) available+=b->ally.uses[i];
        if(!available) { begin_turn(b,-1); return; }
        if(b->ally.uses[b->move_cursor]<=0) {
            message(b,"THAT ATTACK HAS NO USES LEFT.\nCHOOSE ANOTHER ATTACK.",AFTER_MENU);
            return;
        }
        begin_turn(b,b->move_cursor);
        return;
    }
    if(nav) b->cursor=(b->cursor+nav+5)%5;
    if(!input->confirm) return;
    switch(b->cursor) {
    case 0: b->phase=BATTLE_ATTACKS; break;
    case 1:
        message(b,"CAPTURE IS NOT AVAILABLE YET.\nIT ARRIVES IN PHASE 6.",AFTER_MENU); break;
    case 2:
        message(b,"CINDLET IS YOUR ONLY PARTNER.\nTEAM SWITCHING ARRIVES IN PHASE 6.",AFTER_MENU); break;
    case 3:
        message(b,"YOUR ITEM BAG IS EMPTY.\nITEMS ARRIVE IN PHASE 7.",AFTER_MENU); break;
    default:
        ++b->escape_attempts;
        if(b->escape_attempts>=3 || random_next(b)%100<70) {
            b->result=BATTLE_ESCAPED;
            message(b,"YOU GOT AWAY SAFELY.\nPARTNER RESTORED AFTER BATTLE.",AFTER_DONE);
        } else {
            b->choices[1]=choose_enemy(b);
            /* Only the enemy acts after a failed escape. */
            b->order[1]=1; b->turn_index=1;
            message(b,"THE WAY OUT IS BLOCKED.\nTHE WILD VEYLING MOVES CLOSER.",AFTER_TURN);
        }
        break;
    }
}
