#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "battle.h"

static void press(Battle *b) { battle_update(b,&(Input){.confirm=1}); }
static void cancel(Battle *b) { battle_update(b,&(Input){.cancel=1}); }
static void prepare(Battle *b,int enemy_first,unsigned int seed,int reserves)
{
    Party p={0};p.count=reserves+1;
    for(int i=0;i<p.count;++i) {
        creature_create(&p.members[i],SPECIES_CINDLET,5);
        p.members[i].hp=p.members[i].max_hp=1000;
        p.members[i].speed=enemy_first?10:30;
        strcpy(p.members[i].nickname,i?"RESERVE":"ALLY");
        for(int m=0;m<4;++m) { p.members[i].moves[m]=-1;p.members[i].uses[m]=0; }
        p.members[i].moves[0]=MOVE_NUDGE;p.members[i].uses[0]=100;
    }
    battle_begin_party(b,&p,SPECIES_MOSSPRIG,3,seed);
    strcpy(b->enemy.nickname,"ENEMY");
    b->enemy.hp=b->enemy.max_hp=1000;b->enemy.speed=20;
    for(int m=0;m<4;++m) { b->enemy.moves[m]=-1;b->enemy.uses[m]=0; }
    b->enemy.moves[0]=MOVE_NUDGE;b->enemy.uses[0]=100;
    assert(b->turn_number==0 && b->turn_state==TURN_BEGIN && b->acting_side==-1);
}
static void attack(Battle *b)
{
    assert(b->phase==BATTLE_MENU && b->turn_state==TURN_WAIT_PLAYER);
    b->cursor=0;press(b);assert(b->phase==BATTLE_ATTACKS);
    b->move_cursor=0;press(b);
}
static void player_menu(Battle *b,int enemy_first)
{
    if(enemy_first) {
        assert(b->phase==BATTLE_MESSAGE && strstr(b->message,"ENEMY USED"));
        assert(b->turn_index==1 && b->acting_side==1);
        press(b);
    }
    assert(b->phase==BATTLE_MENU && b->turn_state==TURN_WAIT_PLAYER && b->acting_side==-1);
}
static void end_result(Battle *b)
{
    int ally_uses=b->ally.uses[0],enemy_uses=b->enemy.uses[0];
    unsigned int turn=b->turn_number;
    assert(b->acting_side>=0);
    press(b);
    assert(b->phase==BATTLE_MESSAGE && b->acting_side==-1);
    for(int i=0;i<40 && b->phase!=BATTLE_DONE;++i) {
        if(b->phase==BATTLE_LEARN) cancel(b);else press(b);
    }
    assert(b->phase==BATTLE_DONE && b->turn_number==turn);
    for(int i=0;i<10;++i) press(b);
    assert(b->ally.uses[0]==ally_uses && b->enemy.uses[0]==enemy_uses);
}
static void repeated_turns(int enemy_first)
{
    Battle b;prepare(&b,enemy_first,123,0);
    press(&b);
    for(int turn=1;turn<=12;++turn) {
        assert(b.turn_number==(unsigned int)turn);
        assert(b.ally.uses[0]==101-turn);
        assert(b.enemy.uses[0]==101-turn-enemy_first);
        player_menu(&b,enemy_first);
        int hp=b.ally.hp,enemy_hp=b.enemy.hp;
        uint32_t random=b.random;
        /* Idle, cancel, and held navigation never consume a turn or reroll AI. */
        for(int i=0;i<30;++i) battle_update(&b,&(Input){.cancel=1});
        assert(b.ally.hp==hp && b.enemy.hp==enemy_hp && b.random==random);
        attack(&b);
        assert(strstr(b.message,"ALLY USED") && b.ally.uses[0]==100-turn && b.acting_side==0);
        if(!enemy_first) {
            assert(b.enemy.uses[0]==101-turn);
            press(&b);
            assert(strstr(b.message,"ENEMY USED") && b.acting_side==1);
        }
        assert(b.enemy.uses[0]==100-turn && b.turn_index==2);
        hp=b.ally.hp;enemy_hp=b.enemy.hp;
        for(int i=0;i<30;++i) battle_update(&b,&(Input){0});
        cancel(&b); /* Messages require confirmation; cancel cannot replay them. */
        assert(b.ally.hp==hp && b.enemy.hp==enemy_hp);
        press(&b);
    }
}
static void knockouts(void)
{
    /* Both targets on either action: no action is allowed after a knockout. */
    for(int first=0;first<=1;++first) for(int loser=0;loser<=1;++loser) {
        Battle b;prepare(&b,first,123,0);
        if(loser==0) b.ally.hp=1;else b.enemy.hp=1;
        press(&b);
        if(b.result==BATTLE_ONGOING) {
            player_menu(&b,first);attack(&b);
            if(b.result==BATTLE_ONGOING) press(&b);
        }
        assert(b.result==(loser==0?BATTLE_LOSS:BATTLE_WIN));
        assert(b.turn_number==1 && b.turn_state==TURN_COMPLETE);
        assert(b.acting_side==1-loser);
        int loser_acted=loser==first;
        assert((loser==0?b.ally.uses[0]:b.enemy.uses[0])==100-loser_acted);
        assert((loser==0?b.enemy.uses[0]:b.ally.uses[0])==99);
        end_result(&b);
    }
}
static void equal_speed(void)
{
    int seen[2]={0};
    for(unsigned int seed=1;seed<=100;++seed) {
        Battle b;prepare(&b,0,seed,0);b.ally.speed=b.enemy.speed;
        press(&b);int first=b.order[0];++seen[first];
        player_menu(&b,first);
        int order=b.order[0],choice=b.choices[1];uint32_t random=b.random;
        b.cursor=0;press(&b);cancel(&b);
        assert(b.order[0]==order && b.choices[1]==choice && b.random==random);
        attack(&b);
        if(!first) press(&b);
        assert(b.enemy.uses[0]==99 && b.ally.uses[0]==99 && b.turn_index==2);
    }
    assert(seen[0]>0 && seen[1]>0);
}
static void enemy_first_commands(void)
{
    /* Items, failed capture/escape, and switching are the player's one action.
       They must not append a second enemy attack to an enemy-first round. */
    for(int command=1;command<=4;++command) {
        Battle b;prepare(&b,1,123,1);
        press(&b);player_menu(&b,1);
        assert(b.enemy.uses[0]==99 && b.ally.uses[0]==100);
        b.cursor=command;
        if(command==4) b.random=4; /* Failed escape (76/100). */
        press(&b);
        if(command==1) { b.random=1;press(&b); } /* Failed capture (69/100). */
        if(command==2) { b.switch_cursor=1;press(&b); }
        if(command==3) press(&b);
        assert(b.result==BATTLE_ONGOING && b.phase==BATTLE_MESSAGE && b.acting_side==0);
        assert(b.enemy.uses[0]==99 && b.turn_index==2 && b.turn_number==1);
        b.ally.speed=30; /* Next round is player-first, isolating this round. */
        press(&b);
        assert(b.turn_number==2 && b.phase==BATTLE_MENU && b.enemy.uses[0]==99);
    }
    Battle b;prepare(&b,1,123,0);press(&b);player_menu(&b,1);
    uint32_t random=b.random;
    for(int command=0;command<4;++command) {
        b.cursor=command;press(&b);cancel(&b);
        assert(b.phase==BATTLE_MENU && b.turn_number==1 && b.turn_index==1);
        assert(b.random==random && b.enemy.uses[0]==99);
    }
    b.inventory.quantities[0]=0;b.cursor=3;press(&b);press(&b);press(&b);
    assert(b.phase==BATTLE_MENU && b.turn_number==1 && b.enemy.uses[0]==99);
    b.capture_charges=0;b.cursor=1;press(&b);press(&b);press(&b);
    assert(b.phase==BATTLE_MENU && b.turn_number==1 && b.random==random);
    b.ally.moves[1]=MOVE_CINDER;b.ally.uses[1]=10;b.ally.uses[0]=0;
    b.cursor=0;press(&b);press(&b);press(&b);
    assert(b.phase==BATTLE_MENU && b.turn_number==1 && b.enemy.uses[0]==99);
}
static void forced_replacement(void)
{
    for(int fast_reserve=0;fast_reserve<=1;++fast_reserve) {
        Battle b;prepare(&b,1,123,1);b.ally.hp=1;
        b.party.members[1].speed=fast_reserve?30:10;
        press(&b);
        assert(b.forced_switch && b.ally.uses[0]==100 && b.enemy.uses[0]==99 && b.acting_side==1);
        press(&b);assert(b.phase==BATTLE_SWITCH && b.acting_side==-1);
        cancel(&b);assert(b.phase==BATTLE_SWITCH && b.turn_number==1);
        b.switch_cursor=1;press(&b);
        assert(b.enemy.uses[0]==99 && b.ally.hp==1000 && b.turn_number==1 && b.acting_side==0);
        press(&b);
        assert(b.turn_number==2 && b.active==1 && !b.forced_switch);
        player_menu(&b,!fast_reserve);
        assert(b.enemy.uses[0]==(fast_reserve?99:98));
        assert(b.party.members[0].hp==0 && b.party.members[0].uses[0]==100);
        assert(b.ally.uses[0]==100); /* No queued attack transfers to the reserve. */
    }
}
static void fallback_and_locked_order(void)
{
    Battle b;prepare(&b,1,123,0);
    b.enemy.moves[0]=-1; /* Invalid entries with stray uses cannot be selected. */
    press(&b);assert(strstr(b.message,"PRESS ON") && b.enemy.uses[0]==100 && b.acting_side==1);
    player_menu(&b,1);
    for(int i=0;i<4;++i) b.ally.uses[i]=0;
    attack(&b);assert(strstr(b.message,"PRESS ON") && b.acting_side==0);
    prepare(&b,0,123,0);press(&b);assert(b.order[0]==0);
    b.enemy.speed=999;attack(&b);
    assert(strstr(b.message,"ALLY USED") && b.enemy.uses[0]==100);
    press(&b);assert(b.enemy.uses[0]==99);
    press(&b);assert(b.turn_number==2 && b.order[0]==1 && b.enemy.uses[0]==98);
}
int main(void)
{
    repeated_turns(0);repeated_turns(1);equal_speed();knockouts();
    enemy_first_commands();forced_replacement();fallback_and_locked_order();
    puts("PASS: Phase 10 turn order/actions and Phase 11 spotlight actor ownership/menu clearing");
    return 0;
}
