#include <stdio.h>
#include <pspgu.h>
#include "healing.h"
#include "map.h"
#include "graphics.h"
#include "text.h"

#define C(r,g,b) GU_RGBA(r,g,b,255)

static const HealingPoint points[] = {
    {MAP_LODGE,2,1,"WAYFARER DAIS","RESTORE YOUR WHOLE TEAM?"},
    {MAP_REST,2,2,"LANTERN DAIS","RESTORE YOUR WHOLE TEAM?"}
};

int healing_point_valid(const HealingPoint *point)
{
    return point && point->map_id>=0 && point->map_id<MAP_COUNT &&
           point->x>=0 && point->y>=0 && point->name && point->name[0] &&
           point->prompt && point->prompt[0] &&
           map_tile(map_get(point->map_id),point->x,point->y)=='H';
}

int healing_point_count(void)
{
    return (int)(sizeof(points)/sizeof(points[0]));
}

const HealingPoint *healing_point_at(int map_id,int x,int y)
{
    for(int i=0;i<healing_point_count();++i)
        if(points[i].map_id==map_id && points[i].x==x && points[i].y==y)
            return &points[i];
    return 0;
}

void healing_prompt_open(HealingPrompt *prompt,const HealingPoint *point)
{
    if(!prompt || !healing_point_valid(point)) return;
    *prompt=(HealingPrompt){0};
    prompt->active=1;
    prompt->point=point;
}

HealingPromptResult healing_prompt_update(HealingPrompt *prompt,const Input *input)
{
    if(!prompt || !prompt->active) return HEALING_WAITING;
    int direction=input->vertical;
    if(direction && direction!=prompt->previous_direction) prompt->cursor=1-prompt->cursor;
    prompt->previous_direction=direction;
    if(input->cancel) { prompt->active=0;return HEALING_DECLINED; }
    if(!input->confirm) return HEALING_WAITING;
    prompt->active=0;
    return prompt->cursor==0?HEALING_ACCEPTED:HEALING_DECLINED;
}

void healing_prompt_draw(const HealingPrompt *prompt)
{
    if(!prompt || !prompt->active || !prompt->point) return;
    graphics_rectangle(72,38,336,196,C(179,145,91));
    graphics_rectangle(75,41,330,190,C(18,27,34));
    text_draw(133,61,"REST AT HEALING POINT?",C(245,217,166),1);
    text_draw(172,91,prompt->point->name,C(165,195,186),1);
    for(int i=0;i<2;++i) {
        int y=119+i*39;
        if(i==prompt->cursor) {
            graphics_rectangle(132,y,216,31,C(65,83,78));
            graphics_rectangle(132,y,4,31,C(115,214,171));
        }
        text_draw(220,y+11,i==0?"YES":"NO",i==prompt->cursor?C(198,255,212):C(174,190,186),1);
    }
    text_draw(133,211,"UP/DOWN SELECT   X CONFIRM   O NO",C(158,187,179),1);
}
