#include "npc_ai.h"

typedef int (*NpcAiStrategy)(const Creature *actor,uint32_t *random);

int npc_ai_move_available(const Creature *actor,int slot)
{
    return actor && slot>=0 && slot<CREATURE_MOVES &&
           actor->moves[slot]>=0 && actor->moves[slot]<MOVE_COUNT && actor->uses[slot]>0;
}

static uint32_t random_next(uint32_t *random)
{
    uint32_t value=*random?*random:0x3291u;
    value^=value<<13;value^=value>>17;value^=value<<5;
    return *random=value;
}

static int choose_easy(const Creature *actor,uint32_t *random)
{
    int slots[CREATURE_MOVES],count=0;
    if(!actor || !random) return NPC_AI_PRESS_ON;
    for(int i=0;i<CREATURE_MOVES;++i)
        if(npc_ai_move_available(actor,i)) slots[count++]=i;
    if(!count) return NPC_AI_PRESS_ON;
    return slots[random_next(random)%(unsigned int)count];
}

int npc_ai_choose_move(NpcAiProfile profile,const Creature *actor,uint32_t *random)
{
    /* Standard and boss profiles inherit the safe easy policy until their
       own strategy functions are added. The dispatch point remains stable. */
    static NpcAiStrategy const strategies[NPC_AI_PROFILE_COUNT]={
        choose_easy,choose_easy,choose_easy
    };
    if(profile<0 || profile>=NPC_AI_PROFILE_COUNT) profile=NPC_AI_EASY;
    return strategies[profile](actor,random);
}
