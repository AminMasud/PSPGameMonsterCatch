#include <assert.h>
#include <stdio.h>
#include "boss.h"

static const BossData guardian={
    .id=2,.name="ELDER SYLVA",.title="FERNVEIL GUARDIAN",
    .intro={"THE FOREST HEARS EVERY STEP.","SHOW ME THE BOND YOU CARRY."},
    .victory={"THE PATH ACCEPTS YOUR COURAGE.","CARRY ITS TRUST BEYOND THESE TREES."},
    .defeat={"COURAGE ALSO MEANS RETURNING PREPARED.",0},
    .party={{SPECIES_MOSSPRIG,7},{SPECIES_GUSTLET,8}},.party_count=2,
    .ai_profile=NPC_AI_BOSS,.reward_embermarks=300,
    .completion_flag=PROGRESSION_EAST_FOREST_BOSS_DEFEATED,
    .presentation=BATTLE_PRESENTATION_GUARDIAN
};

int main(void)
{
    ProgressionState progression={0};
    BossData changed=guardian;
    assert(boss_data_valid(&guardian));
    assert(!boss_is_defeated(&progression,&guardian));
    assert(boss_mark_victory(&progression,&guardian));
    assert(boss_is_defeated(&progression,&guardian));
    uint32_t completed=progression_save_bits(&progression);
    assert(!boss_mark_victory(&progression,&guardian));
    assert(progression_save_bits(&progression)==completed);

    changed=guardian;changed.id=BOSS_MAX;assert(!boss_data_valid(&changed));
    changed=guardian;changed.name="";assert(!boss_data_valid(&changed));
    changed=guardian;changed.title=0;assert(!boss_data_valid(&changed));
    changed=guardian;changed.intro.first=0;assert(!boss_data_valid(&changed));
    changed=guardian;changed.victory.first="";assert(!boss_data_valid(&changed));
    changed=guardian;changed.defeat=(NpcBattleDialogue){0};assert(boss_data_valid(&changed));
    changed=guardian;changed.party_count=0;assert(!boss_data_valid(&changed));
    changed=guardian;changed.party[0].species=SPECIES_COUNT;assert(!boss_data_valid(&changed));
    changed=guardian;changed.party[0].level=0;assert(!boss_data_valid(&changed));
    changed=guardian;changed.ai_profile=NPC_AI_PROFILE_COUNT;assert(!boss_data_valid(&changed));
    changed=guardian;changed.reward_embermarks=-1;assert(!boss_data_valid(&changed));
    changed=guardian;changed.completion_flag=PROGRESSION_NONE;assert(!boss_data_valid(&changed));
    changed=guardian;changed.completion_flag=PROGRESSION_FLAG_COUNT;assert(!boss_data_valid(&changed));
    changed=guardian;changed.presentation=BATTLE_PRESENTATION_COUNT;assert(!boss_data_valid(&changed));
    puts("PASS: boss definitions, validation, and one-time completion");
    return 0;
}
