#include "attacks.h"
static const Attack attacks[MOVE_COUNT] = {
    {"NUDGE",28,100,ELEMENT_PLAIN,24},
    {"CINDER ARC",40,95,ELEMENT_EMBER,12},
    {"LEAF LASH",40,95,ELEMENT_GROVE,12},
    {"PEBBLE BURST",42,90,ELEMENT_STONE,12},
    {"WHIRL CUT",38,100,ELEMENT_WIND,12},
    {"BOLD LUNGE",55,75,ELEMENT_PLAIN,8}
};
const Attack *attack_get(int id)
{
    return &attacks[id>=0 && id<MOVE_COUNT ? id : MOVE_NUDGE];
}
const char *element_name(Element element)
{
    const char *const names[]={"PLAIN","GROVE","EMBER","STONE","WIND"};
    return names[element>=ELEMENT_PLAIN && element<=ELEMENT_WIND ? element : ELEMENT_PLAIN];
}
int attack_effectiveness(Element a,Element d)
{
    if(a==ELEMENT_PLAIN || d==ELEMENT_PLAIN) return 2;
    if((a==ELEMENT_EMBER && d==ELEMENT_GROVE) ||
       (a==ELEMENT_GROVE && d==ELEMENT_STONE) ||
       (a==ELEMENT_STONE && d==ELEMENT_WIND) ||
       (a==ELEMENT_WIND && d==ELEMENT_EMBER)) return 4;
    if(a==d || (d==ELEMENT_EMBER && a==ELEMENT_GROVE) ||
       (d==ELEMENT_GROVE && a==ELEMENT_STONE) ||
       (d==ELEMENT_STONE && a==ELEMENT_WIND) ||
       (d==ELEMENT_WIND && a==ELEMENT_EMBER)) return 1;
    return 2;
}
