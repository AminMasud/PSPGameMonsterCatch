#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include "battle.h"

static void confirm(Battle *b) { battle_update(b,&(Input){0,0,1,0,0,0}); }
static void victory(Battle *b,Creature *c)
{
    battle_begin(b,c,SPECIES_MOSSPRIG,3,42);
    b->enemy.hp=1;b->ally.speed=999;
    confirm(b);confirm(b);confirm(b); /* Intro, fight, basic attack. */
    assert(b->result==BATTLE_WIN);
    confirm(b); /* Award XP exactly once. */
}
static void close_growth(Battle *b)
{
    for(int i=0;i<30 && b->phase!=BATTLE_DONE;++i) {
        if(b->phase==BATTLE_LEARN) battle_update(b,&(Input){0,0,0,1,0,0});
        else confirm(b);
    }
    assert(b->phase==BATTLE_DONE);
}
int main(void)
{
    const int evolution_levels[SPECIES_COUNT]={10,22,0,12,24,0,13,28,0,14,30,0,20,42,0,
                                                22,46,0,15,32,0,11,23,0,16,34,0,18,36,0};
    assert(SPECIES_COUNT==30);
    for(int id=0;id<SPECIES_COUNT;++id) {
        const Species *s=species_get(id);
        assert((int)s->id==id && s->name[0] && s->description[0]);
        assert(s->base_hp>0 && s->base_attack>0 && s->base_defense>0 && s->base_speed>0);
        assert(s->experience_yield>0 && s->learn_count>0 && s->learn_count<=8);
        assert(s->overworld_sprite==id && s->battle_sprite==id);
        assert(s->evolution_level==evolution_levels[id]);
        assert(s->evolved_species==(id%3==2?-1:id+1));
        EvolutionRequirement requirement=species_evolution_requirement(s);
        if(s->evolution_level) {
            assert(requirement.method==EVOLUTION_LEVEL && (int)requirement.target==s->evolved_species &&
                   requirement.level==s->evolution_level && species_can_evolve(s,requirement.level));
            assert(s->evolved_species>=0 && s->evolved_species<SPECIES_COUNT && s->evolved_species!=id);
        } else assert(requirement.method==EVOLUTION_NONE && !species_can_evolve(s,100));
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
    for(int family=0;family<10;++family) {
        Creature pet;CreatureGrowth growth;
        int middle=species_get(family*3)->evolution_level;
        int final=species_get(family*3+1)->evolution_level;
        creature_create(&pet,family*3,middle-1);
        strcpy(pet.nickname,"BUDDY");
        creature_gain_xp(&pet,creature_xp_for_level(middle)-pet.experience,&growth);
        assert((int)pet.species==family*3+1 && !strcmp(creature_name(&pet),"BUDDY"));
        creature_gain_xp(&pet,creature_xp_for_level(final)-pet.experience,&growth);
        assert((int)pet.species==family*3+2 && pet.level==final);
        creature_create(&pet,family*3,5);
        creature_gain_xp(&pet,creature_xp_for_level(final)-pet.experience,&growth);
        assert((int)pet.species==family*3+2 && pet.level==final);
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
    creature_gain_xp(&c,creature_xp_for_level(10)-c.experience,&g);
    assert(c.species==SPECIES_EMBERYN && c.level==10 && c.attack==48);
    assert(!strcmp(creature_name(&c),"SPARK"));
    assert(g.old_species==SPECIES_CINDLET && c.moves[1]==MOVE_HEAT);
    creature_gain_xp(&c,0,&g);assert(g.move_count==0 && g.old_species==SPECIES_EMBERYN);
    creature_gain_xp(&c,-5,&g);assert(c.experience==creature_xp_for_level(10));
    creature_gain_xp(&c,INT_MAX,&g);
    assert(c.level==100 && c.experience==creature_xp_for_level(100) && !creature_xp_remaining(&c));
    creature_gain_xp(&c,INT_MAX,&g);assert(g.move_count==0);
    creature_create(&c,SPECIES_MOSSPRIG,12);c.hp=0;
    creature_gain_xp(&c,creature_xp_for_level(13)-c.experience,&g);
    assert(c.species==SPECIES_THORNEL && c.hp==0); /* Stats cannot revive a fainted creature. */
    creature_create(&c,SPECIES_GRUBBL,1);
    creature_gain_xp(&c,creature_xp_for_level(20),&g);
    assert(c.species==SPECIES_CRAGBEET && c.level==20 && g.move_count<=MOVE_COUNT);

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
    battle_update(&b,&(Input){0,0,0,1,0,0});close_growth(&b);
    assert(b.ally.moves[1]==MOVE_CINDER);

    creature_create(&c,SPECIES_CINDLET,9);c.experience=creature_xp_for_level(10)-1;
    victory(&b,&c);assert(b.ally.species==SPECIES_EMBERYN);
    close_growth(&b);assert(b.ally.species==SPECIES_EMBERYN);
    creature_create(&c,SPECIES_CINDLET,5);
    battle_begin(&b,&c,SPECIES_MOSSPRIG,3,2);confirm(&b);
    b.cursor=4;b.escape_attempts=2;confirm(&b);confirm(&b);
    assert(b.phase==BATTLE_DONE && b.ally.experience==c.experience);
    puts("PASS: 30 forms, all 10 three-stage evolution lines, 100 levels, XP boundaries/cap, stats, nicknames, move choices, single rewards");
    return 0;
}
