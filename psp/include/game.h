#ifndef EMBERWAKE_GAME_H
#define EMBERWAKE_GAME_H
#include "camera.h"
#include "npc.h"
#include "dialogue.h"
#include "encounter.h"
#include "battle.h"
#include "party_menu.h"
#include "inventory.h"
#include "save_data.h"
#include "player_menu.h"
#include "ready_prompt.h"
#include "npc_battle.h"
#include "boss.h"
#include "healing.h"
#include "world_map.h"
typedef struct {
    SpeciesId species;
    int level;
    uint32_t seed;
} PendingBattle;
typedef enum {
    NPC_BATTLE_FLOW_NONE,
    NPC_BATTLE_FLOW_INTRO,
    NPC_BATTLE_FLOW_READY,
    NPC_BATTLE_FLOW_ACTIVE
} NpcBattleFlow;
typedef struct {
    const NpcBattleData *data;
    uint32_t seed;
    NpcBattleFlow flow;
} PendingNpcBattle;
typedef struct {
    const BossData *data;
    uint32_t seed;
    NpcBattleFlow flow;
} PendingBossBattle;
typedef struct {
    const Map *map;
    Player player;
    Camera camera;
    int map_id;
    Npcs npcs;
    Dialogue dialogue;
    Encounter encounter;
    Party party;
    Inventory inventory;
    PartyMenu roster;
    int roster_open;
    PlayerMenu menu;
    GameOptions options;
    int menu_open, previous_ui_direction;
    float transition, area_label, animation;
    int shop_open, shop_cursor, shop_previous_direction;
    int tavi_shop_pending;
    char shop_message[112];
    SaveStatus save_seen_status;
    ReadyPrompt ready_prompt;
    HealingPrompt healing_prompt;
    WorldMap world_map;
    uint32_t discovered_maps;
    PendingBattle pending_battle;
    NpcBattleProgress npc_battle_progress;
    ProgressionState progression;
    PendingNpcBattle npc_battle;
    PendingBossBattle boss_battle;
    Battle battle;
    int in_battle;
} Game;
void game_init(Game *game);
/* Reusable entry point for future trainer, NPC, and boss battles. */
int game_offer_important_battle(Game *game,const char *opponent,
                                SpeciesId species,int level,uint32_t seed);
int game_offer_npc_battle(Game *game,const NpcBattleData *data,uint32_t seed);
int game_offer_boss_battle(Game *game,const BossData *data,uint32_t seed);
void game_update(Game *game, const Input *input, float seconds);
void game_draw(const Game *game);
#endif
