#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <psputility_savedata.h>
#include "save_data.h"

static SceUtilitySavedataParam *active;
static int state, shutdowns, updates, init_result, shutdown_result;

int sceUtilitySavedataInitStart(SceUtilitySavedataParam *params)
{
    active=params;
    state=PSP_UTILITY_DIALOG_INIT;
    shutdowns=updates=0;
    return init_result;
}
int sceUtilitySavedataGetStatus(void) { return state; }
int sceUtilitySavedataShutdownStart(void)
{
    ++shutdowns;
    return shutdown_result;
}
void sceUtilitySavedataUpdate(int step) { assert(step==1); ++updates; }

static void tick(int next)
{
    state=next;
    save_data_update();
}
static void complete(int result, int see_finished)
{
    active->base.result=result;
    tick(PSP_UTILITY_DIALOG_QUIT);
    assert(save_data_status()==SAVE_STATUS_BUSY && shutdowns==1);
    tick(PSP_UTILITY_DIALOG_QUIT);
    assert(shutdowns==1);
    if (see_finished) {
        tick(PSP_UTILITY_DIALOG_FINISHED);
        assert(save_data_status()==SAVE_STATUS_BUSY && shutdowns==1);
    }
    tick(PSP_UTILITY_DIALOG_NONE);
}

static void put32(unsigned char *bytes,int offset,int value)
{
    int32_t word=value;
    memcpy(bytes+offset,&word,sizeof(word));
}
static void migration_checks(void)
{
    /* Build the old layout by byte offsets, independently of the decoder's
       legacy structs and today's larger collection. */
    unsigned char bytes[1960]={0},original[1960];
    const int replacements[]={0,1,6,7,27,29,12,13,15,14,24,21};
    const int hp[]={24,35,25,39,20,22,27,43,20,33,26,25};
    put32(bytes,0,SAVE_DATA_MAGIC);put32(bytes,4,1);
    put32(bytes,8,4);put32(bytes,12,2);put32(bytes,16,10);
    put32(bytes,24,123);put32(bytes,28,3);
    put32(bytes,1936,4);put32(bytes,1940,2);put32(bytes,1944,24);
    put32(bytes,1948,2);put32(bytes,1952,1);put32(bytes,1956,321);
    for(int slot=0;slot<28;++slot) {
        int offset=32+slot*68,id=slot%12;
        put32(bytes,offset,id);put32(bytes,offset+4,7);
        put32(bytes,offset+8,creature_xp_for_level(7)+20);
        put32(bytes,offset+12,slot==0?0:hp[id]+35-3);
        memcpy(bytes+offset+16,"FRIEND",7);
        for(int move=0;move<4;++move) {
            put32(bytes,offset+36+move*4,move);
            put32(bytes,offset+52+move*4,attack_get(move)->uses-1);
        }
    }
    memcpy(original,bytes,sizeof(bytes));
    SavePayload loaded;
    assert(save_data_decode(bytes,sizeof(bytes),&loaded));
    assert(!memcmp(bytes,original,sizeof(bytes)));
    assert(loaded.version==3 && loaded.party.count==4 && loaded.party.stored==24 && loaded.party.lead==2);
    assert(loaded.npc_defeated==0 && loaded.progression_flags==0);
    assert(loaded.map_id==4 && loaded.tile_x==2 && loaded.tile_y==10 && loaded.embermarks==321);
    assert(loaded.encounter_random==123 && loaded.encounter_safe_steps==3 && loaded.item_quantities[0]==2);
    for(int slot=0;slot<28;++slot) {
        SaveCreature *pet=slot<4?&loaded.party.members[slot]:&loaded.party.collection[slot-4];
        assert(pet->species==replacements[slot%12] && pet->level==7);
        assert(pet->hp==(slot==0?0:species_get(pet->species)->base_hp+35-3));
        assert(!strcmp(pet->nickname,"FRIEND") && pet->experience==creature_xp_for_level(7)+20);
        for(int move=0;move<4;++move) assert(pet->moves[move]==move && pet->uses[move]==attack_get(move)->uses-1);
    }
    for(int i=24;i<COLLECTION_MAX;++i) assert(loaded.party.collection[i].nickname[0]==0);
    assert(save_data_begin_load()==0);
    memcpy(active->dataBuf,bytes,sizeof(bytes));active->dataSize=sizeof(bytes);
    complete(0,1);
    assert(save_data_status()==SAVE_STATUS_SUCCEEDED && save_data_take_loaded(&loaded));
    assert(loaded.version==3 && loaded.party.stored==24);
    for(int level=20;level<=100;level+=80) {
        memcpy(bytes,original,sizeof(bytes));
        /* Old Emberlyn at a high level becomes the eligible final form. */
        put32(bytes,104,level);put32(bytes,108,creature_xp_for_level(level));
        put32(bytes,112,35+5*level-3);
        assert(save_data_decode(bytes,sizeof(bytes),&loaded));
        assert(loaded.party.members[1].species==SPECIES_PYROVERN);
        assert(loaded.party.members[1].hp==50+5*level-3);
    }
    memcpy(bytes,original,sizeof(bytes));
    SavePayload unchanged=loaded;
    put32(bytes,32,12);
    assert(!save_data_decode(bytes,sizeof(bytes),&loaded));
    assert(!memcmp(&loaded,&unchanged,sizeof(loaded)));
    memcpy(bytes,original,sizeof(bytes));put32(bytes,1944,25);
    assert(!save_data_decode(bytes,sizeof(bytes),&loaded));
    memcpy(bytes,original,sizeof(bytes));put32(bytes,4,99);
    assert(!save_data_decode(bytes,sizeof(bytes),&loaded));
    assert(!save_data_decode(original,sizeof(original)-1,&loaded));
    assert(!save_data_decode(original,4,&loaded));
    assert(save_data_decode(&unchanged,sizeof(unchanged),&loaded));
    assert(!memcmp(&loaded,&unchanged,sizeof(loaded)));
}
static void phase2_migration_checks(void)
{
    SavePayload source={.magic=SAVE_DATA_MAGIC,.version=2,.map_id=4,.tile_x=8,
                        .tile_y=9,.encounter_random=77,.embermarks=654};
    unsigned char bytes[2504];
    memcpy(bytes,&source,sizeof(bytes));
    SavePayload loaded;
    assert(save_data_decode(bytes,sizeof(bytes),&loaded));
    assert(loaded.version==SAVE_DATA_VERSION && loaded.map_id==4 && loaded.tile_x==8 &&
           loaded.tile_y==9 && loaded.encounter_random==77 && loaded.embermarks==654);
    assert(loaded.npc_defeated==0 && loaded.progression_flags==0);
    SavePayload unchanged=loaded;
    assert(!save_data_decode(bytes,sizeof(bytes)-1,&loaded));
    assert(!memcmp(&loaded,&unchanged,sizeof(loaded)));
}

