#include <stdio.h>
#include <pspgu.h>
#include "battle.h"
#include "graphics.h"
#include "text.h"
#define C(r,g,b) GU_RGBA(r,g,b,255)

static void creature_draw(int x,int y,const Battler *unit,int back)
{
    Element element=unit->element;
    int sprite=species_get(unit->species)->battle_sprite;
    unsigned int body=element==ELEMENT_EMBER?C(224,123,67):
        element==ELEMENT_GROVE?C(114,167,92):element==ELEMENT_STONE?C(141,149,174):C(155,140,208);
    graphics_rectangle(x-6,y+48,78,8,C(33,45,53));
    graphics_rectangle(x+4,y+18,52,32,body);
    graphics_rectangle(x+12,y+8,36,30,body);
    graphics_rectangle(x+8,y+45,12,10,body);
    graphics_rectangle(x+40,y+45,12,10,body);
    if(element==ELEMENT_EMBER) {
        graphics_rectangle(x+16,y,8,12,C(255,188,91));
        graphics_rectangle(x+33,y-5,8,17,C(255,188,91));
        graphics_rectangle(x+55,y+28,12,8,C(236,162,76));
    } else if(element==ELEMENT_GROVE) {
        graphics_rectangle(x+2,y+1,23,8,C(77,121,74));
        graphics_rectangle(x+34,y-4,22,8,C(168,194,96));
    } else if(element==ELEMENT_STONE) {
        graphics_rectangle(x+7,y+4,15,16,C(190,192,197));
        graphics_rectangle(x+39,y+3,17,19,C(108,115,145));
    } else {
        graphics_rectangle(x-14,y+15,24,12,C(185,169,226));
        graphics_rectangle(x+52,y+15,24,12,C(185,169,226));
    }
    /* Species references select small original silhouette details. */
    if(sprite==1) { /* Emberlyn: large mane and crown. */
        graphics_rectangle(x-2,y+18,8,25,C(245,171,80));
        graphics_rectangle(x+56,y+18,8,25,C(245,171,80));
        graphics_rectangle(x+24,y-9,9,17,C(255,210,104));
    } else if(sprite==3) { /* Mosshorn */
        graphics_rectangle(x+2,y-8,7,24,C(191,177,112));
        graphics_rectangle(x+49,y-8,7,24,C(191,177,112));
    } else if(sprite==7) { /* Flintaur */
        graphics_rectangle(x-6,y+16,14,30,C(107,121,151));
        graphics_rectangle(x+52,y+16,14,30,C(107,121,151));
    } else if(sprite==4) {
        graphics_rectangle(x+23,y-10,5,19,C(162,185,96));
    } else if(sprite==8) {
        graphics_rectangle(x+21,y-8,20,10,C(208,192,234));
    } else if(sprite==9) {
        graphics_rectangle(x+23,y-9,15,17,C(158,174,197));
    }
    if(!back) {
        graphics_rectangle(x+17,y+23,6,7,C(23,31,41));
        graphics_rectangle(x+38,y+23,6,7,C(23,31,41));
        graphics_rectangle(x+27,y+35,8,3,C(58,54,57));
    } else {
        graphics_rectangle(x+21,y+20,23,5,C(255,179,94));
    }
}
static void status(const Battler *unit,int x,int y,int width)
{
    char line[64];
    graphics_rectangle(x,y,width,58,C(25,35,45));
    snprintf(line,sizeof(line),"%s  LV %d",creature_name(unit),unit->level);
    text_draw(x+8,y+7,line,C(241,232,207),1);
    text_draw(x+8,y+20,element_name(unit->element),C(174,192,188),1);
    int bar_width=width-16;
    graphics_rectangle(x+8,y+32,bar_width,6,C(66,72,78));
    int filled=bar_width*unit->hp/unit->max_hp;
    if(filled>0) graphics_rectangle(x+8,y+32,filled,6,
        unit->hp*4<=unit->max_hp?C(228,121,99):C(125,201,154));
    snprintf(line,sizeof(line),"HP %d / %d",unit->hp,unit->max_hp);
    text_draw(x+8,y+44,line,C(224,227,218),1);
}
void battle_draw(const Battle *b)
{
    graphics_rectangle(0,0,480,272,C(48,65,74));
    graphics_rectangle(0,88,480,88,C(65,81,77));
    graphics_rectangle(0,0,480,15,C(19,28,36));
    text_draw(10,4,"WILD VEYLING ENCOUNTER",C(241,204,145),1);
    creature_draw(325,38,&b->enemy,0);
    creature_draw(65,104,&b->ally,1);
    status(&b->enemy,18,24,202);
    status(&b->ally,253,109,210);
    graphics_rectangle(6,176,468,90,C(177,144,94));
    graphics_rectangle(8,178,464,86,C(21,30,38));
    if(b->phase==BATTLE_LEARN) {
        const Attack *move=attack_get(b->growth.moves[b->growth_move]);
        text_draw(18,188,"LEARN A NEW ATTACK?",C(244,198,118),1);
        text_draw(18,205,move->name,C(236,236,218),1);
        char detail[64];
        snprintf(detail,sizeof(detail),"POWER %d  ACCURACY %d",move->power,move->accuracy);
        text_draw(18,220,detail,C(188,204,190),1);
        text_draw(18,239,"X REPLACE   O DECLINE",C(188,204,190),1);
        for(int i=0;i<5;++i) {
            if(i==b->learn_cursor) graphics_rectangle(290,182+i*15,174,14,C(79,92,86));
            text_draw(296,186+i*15,i==4?"KEEP CURRENT MOVES":attack_get(b->ally.moves[i])->name,C(239,227,200),1);
        }
    } else if(b->phase==BATTLE_MESSAGE || b->phase==BATTLE_DONE) {
        text_draw(18,188,b->message,C(236,236,218),2);
        text_draw(18,251,"X CONTINUE",C(167,194,180),1);
    } else if(b->phase==BATTLE_MENU) {
        text_draw(18,191,"CHOOSE YOUR NEXT MOVE.",C(236,236,218),1);
        text_draw(18,210,"D-PAD SELECT   X CONFIRM",C(167,194,180),1);
        char growth[64];
        snprintf(growth,sizeof(growth),"%s - NEXT LEVEL IN %d XP",creature_name(&b->ally),creature_xp_remaining(&b->ally));
        text_draw(18,230,growth,C(233,173,115),1);
        const char *const options[]={"FIGHT","CAPTURE","CREATURES","ITEMS","RUN"};
        for(int i=0;i<5;++i) {
            if(i==b->cursor) graphics_rectangle(302,182+i*15,158,14,C(79,92,86));
            text_draw(310,186+i*15,options[i],i==b->cursor?C(255,213,147):C(187,193,193),1);
        }
    } else {
        int available=0;
        for(int i=0;i<4;++i) available+=b->ally.uses[i];
        if(!available) {
            text_draw(18,190,"ALL ATTACKS ARE SPENT.",C(236,236,218),1);
            text_draw(18,211,"X PRESS ON - WEAK BUT UNLIMITED",C(236,236,218),1);
        } else {
            for(int i=0;i<4;++i) {
                char line[64];
                const Attack *move=attack_get(b->ally.moves[i]);
                if(i==b->move_cursor) graphics_rectangle(14,183+i*15,261,14,C(79,92,86));
                snprintf(line,sizeof(line),"%s  %d/%d",b->ally.moves[i]<0?"EMPTY":move->name,b->ally.uses[i],b->ally.moves[i]<0?0:move->uses);
                text_draw(20,187+i*15,line,b->ally.uses[i]?C(239,227,200):C(155,155,155),1);
            }
            const Attack *selected=attack_get(b->ally.moves[b->move_cursor]);
            char line[40];
            text_draw(295,190,element_name(selected->element),C(245,185,112),1);
            snprintf(line,sizeof(line),"POWER %d",selected->power);
            text_draw(295,208,line,C(220,225,215),1);
            snprintf(line,sizeof(line),"ACCURACY %d/100",selected->accuracy);
            text_draw(295,226,line,C(220,225,215),1);
        }
        text_draw(18,251,"X ATTACK   O BACK",C(167,194,180),1);
    }
}
