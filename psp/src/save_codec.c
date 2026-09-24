#include <string.h>
#include "save_data.h"

/* Frozen Phase 8/9 layout. Never use current capacity constants here. */
typedef struct {
    SaveCreature members[4],collection[24];
    int count,lead,stored;
} LegacyParty;
typedef struct {
    uint32_t magic,version;
    int map_id,tile_x,tile_y,facing;
    uint32_t encounter_random;
    int encounter_safe_steps;
    LegacyParty party;
    int item_quantities[2],embermarks;
} LegacyPayload;
typedef char legacy_size_must_remain_1960[(sizeof(LegacyPayload)==1960)?1:-1];
typedef char current_size_must_remain_2504[(sizeof(SavePayload)==2504)?1:-1];

static int migrate_creature(SaveCreature *out,const SaveCreature *old)
{
    /* Replacement choices preserve similar silhouettes/elements where possible. */
    static const SpeciesId replacement[]={
        SPECIES_CINDLET,SPECIES_EMBERYN,SPECIES_MOSSPRIG,SPECIES_THORNEL,
        SPECIES_GLIMGRUB,SPECIES_LUNARAE,SPECIES_GRUBBL,SPECIES_CRAGBEET,
        SPECIES_VEILFIN,SPECIES_TITANOCERA,SPECIES_TOXLET,SPECIES_GUSTLET
    };
    static const int old_base_hp[]={24,35,25,39,20,22,27,43,20,33,26,25};
    if(old->species<0 || old->species>=12 || old->level<1 || old->level>100 ||
       old->experience<creature_xp_for_level(old->level) ||
       old->experience>creature_xp_for_level(100) ||
       !memchr(old->nickname,0,sizeof(old->nickname))) return 0;
    int old_max=old_base_hp[old->species]+5*old->level;
    if(old->hp<0 || old->hp>old_max) return 0;
    for(int i=0;i<4;++i) {
        if(old->moves[i]<-1 || old->moves[i]>=11 || old->uses[i]<0 ||
           (old->moves[i]<0 && old->uses[i]!=0) ||
           (old->moves[i]>=0 && old->uses[i]>attack_get(old->moves[i])->uses)) return 0;
    }
    *out=*old;
    out->species=replacement[old->species];
    /* Existing high-level partners already qualify for their new forms,
       including level-100 saves that cannot trigger another level-up. */
    const Species *form=species_get(out->species);
    while(form->evolution_level && out->level>=form->evolution_level) {
        out->species=form->evolved_species;form=species_get(out->species);
    }
    int new_max=species_get(out->species)->base_hp+5*out->level;
    out->hp=old->hp?new_max-(old_max-old->hp):0;
    if(old->hp && out->hp<1) out->hp=1;
    return 1;
}

int save_data_decode(const void *bytes,size_t size,SavePayload *payload)
{
    uint32_t header[2];
    if(!bytes || !payload || size<sizeof(header)) return 0;
    memcpy(header,bytes,sizeof(header));
    if(header[0]!=SAVE_DATA_MAGIC) return 0;
    if(header[1]==SAVE_DATA_VERSION) {
        if(size!=sizeof(*payload)) return 0;
        memcpy(payload,bytes,size);
        return 1; /* game.c validates current gameplay values before applying. */
    }
    if(header[1]!=1 || size!=sizeof(LegacyPayload)) return 0;
    LegacyPayload old;
    SavePayload next={0};
    memcpy(&old,bytes,sizeof(old));
    if(old.party.count<1 || old.party.count>4 || old.party.lead<0 ||
       old.party.lead>=old.party.count || old.party.stored<0 || old.party.stored>24) return 0;
    next.magic=SAVE_DATA_MAGIC;next.version=SAVE_DATA_VERSION;
    next.map_id=old.map_id;next.tile_x=old.tile_x;next.tile_y=old.tile_y;next.facing=old.facing;
    next.encounter_random=old.encounter_random;next.encounter_safe_steps=old.encounter_safe_steps;
    next.party.count=old.party.count;next.party.lead=old.party.lead;next.party.stored=old.party.stored;
    for(int i=0;i<old.party.count;++i)
        if(!migrate_creature(&next.party.members[i],&old.party.members[i])) return 0;
    for(int i=0;i<old.party.stored;++i)
        if(!migrate_creature(&next.party.collection[i],&old.party.collection[i])) return 0;
    for(int i=0;i<2;++i) next.item_quantities[i]=old.item_quantities[i];
    next.embermarks=old.embermarks;
    *payload=next;
    return 1;
}
