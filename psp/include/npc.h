#ifndef EMBERWAKE_NPC_H
#define EMBERWAKE_NPC_H
#include "player.h"
#include "npc_battle.h"
#define NPC_MAX 4
typedef struct {
    Player actor;
    const char *name, *first, *second;
    const NpcBattleData *battle;
    int defeated_x, defeated_y;
    int patrol_start, patrol_end, direction;
    float wait;
} Npc;
typedef struct { Npc people[NPC_MAX]; int count; } Npcs;
void npc_load(Npcs *npcs, int map_id);
void npc_apply_progress(Npcs *npcs, uint32_t defeated);
int npc_blocks(void *context, int x, int y);
void npc_update(Npcs *npcs, const Map *map, const Player *player, float seconds);
Npc *npc_facing(Npcs *npcs, const Player *player);
#endif
