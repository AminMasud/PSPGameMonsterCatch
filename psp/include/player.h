#ifndef EMBERWAKE_PLAYER_H
#define EMBERWAKE_PLAYER_H
#include "input.h"
#include "map.h"
typedef enum { FACE_DOWN, FACE_LEFT, FACE_RIGHT, FACE_UP } Direction;
typedef struct {
    int tile_x, tile_y, target_x, target_y;
    float x, y, progress, animation;
    int moving;
    Direction facing;
} Player;
void player_init(Player *player, const Map *map);
void player_update(Player *player, const Map *map, const Input *input, float seconds);
typedef int (*PlayerBlocker)(void *context, int x, int y);
void player_update_blocked(Player *p, const Map *map, const Input *input,
                           float seconds, PlayerBlocker blocker, void *context);
#endif
