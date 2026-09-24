#ifndef EMBERWAKE_SAVE_DATA_H
#define EMBERWAKE_SAVE_DATA_H

#include <stdint.h>
#include "creature.h"
#include "party.h"
#include "inventory.h"

#define SAVE_DATA_MAGIC 0x454D4252u
#define SAVE_DATA_VERSION 1u

typedef struct {
    int species;
    int level;
    int experience;
    int hp;
    char nickname[20];
    int moves[CREATURE_MOVES];
    int uses[CREATURE_MOVES];
} SaveCreature;

typedef struct {
    SaveCreature members[PARTY_MAX];
    SaveCreature collection[COLLECTION_MAX];
    int count;
    int lead;
    int stored;
} SaveParty;

typedef struct {
    uint32_t magic;
    uint32_t version;
    int map_id;
    int tile_x;
    int tile_y;
    int facing;
    uint32_t encounter_random;
    int encounter_safe_steps;
    SaveParty party;
    int item_quantities[ITEM_COUNT];
    int embermarks;
} SavePayload;

typedef enum {
    SAVE_STATUS_IDLE,
    SAVE_STATUS_BUSY,
    SAVE_STATUS_SUCCEEDED,
    SAVE_STATUS_FAILED
} SaveStatus;

int save_data_begin_save(const SavePayload *payload);
int save_data_begin_load(void);
void save_data_update(void);
SaveStatus save_data_status(void);
int save_data_take_loaded(SavePayload *payload);

#endif
