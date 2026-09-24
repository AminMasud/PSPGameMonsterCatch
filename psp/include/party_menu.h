#ifndef EMBERWAKE_PARTY_MENU_H
#define EMBERWAKE_PARTY_MENU_H

#include "input.h"
#include "party.h"

typedef enum { PARTY_MENU_BROWSE, PARTY_MENU_ACTION, PARTY_MENU_SWAP } PartyMenuMode;

typedef struct {
    int tab;
    int cursor[2], scroll[2];
    int action, swap;
    int previous_horizontal, previous_vertical;
    PartyMenuMode mode;
    char message[112];
} PartyMenu;

void party_menu_open(PartyMenu *menu);
/* Returns one while open, zero when Circle/Triangle closes the menu. */
int party_menu_update(PartyMenu *menu, Party *party, const Input *input);
void party_menu_draw(const PartyMenu *menu, const Party *party);
void party_menu_draw_battle(const Party *party,int active,int cursor,int forced,
                            const char *message);

#endif
