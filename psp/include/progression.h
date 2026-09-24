#ifndef EMBERWAKE_PROGRESSION_H
#define EMBERWAKE_PROGRESSION_H

#include <stdint.h>

#define PROGRESSION_FLAG_CAPACITY 32

/* Values are persisted bit positions. Append new flags; never reorder them. */
typedef enum {
    PROGRESSION_NONE = -1,
    PROGRESSION_FIRST_CHALLENGER_DEFEATED,
    PROGRESSION_EAST_FOREST_BOSS_DEFEATED,
    PROGRESSION_NORTH_FOREST_BOSS_DEFEATED,
    PROGRESSION_CAVE_UNLOCKED,
    PROGRESSION_FLAG_COUNT
} ProgressionFlag;

typedef struct {
    uint32_t bits;
} ProgressionState;

void progression_init(ProgressionState *state);
int progression_flag_valid(ProgressionFlag flag);
int progression_has(const ProgressionState *state,ProgressionFlag flag);
int progression_set(ProgressionState *state,ProgressionFlag flag);
int progression_clear(ProgressionState *state,ProgressionFlag flag);
uint32_t progression_save_bits(const ProgressionState *state);
void progression_load_bits(ProgressionState *state,uint32_t bits);

#endif