int main(void)
{
    assert(sizeof(SavePayload)==2512 && SAVE_DATA_VERSION==3);
    assert(SPECIES_CINDLET==0 && SPECIES_LUNARAE==29);
    SavePayload saved={.magic=SAVE_DATA_MAGIC, .version=SAVE_DATA_VERSION,
                       .map_id=1, .tile_x=7, .embermarks=123,
                       .npc_defeated=(1u<<3)|(1u<<17),.progression_flags=1u<<9};
    SavePayload loaded;
    assert(save_data_status()==SAVE_STATUS_IDLE);
    assert(save_data_begin_save(NULL)<0);
    for (int finished=0;finished<=1;++finished) {
        assert(save_data_begin_save(&saved)==0);
        assert(active->mode==PSP_UTILITY_SAVEDATA_LISTSAVE);
        assert(!strcmp(active->gameName,"EMBRWAKE") && !strcmp(active->saveName,"0000"));
        tick(PSP_UTILITY_DIALOG_NONE); /* Initial idle poll is not completion. */
        assert(save_data_status()==SAVE_STATUS_BUSY);
        tick(PSP_UTILITY_DIALOG_VISIBLE);
        assert(updates==1);
        /* Rejected overlapping operations must not alter the in-flight data. */
        assert(save_data_begin_load()<0);
        assert(save_data_begin_save(&(SavePayload){0})<0);
        assert(memcmp(active->dataBuf,&saved,sizeof(saved))==0);
        complete(0,finished);
        assert(save_data_status()==SAVE_STATUS_SUCCEEDED);
        assert(!save_data_take_loaded(&loaded));
    }
    /* Canceled and failed operations must never report success. */
    const int failures[]={1, -1, (int)0x80110383u};
    for (unsigned int i=0;i<sizeof(failures)/sizeof(failures[0]);++i) {
        assert(save_data_begin_save(&saved)==0);
        complete(failures[i],i%2);
        assert(save_data_status()==SAVE_STATUS_FAILED);
    }
    /* The final result can be written during shutdown, before NONE is seen. */
    assert(save_data_begin_save(&saved)==0);
    tick(PSP_UTILITY_DIALOG_QUIT);
    active->base.result=0;
    tick(PSP_UTILITY_DIALOG_NONE);
    assert(save_data_status()==SAVE_STATUS_SUCCEEDED);

    assert(save_data_begin_load()==0);
    assert(active->mode==PSP_UTILITY_SAVEDATA_LISTLOAD && active->dataSize==0);
    memcpy(active->dataBuf,&saved,sizeof(saved));
    active->dataSize=sizeof(saved);
    complete(0,0);
    assert(save_data_status()==SAVE_STATUS_SUCCEEDED);
    assert(save_data_take_loaded(&loaded) && memcmp(&saved,&loaded,sizeof(saved))==0);
    assert(!save_data_take_loaded(&loaded));
    /* Reject a truncated file even when its magic and version are intact. */
    assert(save_data_begin_load()==0);
    memcpy(active->dataBuf,&saved,sizeof(saved));
    active->dataSize=sizeof(saved)-1;
    complete(0,1);
    assert(save_data_status()==SAVE_STATUS_FAILED && !save_data_take_loaded(&loaded));
    assert(save_data_begin_load()==0);
    memcpy(active->dataBuf,&saved,sizeof(saved));
    ((SavePayload *)active->dataBuf)->magic=0;
    active->dataSize=sizeof(saved);
    complete(0,0);
    assert(save_data_status()==SAVE_STATUS_FAILED && !save_data_take_loaded(&loaded));
    /* Retry shutdown if the utility is temporarily not ready. */
    assert(save_data_begin_save(&saved)==0);
    shutdown_result=-1;
    tick(PSP_UTILITY_DIALOG_QUIT);
    assert(save_data_status()==SAVE_STATUS_BUSY);
    shutdown_result=0;
    active->base.result=0;
    tick(PSP_UTILITY_DIALOG_QUIT);
    assert(shutdowns==2);
    tick(PSP_UTILITY_DIALOG_NONE);
    assert(save_data_status()==SAVE_STATUS_SUCCEEDED);
    migration_checks();
    phase2_migration_checks();
    init_result=-1;
    assert(save_data_begin_load()<0 && save_data_status()==SAVE_STATUS_FAILED);
    puts("PASS: savedata lifecycle, v3 roundtrip, v1/v2 migration, old ID remapping, NPC progress, corrupt save rejection");
    return 0;
}
