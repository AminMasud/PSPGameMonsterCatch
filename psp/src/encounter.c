#include "encounter.h"
typedef struct { SpeciesId species; int weight, minimum, maximum; } Entry;
static const Entry forest[] = {{SPECIES_MOSSLET,60,2,4},{SPECIES_TWIGLINT,30,3,5},{SPECIES_GLOWMOTH,10,4,6}};
static const Entry cave[] = {{SPECIES_FLINTLING,60,3,5},{SPECIES_DUSKWISP,30,4,6},{SPECIES_ECHOCRAG,10,5,7}};
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
    if (area < 1 || area > 2 || next(e)%100 >= 18) return 0;
    const Entry *table = area == 1 ? forest : cave;
    int roll = (int)(next(e)%100);
    int index = 0;
    while (index < 2 && roll >= table[index].weight) {
        roll -= table[index].weight;
        ++index;
    }
    result->species = table[index].species;
    result->name = species_get(result->species)->name;
    result->level = table[index].minimum +
        (int)(next(e)%(table[index].maximum-table[index].minimum+1));
    e->safe_steps = 4;
    return 1;
}
