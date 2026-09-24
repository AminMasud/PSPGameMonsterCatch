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
    Battle battle;
    int in_battle;
} Game;
void game_init(Game *game);
void game_update(Game *game, const Input *input, float seconds);
void game_draw(const Game *game);
#endif
