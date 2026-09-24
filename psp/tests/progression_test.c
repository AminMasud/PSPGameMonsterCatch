#include <assert.h>
#include <stdio.h>
#include "progression.h"

int main(void)
{
    ProgressionState state;
    progression_init(&state);
    assert(progression_save_bits(&state)==0);
    assert(!progression_has(&state,PROGRESSION_FIRST_CHALLENGER_DEFEATED));

    ProgressionFlag flags[]={
        PROGRESSION_FIRST_CHALLENGER_DEFEATED,
        PROGRESSION_EAST_FOREST_BOSS_DEFEATED,
        PROGRESSION_NORTH_FOREST_BOSS_DEFEATED,
        PROGRESSION_CAVE_UNLOCKED
    };
    for(int i=0;i<PROGRESSION_FLAG_COUNT;++i) {
        assert(flags[i]==i && progression_flag_valid(flags[i]));
        assert(progression_set(&state,flags[i]));
        assert(progression_has(&state,flags[i]));
        assert(!progression_set(&state,flags[i]));
    }
    assert(!progression_flag_valid(PROGRESSION_NONE));
    assert(!progression_flag_valid(PROGRESSION_FLAG_COUNT));
    assert(!progression_set(&state,PROGRESSION_NONE));

    assert(progression_clear(&state,PROGRESSION_CAVE_UNLOCKED));
    assert(!progression_has(&state,PROGRESSION_CAVE_UNLOCKED));
    assert(!progression_clear(&state,PROGRESSION_CAVE_UNLOCKED));

    uint32_t saved=progression_save_bits(&state)|(1u<<31);
    ProgressionState restored;progression_load_bits(&restored,saved);
    assert(progression_save_bits(&restored)==saved);
    assert(progression_has(&restored,PROGRESSION_FIRST_CHALLENGER_DEFEATED));
    assert(progression_has(&restored,PROGRESSION_EAST_FOREST_BOSS_DEFEATED));
    assert(progression_has(&restored,PROGRESSION_NORTH_FOREST_BOSS_DEFEATED));
    assert(!progression_has(&restored,PROGRESSION_CAVE_UNLOCKED));

    assert(!progression_has(0,PROGRESSION_FIRST_CHALLENGER_DEFEATED));
    assert(!progression_set(0,PROGRESSION_FIRST_CHALLENGER_DEFEATED));
    assert(!progression_clear(0,PROGRESSION_FIRST_CHALLENGER_DEFEATED));
    assert(progression_save_bits(0)==0);
    progression_load_bits(0,saved);

    puts("PASS: generic named progression flags, queries, updates, and raw save roundtrip");
    return 0;
}
