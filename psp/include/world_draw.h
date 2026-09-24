#ifndef EMBERWAKE_WORLD_DRAW_H
#define EMBERWAKE_WORLD_DRAW_H
#include "camera.h"
void world_draw(const Map *map, const Player *player, const Camera *camera,float animation);
void world_actor_draw(const Player *p, const Camera *camera, int npc);
#endif
