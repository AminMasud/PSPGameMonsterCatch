#ifndef EMBERWAKE_INVENTORY_H
#define EMBERWAKE_INVENTORY_H

#include "creature.h"

typedef enum {
    ITEM_PULSE_TONIC,
    ITEM_HEARTH_RESTORE,
    ITEM_COUNT
} ItemId;

typedef struct {
    int quantities[ITEM_COUNT];
    int embermarks;
} Inventory;

void inventory_init(Inventory *inventory);
const char *inventory_item_name(ItemId item);
const char *inventory_item_description(ItemId item);
int inventory_item_max(ItemId item);
int inventory_item_price(ItemId item);
int inventory_buy(Inventory *inventory, ItemId item);
int inventory_use_healing(Inventory *inventory, ItemId item, Creature *target);

#endif
