#ifndef EMBERWAKE_BOSS_H
#define EMBERWAKE_BOSS_H

#include "battle.h"

#define BOSS_MAX 16

typedef enum {
    BOSS_EAST_FOREST_GUARDIAN,
    BOSS_NORTHERN_WOODS_GUARDIAN,
    BOSS_COUNT
} BossId;

typedef struct BossData {
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
    /* NONE means the boss is available without an earlier boss victory. */
    ProgressionFlag required_flag;
    ProgressionFlag completion_flag;
    BattlePresentation presentation;
} BossData;

int boss_data_valid(const BossData *boss);
int boss_is_available(const ProgressionState *progression,const BossData *boss);
int boss_is_defeated(const ProgressionState *progression,const BossData *boss);
int boss_mark_victory(ProgressionState *progression,const BossData *boss);
int boss_catalog_count(void);
const BossData *boss_get(BossId id);

#endif
