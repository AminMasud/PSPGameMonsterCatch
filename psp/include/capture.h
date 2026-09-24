#ifndef EMBERWAKE_CAPTURE_H
#define EMBERWAKE_CAPTURE_H
#include "creature.h"

/* 0 common, 1 uncommon, 2 rare. Unknown species are conservatively treated as rare. */
int capture_rarity(SpeciesId species);
/* Strength is 1 (standard Resonance Loom) through 5, clamped to this range.
   Returns percent 5..95, or zero for an invalid/fainted creature. */
int capture_chance(const Creature *creature,int strength);
/* Caller supplies a uniformly distributed roll 0..99; invalid rolls always fail. */
int capture_attempt(const Creature *creature,int strength,unsigned int roll);
#endif
