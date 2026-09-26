#ifndef EMBERWAKE_PLAYER_MENU_H
#define EMBERWAKE_PLAYER_MENU_H
#include "input.h"
#include "party.h"
#include "inventory.h"
typedef enum { MENU_HOME, MENU_BAG, MENU_PLAYER, MENU_OPTIONS } PlayerMenuPage;
typedef enum { MENU_STAY, MENU_CLOSE, MENU_PARTY, MENU_COLLECTION, MENU_MAP, MENU_SAVE, MENU_LOAD } PlayerMenuAction;
typedef struct {
    int music, effects, motion;
} GameOptions;
typedef struct {
    PlayerMenuPage page;
    int cursor, previous_direction;
    char message[120];
} PlayerMenu;
void player_menu_open(PlayerMenu *m);
PlayerMenuAction player_menu_update(PlayerMenu *m,Party *party,Inventory *bag,GameOptions *options,const Input *input);
void player_menu_draw(const PlayerMenu *m,const Party *party,const Inventory *bag,const GameOptions *options,int map_id);
#endif
