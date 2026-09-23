#ifndef EMBERWAKE_MAP_H
#define EMBERWAKE_MAP_H
#define TILE_SIZE 32
typedef struct {
    int width, height;
    const char *const *rows;
    int spawn_x, spawn_y;
} Map;
const Map *map_test(void);
char map_tile(const Map *map, int x, int y);
int map_walkable(const Map *map, int x, int y);
enum { MAP_CLEARING, MAP_FOREST, MAP_LODGE, MAP_CAVE, MAP_COUNT };
typedef struct { int from, x, y, to, arrival_x, arrival_y; } Portal;
const Map *map_get(int id);
const char *map_name(int id);
const Portal *map_portal(int id, int x, int y);
int map_encounter_area(int id, int x, int y);
#endif
