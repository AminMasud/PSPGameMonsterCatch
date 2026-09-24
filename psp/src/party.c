#include <string.h>
#include "party.h"

static int valid(const Party *p)
{
    return p && p->count>=1 && p->count<=PARTY_MAX &&
        p->stored>=0 && p->stored<=COLLECTION_MAX;
}

/* Retain the selected slot when usable. An all-fainted party still has a valid lead. */
static void repair_lead(Party *p)
{
    if(p->lead<0 || p->lead>=p->count) p->lead=0;
    if(p->members[p->lead].hp>0) return;
    for(int i=0;i<p->count;++i) {
        if(p->members[i].hp>0) { p->lead=i;return; }
    }
}

void party_init(Party *p)
{
    if(!p) return;
    memset(p,0,sizeof(*p));
    creature_create(&p->members[0],SPECIES_CINDLET,5);
    p->count=1;
}

int party_has_space(const Party *p)
{
    return valid(p) && (p->count<PARTY_MAX || p->stored<COLLECTION_MAX);
}

int party_add(Party *p,const Creature *c)
{
    if(!c || !party_has_space(p)) return 0;
    if(p->count<PARTY_MAX) {
        p->members[p->count]=*c;
        ++p->count;
        repair_lead(p);
        return 1;
    }
    p->collection[p->stored]=*c;
    ++p->stored;
    return 2;
}

void party_restore(Party *p)
{
    if(!valid(p)) return;
    for(int i=0;i<p->count;++i) creature_restore(&p->members[i]);
    for(int i=0;i<p->stored;++i) creature_restore(&p->collection[i]);
    repair_lead(p);
}

int party_set_lead(Party *p,int index)
{
    if(!valid(p) || index<0 || index>=p->count || p->members[index].hp<=0) return 0;
    p->lead=index;
    return 1;
}

int party_exchange(Party *p,int stored_index,int party_index)
{
    if(!valid(p) || stored_index<0 || stored_index>=p->stored ||
       party_index<0 || party_index>p->count || party_index>=PARTY_MAX) return 0;
    Creature withdrawn=p->collection[stored_index];
    if(party_index<p->count) {
        p->collection[stored_index]=p->members[party_index];
        p->members[party_index]=withdrawn;
    } else {
        p->members[p->count++]=withdrawn;
        --p->stored;
        for(int i=stored_index;i<p->stored;++i) p->collection[i]=p->collection[i+1];
        memset(&p->collection[p->stored],0,sizeof(Creature));
    }
    repair_lead(p);
    return 1;
}

int party_deposit(Party *p,int index)
{
    if(!valid(p) || p->count<=1 || p->stored>=COLLECTION_MAX || index<0 || index>=p->count) return 0;
    p->collection[p->stored++]=p->members[index];
    --p->count;
    for(int i=index;i<p->count;++i) p->members[i]=p->members[i+1];
    memset(&p->members[p->count],0,sizeof(Creature));
    if(p->lead>index) --p->lead;
    else if(p->lead==index) p->lead=0;
    repair_lead(p);
    return 1;
}
