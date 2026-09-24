#include "boss.h"

static int dialogue_valid(NpcBattleDialogue dialogue,int optional)
{
    if(!dialogue.first || !dialogue.first[0]) return optional && !dialogue.second;
    return !dialogue.second || dialogue.second[0];
}

int boss_data_valid(const BossData *boss)
{
    if(!boss || boss->id<0 || boss->id>=BOSS_MAX || !boss->name || !boss->name[0] ||
       !boss->title || !boss->title[0] || !dialogue_valid(boss->intro,0) ||
       !dialogue_valid(boss->victory,0) || !dialogue_valid(boss->defeat,1) ||
       boss->party_count<1 || boss->party_count>NPC_BATTLE_PARTY_MAX ||
       boss->ai_profile<0 || boss->ai_profile>=NPC_AI_PROFILE_COUNT ||
       boss->reward_embermarks<0 || !progression_flag_valid(boss->completion_flag) ||
       boss->presentation<0 || boss->presentation>=BATTLE_PRESENTATION_COUNT) return 0;
    for(int i=0;i<boss->party_count;++i)
        if(boss->party[i].species<0 || boss->party[i].species>=SPECIES_COUNT ||
           boss->party[i].level<1 || boss->party[i].level>CREATURE_MAX_LEVEL) return 0;
    return 1;
}

int boss_is_defeated(const ProgressionState *progression,const BossData *boss)
{
    return boss_data_valid(boss) && progression_has(progression,boss->completion_flag);
}

int boss_mark_victory(ProgressionState *progression,const BossData *boss)
{
    return boss_data_valid(boss) && progression_set(progression,boss->completion_flag);
}
