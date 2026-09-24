#ifndef EMBERWAKE_NPC_BATTLE_H
#define EMBERWAKE_NPC_BATTLE_H

#include <stdint.h>
#include "creature.h"

#define NPC_BATTLE_MAX 32
#define NPC_BATTLE_PARTY_MAX 4

typedef enum {
    NPC_AI_EASY,
    NPC_AI_STANDARD,
    NPC_AI_BOSS,
    NPC_AI_PROFILE_COUNT
} NpcAiProfile;

typedef struct {
    SpeciesId species;
    int level;
} NpcBattleMember;

typedef struct {
    const char *first;
    const char *second;
} NpcBattleDialogue;

typedef struct {
    int id;
    const char *name;
    NpcBattleDialogue before;
    NpcBattleDialogue victory;
    NpcBattleDialogue defeat;
    NpcBattleMember party[NPC_BATTLE_PARTY_MAX];
    int party_count;
    NpcAiProfile ai_profile;
    int reward_embermarks;
    uint32_t progression_flag;
} NpcBattleData;

typedef struct {
    uint32_t defeated;
    uint32_t progression;
} NpcBattleProgress;

int npc_battle_data_valid(const NpcBattleData *data);
int npc_battle_is_defeated(const NpcBattleProgress *progress,int npc_id);
int npc_battle_mark_defeated(NpcBattleProgress *progress,const NpcBattleData *data);

#endif
