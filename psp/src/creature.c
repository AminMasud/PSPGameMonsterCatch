#include <limits.h>
#include <string.h>
#include "creature.h"
#define L(l,m) {l,m}
static const Species species[SPECIES_COUNT] = {
 {SPECIES_CINDLET,"CINDLET","A SMALL HEARTH DWELLER.\nITS EARS GLOW LIKE WARM COALS.",ELEMENT_EMBER,
  24,10,10,8,24,8,SPECIES_EMBERLYN,0,0,
  {L(1,MOVE_NUDGE),L(2,MOVE_CINDER),L(4,MOVE_LUNGE),L(5,MOVE_GUST),L(6,MOVE_HEAT),L(9,MOVE_FLARE)},6},
 {SPECIES_EMBERLYN,"EMBERLYN","ITS COAL MANE KEEPS TRAVELERS\nWARM THROUGH LONG WINTER NIGHTS.",ELEMENT_EMBER,
  35,18,16,14,45,0,-1,1,1,
  {L(1,MOVE_NUDGE),L(2,MOVE_CINDER),L(4,MOVE_LUNGE),L(5,MOVE_GUST),L(6,MOVE_HEAT),L(8,MOVE_PEBBLE),L(9,MOVE_FLARE)},7},
 {SPECIES_MOSSLET,"MOSSLET","IT GATHERS DEW IN A SOFT MOSS COAT.\nIT SLEEPS BENEATH FALLEN LEAVES.",ELEMENT_GROVE,
  25,8,12,6,22,8,SPECIES_MOSSHORN,2,2,
  {L(1,MOVE_NUDGE),L(2,MOVE_LEAF),L(4,MOVE_LUNGE),L(6,MOVE_VINE)},4},
 {SPECIES_MOSSHORN,"MOSSHORN","BRANCHING HORNS SHELTER SEEDLINGS.\nIT WALKS SOFTLY THROUGH THE WOODS.",ELEMENT_GROVE,
  39,15,22,8,44,0,-1,3,3,
  {L(1,MOVE_NUDGE),L(2,MOVE_LEAF),L(4,MOVE_LUNGE),L(6,MOVE_VINE),L(9,MOVE_PEBBLE)},5},
 {SPECIES_TWIGLINT,"TWIGLINT","THIS NIMBLE BRANCH DWELLER HIDES\nIN PATCHES OF SHIFTING SUNLIGHT.",ELEMENT_GROVE,
  20,12,8,14,27,0,-1,4,4,
  {L(1,MOVE_NUDGE),L(2,MOVE_LEAF),L(4,MOVE_GUST),L(7,MOVE_VINE)},4},
 {SPECIES_GLOWMOTH,"GLOWMOTH","ITS DUSTY WINGS SHIMMER WHEN\nNIGHT BREEZES CROSS THE CANOPY.",ELEMENT_WIND,
  22,11,9,16,34,0,-1,5,5,
  {L(1,MOVE_NUDGE),L(2,MOVE_GUST),L(4,MOVE_LUNGE),L(7,MOVE_TEMPEST)},4},
 {SPECIES_FLINTLING,"FLINTLING","IT TAPS STONES TO LEAVE SPARKING\nTRAILS THROUGH THE DARK.",ELEMENT_STONE,
  27,12,15,5,28,9,SPECIES_FLINTAUR,6,6,
  {L(1,MOVE_NUDGE),L(2,MOVE_PEBBLE),L(4,MOVE_LUNGE),L(7,MOVE_FAULT)},4},
 {SPECIES_FLINTAUR,"FLINTAUR","A HEAVY CAVE WANDERER WITH\nRIDGES OF POLISHED FLINT.",ELEMENT_STONE,
  43,22,25,7,48,0,-1,7,7,
  {L(1,MOVE_NUDGE),L(2,MOVE_PEBBLE),L(4,MOVE_LUNGE),L(7,MOVE_FAULT),L(9,MOVE_CINDER)},5},
 {SPECIES_DUSKWISP,"DUSKWISP","A PALE DRIFTER THAT FOLLOWS\nTHE QUIET AIR INSIDE CAVERNS.",ELEMENT_WIND,
  20,14,8,17,31,0,-1,8,8,
  {L(1,MOVE_NUDGE),L(2,MOVE_GUST),L(4,MOVE_CINDER),L(7,MOVE_TEMPEST)},4},
 {SPECIES_ECHOCRAG,"ECHOCRAG","ITS HOLLOW CREST CARRIES SOUND\nACROSS UNDERGROUND CHAMBERS.",ELEMENT_STONE,
  33,14,18,4,38,0,-1,9,9,
  {L(1,MOVE_NUDGE),L(2,MOVE_PEBBLE),L(4,MOVE_LUNGE),L(6,MOVE_FAULT)},4}
};
const Species *species_get(int id)
{
    return &species[id>=0 && id<SPECIES_COUNT?id:SPECIES_CINDLET];
}
const char *creature_name(const Creature *c) { return c->nickname[0]?c->nickname:c->name; }
int creature_xp_for_level(int level)
{
    if(level<1) level=1;
    if(level>CREATURE_MAX_LEVEL) level=CREATURE_MAX_LEVEL;
    return 20*(level-1)*(level-1);
}
int creature_xp_remaining(const Creature *c)
{
    return c->level>=CREATURE_MAX_LEVEL?0:creature_xp_for_level(c->level+1)-c->experience;
}
static void recalculate(Creature *c)
{
    const Species *s=species_get(c->species);
    int old_max=c->max_hp,old_hp=c->hp;
    c->name=s->name; c->element=s->element;
    c->max_hp=s->base_hp+c->level*5;
    c->attack=s->base_attack+c->level*3;
    c->defense=s->base_defense+c->level*2;
    c->speed=s->base_speed+c->level*2;
    c->hp=old_hp>0?old_hp+c->max_hp-old_max:0;
    if(c->hp>c->max_hp) c->hp=c->max_hp;
}
void creature_restore(Creature *c)
{
    c->hp=c->max_hp;
    for(int i=0;i<4;++i) c->uses[i]=c->moves[i]<0?0:attack_get(c->moves[i])->uses;
}
static int knows(const Creature *c,int move)
{
    for(int i=0;i<4;++i) if(c->moves[i]==move) return 1;
    return 0;
}
int creature_learn(Creature *c,int move,int slot)
{
    if(move<0 || move>=MOVE_COUNT || slot<0 || slot>=4 || knows(c,move)) return 0;
    c->moves[slot]=move; c->uses[slot]=attack_get(move)->uses;
    return 1;
}
void creature_create(Creature *c,int id,int level)
{
    *c=(Creature){0};
    c->species=species_get(id)->id;
    c->level=level<1?1:level>100?100:level;
    c->experience=creature_xp_for_level(c->level);
    for(int i=0;i<4;++i) c->moves[i]=-1;
    recalculate(c);
    const Species *s=species_get(c->species);
    int slot=0;
    for(int i=0;i<s->learn_count;++i)
        if(s->learnset[i].level<=c->level && !knows(c,s->learnset[i].move)) {
            /* Fresh wild creatures use their latest four unlocked attacks. */
            if(slot==4) {
                for(int j=0;j<3;++j) c->moves[j]=c->moves[j+1];
                slot=3;
            }
            c->moves[slot++]=s->learnset[i].move;
        }
    creature_restore(c);
}
static void queue_moves(const Creature *c,const Species *s,int exact,CreatureGrowth *g)
{
    for(int i=0;i<s->learn_count;++i) {
        LearnMove learned=s->learnset[i];
        if((exact?learned.level!=c->level:learned.level>c->level) || knows(c,learned.move)) continue;
        int duplicate=0;
        for(int j=0;j<g->move_count;++j) if(g->moves[j]==learned.move) duplicate=1;
        if(!duplicate && g->move_count<MOVE_COUNT) g->moves[g->move_count++]=learned.move;
    }
}
void creature_gain_xp(Creature *c,int amount,CreatureGrowth *g)
{
    *g=(CreatureGrowth){0};
    g->old_level=c->level;g->old_species=c->species;
    if(amount<=0 || c->level>=100) return;
    int cap=creature_xp_for_level(100);
    c->experience=amount>=cap-c->experience?cap:c->experience+amount;
    while(c->level<100 && c->experience>=creature_xp_for_level(c->level+1)) {
        ++c->level;
        const Species *s=species_get(c->species);
        queue_moves(c,s,1,g);
        if(s->evolution_level>0 && c->level>=s->evolution_level) {
            c->species=(SpeciesId)s->evolved_species;
            queue_moves(c,species_get(c->species),0,g);
        }
        recalculate(c);
    }
}
