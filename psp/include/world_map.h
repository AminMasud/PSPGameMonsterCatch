#ifndef EMBERWAKE_WORLD_MAP_H
#define EMBERWAKE_WORLD_MAP_H

#include "input.h"
typedef struct { int active,map_id; } WorldMap;
void world_map_open(WorldMap *map,int map_id);
int world_map_update(WorldMap *map,const Input *input);
void world_map_draw(const WorldMap *map);
#endif
