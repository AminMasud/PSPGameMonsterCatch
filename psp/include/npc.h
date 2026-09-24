#ifndef EMBERWAKE_NPC_H
#define EMBERWAKE_NPC_H
#include "player.h"
#include "npc_battle.h"
#include "boss.h"
#define NPC_MAX 4
typedef struct {
    Player actor;
    const char *name, *first, *second;
    const NpcBattleData *battle;
    const BossData *boss;
    const Gate *gate;
    int open_x, open_y;
    int patrol_start, patrol_end, direction;
    float wait;
} Npc;
typedef struct { Npc people[NPC_MAX]; int count; } Npcs;
void npc_load(Npcs *npcs, int map_id);
void npc_apply_progress(Npcs *npcs,uint32_t defeated,const ProgressionState *progression);
void npc_reconcile_progression(uint32_t defeated,ProgressionState *progression);
int npc_blocks(void *context, int x, int y);
void npc_update(Npcs *npcs, const Map *map, const Player *player, float seconds);
Npc *npc_facing(Npcs *npcs, const Player *player);
#endif
