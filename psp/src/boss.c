#include "boss.h"

typedef char BossCatalogFits[(BOSS_COUNT<=BOSS_MAX)?1:-1];

static const BossData catalog[BOSS_COUNT] = {
    [BOSS_EAST_FOREST_GUARDIAN] = {
        .id=BOSS_EAST_FOREST_GUARDIAN,.name="ELDER SYLVA",.title="FERNVEIL GUARDIAN",
        .intro={"I AM SYLVA, KEEPER OF FERNVEIL.",
                "SHOW ME THE BOND THAT GUIDES YOUR VEYLINGS."},
        .victory={"FERNVEIL RECOGNIZES YOUR BOND.",
                  "THE SUNTHREAD WAY NOW OPENS TO YOU."},
        .defeat={"THE FOREST ASKS FOR PATIENCE.",
                 "RETURN WHEN YOUR TEAM IS READY."},
        .party={{SPECIES_MOSSPRIG,6},{SPECIES_GUSTLET,7}},.party_count=2,
        .ai_profile=NPC_AI_BOSS,.reward_embermarks=300,
        .required_flag=PROGRESSION_NONE,
        .completion_flag=PROGRESSION_EAST_FOREST_BOSS_DEFEATED,
        .presentation=BATTLE_PRESENTATION_GUARDIAN
    },
    [BOSS_NORTHERN_WOODS_GUARDIAN] = {
        .id=BOSS_NORTHERN_WOODS_GUARDIAN,.name="WARDEN RUNE",.title="NORTHWOOD GUARDIAN",
        .intro={"THE NORTHERN WOODS REMEMBER EVERY STORM.",
                "PROVE THAT FERNVEIL'S TRUST ENDURES."},
        .victory={"THE NORTHWOOD BOWS TO YOUR RESOLVE.",
                  "A NEW WAY CAN OPEN BEYOND THE PINES."},
        .defeat={"THE PINES WILL WAIT FOR YOUR RETURN.",0},
        .party={{SPECIES_TOXLET,12},{SPECIES_PEBCHICK,13},{SPECIES_GALETALON,14}},.party_count=3,
        .ai_profile=NPC_AI_STANDARD,.reward_embermarks=650,
        .required_flag=PROGRESSION_EAST_FOREST_BOSS_DEFEATED,
        .completion_flag=PROGRESSION_NORTH_FOREST_BOSS_DEFEATED,
        .presentation=BATTLE_PRESENTATION_GUARDIAN
    }
};

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
       boss->reward_embermarks<0 ||
       (boss->required_flag!=PROGRESSION_NONE && !progression_flag_valid(boss->required_flag)) ||
       !progression_flag_valid(boss->completion_flag) ||
       boss->presentation<0 || boss->presentation>=BATTLE_PRESENTATION_COUNT) return 0;
    for(int i=0;i<boss->party_count;++i)
        if(boss->party[i].species<0 || boss->party[i].species>=SPECIES_COUNT ||
           boss->party[i].level<1 || boss->party[i].level>CREATURE_MAX_LEVEL) return 0;
    return 1;
}

int boss_is_available(const ProgressionState *progression,const BossData *boss)
{
    return boss_data_valid(boss) && (boss->required_flag==PROGRESSION_NONE ||
                                     progression_has(progression,boss->required_flag));
}

int boss_is_defeated(const ProgressionState *progression,const BossData *boss)
{
    return boss_data_valid(boss) && progression_has(progression,boss->completion_flag);
}

int boss_mark_victory(ProgressionState *progression,const BossData *boss)
{
    return boss_data_valid(boss) && progression_set(progression,boss->completion_flag);
}

int boss_catalog_count(void) { return BOSS_COUNT; }

const BossData *boss_get(BossId id)
{
    return id>=0 && id<BOSS_COUNT ? &catalog[id] : 0;
}
