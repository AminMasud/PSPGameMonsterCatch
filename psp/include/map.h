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
#endif
