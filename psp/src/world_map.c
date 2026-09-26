#include <pspgu.h>
#include "world_map.h"
#include "graphics.h"
#include "text.h"
#define C(r,g,b) GU_RGBA(r,g,b,255)
void world_map_open(WorldMap *map) { if(map) map->active=1; }
int world_map_update(WorldMap *map,const Input *input)
{
    if(!map || !map->active) return 0;
    if(input->cancel || (input->menu&INPUT_MENU_OPEN)) { map->active=0;return 1; }
    return 0;
}
void world_map_draw(const WorldMap *map)
{
    if(!map || !map->active) return;
    graphics_rectangle(0,0,480,272,C(15,27,34));graphics_rectangle(0,0,480,5,C(214,137,75));
    text_draw(24,22,"WORLD MAP",C(246,213,158),2);text_draw(24,54,"THE WIDER JOURNEY",C(150,188,179),1);
    graphics_rectangle(24,82,432,108,C(24,42,49));
    text_wrap(43,106,390,54,"A WORLD OVERVIEW WILL GUIDE YOUR ROUTE HERE.\nFOR NOW, RETURN TO THE FIELD KIT TO CONTINUE EXPLORING.",C(198,214,202),1);
    graphics_rectangle(24,221,432,1,C(66,81,81));text_draw(24,241,"O RETURN TO FIELD KIT",C(169,195,182),1);
}
