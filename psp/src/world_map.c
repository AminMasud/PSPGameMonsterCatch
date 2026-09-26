#include <pspgu.h>
#include "world_map.h"
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
    text_draw(24,242,"O RETURN TO FIELD KIT",C(169,195,182),1);
}
