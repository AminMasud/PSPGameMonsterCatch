#include <stdio.h>
#include <string.h>
#include "battle.h"
#include "capture.h"

static uint32_t random_next(Battle *b)
{
    uint32_t x=b->random;
    x^=x<<13; x^=x>>17; x^=x<<5;
    return b->random=x;
}
void battler_restore(Battler *b) { creature_restore(b); }
void battler_starter(Battler *b) { creature_create(b,SPECIES_CINDLET,5); }
void battle_begin(Battle *b,const Battler *ally,SpeciesId species,int level,uint32_t seed)
{
    Party party={0};party.count=1;party.members[0]=*ally;
    battle_begin_party_with_inventory(b,&party,0,species,level,seed);
}
void battle_begin_party(Battle *b,const Party *party,SpeciesId species,int level,uint32_t seed)
{
    battle_begin_party_with_inventory(b,party,0,species,level,seed);
}
void battle_begin_party_with_inventory(Battle *b,const Party *party,const Inventory *inventory,SpeciesId species,int level,uint32_t seed)
{
    *b=(Battle){0};
    b->party=*party;b->active=party->lead;
    if (inventory) b->inventory=*inventory; else inventory_init(&b->inventory);
    b->ally=party->members[b->active]; b->random=seed?seed:0x3291u;
    b->capture_charges=3;
    creature_create(&b->enemy,species,level);
    b->ally_hp_shown=(float)b->ally.hp;b->enemy_hp_shown=(float)b->enemy.hp;
    b->acting_side=-1;
    b->phase=BATTLE_MESSAGE; b->after=AFTER_BEGIN_TURN;b->turn_state=TURN_BEGIN;
    snprintf(b->message,sizeof(b->message),"A WILD %s APPEARS.\n%s IS READY.",creature_name(&b->enemy),creature_name(&b->ally));
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
static int valid_move(const Battler *unit,int slot)
{
    return slot>=0 && slot<BATTLE_MOVES && unit->moves[slot]>=0 &&
           unit->moves[slot]<MOVE_COUNT && unit->uses[slot]>0;
}
static int choose_enemy(Battle *b)
{
    int slots[4],count=0;
    for(int i=0;i<4;++i) if(valid_move(&b->enemy,i)) slots[count++]=i;
    return count?slots[random_next(b)%(unsigned int)count]:-1;
}
static void sync_active(Battle *b)
{
    b->party.members[b->active]=b->ally;
    b->party.lead=b->active;
}
static int reserve_available(const Battle *b)
{
    for(int i=0;i<b->party.count;++i)
        if(i!=b->active && b->party.members[i].hp>0) return 1;
    return 0;
}
static void check_action_result(Battle *b)
{
    b->turn_state=TURN_CHECK_FAINTED;
    sync_active(b);
    int enemy_fainted=b->enemy.hp<=0,ally_fainted=b->ally.hp<=0;
    b->forced_switch=ally_fainted && !enemy_fainted && reserve_available(b);
    b->turn_state=TURN_CHECK_RESULT;
    if(enemy_fainted) b->result=BATTLE_WIN;
    else if(ally_fainted && !b->forced_switch) b->result=BATTLE_LOSS;
    if(enemy_fainted || ally_fainted) {
        /* A knocked-out participant cannot finish a queued action. A reserve
           starts a fresh round after the forced replacement is acknowledged. */
        b->turn_index=2;b->turn_state=TURN_COMPLETE;
    }
}
static void complete_player_action(Battle *b)
{
    b->acting_side=0;
    b->turn_state=b->turn_index==0?TURN_RESOLVE_FIRST:TURN_RESOLVE_SECOND;
    ++b->turn_index;
    check_action_result(b);
}
static void resolve_attack(Battle *b)
{
    if(b->turn_index>=2 || b->result!=BATTLE_ONGOING) return;
    b->turn_state=b->turn_index==0?TURN_RESOLVE_FIRST:TURN_RESOLVE_SECOND;
    int side=b->order[b->turn_index++];
    b->acting_side=side;
    Battler *a=side==0?&b->ally:&b->enemy;
    Battler *d=side==0?&b->enemy:&b->ally;
    int slot=b->choices[side];
    if(!valid_move(a,slot)) slot=-1;
    /* PRESS ON is an unlimited weak fallback, only when every move is spent. */
    const Attack fallback={"PRESS ON",15,100,ELEMENT_PLAIN,0};
    const Attack *move=slot<0?&fallback:attack_get(a->moves[slot]);
    if(slot>=0) --a->uses[slot];
    int hit=(int)(random_next(b)%100)<move->accuracy;
    if(!hit) {
        snprintf(b->message,sizeof(b->message),"%s USED %s.\nTHE ATTACK MISSED.",creature_name(a),move->name);
    } else {
        int damage=battle_damage(a,d,move,90+(int)(random_next(b)%11));
        if(damage>d->hp) damage=d->hp;
        d->hp-=damage;
        b->hit_side=1-side;b->hit_time=0.24f;
        int effect=attack_effectiveness(move->element,d->element);
        snprintf(b->message,sizeof(b->message),"%s USED %s.\n%d DAMAGE. %s",creature_name(a),move->name,damage,
                 effect==4?"STRONG MATCH.":effect==1?"RESISTED.":"");
    }
    b->phase=BATTLE_MESSAGE; b->after=AFTER_TURN;
    check_action_result(b);
}
static void begin_turn(Battle *b);
static void advance_turn(Battle *b)
{
    if(b->turn_index>=2) {
        b->turn_state=TURN_COMPLETE;
        begin_turn(b);
    } else if(b->order[b->turn_index]==0) {
        b->acting_side=-1;b->turn_state=TURN_WAIT_PLAYER;b->phase=BATTLE_MENU;
    } else resolve_attack(b);
}
static float approach_hp(float shown,int hp,float step)
{
    if (shown<hp) return shown+step>hp?(float)hp:shown+step;
    return shown-step<hp?(float)hp:shown-step;
}
void battle_animate(Battle *b,float seconds,int motion)
{
    if (!motion) {
        b->animation=0;b->hit_time=0;
        b->ally_hp_shown=(float)b->ally.hp;b->enemy_hp_shown=(float)b->enemy.hp;
        return;
    }
    if (seconds<0) seconds=0;
    if (seconds>0.05f) seconds=0.05f;
    b->animation+=seconds;
    if (b->animation>=8) b->animation-=8;
    if (b->hit_time>0) b->hit_time-=seconds;
    b->ally_hp_shown=approach_hp(b->ally_hp_shown,b->ally.hp,b->ally.max_hp*seconds*3);
    b->enemy_hp_shown=approach_hp(b->enemy_hp_shown,b->enemy.hp,b->enemy.max_hp*seconds*3);
}
static void begin_turn(Battle *b)
{
    b->acting_side=-1;b->turn_state=TURN_BEGIN;++b->turn_number;
    b->turn_index=0;b->choices[0]=-1;
    b->turn_state=TURN_SELECT_ENEMY;
    b->choices[1]=choose_enemy(b);
    int enemy_first=b->enemy.speed>b->ally.speed;
    if(b->enemy.speed==b->ally.speed) enemy_first=(int)(random_next(b)%2);
    b->order[0]=enemy_first; b->order[1]=1-enemy_first;
    /* Faster enemies act before we open command selection. Order and their
       chosen move remain fixed while the player navigates/cancels menus. */
    advance_turn(b);
}
static int navigation(Battle *b,const Input *input)
{
    int direction=input->vertical?input->vertical:input->horizontal;
    int edge=direction && direction!=b->previous_direction;
    b->previous_direction=direction;
    return edge?direction:0;
}
static void growth_next(Battle *b)
{
    if(b->growth_stage==0) {
        b->growth_stage=1;
        if(b->growth.old_level!=b->ally.level) {
            snprintf(b->message,sizeof(b->message),"LEVEL UP. %d TO %d\nHP %d  ATTACK %d\nDEFENSE %d  SPEED %d",b->growth.old_level,b->ally.level,
                     b->ally.max_hp,b->ally.attack,b->ally.defense,b->ally.speed);
            b->phase=BATTLE_MESSAGE;b->after=AFTER_GROWTH;return;
        }
    }
    if(b->growth_stage==1) {
        b->growth_stage=2;
        if(b->growth.old_species!=b->ally.species) {
            snprintf(b->message,sizeof(b->message),"EVOLUTION.\n%s BECAME %s.\nA NEW FORM. A STRONGER PARTNER.",
                     species_get(b->growth.old_species)->name,b->ally.name);
            b->phase=BATTLE_MESSAGE;b->after=AFTER_GROWTH;return;
        }
    }
    while(b->growth_move<b->growth.move_count) {
        int move=b->growth.moves[b->growth_move];
        int empty=-1,known=0;
        for(int i=0;i<4;++i) {
            if(b->ally.moves[i]==move) known=1;
            if(b->ally.moves[i]<0 && empty<0) empty=i;
        }
        if(known) { ++b->growth_move;continue; }
        if(empty>=0) {
            creature_learn(&b->ally,move,empty);++b->growth_move;
            snprintf(b->message,sizeof(b->message),"%s LEARNED\n%s.",creature_name(&b->ally),attack_get(move)->name);
            b->phase=BATTLE_MESSAGE;b->after=AFTER_GROWTH;return;
        }
        b->learn_cursor=4; /* Default to KEEP CURRENT MOVES; no silent replacement. */
        b->phase=BATTLE_LEARN;return;
    }
    message(b,"TEAM RESTORED AFTER BATTLE.\nX RETURN TO EXPLORING",AFTER_DONE);
}
void battle_update(Battle *b,const Input *input)
{
    int nav=navigation(b,input);
    if(b->phase==BATTLE_DONE) return;
    if(b->phase==BATTLE_CAPTURE) {
        if(input->cancel) { b->acting_side=-1;b->phase=BATTLE_MENU;return; }
        if(!input->confirm) return;
        if(!party_has_space(&b->party)) {
            message(b,"PARTY AND COLLECTION ARE FULL.\nNO MORE SPACE FOR NEW VEYLINGS.",AFTER_MENU);return;
        }
        if(b->capture_charges<=0) {
            message(b,"THE RESONANCE LOOM IS EMPTY.\nIT RECHARGES AFTER THIS BATTLE.",AFTER_MENU);return;
        }
        --b->capture_charges;
        complete_player_action(b);
        if(capture_attempt(&b->enemy,1,random_next(b)%100)) {
            sync_active(b);
            int destination=party_add(&b->party,&b->enemy);
            b->result=BATTLE_CAUGHT;
            b->turn_state=TURN_COMPLETE;
            snprintf(b->message,sizeof(b->message),"%s JOINS YOU.\n%s\nTEAM RESTORED AFTER BATTLE.",creature_name(&b->enemy),
                     destination==1?"ADDED TO YOUR PARTY.":"SENT TO YOUR COLLECTION.");
            b->phase=BATTLE_MESSAGE;b->after=AFTER_DONE;
        } else {
            message(b,"THE RESONANCE THREAD FADES.\nTHE WILD VEYLING STAYS ALERT.",AFTER_TURN);
        }
        return;
    }
    if(b->phase==BATTLE_SWITCH) {
        if(input->cancel && !b->forced_switch) { b->acting_side=-1;b->phase=BATTLE_MENU;return; }
        if(nav) b->switch_cursor=(b->switch_cursor+nav+b->party.count)%b->party.count;
        if(!input->confirm) return;
        int selected=b->switch_cursor;
        if(selected==b->active || b->party.members[selected].hp<=0) {
            message(b,selected==b->active?"THAT VEYLING IS ALREADY ACTIVE.":"THAT VEYLING NEEDS A REST.",AFTER_SWITCH);return;
        }
        sync_active(b);b->active=selected;b->ally=b->party.members[selected];
        int forced=b->forced_switch;b->forced_switch=0;
        b->move_cursor=0;
        snprintf(b->message,sizeof(b->message),"%s TAKES THE FIELD.",creature_name(&b->ally));
        b->phase=BATTLE_MESSAGE;b->after=forced?AFTER_BEGIN_TURN:AFTER_TURN;
        if(!forced) complete_player_action(b);else b->acting_side=0;
        return;
    }
    if(b->phase==BATTLE_ITEMS) {
        if(input->cancel) { b->acting_side=-1;b->phase=BATTLE_MENU;return; }
        if(nav) b->item_cursor=(b->item_cursor+nav+ITEM_COUNT)%ITEM_COUNT;
        if(!input->confirm) return;
        int restored=inventory_use_healing(&b->inventory,(ItemId)b->item_cursor,&b->ally);
        if(!restored) {
            message(b,b->inventory.quantities[b->item_cursor]<=0 ? "THAT ITEM IS OUT OF STOCK." :
                    b->ally.hp>=b->ally.max_hp ? "YOUR ACTIVE VEYLING IS ALREADY AT FULL HP." :
                    "THAT ITEM CANNOT BE USED HERE.",AFTER_MENU);
            return;
        }
        complete_player_action(b);
        snprintf(b->message,sizeof(b->message),"%s USED %s.\n%d HP RESTORED.",creature_name(&b->ally),
                 inventory_item_name((ItemId)b->item_cursor),restored);
        b->phase=BATTLE_MESSAGE;b->after=AFTER_TURN;
        return;
    }
    if(b->phase==BATTLE_LEARN) {
        if(nav) b->learn_cursor=(b->learn_cursor+nav+5)%5;
        if(input->cancel || (input->confirm && b->learn_cursor==4)) {
            ++b->growth_move;growth_next(b);return;
        }
        if(input->confirm) {
            int move=b->growth.moves[b->growth_move++];
            const char *old=attack_get(b->ally.moves[b->learn_cursor])->name;
            creature_learn(&b->ally,move,b->learn_cursor);
            snprintf(b->message,sizeof(b->message),"%s LEARNED %s.\nREPLACED %s.",creature_name(&b->ally),attack_get(move)->name,old);
            b->phase=BATTLE_MESSAGE;b->after=AFTER_GROWTH;
        }
        return;
    }
    if(b->phase==BATTLE_MESSAGE) {
        if(!input->confirm) return; /* Results cannot be accidentally canceled. */
        if(b->after==AFTER_DONE) { b->acting_side=-1;sync_active(b);b->phase=BATTLE_DONE; return; }
        if(b->after==AFTER_BEGIN_TURN) { begin_turn(b);return; }
        if(b->after==AFTER_MENU) { b->acting_side=-1;b->phase=BATTLE_MENU; return; }
        if(b->after==AFTER_GROWTH) { b->acting_side=-1;growth_next(b);return; }
        if(b->after==AFTER_SWITCH || b->forced_switch) {
            b->acting_side=-1;sync_active(b);b->phase=BATTLE_SWITCH;b->switch_cursor=b->active;return;
        }
        if(b->result==BATTLE_WIN) {
            b->acting_side=-1;
            if(!b->reward_given) {
                int old_xp=b->ally.experience;
                b->experience_reward=b->ally.level>=100?0:species_get(b->enemy.species)->experience_yield*b->enemy.level;
                creature_gain_xp(&b->ally,b->experience_reward,&b->growth);
                b->experience_reward=b->ally.experience-old_xp;
                b->reward_given=1;
            }
            snprintf(b->message,sizeof(b->message),"VICTORY. %d XP EARNED.\n%s - LEVEL %d\n%d XP TO NEXT LEVEL",b->experience_reward,
                     creature_name(&b->ally),b->ally.level,creature_xp_remaining(&b->ally));
            if(b->ally.level==100)
                snprintf(b->message,sizeof(b->message),"VICTORY. %d XP EARNED.\n%s - LEVEL 100\nMAX LEVEL REACHED",b->experience_reward,creature_name(&b->ally));
            b->phase=BATTLE_MESSAGE;b->after=AFTER_GROWTH;
        } else if(b->result==BATTLE_LOSS) {
            b->acting_side=-1;
            message(b,"YOUR TEAM NEEDS A REST.\nRETURNING TO HEARTH CLEARING.\nTEAM RESTORED AFTER BATTLE.",AFTER_DONE);
        } else advance_turn(b);
        return;
    }
    if(b->phase==BATTLE_ATTACKS) {
        if(input->cancel) { b->acting_side=-1;b->phase=BATTLE_MENU; return; }
        if(nav) b->move_cursor=(b->move_cursor+nav+4)%4;
        if(!input->confirm) return;
        int available=0;
        for(int i=0;i<4;++i) available+=valid_move(&b->ally,i);
        if(!available) { b->choices[0]=-1;resolve_attack(b);return; }
        if(!valid_move(&b->ally,b->move_cursor)) {
            message(b,"THAT ATTACK HAS NO USES LEFT.\nCHOOSE ANOTHER ATTACK.",AFTER_MENU);
            return;
        }
        b->choices[0]=b->move_cursor;resolve_attack(b);
        return;
    }
    if(nav) b->cursor=(b->cursor+nav+5)%5;
    if(!input->confirm) return;
    switch(b->cursor) {
    case 0: b->phase=BATTLE_ATTACKS; break;
    case 1:
        b->phase=BATTLE_CAPTURE;break;
    case 2:
        sync_active(b);b->switch_cursor=b->active;b->phase=BATTLE_SWITCH;break;
    case 3:
        b->item_cursor=0;b->phase=BATTLE_ITEMS;break;
    default:
        ++b->escape_attempts;
        complete_player_action(b);
        if(b->escape_attempts>=3 || random_next(b)%100<70) {
            b->result=BATTLE_ESCAPED;
            b->turn_state=TURN_COMPLETE;
            message(b,"YOU GOT AWAY SAFELY.\nTEAM RESTORED AFTER BATTLE.",AFTER_DONE);
        } else {
            message(b,"THE WAY OUT IS BLOCKED.\nTHE WILD VEYLING MOVES CLOSER.",AFTER_TURN);
        }
        break;
    }
}
