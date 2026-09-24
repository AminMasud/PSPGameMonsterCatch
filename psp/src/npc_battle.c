#include "npc_battle.h"

static int dialogue_valid(NpcBattleDialogue dialogue,int optional)
{
    if(!dialogue.first || !dialogue.first[0]) return optional && !dialogue.second;
    return !dialogue.second || dialogue.second[0];
}

int npc_battle_data_valid(const NpcBattleData *data)
{
    if(!data || data->id<0 || data->id>=NPC_BATTLE_MAX || !data->name || !data->name[0] ||
       !dialogue_valid(data->before,0) || !dialogue_valid(data->victory,0) ||
       !dialogue_valid(data->defeat,1) || data->party_count<1 ||
       data->party_count>NPC_BATTLE_PARTY_MAX || data->ai_profile<0 ||
       data->ai_profile>=NPC_AI_PROFILE_COUNT || data->reward_embermarks<0) return 0;
    for(int i=0;i<data->party_count;++i)
        if(data->party[i].species<0 || data->party[i].species>=SPECIES_COUNT ||
           data->party[i].level<1 || data->party[i].level>CREATURE_MAX_LEVEL) return 0;
    return 1;
}

int npc_battle_is_defeated(const NpcBattleProgress *progress,int npc_id)
{
    return progress && npc_id>=0 && npc_id<NPC_BATTLE_MAX &&
           (progress->defeated&(1u<<(unsigned int)npc_id))!=0;
}

int npc_battle_mark_defeated(NpcBattleProgress *progress,const NpcBattleData *data)
{
    if(!progress || !npc_battle_data_valid(data) || npc_battle_is_defeated(progress,data->id)) return 0;
    progress->defeated|=1u<<(unsigned int)data->id;
    progress->progression|=data->progression_flag;
    return 1;
}
