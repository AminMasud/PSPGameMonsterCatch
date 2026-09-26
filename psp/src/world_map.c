#include <pspgu.h>
#include "world_map.h"
#include "map.h"
#include "graphics.h"
#include "text.h"
#define C(r,g,b) GU_RGBA(r,g,b,255)

static void road(int x1,int y1,int x2,int y2)
{
    int steps=24;
    for(int i=0;i<=steps;++i) {
        int x=x1+(x2-x1)*i/steps,y=y1+(y2-y1)*i/steps;
        graphics_rectangle(x,y,8,6,C(189,158,100));
    }
}

static void place(int x,int y,int w,int h,unsigned int fill,const char *name)
{
    graphics_rectangle(x-3,y-3,w+6,h+6,C(38,58,58));
    graphics_rectangle(x,y,w,h,fill);
    graphics_rectangle(x+4,y+4,w-8,4,C(255,224,145));
    text_draw(x,y+h+9,name,C(239,225,184),1);
}
void world_map_open(WorldMap *map,int map_id,uint32_t discovered)
{
    if(!map) return;
    map->active=1;map->map_id=map_id;map->cursor=map_id;map->previous_direction=0;map->discovered=discovered;
}

static void marker(int map_id)
{
    int x=44,y=135;
    const char *name="HEARTH CLEARING";
    if(map_id==MAP_FOREST) { x=148;y=103;name="FERNVEIL WOODS"; }
    else if(map_id==MAP_CAVE) { x=246;y=135;name="HOLLOWSTONE CAVE"; }
    else if(map_id==MAP_MARSH) { x=351;y=94;name="SUNTHREAD MARSH"; }
    else if(map_id==MAP_LODGE) { x=119;y=76;name="WAYFARER LODGE"; }
    else if(map_id==MAP_REST) { x=401;y=151;name="LANTERN REST"; }
    graphics_rectangle(x-7,y-7,8,4,C(255,224,112));graphics_rectangle(x-7,y-7,4,8,C(255,224,112));
    graphics_rectangle(x+51,y-7,8,4,C(255,224,112));graphics_rectangle(x+55,y-7,4,8,C(255,224,112));
    text_draw(24,221,"YOU ARE IN",C(150,188,179),1);text_draw(102,221,name,C(246,213,158),1);
}

static void cursor_draw(int map_id)
{
    int x=68,y=149;
    if(map_id==MAP_FOREST) { x=173;y=120; }
    else if(map_id==MAP_CAVE) { x=272;y=149; }
    else if(map_id==MAP_MARSH) { x=375;y=111; }
    else if(map_id==MAP_LODGE) { x=129;y=84; }
    else if(map_id==MAP_REST) { x=411;y=160; }
    graphics_rectangle(x-7,y-10,14,3,C(255,244,164));
    graphics_rectangle(x-7,y-10,3,14,C(255,244,164));
}
int world_map_update(WorldMap *map,const Input *input)
{
    if(!map || !map->active) return 0;
    int direction=input->vertical?input->vertical:input->horizontal;
    if(direction && direction!=map->previous_direction)
        map->cursor=(map->cursor+direction+MAP_COUNT)%MAP_COUNT;
    map->previous_direction=direction;
    if(input->cancel || (input->menu&INPUT_MENU_OPEN)) { map->active=0;return 1; }
    return 0;
}
void world_map_draw(const WorldMap *map)
{
    if(!map || !map->active) return;
    graphics_rectangle(0,0,480,272,C(17,31,38));graphics_rectangle(0,0,480,5,C(214,137,75));
    text_draw(18,15,"WORLD MAP",C(246,213,158),2);
    text_draw(18,39,"THE EMBERWAKE TRAIL",C(150,188,179),1);
    graphics_rectangle(16,58,448,174,C(38,76,78));
    graphics_rectangle(20,62,440,166,C(66,113,87));
    for(int i=0;i<9;++i) graphics_rectangle(25+i*49,67+(i%3)*45,19,11,C(55,102,77));
    road(92,150,161,123);road(196,121,255,149);road(294,149,365,112);road(273,156,274,196);
    road(143,126,129,91);road(373,113,406,154);
    place(44,135,48,27,C(168,128,77),"HEARTH");
    place(148,103,50,34,C(44,96,59),"FERNVEIL");
    place(246,135,52,28,C(91,77,71),"HOLLOWSTONE");
    place(351,94,48,34,C(50,103,86),"SUNTHREAD");
    place(244,184,60,23,C(79,63,88),"NORTHERN WOODS");
    graphics_rectangle(119,76,20,17,C(94,81,69));graphics_rectangle(122,72,14,7,C(171,142,95));
    text_draw(93,54,"WAYFARER",C(235,211,166),1);
    graphics_rectangle(401,151,20,17,C(94,81,69));graphics_rectangle(404,147,14,7,C(171,142,95));
    text_draw(389,178,"LANTERN REST",C(235,211,166),1);
    marker(map->map_id);
    cursor_draw(map->cursor);
    text_draw(24,240,"SELECTED",C(150,188,179),1);
    text_draw(92,240,(map->discovered&(1u<<map->cursor))?map_name(map->cursor):"UNKNOWN",C(246,213,158),1);
    text_draw(280,240,map->cursor==map->map_id?"CURRENT":(map->discovered&(1u<<map->cursor))?"DISCOVERED":"UNKNOWN",C(235,211,166),1);
    text_draw(24,259,"D-PAD INSPECT   O RETURN",C(169,195,182),1);
}
