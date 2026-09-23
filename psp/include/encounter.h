#ifndef EMBERWAKE_ENCOUNTER_H
#define EMBERWAKE_ENCOUNTER_H
#include <stdint.h>
#include "creature.h"
typedef struct { uint32_t random; int safe_steps; } Encounter;
typedef struct { const char *name; int level; SpeciesId species; } EncounterResult;
void encounter_init(Encounter *e, uint32_t seed);
int encounter_step(Encounter *e, int area, EncounterResult *result);
#endif
