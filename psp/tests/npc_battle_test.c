#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "battle.h"
#include "npc_ai.h"
#include "npc_battle.h"

static const NpcBattleData challenger={
    .id=7,
    .name="TEST CHALLENGER",
    .before={"LET US TEST OUR TEAMS.","TWO VEYLINGS ARE READY."},
    .victory={"YOU EARNED THIS VICTORY.",0},
    .defeat={"REST, THEN TRY AGAIN.",0},
    .party={{SPECIES_MOSSPRIG,3},{SPECIES_ZAPPIP,4}},
    .party_count=2,
    .ai_profile=NPC_AI_STANDARD,
    .reward_embermarks=125,
    .progression_flag=1u<<9
};

static void confirm(Battle *battle)
{
    battle_update(battle,&(Input){.confirm=1});
}

static void attack(Battle *battle)
{
    assert(battle->phase==BATTLE_MENU);
    battle->cursor=0;confirm(battle);
    assert(battle->phase==BATTLE_ATTACKS);
    battle->move_cursor=0;confirm(battle);
    assert(battle->phase==BATTLE_MESSAGE && battle->result==BATTLE_WIN);
}

static void data_and_progress(void)
{
    assert(npc_battle_data_valid(&challenger));
    NpcBattleData bad=challenger;
    bad.id=NPC_BATTLE_MAX;assert(!npc_battle_data_valid(&bad));
    bad=challenger;bad.party_count=0;assert(!npc_battle_data_valid(&bad));
    bad=challenger;bad.party[1].level=0;assert(!npc_battle_data_valid(&bad));
    bad=challenger;bad.ai_profile=NPC_AI_PROFILE_COUNT;assert(!npc_battle_data_valid(&bad));
    bad=challenger;bad.reward_embermarks=-1;assert(!npc_battle_data_valid(&bad));
    bad=challenger;bad.before.first=0;assert(!npc_battle_data_valid(&bad));
    bad=challenger;bad.defeat.first=0;bad.defeat.second=0;assert(npc_battle_data_valid(&bad));

    NpcBattleProgress progress={0};
    assert(!npc_battle_is_defeated(&progress,challenger.id));
    assert(npc_battle_mark_defeated(&progress,&challenger));
    assert(npc_battle_is_defeated(&progress,challenger.id));
    assert(progress.progression==challenger.progression_flag);
    assert(!npc_battle_mark_defeated(&progress,&challenger));
}

static void party_battle(void)
{
    Party party={0};party.count=1;
    creature_create(&party.members[0],SPECIES_CINDLET,100);
    party.members[0].speed=999;
    Inventory inventory;inventory_init(&inventory);
    Battle battle;
    assert(!battle_begin_npc_party_with_inventory(&battle,&party,&inventory,
           challenger.name,challenger.party,0,challenger.ai_profile,42));
    assert(battle_begin_npc_party_with_inventory(&battle,&party,&inventory,
           challenger.name,challenger.party,challenger.party_count,challenger.ai_profile,42));
    assert(battle.npc_battle && battle.enemy_count==2 && battle.enemy_active==0);
    assert(battle.ai_profile==NPC_AI_STANDARD && !strcmp(battle.opponent_name,challenger.name));
    assert(battle.enemy.species==SPECIES_MOSSPRIG && battle.capture_charges==0);
    confirm(&battle);
    assert(battle.phase==BATTLE_MENU);

    unsigned int turn=battle.turn_number;
    uint32_t random=battle.random;
    int locked_choice=battle.choices[1];
    assert(npc_ai_move_available(&battle.enemy,locked_choice));
    battle.cursor=1;confirm(&battle);
    assert(battle.phase==BATTLE_MESSAGE && strstr(battle.message,"CANNOT BE CAPTURED"));
    confirm(&battle);
    assert(battle.phase==BATTLE_MENU && battle.turn_number==turn && battle.random==random &&
           battle.choices[1]==locked_choice);
    battle.cursor=4;confirm(&battle);
    assert(battle.phase==BATTLE_MESSAGE && strstr(battle.message,"CANNOT RUN"));
    confirm(&battle);
    assert(battle.phase==BATTLE_MENU && battle.turn_number==turn && battle.random==random &&
           battle.choices[1]==locked_choice);

    battle.enemy.hp=1;
    attack(&battle);
    confirm(&battle);
    assert(battle.next_enemy_pending && battle.reward_given);
    confirm(&battle);
    assert(battle.phase==BATTLE_MESSAGE && battle.after==AFTER_BEGIN_TURN);
    assert(battle.result==BATTLE_ONGOING && battle.enemy_active==1);
    assert(battle.enemy.species==SPECIES_ZAPPIP && strstr(battle.message,"SENDS OUT"));
    confirm(&battle);
    assert(battle.phase==BATTLE_MENU);
    battle.enemy.hp=1;
    attack(&battle);
    confirm(&battle);
    assert(!battle.next_enemy_pending && battle.reward_given);
    confirm(&battle);
    assert(battle.phase==BATTLE_MESSAGE && battle.after==AFTER_DONE);
    confirm(&battle);
    assert(battle.phase==BATTLE_DONE && battle.result==BATTLE_WIN);
}

int main(void)
{
    data_and_progress();
    party_battle();
    puts("PASS: reusable NPC battle data, parties, AI profiles, locked capture/run, progression and defeat state");
    return 0;
}
