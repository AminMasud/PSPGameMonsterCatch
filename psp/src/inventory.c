#include "inventory.h"

void inventory_init(Inventory *inventory)
{
    if (!inventory) return;
    *inventory = (Inventory){{3, 1}, 250};
}

const char *inventory_item_name(ItemId item)
{
    switch (item) {
    case ITEM_PULSE_TONIC: return "PULSE TONIC";
    case ITEM_HEARTH_RESTORE: return "HEARTH RESTORE";
    default: return "UNKNOWN ITEM";
    }
}

const char *inventory_item_description(ItemId item)
{
    switch (item) {
    case ITEM_PULSE_TONIC: return "RESTORES 25 HP.";
    case ITEM_HEARTH_RESTORE: return "RESTORES THE FULL HP OF A VEYLING.";
    default: return "NO EFFECT.";
    }
}

int inventory_item_max(ItemId item)
{
    switch (item) {
    case ITEM_PULSE_TONIC: return 99;
    case ITEM_HEARTH_RESTORE: return 20;
    default: return 0;
    }
}

int inventory_item_price(ItemId item)
{
    switch (item) {
    case ITEM_PULSE_TONIC: return 50;
    case ITEM_HEARTH_RESTORE: return 120;
    default: return 0;
    }
}

int inventory_buy(Inventory *inventory, ItemId item)
{
    int price=inventory_item_price(item), maximum=inventory_item_max(item);
    if (!inventory || item < 0 || item >= ITEM_COUNT || !price ||
        inventory->quantities[item] >= maximum || inventory->embermarks < price)
        return 0;
    inventory->embermarks-=price;
    ++inventory->quantities[item];
    return 1;
}

int inventory_use_healing(Inventory *inventory, ItemId item, Creature *target)
{
    int before;
    if (!inventory || !target || item < 0 || item >= ITEM_COUNT ||
        inventory->quantities[item] <= 0 || target->hp <= 0 || target->hp >= target->max_hp)
        return 0;
    before = target->hp;
    if (item == ITEM_PULSE_TONIC) target->hp += 25;
    else if (item == ITEM_HEARTH_RESTORE) target->hp = target->max_hp;
    else return 0;
    if (target->hp > target->max_hp) target->hp = target->max_hp;
    if (target->hp == before) return 0;
    --inventory->quantities[item];
    return target->hp - before;
}
