#include <stdio.h>
#include "player_menu.h"
#include "map.h"
#include "graphics.h"
#include "text.h"
#define C(r,g,b) (0xff000000u|((unsigned int)(b)<<16)|((unsigned int)(g)<<8)|(r))

void player_menu_open(PlayerMenu *m) { *m=(PlayerMenu){0}; }
PlayerMenuAction player_menu_update(PlayerMenu *m,Party *party,Inventory *bag,GameOptions *options,const Input *input)
{
    int direction=input->vertical?input->vertical:input->horizontal;
    int edge=direction && direction!=m->previous_direction;
    m->previous_direction=direction;
    if (input->menu&INPUT_MENU_OPEN) return MENU_CLOSE;
    if (input->cancel) {
        if (m->page==MENU_HOME) return MENU_CLOSE;
        player_menu_open(m);return MENU_STAY;
    }
    if (input->menu&INPUT_MENU_LOAD) return MENU_LOAD;
    if (input->menu&INPUT_MENU_SAVE) return MENU_SAVE;
    int count=m->page==MENU_HOME?6:m->page==MENU_BAG?ITEM_COUNT:m->page==MENU_OPTIONS?3:1;
    if (edge) m->cursor=(m->cursor+direction+count)%count;
    if (!input->confirm) return MENU_STAY;
    if (m->page==MENU_HOME) {
        if (m->cursor==0) return MENU_PARTY;
        if (m->cursor==2) return MENU_COLLECTION;
        if (m->cursor==4) return MENU_SAVE;
        m->page=m->cursor==1?MENU_BAG:m->cursor==3?MENU_PLAYER:MENU_OPTIONS;
        m->cursor=0;
    } else if (m->page==MENU_BAG) {
        int healed=inventory_use_healing(bag,(ItemId)m->cursor,&party->members[party->lead]);
        if (healed) snprintf(m->message,sizeof(m->message),"%s RECOVERED %d HP.",creature_name(&party->members[party->lead]),healed);
        else snprintf(m->message,sizeof(m->message),"NO ITEM USED. CHECK STOCK AND YOUR LEAD'S HP. THE LODGE CAN REVIVE YOUR TEAM.");
    } else if (m->page==MENU_OPTIONS) {
        if (m->cursor==0) options->music=!options->music;
        else if (m->cursor==1) options->effects=!options->effects;
        else options->motion=!options->motion;
    }
    return MENU_STAY;
}
static void row(int y,const char *label,int selected)
{
    graphics_rectangle(252,y,208,25,selected?C(52,80,76):C(25,37,43));
    if (selected) graphics_rectangle(252,y,3,25,C(240,179,97));
    text_wrap(264,y+9,188,9,label,selected?C(255,216,155):C(180,199,193),1);
}
void player_menu_draw(const PlayerMenu *m,const Party *party,const Inventory *bag,const GameOptions *options,int map_id)
{
    char line[100];
    const Creature *lead=&party->members[party->lead];
    graphics_rectangle(0,0,480,272,C(16,26,32));
    graphics_rectangle(0,0,5,272,C(214,137,75));
    text_draw(20,15,"EMBERWAKE",C(246,213,158),2);
    text_draw(20,37,"WAYFARER'S FIELD KIT",C(145,182,174),1);
    graphics_rectangle(20,61,216,149,C(24,39,46));
    text_wrap(32,75,192,18,map_name(map_id),C(232,220,183),1);
    text_draw(32,105,"LEAD PARTNER",C(143,181,172),1);
    text_box(32,121,188,18,creature_name(lead),C(239,215,169));
    snprintf(line,sizeof(line),"LEVEL %d  HP %d/%d",lead->level,lead->hp,lead->max_hp);
    text_draw(32,148,line,C(191,212,199),1);
    snprintf(line,sizeof(line),"%d EMBERMARKS",bag->embermarks);
    text_draw(32,174,line,C(241,181,108),1);
    snprintf(line,sizeof(line),"TEAM %d/4   STORED %d/24",party->count,party->stored);
    text_draw(32,191,line,C(160,190,181),1);
    if (m->page==MENU_HOME) {
        const char *const names[]={"CREATURES","ITEMS","COLLECTION","PLAYER","SAVE","OPTIONS"};
        for (int i=0;i<6;++i) row(51+i*29,names[i],i==m->cursor);
        text_draw(20,233,"TAKE A MOMENT. YOUR JOURNEY IS PAUSED.",C(170,191,181),1);
    } else if (m->page==MENU_BAG) {
        text_draw(256,57,"ITEMS",C(244,198,118),2);
        for (int i=0;i<ITEM_COUNT;++i) {
            snprintf(line,sizeof(line),"%s  X%d",inventory_item_name((ItemId)i),bag->quantities[i]);
            row(86+i*31,line,i==m->cursor);
        }
        text_wrap(258,158,195,44,inventory_item_description((ItemId)m->cursor),C(171,202,188),1);
        text_wrap(20,221,438,23,m->message[0]?m->message:"X USE ON LEAD PARTNER. SET YOUR LEAD IN CREATURES TO HEAL SOMEONE ELSE.",C(226,211,175),1);
    } else if (m->page==MENU_OPTIONS) {
        text_draw(256,57,"OPTIONS",C(244,198,118),2);
        const int values[]={options->music,options->effects,options->motion};
        const char *const labels[]={"MUSIC","SOUND EFFECTS","ANIMATION"};
        for (int i=0;i<3;++i) {
            snprintf(line,sizeof(line),"%s  %s",labels[i],values[i]?"ON":"OFF");
            row(86+i*32,line,i==m->cursor);
        }
        text_draw(20,226,"X TOGGLE. OPTIONS APPLY TO THIS PLAY SESSION.",C(170,191,181),1);
    } else {
        text_draw(256,57,"PLAYER",C(244,198,118),2);
        text_wrap(258,90,195,100,"THE FERNVEIL JOURNEY\n\nTRAVEL EAST THROUGH THE WOODS TO REACH SUNTHREAD MARSH. SEEK TWO NEW VEYLINGS AMONG ITS REEDS.",C(191,212,199),1);
        text_draw(20,226,"START SAVE   L LOAD   O RETURN TO FIELD KIT",C(170,191,181),1);
    }
    graphics_rectangle(20,247,440,1,C(66,81,81));
    text_draw(20,256,"D-PAD SELECT   X CONFIRM   O BACK   TRI CLOSE",C(169,195,182),1);
}
