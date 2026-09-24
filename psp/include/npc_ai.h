#ifndef EMBERWAKE_NPC_AI_H
#define EMBERWAKE_NPC_AI_H

#include <stdint.h>
#include "npc_battle.h"

#define NPC_AI_PRESS_ON (-1)

int npc_ai_move_available(const Creature *actor,int slot);
/* The AI sees only its own actor and RNG state. It cannot inspect a queued player action. */
int npc_ai_choose_move(NpcAiProfile profile,const Creature *actor,uint32_t *random);

#endif
