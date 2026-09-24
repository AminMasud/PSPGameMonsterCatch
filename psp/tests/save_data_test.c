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

int main(void)
{
    /* Phase 8 format/IDs remain unchanged; Phase 9 only appends content IDs. */
    assert(sizeof(SavePayload)==1960 && SAVE_DATA_VERSION==1);
    assert(SPECIES_CINDLET==0 && SPECIES_ECHOCRAG==9);
    SavePayload saved={.magic=SAVE_DATA_MAGIC, .version=SAVE_DATA_VERSION,
                       .map_id=1, .tile_x=7, .embermarks=123};
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
    init_result=-1;
    assert(save_data_begin_load()<0 && save_data_status()==SAVE_STATUS_FAILED);
    puts("PASS: savedata completion with/without FINISHED, errors, cancellation, load validation, busy guard");
    return 0;
}
