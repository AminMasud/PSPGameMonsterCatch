#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "battle.h"

static void confirm(Battle *b) { battle_update(b,&(Input){.confirm=1}); }
static void cancel(Battle *b) { battle_update(b,&(Input){.cancel=1}); }

static Party make_party(int count)
{
    Party p={0};
    p.count=count;
    for(int i=0;i<count;++i) {
        creature_create(&p.members[i],SPECIES_CINDLET,5);
        snprintf(p.members[i].nickname,sizeof(p.members[i].nickname),"PARTNER %d",i+1);
    }
    return p;
}

static void same_creature(const Creature *a,const Creature *b)
{
    assert(a->species==b->species && !strcmp(a->nickname,b->nickname));
    assert(a->element==b->element && a->level==b->level);
    assert(a->experience==b->experience && a->hp==b->hp && a->max_hp==b->max_hp);
    assert(a->attack==b->attack && a->defense==b->defense && a->speed==b->speed);
    for(int i=0;i<CREATURE_MOVES;++i) {
        assert(a->moves[i]==b->moves[i]);
        assert(a->uses[i]==b->uses[i]);
    }
}

static int total_uses(const Creature *c)
{
    int total=0;
    for(int i=0;i<CREATURE_MOVES;++i) total+=c->uses[i];
    return total;
}

static void prepare(Battle *b,const Party *p,unsigned int seed)
{
    battle_begin_party(b,p,SPECIES_MOSSPRIG,3,seed);
    assert(b->phase==BATTLE_MESSAGE && b->capture_charges==3);
    assert(b->active==p->lead);
    same_creature(&b->ally,&p->members[p->lead]);
    /* Use a 100%-accurate move for checks that count enemy responses. */
    for(int i=0;i<CREATURE_MOVES;++i) {
        b->enemy.moves[i]=MOVE_NUDGE;
        b->enemy.uses[i]=24;
    }
}

static void start(Battle *b,const Party *p,unsigned int seed)
{
    prepare(b,p,seed);
    confirm(b);
    assert(b->phase==BATTLE_MENU);
}

static void messages(Battle *b)
{
    int count=0;
    while(b->phase==BATTLE_MESSAGE && count++<30) confirm(b);
    assert(count<30);
}

static void open_capture(Battle *b)
{
    assert(b->phase==BATTLE_MENU);
    b->cursor=1;confirm(b);
    assert(b->phase==BATTLE_CAPTURE);
}

static void open_switch(Battle *b)
{
    assert(b->phase==BATTLE_MENU);
    b->cursor=2;confirm(b);
    assert(b->phase==BATTLE_SWITCH);
}

static void attack(Battle *b)
{
    assert(b->phase==BATTLE_MENU);
    b->cursor=0;confirm(b);
    assert(b->phase==BATTLE_ATTACKS);
    b->move_cursor=0;confirm(b);
}

static void item_use(void)
{
    Party p=make_party(1);
    Battle b;
    start(&b,&p,19);
    b.ally.hp-=10;
    b.cursor=3;confirm(&b);
    assert(b.phase==BATTLE_ITEMS && b.inventory.quantities[ITEM_PULSE_TONIC]==3);
    confirm(&b);
    /* The tonic restores the missing 10 HP and cannot exceed maximum HP. */
    assert(b.phase==BATTLE_MESSAGE && b.ally.hp==b.ally.max_hp &&
           b.inventory.quantities[ITEM_PULSE_TONIC]==2 && total_uses(&b.enemy)==96);
    messages(&b);
    assert(b.phase==BATTLE_MENU && total_uses(&b.enemy)==95);
}

