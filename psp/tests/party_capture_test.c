#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include "party.h"
#include "capture.h"

static void same_creature(const Creature *a,const Creature *b)
{
    assert(a->species==b->species && !strcmp(a->nickname,b->nickname));
    assert(a->name==b->name && a->element==b->element);
    assert(a->level==b->level && a->experience==b->experience);
    assert(a->max_hp==b->max_hp && a->hp==b->hp && a->attack==b->attack);
    assert(a->defense==b->defense && a->speed==b->speed);
    for(int i=0;i<CREATURE_MOVES;++i) {
        assert(a->moves[i]==b->moves[i] && a->uses[i]==b->uses[i]);
    }
}

static Creature distinctive(int id)
{
    Creature c;
    creature_create(&c,id%SPECIES_COUNT,5+id%6);
    snprintf(c.nickname,sizeof(c.nickname),"FRIEND %d",id);
    c.experience+=id+1;
    c.hp-=1+id%5;
    c.uses[0]=id%3;
    return c;
}

static void party_checks(void)
{
    Party p;party_init(&p);
    assert(p.count==1 && p.stored==0 && p.lead==0 && party_has_space(&p));
    assert(p.members[0].species==SPECIES_CINDLET && p.members[0].level==5);
    Creature original=p.members[0],c;
    assert(!party_deposit(&p,0) && !party_set_lead(&p,1));
    assert(!party_exchange(&p,0,0) && !party_add(&p,NULL));
    for(int i=1;i<PARTY_MAX+COLLECTION_MAX;++i) {
        c=distinctive(i);
        assert(party_add(&p,&c)==(i<PARTY_MAX?1:2));
        same_creature(&c,i<PARTY_MAX?&p.members[i]:&p.collection[i-PARTY_MAX]);
    }
    assert(p.count==PARTY_MAX && p.stored==COLLECTION_MAX && !party_has_space(&p));
    same_creature(&p.members[0],&original);
    Party before=p;
    assert(!party_add(&p,&c) && !party_deposit(&p,0));
    assert(!party_exchange(&p,COLLECTION_MAX,0) && !party_exchange(&p,0,PARTY_MAX));
    assert(!party_exchange(&p,-1,0) && !party_exchange(&p,0,-1));
    assert(!party_set_lead(&p,-1) && !party_set_lead(&p,PARTY_MAX));
    for(int i=0;i<PARTY_MAX;++i) same_creature(&p.members[i],&before.members[i]);
    for(int i=0;i<COLLECTION_MAX;++i) same_creature(&p.collection[i],&before.collection[i]);
    assert(party_set_lead(&p,2));
    Creature outgoing=p.members[2],incoming=p.collection[COLLECTION_MAX-1];
    assert(party_exchange(&p,COLLECTION_MAX-1,2) && p.lead==2);
    same_creature(&p.members[2],&incoming);
    same_creature(&p.collection[COLLECTION_MAX-1],&outgoing);
    assert(p.count==PARTY_MAX && p.stored==COLLECTION_MAX);

    party_init(&p);
    for(int i=1;i<4;++i) { c=distinctive(i);assert(party_add(&p,&c)==1); }
    assert(party_set_lead(&p,3));
    Creature leader=p.members[3],deposited=p.members[1];
    assert(party_deposit(&p,1) && p.count==3 && p.stored==1 && p.lead==2);
    same_creature(&p.members[p.lead],&leader);
    same_creature(&p.collection[0],&deposited);
    assert(party_exchange(&p,0,p.count) && p.count==4 && p.stored==0 && p.lead==2);
    same_creature(&p.members[3],&deposited);
    assert(party_deposit(&p,p.lead) && p.lead==0);
    same_creature(&p.collection[0],&leader);
    assert(party_deposit(&p,1));
    Creature second=p.collection[1];
    assert(party_exchange(&p,0,p.count) && p.count==3 && p.stored==1);
    same_creature(&p.members[2],&leader);
    same_creature(&p.collection[0],&second); /* Withdrawal compacts collection. */

    p.members[1].hp=0;
    assert(!party_set_lead(&p,1) && p.lead==0);
    p.collection[0].hp=0;
    assert(party_exchange(&p,0,0));
    assert(p.lead==2 && p.members[0].hp==0); /* Select a healthy remaining member. */
    p.members[2].hp=0;
    assert(party_deposit(&p,2) && p.count==2 && p.lead>=0 && p.lead<p.count);
    before=p;
    party_restore(&p);
    for(int i=0;i<p.count;++i) {
        creature_restore(&before.members[i]);
        same_creature(&p.members[i],&before.members[i]);
    }
    for(int i=0;i<p.stored;++i) {
        creature_restore(&before.collection[i]);
        same_creature(&p.collection[i],&before.collection[i]);
    }
    assert(party_deposit(&p,0) && p.count==1 && !party_deposit(&p,0));
    assert(!party_has_space(NULL) && !party_add(NULL,&c));
    assert(!party_set_lead(NULL,0) && !party_exchange(NULL,0,0) && !party_deposit(NULL,0));
    party_init(NULL);party_restore(NULL);
}

