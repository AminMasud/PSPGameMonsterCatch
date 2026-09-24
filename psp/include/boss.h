#ifndef EMBERWAKE_BOSS_H
#define EMBERWAKE_BOSS_H

#include "battle.h"

#define BOSS_MAX 16

typedef enum {
    BOSS_EAST_FOREST_GUARDIAN,
    BOSS_COUNT
} BossId;

typedef struct {
    int id;
    const char *name;
    const char *title;
    NpcBattleDialogue intro;
    NpcBattleDialogue victory;
    NpcBattleDialogue defeat;
    NpcBattleMember party[NPC_BATTLE_PARTY_MAX];
    int party_count;
    NpcAiProfile ai_profile;
    int reward_embermarks;
    ProgressionFlag completion_flag;
    BattlePresentation presentation;
} BossData;

int boss_data_valid(const BossData *boss);
int boss_is_defeated(const ProgressionState *progression,const BossData *boss);
int boss_mark_victory(ProgressionState *progression,const BossData *boss);

#endif
