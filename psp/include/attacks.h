#ifndef EMBERWAKE_ATTACKS_H
#define EMBERWAKE_ATTACKS_H
typedef enum { ELEMENT_PLAIN, ELEMENT_GROVE, ELEMENT_EMBER, ELEMENT_STONE, ELEMENT_WIND } Element;
typedef enum { MOVE_NUDGE, MOVE_CINDER, MOVE_LEAF, MOVE_PEBBLE, MOVE_GUST, MOVE_LUNGE,
               MOVE_HEAT, MOVE_FLARE, MOVE_VINE, MOVE_FAULT, MOVE_TEMPEST, MOVE_COUNT } MoveId;
typedef struct { const char *name; int power, accuracy; Element element; int uses; } Attack;
const Attack *attack_get(int id);
const char *element_name(Element element);
int attack_effectiveness(Element attack, Element defender); /* 1 half, 2 normal, 4 double */
#endif
