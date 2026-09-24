#include <assert.h>
#include <stdio.h>
#include "inventory.h"

int main(void)
{
    Inventory bag;
    Creature c;
    inventory_init(&bag);
    creature_create(&c,SPECIES_CINDLET,5);
    c.hp=10;
    assert(bag.quantities[ITEM_PULSE_TONIC]==3 && bag.quantities[ITEM_HEARTH_RESTORE]==1 && bag.embermarks==250);
    assert(inventory_buy(&bag,ITEM_PULSE_TONIC) && bag.quantities[ITEM_PULSE_TONIC]==4 && bag.embermarks==200);
    assert(inventory_use_healing(&bag,ITEM_PULSE_TONIC,&c)==25 && c.hp==35);
    assert(bag.quantities[ITEM_PULSE_TONIC]==3);
    c.hp=1;
    assert(inventory_use_healing(&bag,ITEM_HEARTH_RESTORE,&c)==c.max_hp-1 && c.hp==c.max_hp);
    assert(bag.quantities[ITEM_HEARTH_RESTORE]==0);
    assert(!inventory_use_healing(&bag,ITEM_HEARTH_RESTORE,&c));
    c.hp=0;
    assert(!inventory_use_healing(&bag,ITEM_PULSE_TONIC,&c));
    puts("PASS: inventory defaults, capped tonic healing, full restore, and invalid item use");
    return 0;
}
