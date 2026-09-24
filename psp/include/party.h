#ifndef EMBERWAKE_PARTY_H
#define EMBERWAKE_PARTY_H
#include "creature.h"

#define PARTY_MAX 4
#define COLLECTION_MAX 24

typedef struct {
    Creature members[PARTY_MAX];
    int count, lead;
    Creature collection[COLLECTION_MAX];
    int stored;
} Party;

void party_init(Party *party);
/* Copies the complete creature unchanged; 1 = party, 2 = collection, 0 = full/invalid. */
int party_add(Party *party,const Creature *creature);
int party_has_space(const Party *party);
void party_restore(Party *party);
int party_set_lead(Party *party,int index);
/* Swap with a party slot, or withdraw when party_index == count and count < PARTY_MAX. */
int party_exchange(Party *party,int stored_index,int party_index);
/* Keep at least one party member; no operation below heals or replenishes moves. */
int party_deposit(Party *party,int index);
#endif
