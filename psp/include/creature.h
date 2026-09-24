#ifndef EMBERWAKE_CREATURE_H
#define EMBERWAKE_CREATURE_H
#include "attacks.h"
#define CREATURE_MOVES 4
#define CREATURE_MAX_LEVEL 100
typedef enum {
    SPECIES_CINDLET, SPECIES_EMBERYN, SPECIES_PYROVERN,
    SPECIES_BUBFIN, SPECIES_RIVAFIN, SPECIES_TIDERION,
    SPECIES_MOSSPRIG, SPECIES_THORNEL, SPECIES_ELDERTHORN,
    SPECIES_ZAPPIP, SPECIES_AMPREEL, SPECIES_VOLTRENCH,
    SPECIES_GRUBBL, SPECIES_CRAGBEET, SPECIES_TITANOCERA,
    SPECIES_VEILFIN, SPECIES_SPECTRAY, SPECIES_ABYSSVEIL,
    SPECIES_PEBCHICK, SPECIES_FROSTUIN, SPECIES_GLACIMPER,
    SPECIES_GUSTLET, SPECIES_GALETALON, SPECIES_SKYRAPTOR,
    SPECIES_TOXLET, SPECIES_VENOFROG, SPECIES_DREADART,
    SPECIES_GLIMGRUB, SPECIES_COCOGLOW, SPECIES_LUNARAE, SPECIES_COUNT
} SpeciesId;
typedef struct { int level, move; } LearnMove;
typedef struct {
    SpeciesId id;
    const char *name, *description;
    Element element;
    int base_hp, base_attack, base_defense, base_speed, experience_yield;
    int evolution_level, evolved_species;
    int overworld_sprite, battle_sprite;
    LearnMove learnset[8];
    int learn_count;
} Species;
typedef struct {
    SpeciesId species;
    char nickname[20];
    const char *name; /* Derived cache of immutable species data, not save data. */
    Element element;
    int level, experience, max_hp, hp, attack, defense, speed;
    int moves[CREATURE_MOVES], uses[CREATURE_MOVES];
} Creature;
typedef struct {
    int old_level;
    SpeciesId old_species;
    int moves[MOVE_COUNT], move_count;
} CreatureGrowth;
const Species *species_get(int id);
const char *creature_name(const Creature *c);
void creature_create(Creature *c,int species,int level);
void creature_restore(Creature *c);
int creature_xp_for_level(int level);
int creature_xp_remaining(const Creature *c);
void creature_gain_xp(Creature *c,int amount,CreatureGrowth *growth);
int creature_learn(Creature *c,int move,int slot);
#endif
