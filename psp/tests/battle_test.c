#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "battle.h"

static void press(Battle *b)
{
    battle_update(b,&(Input){0,0,1,0});
}
static void start(Battle *b,unsigned int seed)
{
    Battler ally; battler_starter(&ally);
    battle_begin(b,&ally,"MOSSLET",3,seed);
    press(b); assert(b->phase==BATTLE_MENU);
}
static void choose(Battle *b,int move)
{
    b->cursor=0; press(b); assert(b->phase==BATTLE_ATTACKS);
    b->move_cursor=move; press(b);
}
static void finish_messages(Battle *b)
{
    int limit=8;
    while(b->phase==BATTLE_MESSAGE && limit-->0) press(b);
    assert(limit>0);
}
int main(void)
{
    Battle b;
    start(&b,12);
    assert(b.ally.level==5 && b.ally.hp==b.ally.max_hp);
    int hp=b.enemy.hp;
    choose(&b,0);
    assert(b.ally.uses[0]==23 && b.enemy.hp<hp);
    assert(b.ally.hp==b.ally.max_hp); /* Enemy has not acted yet. */
    press(&b);
    assert(b.ally.hp<b.ally.max_hp);
    press(&b); assert(b.phase==BATTLE_MENU);

    start(&b,12);
    b.enemy.hp=1;
    choose(&b,0);
    assert(b.result==BATTLE_WIN && b.enemy.hp==0);
    finish_messages(&b);
    assert(b.phase==BATTLE_DONE && b.ally.hp==b.ally.max_hp);
    battle_update(&b,&(Input){0,0,1,0}); assert(b.phase==BATTLE_DONE);

    start(&b,12);
    b.ally.hp=1; b.enemy.speed=999;
    for(int i=0;i<4;++i) b.enemy.moves[i]=MOVE_NUDGE;
    choose(&b,0);
    assert(b.result==BATTLE_LOSS && b.ally.hp==0 && b.ally.uses[0]==24);
    finish_messages(&b); assert(b.phase==BATTLE_DONE);

    start(&b,12);
    b.cursor=0; press(&b);
    battle_update(&b,&(Input){0,0,0,1});
    assert(b.phase==BATTLE_MENU && b.enemy.hp==b.enemy.max_hp);
    /* Menu navigation advances once per new direction, not once per frame. */
    for(int i=0;i<20;++i) battle_update(&b,&(Input){0,1,0,0});
    assert(b.cursor==1);
    battle_update(&b,&(Input){0});
    battle_update(&b,&(Input){0,1,0,0}); assert(b.cursor==2);
    for(int i=1;i<=3;++i) {
        b.cursor=i; press(&b); assert(b.phase==BATTLE_MESSAGE);
        finish_messages(&b);
        assert(b.phase==BATTLE_MENU && b.ally.hp==b.ally.max_hp);
    }
    b.ally.uses[0]=0; choose(&b,0);
    assert(strstr(b.message,"NO USES") && b.enemy.hp==b.enemy.max_hp);
    finish_messages(&b);
    for(int i=0;i<4;++i) b.ally.uses[i]=0;
    choose(&b,1); assert(strstr(b.message,"PRESS ON"));
    finish_messages(&b);
    for(int i=0;i<4;++i) assert(b.ally.uses[i]==0);

    int saw_fail=0,saw_success=0,saw_miss=0;
    for(unsigned int seed=1;seed<100;++seed) {
        start(&b,seed); b.cursor=4; press(&b);
        if(b.result==BATTLE_ESCAPED) {
            saw_success=1;finish_messages(&b);assert(b.phase==BATTLE_DONE);
        } else {
            saw_fail=1;
            int uses=0;for(int i=0;i<4;++i) uses+=b.enemy.uses[i];
            finish_messages(&b);assert(b.phase==BATTLE_MENU);
            int after=0;for(int i=0;i<4;++i) after+=b.enemy.uses[i];
            assert(after==uses-1); /* Exactly one response to failed escape. */
            b.escape_attempts=2;b.cursor=4;press(&b);
            assert(b.result==BATTLE_ESCAPED);
        }
        start(&b,seed);choose(&b,2);
        if(strstr(b.message,"MISSED")) {
            saw_miss=1;assert(b.enemy.hp==b.enemy.max_hp && b.ally.uses[2]==7);
        }
    }
    assert(saw_fail && saw_success && saw_miss);
    start(&b,7);
    int weak=battle_damage(&b.ally,&b.enemy,attack_get(MOVE_NUDGE),90);
    int strong=battle_damage(&b.ally,&b.enemy,attack_get(MOVE_NUDGE),100);
    assert(weak>=1 && strong>=weak);
    assert(attack_effectiveness(ELEMENT_EMBER,ELEMENT_GROVE)==4);
    assert(attack_effectiveness(ELEMENT_GROVE,ELEMENT_EMBER)==1);
    assert(attack_effectiveness(ELEMENT_PLAIN,ELEMENT_STONE)==2);
    assert(attack_effectiveness(ELEMENT_EMBER,ELEMENT_EMBER)==1);
    b.enemy.defense=0;assert(battle_damage(&b.ally,&b.enemy,attack_get(MOVE_NUDGE),100)>0);
    puts("PASS: turns, speed, HP, victory/defeat, accuracy, uses, fallback, menu, escape, damage");
    return 0;
}
