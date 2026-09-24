#include <stdint.h>
#include "capture.h"

int capture_rarity(SpeciesId species)
{
    switch(species) {
    case SPECIES_MOSSLET:case SPECIES_FLINTLING:case SPECIES_REEDSKIP:return 0;
    case SPECIES_CINDLET:case SPECIES_TWIGLINT:case SPECIES_DUSKWISP:return 1;
    default:return 2;
    }
}

int capture_chance(const Creature *c,int strength)
{
    if(!c || c->species<0 || c->species>=SPECIES_COUNT ||
       c->max_hp<=0 || c->hp<=0 || c->hp>c->max_hp) return 0;
    if(strength<1) strength=1;
    if(strength>5) strength=5;
    /* A common creature starts at 35%; each rarity tier subtracts 12 points.
       Lost health adds up to 50 points, stronger looms add 10 per tier.
       Integer rounding is deterministic and 64-bit multiplication avoids overflow. */
    int wounded=(int)((int64_t)(c->max_hp-c->hp)*50/c->max_hp);
    int chance=35-12*capture_rarity(c->species)+wounded+10*(strength-1);
    return chance<5?5:chance>95?95:chance;
}

int capture_attempt(const Creature *c,int strength,unsigned int roll)
{
    return roll<100 && roll<(unsigned int)capture_chance(c,strength);
}