static void capture_checks(void)
{
    assert(capture_rarity(SPECIES_MOSSPRIG)==0 && capture_rarity(SPECIES_GRUBBL)==0);
    assert(capture_rarity(SPECIES_CINDLET)==0 && capture_rarity(SPECIES_GALETALON)==1);
    assert(capture_rarity(SPECIES_SPECTRAY)==1);
    const int rare[]={SPECIES_PYROVERN,SPECIES_ELDERTHORN,SPECIES_TITANOCERA,SPECIES_LUNARAE,SPECIES_SKYRAPTOR};
    for(unsigned int i=0;i<sizeof(rare)/sizeof(rare[0]);++i) assert(capture_rarity((SpeciesId)rare[i])==2);
    Creature c;creature_create(&c,SPECIES_MOSSPRIG,5);
    assert(capture_chance(&c,1)==35);
    c.species=SPECIES_GALETALON;assert(capture_chance(&c,1)==23);
    c.species=SPECIES_TITANOCERA;assert(capture_chance(&c,1)==11);
    for(int id=0;id<SPECIES_COUNT;++id) {
        creature_create(&c,id,10);
        int last=0;
        for(c.hp=c.max_hp;c.hp>0;--c.hp) {
            int chance=capture_chance(&c,1);
            assert(chance>=last && chance>=5 && chance<=95);last=chance;
            for(int strength=1;strength<=5;++strength) {
                int probability=capture_chance(&c,strength),successes=0;
                assert(probability>=chance && probability<=95);chance=probability;
                for(unsigned int roll=0;roll<100;++roll) successes+=capture_attempt(&c,strength,roll);
                assert(successes==probability);
                assert(capture_attempt(&c,strength,(unsigned int)probability-1));
                assert(!capture_attempt(&c,strength,(unsigned int)probability));
            }
        }
        assert(capture_chance(&c,5)==0 && !capture_attempt(&c,5,0));
    }
    creature_create(&c,SPECIES_MOSSPRIG,5);c.hp=1;
    assert(capture_chance(&c,INT_MAX)==95);
    assert(capture_chance(&c,INT_MIN)==capture_chance(&c,1));
    assert(!capture_attempt(&c,5,100) && !capture_attempt(&c,5,UINT_MAX));
    c.max_hp=INT_MAX;
    assert(capture_chance(&c,1)==84); /* Health arithmetic cannot overflow. */
    c.hp=c.max_hp;assert(capture_chance(&c,1)==35);
    c.max_hp=0;assert(!capture_chance(&c,1));
    c.max_hp=50;c.hp=51;assert(!capture_chance(&c,1));
    c.hp=-1;assert(!capture_chance(&c,1));
    c.hp=25;c.species=SPECIES_COUNT;assert(!capture_chance(&c,1));
    c.species=(SpeciesId)-1;assert(!capture_chance(&c,1));
    assert(!capture_chance(NULL,1) && !capture_attempt(NULL,1,0));
}

int main(void)
{
    party_checks();capture_checks();
    puts("PASS: party/storage limits, lossless transfers, lead selection, restoration, capture rarity/health/strength odds and roll boundaries");
    return 0;
}
