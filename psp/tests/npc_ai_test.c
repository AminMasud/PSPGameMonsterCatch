#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "npc_ai.h"

int main(void)
{
    Creature actor={0};
    for(int i=0;i<CREATURE_MOVES;++i) actor.moves[i]=-1;
    actor.moves[0]=MOVE_NUDGE;actor.uses[0]=2;
    actor.moves[1]=-1;actor.uses[1]=99;
    actor.moves[2]=MOVE_CINDER;actor.uses[2]=1;
    actor.moves[3]=MOVE_COUNT;actor.uses[3]=99;
    assert(npc_ai_move_available(&actor,0));
    assert(!npc_ai_move_available(&actor,1));
    assert(npc_ai_move_available(&actor,2));
    assert(!npc_ai_move_available(&actor,3));
    assert(!npc_ai_move_available(&actor,-1) && !npc_ai_move_available(&actor,4));
    assert(!npc_ai_move_available(0,0));

    Creature unchanged=actor;
    int seen[CREATURE_MOVES]={0};
    for(uint32_t seed=1;seed<=100;++seed) {
        uint32_t random=seed;
        int choice=npc_ai_choose_move(NPC_AI_EASY,&actor,&random);
        assert(choice==0 || choice==2);
        assert(npc_ai_move_available(&actor,choice));
        ++seen[choice];
    }
    assert(seen[0] && seen[2]);
    assert(!memcmp(&actor,&unchanged,sizeof(actor))); /* Choosing never spends the move. */

    actor.uses[0]=0;
    for(int profile=NPC_AI_EASY;profile<NPC_AI_PROFILE_COUNT;++profile) {
        uint32_t random=45;
        assert(npc_ai_choose_move((NpcAiProfile)profile,&actor,&random)==2);
    }
    actor.uses[2]=0;
    uint32_t random=91;
    assert(npc_ai_choose_move(NPC_AI_EASY,&actor,&random)==NPC_AI_PRESS_ON);
    assert(random==91); /* No selection roll is consumed when PRESS ON is required. */
    assert(npc_ai_choose_move(NPC_AI_EASY,&actor,0)==NPC_AI_PRESS_ON);
    puts("PASS: simple NPC AI selects only usable attacks, falls back safely, and exposes expandable profiles");
    return 0;
}
