#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include "battle.h"

static void confirm(Battle *b) { battle_update(b,&(Input){0,0,1,0,0}); }
static void victory(Battle *b,Creature *c)
{
    battle_begin(b,c,SPECIES_MOSSLET,3,42);
    b->enemy.hp=1;b->ally.speed=999;
    confirm(b);confirm(b);confirm(b); /* Intro, fight, basic attack. */
    assert(b->result==BATTLE_WIN);
    confirm(b); /* Award XP exactly once. */
}
static void close_growth(Battle *b)
{
    for(int i=0;i<30 && b->phase!=BATTLE_DONE;++i) {
        if(b->phase==BATTLE_LEARN) battle_update(b,&(Input){0,0,0,1,0});
        else confirm(b);
    }
    assert(b->phase==BATTLE_DONE);
}
int main(void)
{
    assert(SPECIES_COUNT==10);
    for(int id=0;id<SPECIES_COUNT;++id) {
        const Species *s=species_get(id);
        assert((int)s->id==id && s->name[0] && s->description[0]);
        assert(s->base_hp>0 && s->base_attack>0 && s->base_defense>0 && s->base_speed>0);
        assert(s->experience_yield>0 && s->learn_count>0 && s->learn_count<=8);
        assert(s->overworld_sprite==id && s->battle_sprite==id);
        if(s->evolution_level) assert(s->evolved_species>=0 && s->evolved_species<SPECIES_COUNT && s->evolved_species!=id);
        for(int i=0;i<s->learn_count;++i) {
            assert(s->learnset[i].move>=0 && s->learnset[i].move<MOVE_COUNT);
            assert(s->learnset[i].level>=1 && s->learnset[i].level<=100);
        }
        for(int level=1;level<=100;++level) {
            Creature c;creature_create(&c,id,level);
            assert(c.level==level && c.hp==c.max_hp && c.experience==creature_xp_for_level(level));
            for(int i=0;i<4;++i) {
                assert(c.moves[i]>=-1 && c.moves[i]<MOVE_COUNT);
                assert(c.uses[i]==(c.moves[i]<0?0:attack_get(c.moves[i])->uses));
                for(int j=0;j<i;++j) assert(c.moves[i]<0 || c.moves[i]!=c.moves[j]);
            }
        }
    }
    Creature c;CreatureGrowth g;
    creature_create(&c,SPECIES_CINDLET,5);
    int hp=c.hp;c.hp-=7;
    creature_gain_xp(&c,179,&g);assert(c.level==5 && g.move_count==0);
    creature_gain_xp(&c,1,&g);assert(c.level==6 && c.hp==hp-7+5);
    assert(g.move_count==1 && g.moves[0]==MOVE_HEAT);
    assert(c.moves[1]==MOVE_CINDER); /* Learning requires a choice. */
    assert(creature_learn(&c,MOVE_HEAT,1));
    assert(c.moves[1]==MOVE_HEAT && c.uses[1]==10);
    assert(!creature_learn(&c,MOVE_HEAT,2));
    assert(!creature_learn(&c,MOVE_COUNT,0));
    assert(!creature_learn(&c,MOVE_LEAF,4));

    strcpy(c.nickname,"SPARK");
    creature_gain_xp(&c,creature_xp_for_level(8)-c.experience,&g);
    assert(c.species==SPECIES_EMBERLYN && c.level==8 && c.attack==42);
    assert(!strcmp(creature_name(&c),"SPARK"));
    assert(g.old_species==SPECIES_CINDLET && c.moves[1]==MOVE_HEAT);
    creature_gain_xp(&c,0,&g);assert(g.move_count==0 && g.old_species==SPECIES_EMBERLYN);
    creature_gain_xp(&c,-5,&g);assert(c.experience==creature_xp_for_level(8));
    creature_gain_xp(&c,INT_MAX,&g);
    assert(c.level==100 && c.experience==creature_xp_for_level(100) && !creature_xp_remaining(&c));
    creature_gain_xp(&c,INT_MAX,&g);assert(g.move_count==0);
    creature_create(&c,SPECIES_MOSSLET,7);c.hp=0;
    creature_gain_xp(&c,creature_xp_for_level(8)-c.experience,&g);
    assert(c.species==SPECIES_MOSSHORN && c.hp==0); /* Stats cannot revive a fainted creature. */
    creature_create(&c,SPECIES_FLINTLING,1);
    creature_gain_xp(&c,creature_xp_for_level(12),&g);
    assert(c.species==SPECIES_FLINTAUR && c.level==12 && g.move_count<=MOVE_COUNT);

    Battle b;
    creature_create(&c,SPECIES_CINDLET,5);c.experience=creature_xp_for_level(6)-1;
    victory(&b,&c);int earned=b.ally.experience;
    assert(b.ally.level==6 && b.reward_given);
    for(int i=0;i<8 && b.phase!=BATTLE_LEARN;++i) confirm(&b);
    assert(b.phase==BATTLE_LEARN && b.learn_cursor==4);
    b.learn_cursor=1;confirm(&b);
    assert(b.ally.moves[1]==MOVE_HEAT);
    close_growth(&b);confirm(&b);assert(b.ally.experience==earned);
    victory(&b,&c);
    for(int i=0;i<8 && b.phase!=BATTLE_LEARN;++i) confirm(&b);
    battle_update(&b,&(Input){0,0,0,1,0});close_growth(&b);
    assert(b.ally.moves[1]==MOVE_CINDER);

    creature_create(&c,SPECIES_CINDLET,7);c.experience=creature_xp_for_level(8)-1;
    victory(&b,&c);assert(b.ally.species==SPECIES_EMBERLYN);
    close_growth(&b);assert(b.ally.species==SPECIES_EMBERLYN);
    creature_create(&c,SPECIES_CINDLET,5);
    battle_begin(&b,&c,SPECIES_MOSSLET,3,2);confirm(&b);
    b.cursor=4;b.escape_attempts=2;confirm(&b);confirm(&b);
    assert(b.phase==BATTLE_DONE && b.ally.experience==c.experience);
    puts("PASS: 10 species, 100 levels, XP boundaries/cap, evolution, stats, nicknames, move choices, single rewards");
    return 0;
}