static void capture_cases(unsigned int *success_seed,unsigned int *failure_seed)
{
    Party p=make_party(2);
    Battle b;
    int healthy=0,weakened=0;
    *success_seed=0;*failure_seed=0;
    /* The identical seed sequence is used at full and one HP. This checks the
       battle actually passes current HP into capture, rather than only testing
       a capture probability helper in isolation. */
    for(unsigned int seed=1;seed<=400;++seed) {
        int caught[2]={0};
        for(int weak=0;weak<=1;++weak) {
            start(&b,&p,seed);
            if(weak) b.enemy.hp=1;
            open_capture(&b);confirm(&b);
            assert(b.capture_charges==2);
            caught[weak]=b.result==BATTLE_CAUGHT;
            if(caught[weak]) {
                if(!weak && !*success_seed) *success_seed=seed;
                assert(b.party.count==3 && b.party.stored==0);
                assert(b.party.members[2].species==SPECIES_MOSSPRIG);
                assert(b.party.members[2].level==3);
                assert(!b.reward_given && b.experience_reward==0);
                messages(&b);
                assert(b.phase==BATTLE_DONE);
                assert(b.ally.experience==p.members[0].experience);
                for(int i=0;i<p.count;++i)
                    same_creature(&b.party.members[i],&p.members[i]);
            } else {
                if(!weak && !*failure_seed) *failure_seed=seed;
                assert(b.result==BATTLE_ONGOING);
                assert(total_uses(&b.enemy)==96); /* Reply awaits dismissal. */
                messages(&b);
                assert(b.phase==BATTLE_MENU && total_uses(&b.enemy)==95);
                assert(b.party.count==p.count && b.party.stored==0);
                assert(!b.reward_given && b.ally.experience==p.members[0].experience);
            }
        }
        assert(!caught[0] || caught[1]); /* Weakening cannot reduce the chance. */
        healthy+=caught[0];weakened+=caught[1];
    }
    assert(healthy>0 && healthy<400 && weakened>healthy);
    assert(*success_seed && *failure_seed);

    p=make_party(4);
    start(&b,&p,*success_seed);
    b.ally.hp-=7;b.ally.uses[0]-=2;
    Creature final_active=b.ally;
    open_capture(&b);confirm(&b);
    assert(b.result==BATTLE_CAUGHT && b.party.count==4 && b.party.stored==1);
    assert(b.party.collection[0].species==SPECIES_MOSSPRIG);
    assert(!b.reward_given && b.experience_reward==0);
    messages(&b);assert(b.phase==BATTLE_DONE);
    same_creature(&b.party.members[0],&final_active);
    for(int i=1;i<p.count;++i) same_creature(&b.party.members[i],&p.members[i]);

    /* A full collection blocks the action before spending a charge or RNG. */
    p.stored=(int)(sizeof(p.collection)/sizeof(p.collection[0]));
    for(int i=0;i<p.stored;++i) creature_create(&p.collection[i],SPECIES_GRUBBL,3);
    start(&b,&p,*success_seed);
    unsigned int random_before=b.random;
    open_capture(&b);confirm(&b);messages(&b);
    assert(b.phase==BATTLE_MENU && b.result==BATTLE_ONGOING);
    assert(b.capture_charges==3 && total_uses(&b.enemy)==96);
    assert(b.random==random_before && b.party.count==p.count && b.party.stored==p.stored);
    for(int i=0;i<p.stored;++i) same_creature(&b.party.collection[i],&p.collection[i]);
}

static void capture_charges_and_cancel(unsigned int failure_seed)
{
    Party p=make_party(1);
    Battle b;
    start(&b,&p,failure_seed);
    unsigned int random_before=b.random;
    open_capture(&b);cancel(&b);
    assert(b.phase==BATTLE_MENU && b.capture_charges==3);
    assert(b.random==random_before && total_uses(&b.enemy)==96);
    assert(b.ally.hp==p.members[0].hp);
    for(int i=0;i<3;++i) {
        /* Enemy move selection now consumes RNG at round start. Reset to a
           known failed capture roll instead of the pre-turn seed. */
        b.random=1;
        open_capture(&b);confirm(&b);
        assert(b.capture_charges==2-i && b.result==BATTLE_ONGOING);
        assert(total_uses(&b.enemy)==96-i);
        messages(&b);
        assert(b.phase==BATTLE_MENU && total_uses(&b.enemy)==95-i);
        assert(b.party.count==1 && b.party.stored==0);
    }
    random_before=b.random;
    int hp=b.ally.hp;
    open_capture(&b);confirm(&b);messages(&b);
    assert(b.phase==BATTLE_MENU && b.capture_charges==0);
    assert(b.random==random_before && b.ally.hp==hp);
    assert(total_uses(&b.enemy)==93 && b.result==BATTLE_ONGOING);
}

static void voluntary_switch(void)
{
    Party p=make_party(3);
    p.members[2].hp=0;
    Battle b;
    start(&b,&p,12);
    b.ally.hp-=11;b.ally.uses[0]=3;
    Creature outgoing=b.ally;
    open_switch(&b);cancel(&b);
    assert(b.phase==BATTLE_MENU && b.active==0 && total_uses(&b.enemy)==96);
    open_switch(&b);
    b.switch_cursor=0;confirm(&b);messages(&b);
    assert(b.phase==BATTLE_SWITCH && b.active==0 && total_uses(&b.enemy)==96 &&
           strstr(b.switch_message,"ACTIVE"));
    b.switch_cursor=2;confirm(&b);messages(&b);
    assert(b.phase==BATTLE_SWITCH && b.active==0 && total_uses(&b.enemy)==96 &&
           strstr(b.switch_message,"REST"));
    b.switch_cursor=1;confirm(&b);
    assert(b.phase==BATTLE_MESSAGE && b.active==1 && total_uses(&b.enemy)==95);
    assert(b.acting_side==1 && b.ally.hp<p.members[1].hp);
    same_creature(&b.party.members[0],&outgoing);
    assert(b.ally.species==p.members[1].species);
    confirm(&b);
    assert(b.phase==BATTLE_MENU && total_uses(&b.enemy)==95);
    for(int i=0;i<4;++i) battle_update(&b,&(Input){0});
    assert(total_uses(&b.enemy)==95);
    same_creature(&b.party.members[0],&outgoing);
    same_creature(&b.party.members[2],&p.members[2]);

    /* End-of-battle synchronization must keep the actual outgoing and active
       creatures instead of overwriting the original lead with its replacement. */
    Creature final_active=b.ally;
    b.cursor=4;b.escape_attempts=2;confirm(&b);messages(&b);
    assert(b.phase==BATTLE_DONE && b.result==BATTLE_ESCAPED && b.party.lead==1);
    same_creature(&b.party.members[0],&outgoing);
    same_creature(&b.party.members[1],&final_active);
    same_creature(&b.party.members[2],&p.members[2]);
}

