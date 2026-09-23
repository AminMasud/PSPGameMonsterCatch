#include "map.h"
static const char *const clearing_rows[] = {
#include "../assets/maps/clearing.inc"
};
static const Map clearing = {40, 24, clearing_rows, 5, 11};
static const char *const forest_rows[] = {
#include "../assets/maps/forest.inc"
};
static const char *const lodge_rows[] = {
#include "../assets/maps/lodge.inc"
};
static const char *const cave_rows[] = {
#include "../assets/maps/cave.inc"
};
static const Map forest = {32,20,forest_rows,2,11};
static const Map lodge = {15,9,lodge_rows,7,6};
static const Map cave = {20,14,cave_rows,2,10};
static const Portal portals[] = {
    {MAP_CLEARING,38,11,MAP_FOREST,2,11},
    {MAP_FOREST,1,11,MAP_CLEARING,37,11},
    {MAP_CLEARING,5,9,MAP_LODGE,7,6},
    {MAP_LODGE,7,7,MAP_CLEARING,5,10},
    {MAP_FOREST,28,5,MAP_CAVE,2,10},
    {MAP_CAVE,2,11,MAP_FOREST,28,6}
};
const Map *map_get(int id)
{
    const Map *const maps[] = {&clearing,&forest,&lodge,&cave};
    return id >= 0 && id < MAP_COUNT ? maps[id] : &clearing;
}
const char *map_name(int id)
{
    const char *const names[] = {"HEARTH CLEARING","FERNVEIL WOODS","WAYFARER LODGE","HOLLOWSTONE CAVE"};
    return id >= 0 && id < MAP_COUNT ? names[id] : names[0];
}
const Portal *map_portal(int id, int x, int y)
{
    for (unsigned int i = 0; i < sizeof(portals)/sizeof(portals[0]); ++i)
        if (portals[i].from == id && portals[i].x == x && portals[i].y == y)
            return &portals[i];
    return 0;
}
int map_encounter_area(int id, int x, int y)
{
    char tile = map_tile(map_get(id),x,y);
    return tile == 'g' && id == MAP_FOREST ? 1 : tile == 'c' && id == MAP_CAVE ? 2 : 0;
}
const Map *map_test(void) { return &clearing; }
char map_tile(const Map *map, int x, int y)
{
    if (x < 0 || y < 0 || x >= map->width || y >= map->height) return '#';
    return map->rows[y][x];
}
int map_walkable(const Map *map, int x, int y)
{
    char tile = map_tile(map, x, y);
    return tile == '.' || tile == '=' || tile == ',' || tile == 'D' ||
           tile == '>' || tile == 'g' || tile == 'c' || tile == '_';
}
