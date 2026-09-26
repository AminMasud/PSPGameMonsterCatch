#include <limits.h>
#include <string.h>
#include "creature.h"
#define L(l,m) {l,m}
/* IDs match PNG number minus one. Each consecutive triple is a family. */
static const Species species[SPECIES_COUNT] = {
 {SPECIES_CINDLET,"CINDLET","ITS COAL-DARK FUR HOLDS A TINY FLAME. ITS WARM EARS PRICK UP AT FOOTSTEPS.",ELEMENT_EMBER,
  24,10,10,8,24,8,SPECIES_EMBERYN,0,0,
  {L(1,MOVE_NUDGE),L(2,MOVE_CINDER),L(4,MOVE_LUNGE),L(5,MOVE_GUST),L(6,MOVE_HEAT),L(9,MOVE_FLARE)},6},
 {SPECIES_EMBERYN,"EMBERYN","FLAMES FLOW ALONG ITS MANE. IT RUNS BESIDE TRAVELERS ON COLD NIGHTS.",ELEMENT_EMBER,
  35,18,16,14,48,16,SPECIES_PYROVERN,1,1,
  {L(1,MOVE_NUDGE),L(2,MOVE_CINDER),L(4,MOVE_LUNGE),L(5,MOVE_GUST),L(6,MOVE_HEAT),L(8,MOVE_PEBBLE),L(9,MOVE_FLARE)},7},
 {SPECIES_PYROVERN,"PYROVERN","A BLAZING GUARDIAN WITH A CROWN OF FIRE. ITS ROAR SENDS EMBERS SKYWARD.",ELEMENT_EMBER,
  50,30,26,22,72,0,-1,2,2,
  {L(1,MOVE_NUDGE),L(2,MOVE_CINDER),L(4,MOVE_LUNGE),L(5,MOVE_GUST),L(6,MOVE_HEAT),L(8,MOVE_PEBBLE),L(9,MOVE_FLARE),L(16,MOVE_FAULT)},8},
 {SPECIES_BUBFIN,"BUBFIN","IT BLOWS BUBBLES WHILE PLAYING IN THE SHALLOWS. ITS FINS GLINT LIKE GLASS.",ELEMENT_TIDE,
  26,9,11,10,23,8,SPECIES_RIVAFIN,3,3,
  {L(1,MOVE_NUDGE),L(2,MOVE_BUBBLE),L(4,MOVE_LUNGE),L(5,MOVE_LEAF),L(6,MOVE_SURGE),L(9,MOVE_GUST)},6},
 {SPECIES_RIVAFIN,"RIVAFIN","IT CUTS THROUGH RIVER CURRENTS WITH LONG FINS AND A SWEEPING TAIL.",ELEMENT_TIDE,
  37,17,17,16,47,16,SPECIES_TIDERION,4,4,
  {L(1,MOVE_NUDGE),L(2,MOVE_BUBBLE),L(4,MOVE_LUNGE),L(5,MOVE_LEAF),L(6,MOVE_SURGE),L(9,MOVE_GUST)},6},
 {SPECIES_TIDERION,"TIDERION","THE RIVER FALLS QUIET WHEN THIS MAJESTIC WATER GUARDIAN PASSES.",ELEMENT_TIDE,
  52,29,27,24,71,0,-1,5,5,
  {L(1,MOVE_NUDGE),L(2,MOVE_BUBBLE),L(4,MOVE_LUNGE),L(5,MOVE_LEAF),L(6,MOVE_SURGE),L(9,MOVE_GUST),L(16,MOVE_FAULT)},7},
 {SPECIES_MOSSPRIG,"MOSSPRIG","NEW LEAVES SPROUT FROM ITS SOFT COAT. IT FOLLOWS PATCHES OF SUNLIGHT.",ELEMENT_GROVE,
  25,8,12,6,22,8,SPECIES_THORNEL,6,6,
  {L(1,MOVE_NUDGE),L(2,MOVE_LEAF),L(4,MOVE_LUNGE),L(5,MOVE_GUST),L(6,MOVE_VINE),L(9,MOVE_PEBBLE)},6},
 {SPECIES_THORNEL,"THORNEL","ITS BRANCHING CREST CARRIES FRESH LEAVES THROUGH EVERY SEASON.",ELEMENT_GROVE,
  36,16,18,12,46,16,SPECIES_ELDERTHORN,7,7,
  {L(1,MOVE_NUDGE),L(2,MOVE_LEAF),L(4,MOVE_LUNGE),L(5,MOVE_GUST),L(6,MOVE_VINE),L(9,MOVE_PEBBLE)},6},
 {SPECIES_ELDERTHORN,"ELDERTHORN","ANCIENT ROOTS AND FLOWERS ADORN THIS PATIENT FOREST GUARDIAN.",ELEMENT_GROVE,
  51,28,28,20,70,0,-1,8,8,
  {L(1,MOVE_NUDGE),L(2,MOVE_LEAF),L(4,MOVE_LUNGE),L(5,MOVE_GUST),L(6,MOVE_VINE),L(9,MOVE_PEBBLE),L(16,MOVE_FAULT)},7},
 {SPECIES_ZAPPIP,"ZAPPIP","TINY SPARKS LEAP BETWEEN ITS FINS WHEN IT DARTS THROUGH THE REEDS.",ELEMENT_SPARK,
  22,12,8,16,27,8,SPECIES_AMPREEL,9,9,
  {L(1,MOVE_NUDGE),L(2,MOVE_SPARK),L(4,MOVE_LUNGE),L(5,MOVE_BUBBLE),L(6,MOVE_VOLT),L(9,MOVE_GUST)},6},
 {SPECIES_AMPREEL,"AMPREEL","ITS COILED BODY STORES ELECTRICITY. GOLDEN MARKINGS FLASH BEFORE A STRIKE.",ELEMENT_SPARK,
  33,20,14,22,51,16,SPECIES_VOLTRENCH,10,10,
  {L(1,MOVE_NUDGE),L(2,MOVE_SPARK),L(4,MOVE_LUNGE),L(5,MOVE_BUBBLE),L(6,MOVE_VOLT),L(9,MOVE_GUST)},6},
 {SPECIES_VOLTRENCH,"VOLTRENCH","A GOLDEN CURRENT RUNS ALONG ITS SPINES. IT CHURNS THE WATER INTO LIGHT.",ELEMENT_SPARK,
  48,32,24,30,75,0,-1,11,11,
  {L(1,MOVE_NUDGE),L(2,MOVE_SPARK),L(4,MOVE_LUNGE),L(5,MOVE_BUBBLE),L(6,MOVE_VOLT),L(9,MOVE_GUST),L(16,MOVE_FAULT)},7},
 {SPECIES_GRUBBL,"GRUBBL","IT CARRIES PEBBLES ON ITS SHELL AND BURROWS UNDER WARM STONES.",ELEMENT_STONE,
  27,12,15,5,28,8,SPECIES_CRAGBEET,12,12,
  {L(1,MOVE_NUDGE),L(2,MOVE_PEBBLE),L(4,MOVE_LUNGE),L(5,MOVE_LEAF),L(6,MOVE_FAULT),L(9,MOVE_CINDER)},6},
 {SPECIES_CRAGBEET,"CRAGBEET","ITS ARMORED LEGS GRIP THE CAVE WALLS. IT POLISHES ITS SHELL ON ROCK.",ELEMENT_STONE,
  38,20,21,11,52,16,SPECIES_TITANOCERA,13,13,
  {L(1,MOVE_NUDGE),L(2,MOVE_PEBBLE),L(4,MOVE_LUNGE),L(5,MOVE_LEAF),L(6,MOVE_FAULT),L(9,MOVE_CINDER)},6},
 {SPECIES_TITANOCERA,"TITANOCERA","HEAVY STONE PLATES PROTECT THIS POWERFUL HORNED CAVE DWELLER.",ELEMENT_STONE,
  53,32,31,19,76,0,-1,14,14,
  {L(1,MOVE_NUDGE),L(2,MOVE_PEBBLE),L(4,MOVE_LUNGE),L(5,MOVE_LEAF),L(6,MOVE_FAULT),L(9,MOVE_CINDER)},6},
 {SPECIES_VEILFIN,"VEILFIN","A QUIET RAY THAT GLIDES THROUGH SHADOWS. ITS PALE EDGES GLOW AT DUSK.",ELEMENT_VEIL,
  22,13,9,15,28,8,SPECIES_SPECTRAY,15,15,
  {L(1,MOVE_NUDGE),L(2,MOVE_VEIL),L(4,MOVE_LUNGE),L(5,MOVE_BUBBLE),L(6,MOVE_ABYSS),L(9,MOVE_GUST)},6},
 {SPECIES_SPECTRAY,"SPECTRAY","ITS LONG FINS LEAVE RIBBONS OF VIOLET LIGHT IN THE STILL CAVE AIR.",ELEMENT_VEIL,
  33,21,15,21,52,16,SPECIES_ABYSSVEIL,16,16,
  {L(1,MOVE_NUDGE),L(2,MOVE_VEIL),L(4,MOVE_LUNGE),L(5,MOVE_BUBBLE),L(6,MOVE_ABYSS),L(9,MOVE_GUST)},6},
 {SPECIES_ABYSSVEIL,"ABYSSVEIL","STARS SEEM TO SHIMMER INSIDE ITS WINGS AS IT DRIFTS THROUGH DARKNESS.",ELEMENT_VEIL,
  48,33,25,29,76,0,-1,17,17,
  {L(1,MOVE_NUDGE),L(2,MOVE_VEIL),L(4,MOVE_LUNGE),L(5,MOVE_BUBBLE),L(6,MOVE_ABYSS),L(9,MOVE_GUST),L(16,MOVE_FAULT)},7},
 {SPECIES_PEBCHICK,"PEBCHICK","ITS FLUFFY COAT CATCHES SNOWFLAKES. IT WADDLES ALONG COOL RIVERBANKS.",ELEMENT_FROST,
  28,10,13,7,25,8,SPECIES_FROSTUIN,18,18,
  {L(1,MOVE_NUDGE),L(2,MOVE_FROST),L(4,MOVE_LUNGE),L(5,MOVE_GUST),L(6,MOVE_GLACIER),L(9,MOVE_BUBBLE)},6},
 {SPECIES_FROSTUIN,"FROSTUIN","A CREST OF ICE FORMS WHEN IT CALLS. IT STANDS WATCH OVER CHILLED POOLS.",ELEMENT_FROST,
  39,18,19,13,49,16,SPECIES_GLACIMPER,19,19,
  {L(1,MOVE_NUDGE),L(2,MOVE_FROST),L(4,MOVE_LUNGE),L(5,MOVE_GUST),L(6,MOVE_GLACIER),L(9,MOVE_BUBBLE)},6},
 {SPECIES_GLACIMPER,"GLACIMPER","IT WEARS A CROWN OF FROST AND SPREADS ITS GREAT WINGS TO SUMMON SNOW.",ELEMENT_FROST,
  54,30,29,21,73,0,-1,20,20,
  {L(1,MOVE_NUDGE),L(2,MOVE_FROST),L(4,MOVE_LUNGE),L(5,MOVE_GUST),L(6,MOVE_GLACIER),L(9,MOVE_BUBBLE),L(16,MOVE_FAULT)},7},
 {SPECIES_GUSTLET,"GUSTLET","IT PRACTICES SHORT HOPS BEFORE RIDING GENTLE BREEZES ABOVE THE WOODS.",ELEMENT_WIND,
  23,12,9,16,25,8,SPECIES_GALETALON,21,21,
  {L(1,MOVE_NUDGE),L(2,MOVE_GUST),L(4,MOVE_LUNGE),L(5,MOVE_CINDER),L(6,MOVE_TEMPEST),L(9,MOVE_PEBBLE)},6},
 {SPECIES_GALETALON,"GALETALON","ITS SHARP TALONS HOLD FAST WHILE STRONG WINDS RUFFLE ITS FEATHERS.",ELEMENT_WIND,
  34,20,15,22,49,16,SPECIES_SKYRAPTOR,22,22,
  {L(1,MOVE_NUDGE),L(2,MOVE_GUST),L(4,MOVE_LUNGE),L(5,MOVE_CINDER),L(6,MOVE_TEMPEST),L(9,MOVE_PEBBLE)},6},
 {SPECIES_SKYRAPTOR,"SKYRAPTOR","A SWEEP OF ITS BROAD WINGS SENDS SPIRALING GUSTS THROUGH THE CANOPY.",ELEMENT_WIND,
  49,32,25,30,73,0,-1,23,23,
  {L(1,MOVE_NUDGE),L(2,MOVE_GUST),L(4,MOVE_LUNGE),L(5,MOVE_CINDER),L(6,MOVE_TEMPEST),L(9,MOVE_PEBBLE),L(16,MOVE_FAULT)},7},
 {SPECIES_TOXLET,"TOXLET","BRIGHT SPOTS WARN OTHERS TO KEEP THEIR DISTANCE. IT HIDES AMONG LEAVES.",ELEMENT_TOXIN,
  24,12,10,12,26,8,SPECIES_VENOFROG,24,24,
  {L(1,MOVE_NUDGE),L(2,MOVE_TOXIN),L(4,MOVE_LUNGE),L(5,MOVE_BUBBLE),L(6,MOVE_VENOM),L(9,MOVE_LEAF)},6},
 {SPECIES_VENOFROG,"VENOFROG","ITS COLORFUL SKIN SHIMMERS AS IT LEAPS BETWEEN DAMP FOREST ROOTS.",ELEMENT_TOXIN,
  35,20,16,18,50,16,SPECIES_DREADART,25,25,
  {L(1,MOVE_NUDGE),L(2,MOVE_TOXIN),L(4,MOVE_LUNGE),L(5,MOVE_BUBBLE),L(6,MOVE_VENOM),L(9,MOVE_LEAF)},6},
 {SPECIES_DREADART,"DREADART","A SWIFT AND VIVID HUNTER. IT STRIKES FROM COVER WITH PERFECT TIMING.",ELEMENT_TOXIN,
  50,32,26,26,74,0,-1,26,26,
  {L(1,MOVE_NUDGE),L(2,MOVE_TOXIN),L(4,MOVE_LUNGE),L(5,MOVE_BUBBLE),L(6,MOVE_VENOM),L(9,MOVE_LEAF),L(16,MOVE_FAULT)},7},
 {SPECIES_GLIMGRUB,"GLIMGRUB","ITS SOFT GREEN SEGMENTS GLOW FAINTLY AS IT MUNCHES TENDER LEAVES.",ELEMENT_GROVE,
  24,9,12,9,23,8,SPECIES_COCOGLOW,27,27,
  {L(1,MOVE_NUDGE),L(2,MOVE_LEAF),L(4,MOVE_LUNGE),L(5,MOVE_GUST),L(6,MOVE_VINE),L(9,MOVE_TEMPEST)},6},
 {SPECIES_COCOGLOW,"COCOGLOW","GOLDEN SILK PROTECTS ITS CHANGING BODY. A SOFT LIGHT SHINES WITHIN.",ELEMENT_GROVE,
  35,17,18,15,47,16,SPECIES_LUNARAE,28,28,
  {L(1,MOVE_NUDGE),L(2,MOVE_LEAF),L(4,MOVE_LUNGE),L(5,MOVE_GUST),L(6,MOVE_VINE),L(9,MOVE_TEMPEST)},6},
 {SPECIES_LUNARAE,"LUNARAE","MOONLIKE PATTERNS SHINE ON ITS WINGS. IT DRIFTS ABOVE THE SLEEPING WOODS.",ELEMENT_WIND,
  50,29,28,23,71,0,-1,29,29,
  {L(1,MOVE_NUDGE),L(2,MOVE_LEAF),L(4,MOVE_LUNGE),L(5,MOVE_GUST),L(6,MOVE_VINE),L(9,MOVE_TEMPEST),L(16,MOVE_FAULT)},7}
};
const Species *species_get(int id)
{
    return &species[id>=0 && id<SPECIES_COUNT?id:SPECIES_CINDLET];
}
EvolutionRequirement species_evolution_requirement(const Species *s)
{
    EvolutionRequirement requirement={EVOLUTION_NONE,(SpeciesId)-1,0};
    if(s && s->evolution_level>0 && s->evolved_species>=0) {
        requirement.method=EVOLUTION_LEVEL;requirement.target=(SpeciesId)s->evolved_species;
        requirement.level=s->evolution_level;
    }
    return requirement;
}
int species_can_evolve(const Species *s,int level)
{
    EvolutionRequirement requirement=species_evolution_requirement(s);
    return requirement.method==EVOLUTION_LEVEL && level>=requirement.level;
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
        while(species_can_evolve(s,c->level)) {
            c->species=species_evolution_requirement(s).target;
            s=species_get(c->species);
            queue_moves(c,s,0,g);
        }
        recalculate(c);
    }
}