static void forced_replacement_and_team_loss(void)
{
    Party p=make_party(3);
    p.members[0].hp=1;p.members[2].hp=0;
    Battle b;
    prepare(&b,&p,12);b.enemy.speed=999;
    int ally_uses=total_uses(&b.ally);
    confirm(&b);
    assert(b.ally.hp==0 && b.result==BATTLE_ONGOING && b.forced_switch);
    assert(total_uses(&b.ally)==ally_uses && total_uses(&b.enemy)==95);
    confirm(&b);
    assert(b.phase==BATTLE_SWITCH && b.party.members[0].hp==0);
    cancel(&b);
    assert(b.phase==BATTLE_SWITCH && b.forced_switch && total_uses(&b.enemy)==95);
    b.switch_cursor=0;confirm(&b);messages(&b);
    assert(b.phase==BATTLE_SWITCH && b.forced_switch);
    b.switch_cursor=2;confirm(&b);messages(&b);
    assert(b.phase==BATTLE_SWITCH && b.forced_switch && total_uses(&b.enemy)==95);
    /* Make the replacement faster so its fresh round opens at the command
       menu; the dedicated turn test also covers a slower replacement. */
    b.enemy.speed=0;
    b.switch_cursor=1;confirm(&b);messages(&b);
    assert(b.phase==BATTLE_MENU && b.active==1 && !b.forced_switch);
    assert(total_uses(&b.enemy)==95); /* Replacement is free after a knockout. */
    same_creature(&b.ally,&p.members[1]);
    assert(b.enemy.hp==b.enemy.max_hp); /* Knocked-out ally's turn was discarded. */
    b.ally.hp=1;
    attack(&b);
    assert(b.ally.hp==1 && b.result==BATTLE_ONGOING);
    confirm(&b);
    assert(b.ally.hp==0 && b.result==BATTLE_LOSS && !b.forced_switch);
    messages(&b);
    assert(b.phase==BATTLE_DONE && total_uses(&b.enemy)==94);
    for(int i=0;i<b.party.count;++i) assert(b.party.members[i].hp==0);
    assert(b.party.lead==1 && !b.reward_given);
}

static void victory_updates_only_active(void)
{
    Party p=make_party(3);
    p.members[1].experience=creature_xp_for_level(6)-1;
    p.members[2].hp-=9;p.members[2].uses[0]-=4;
    Battle b;
    start(&b,&p,42);
    b.ally.hp-=7;b.ally.uses[0]-=3;
    Creature outgoing=b.ally;
    open_switch(&b);b.switch_cursor=1;confirm(&b);messages(&b);
    assert(b.phase==BATTLE_MENU && b.active==1);
    b.enemy.hp=1;b.ally.speed=999;
    attack(&b);
    assert(b.result==BATTLE_WIN);
    confirm(&b);
    assert(b.reward_given && b.ally.level==6);
    int earned=b.ally.experience;
    assert(earned==p.members[1].experience+species_get(SPECIES_MOSSPRIG)->experience_yield*3);
    messages(&b);
    assert(b.phase==BATTLE_LEARN);
    b.learn_cursor=1;confirm(&b);messages(&b);
    assert(b.phase==BATTLE_DONE && b.party.lead==1);
    assert(b.ally.moves[1]==MOVE_HEAT && b.party.members[1].moves[1]==MOVE_HEAT);
    same_creature(&b.party.members[1],&b.ally);
    same_creature(&b.party.members[0],&outgoing);
    same_creature(&b.party.members[2],&p.members[2]);
    confirm(&b);confirm(&b);
    assert(b.ally.experience==earned && b.party.members[1].experience==earned);
    /* The caller owns the original team; a battle works on its own snapshot. */
    assert(p.members[0].hp==p.members[0].max_hp);
    assert(p.members[1].level==5 && p.members[1].moves[1]==MOVE_CINDER);

    /* Beginning with a changed lead must load that member, not slot zero. */
    p.lead=2;
    start(&b,&p,42);
    assert(b.active==2);
}

int main(void)
{
    unsigned int success_seed,failure_seed;
    capture_cases(&success_seed,&failure_seed);
    capture_charges_and_cancel(failure_seed);
    voluntary_switch();
    forced_replacement_and_team_loss();
    victory_updates_only_active();
    item_use();
    puts("PASS: capture/storage, automatic battle swaps, invalid/forced replacement, team loss, growth, items and party sync");
    return 0;
}
