#include "encounter.h"
/* Choose a family, then its stage: base 90%, middle 9%, final 1%.
   Every supplied form is obtainable in the wild as well as by evolution. */
static const SpeciesId forest[]={SPECIES_MOSSPRIG,SPECIES_GUSTLET,SPECIES_TOXLET,SPECIES_GLIMGRUB};
static const SpeciesId cave[]={SPECIES_CINDLET,SPECIES_GRUBBL,SPECIES_VEILFIN};
static const SpeciesId marsh[]={SPECIES_BUBFIN,SPECIES_ZAPPIP,SPECIES_PEBCHICK};
static uint32_t next(Encounter *e)
{
    uint32_t x = e->random;
    x ^= x << 13; x ^= x >> 17; x ^= x << 5;
    return e->random = x;
}
void encounter_init(Encounter *e, uint32_t seed)
{
    e->random = seed ? seed : 0x712a9u;
    e->safe_steps = 4;
}
int encounter_step(Encounter *e, int area, EncounterResult *result)
{
    if (e->safe_steps > 0) { --e->safe_steps; return 0; }
    if (area < 1 || area > 3 || next(e)%100 >= 18) return 0;
    const SpeciesId *table = area == 1 ? forest : area == 2 ? cave : marsh;
    int count=area==1?4:3;
    int family=(int)(next(e)%(unsigned int)count);
    int roll = (int)(next(e)%100);
    int stage=roll<90?0:roll<99?1:2;
    result->species = (SpeciesId)(table[family]+stage);
    result->name = species_get(result->species)->name;
    int minimum=stage==2?16:stage==1?8:area==1?2:area==2?3:5;
    result->level = minimum+(int)(next(e)%3);
    e->safe_steps = 4;
    return 1;
}
