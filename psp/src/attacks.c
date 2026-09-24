#include "attacks.h"
static const Attack attacks[MOVE_COUNT] = {
    {"NUDGE",28,100,ELEMENT_PLAIN,24},
    {"CINDER ARC",40,95,ELEMENT_EMBER,12},
    {"LEAF LASH",40,95,ELEMENT_GROVE,12},
    {"PEBBLE BURST",42,90,ELEMENT_STONE,12},
    {"WHIRL CUT",38,100,ELEMENT_WIND,12},
    {"BOLD LUNGE",55,75,ELEMENT_PLAIN,8},
    {"HEAT SPIRAL",55,95,ELEMENT_EMBER,10},
    {"FLARE CREST",70,90,ELEMENT_EMBER,8},
    {"VINE SWEEP",58,95,ELEMENT_GROVE,10},
    {"FAULT DRUM",62,90,ELEMENT_STONE,10},
    {"TEMPEST THREAD",56,100,ELEMENT_WIND,10},
    {"BUBBLE JET",40,95,ELEMENT_TIDE,12},
    {"TIDAL SURGE",65,90,ELEMENT_TIDE,8},
    {"SPARK FLICK",38,100,ELEMENT_SPARK,12},
    {"VOLT COIL",62,95,ELEMENT_SPARK,8},
    {"VEIL RIPPLE",40,95,ELEMENT_VEIL,12},
    {"ABYSS PULSE",65,90,ELEMENT_VEIL,8},
    {"FROST CHIP",40,95,ELEMENT_FROST,12},
    {"GLACIER CALL",62,95,ELEMENT_FROST,8},
    {"TOXIN DART",38,100,ELEMENT_TOXIN,12},
    {"VENOM RUSH",62,95,ELEMENT_TOXIN,8}
};
const Attack *attack_get(int id)
{
    return &attacks[id>=0 && id<MOVE_COUNT ? id : MOVE_NUDGE];
}
const char *element_name(Element element)
{
    const char *const names[]={"PLAIN","GROVE","EMBER","STONE","WIND","TIDE","SPARK","VEIL","FROST","TOXIN"};
    return names[element>=ELEMENT_PLAIN && element<ELEMENT_COUNT ? element : ELEMENT_PLAIN];
}
int attack_effectiveness(Element a,Element d)
{
    if(a<=ELEMENT_PLAIN || d<=ELEMENT_PLAIN || a>=ELEMENT_COUNT || d>=ELEMENT_COUNT) return 2;
    const Element strong[][2]={
        {ELEMENT_EMBER,ELEMENT_GROVE},{ELEMENT_GROVE,ELEMENT_STONE},
        {ELEMENT_STONE,ELEMENT_WIND},{ELEMENT_WIND,ELEMENT_EMBER},
        {ELEMENT_TIDE,ELEMENT_EMBER},{ELEMENT_SPARK,ELEMENT_TIDE},
        {ELEMENT_GROVE,ELEMENT_TIDE},{ELEMENT_STONE,ELEMENT_SPARK},
        {ELEMENT_EMBER,ELEMENT_FROST},{ELEMENT_FROST,ELEMENT_WIND},
        {ELEMENT_VEIL,ELEMENT_TOXIN},{ELEMENT_TOXIN,ELEMENT_GROVE},
        {ELEMENT_WIND,ELEMENT_TOXIN}
    };
    if(a==d) return 1;
    for(unsigned int i=0;i<sizeof(strong)/sizeof(strong[0]);++i) {
        if(a==strong[i][0] && d==strong[i][1]) return 4;
        if(d==strong[i][0] && a==strong[i][1]) return 1;
    }
    return 2;
}
