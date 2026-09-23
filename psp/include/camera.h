#ifndef EMBERWAKE_CAMERA_H
#define EMBERWAKE_CAMERA_H
#include "player.h"
typedef struct { int x, y; } Camera;
void camera_update(Camera *camera, const Player *player, const Map *map);
#endif
