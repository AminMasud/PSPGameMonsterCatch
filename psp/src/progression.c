#include "progression.h"

typedef char ProgressionFlagsFitInSave[(PROGRESSION_FLAG_COUNT<=PROGRESSION_FLAG_CAPACITY)?1:-1];

void progression_init(ProgressionState *state)
{
    if(state) state->bits=0;
}

int progression_flag_valid(ProgressionFlag flag)
{
    return flag>=0 && flag<PROGRESSION_FLAG_COUNT;
}

int progression_has(const ProgressionState *state,ProgressionFlag flag)
{
    return state && progression_flag_valid(flag) &&
           (state->bits&(1u<<(unsigned int)flag))!=0;
}

int progression_set(ProgressionState *state,ProgressionFlag flag)
{
    uint32_t bit;
    if(!state || !progression_flag_valid(flag)) return 0;
    bit=1u<<(unsigned int)flag;
    if(state->bits&bit) return 0;
    state->bits|=bit;
    return 1;
}

int progression_clear(ProgressionState *state,ProgressionFlag flag)
{
    uint32_t bit;
    if(!state || !progression_flag_valid(flag)) return 0;
    bit=1u<<(unsigned int)flag;
    if(!(state->bits&bit)) return 0;
    state->bits&=~bit;
    return 1;
}

uint32_t progression_save_bits(const ProgressionState *state)
{
    return state?state->bits:0;
}

void progression_load_bits(ProgressionState *state,uint32_t bits)
{
    if(state) state->bits=bits;
}
