#ifndef EMBERWAKE_GAME_H
#define EMBERWAKE_GAME_H
#include "camera.h"
#include "npc.h"
#include "dialogue.h"
#include "encounter.h"
#include "battle.h"
typedef struct {
    const Map *map;
    Player player;
    Camera camera;
    int map_id;
    Npcs npcs;
    Dialogue dialogue;
    Encounter encounter;
    Battler partner;
    Battle battle;
    int in_battle;
} Game;
void game_init(Game *game);
void game_update(Game *game, const Input *input, float seconds);
void game_draw(const Game *game);
#endif
