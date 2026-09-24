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
static const char *const marsh_rows[] = {
#include "../assets/maps/marsh.inc"
};
static const char *const rest_rows[] = {
#include "../assets/maps/rest.inc"
};
static const Map marsh = {28,18,marsh_rows,2,10};
static const Map rest = {15,9,rest_rows,7,6};
#define OPEN_PORTAL(f,px,py,t,ax,ay) \
    {.from=f,.x=px,.y=py,.to=t,.arrival_x=ax,.arrival_y=ay,.required_flag=PROGRESSION_NONE}
static const Gate portals[] = {
    {.from=MAP_CLEARING,.x=38,.y=11,.to=MAP_FOREST,.arrival_x=2,.arrival_y=11,
     .required_flag=PROGRESSION_FIRST_CHALLENGER_DEFEATED,.gatekeeper="REN",
     .locked_dialogue={"THE EAST PATH LEADS INTO FERNVEIL.","SHOW ME ONE CALM BATTLE FIRST."},
     .unlocked_dialogue={"YOU ARE READY FOR THE EAST WOODS.","THE PATH IS OPEN. TRAVEL SAFELY."}},
    OPEN_PORTAL(MAP_FOREST,1,11,MAP_CLEARING,37,11),
    OPEN_PORTAL(MAP_CLEARING,5,9,MAP_LODGE,7,6),
    OPEN_PORTAL(MAP_LODGE,7,7,MAP_CLEARING,5,10),
    OPEN_PORTAL(MAP_FOREST,28,5,MAP_CAVE,2,10),
    OPEN_PORTAL(MAP_CAVE,2,11,MAP_FOREST,28,6),
    OPEN_PORTAL(MAP_FOREST,30,11,MAP_MARSH,2,10),
    OPEN_PORTAL(MAP_MARSH,1,10,MAP_FOREST,29,11),
    OPEN_PORTAL(MAP_MARSH,24,4,MAP_REST,7,6),
    OPEN_PORTAL(MAP_REST,7,7,MAP_MARSH,24,5)
};
#undef OPEN_PORTAL
const Map *map_get(int id)
{
    const Map *const maps[] = {&clearing,&forest,&lodge,&cave,&marsh,&rest};
    return id >= 0 && id < MAP_COUNT ? maps[id] : &clearing;
}
const char *map_name(int id)
{
    const char *const names[] = {"HEARTH CLEARING","FERNVEIL WOODS","WAYFARER LODGE","HOLLOWSTONE CAVE","SUNTHREAD MARSH","LANTERN REST"};
    return id >= 0 && id < MAP_COUNT ? names[id] : names[0];
}
const Portal *map_portal(int id, int x, int y)
{
    for (unsigned int i = 0; i < sizeof(portals)/sizeof(portals[0]); ++i)
        if (portals[i].from == id && portals[i].x == x && portals[i].y == y)
            return &portals[i];
    return 0;
}
const Gate *map_gate(int id, int x, int y)
{
    return map_portal(id,x,y);
}
int map_encounter_area(int id, int x, int y)
{
    char tile = map_tile(map_get(id),x,y);
    return tile == 'g' && id == MAP_FOREST ? 1 : tile == 'c' && id == MAP_CAVE ? 2 :
           tile == 'r' && id == MAP_MARSH ? 3 : 0;
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
           tile == '>' || tile == '<' || tile == 'g' || tile == 'r' || tile == 'c' || tile == '_' || tile == 'H';
}
