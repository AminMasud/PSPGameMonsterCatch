#include <psputility.h>
#include <psputility_savedata.h>
#include <psputility_sysparam.h>
#include <stdio.h>
#include <string.h>
#include "save_data.h"

#define SAVE_GAME_NAME "EMBRWAKE"
#define SAVE_NAME "0000"
#define SAVE_FILE "DATA.BIN"

/* 144 x 80 PNG used for the new-slot preview and saved entry. */
static unsigned char save_icon0[] __attribute__((aligned(16))) = {
    0x89,0x50,0x4E,0x47,0x0D,0x0A,0x1A,0x0A,0x00,0x00,0x00,0x0D,0x49,0x48,0x44,0x52,
    0x00,0x00,0x00,0x90,0x00,0x00,0x00,0x50,0x08,0x02,0x00,0x00,0x00,0x79,0xCC,0x6B,
    0x5B,0x00,0x00,0x00,0x8F,0x49,0x44,0x41,0x54,0x78,0xDA,0xED,0xD1,0x01,0x09,0x00,
    0x00,0x08,0x04,0xB1,0x0F,0x61,0x3A,0xFB,0xF7,0xD0,0x1A,0x0A,0x83,0x4B,0x70,0xCB,
    0x74,0xE9,0x51,0xB1,0x00,0x98,0x80,0x09,0x18,0x30,0x01,0x13,0x30,0x60,0x02,0x26,
    0x60,0xC0,0x04,0x4C,0xC0,0x80,0x09,0x98,0x80,0x01,0x13,0x30,0x01,0x03,0x26,0x60,
    0x02,0x06,0x4C,0xC0,0x04,0x0C,0x98,0x80,0x09,0x18,0x30,0x17,0x80,0x09,0x98,0x80,
    0x01,0x13,0x30,0x01,0x03,0x26,0x60,0x02,0x06,0x4C,0xC0,0x04,0x0C,0x98,0x80,0x09,
    0x18,0x30,0x01,0x13,0x30,0x60,0x02,0x26,0x60,0xC0,0x04,0x4C,0xC0,0x80,0x09,0x98,
    0x80,0x01,0x13,0x30,0x01,0x13,0x30,0x60,0x02,0x26,0x60,0xC0,0x04,0x4C,0xC0,0x80,
    0x09,0x98,0x80,0x01,0x13,0x30,0x01,0x03,0x26,0x60,0x02,0x06,0x4C,0xC0,0x04,0x0C,
    0x98,0x6E,0x82,0x2D,0x28,0x32,0x2C,0xC1,0x09,0x5E,0x91,0xC2,0x00,0x00,0x00,0x00,
    0x49,0x45,0x4E,0x44,0xAE,0x42,0x60,0x82
};

typedef struct {
    SceUtilitySavedataParam params;
    PspUtilitySavedataListSaveNewData new_data;
    char save_name_list[2][20];
    char title[32];
    SavePayload payload;
    SavePayload loaded;
    SaveStatus status;
    int loading;
    int shutdown_requested;
    int loaded_valid;
} SaveService;

static SaveService service __attribute__((aligned(16)));

static void savedata_prepare(int mode)
{
    memset(&service.new_data,0,sizeof(service.new_data));
    memset(&service.params,0,sizeof(service.params));
    service.params.base.size=sizeof(service.params);
    service.params.base.language=PSP_SYSTEMPARAM_LANGUAGE_ENGLISH;
    service.params.base.buttonSwap=PSP_UTILITY_ACCEPT_CROSS;
    service.params.base.graphicsThread=0x11;
    service.params.base.accessThread=0x13;
    service.params.base.fontThread=0x12;
    service.params.base.soundThread=0x10;
    /* Only an explicit success result from the utility completes a save. */
    service.params.base.result=-1;
    service.params.mode=(PspUtilitySavedataMode)mode;
    service.params.overwrite=1;
    service.params.focus=mode==PSP_UTILITY_SAVEDATA_LISTSAVE ?
        PSP_UTILITY_SAVEDATA_FOCUS_FIRSTEMPTY : PSP_UTILITY_SAVEDATA_FOCUS_LATEST;
    snprintf(service.params.gameName,sizeof(service.params.gameName),"%s",SAVE_GAME_NAME);
    snprintf(service.params.saveName,sizeof(service.params.saveName),"%s",SAVE_NAME);
    snprintf(service.params.fileName,sizeof(service.params.fileName),"%s",SAVE_FILE);
    snprintf(service.save_name_list[0],sizeof(service.save_name_list[0]),"%s",SAVE_NAME);
    service.save_name_list[1][0]='\0';
    service.params.saveNameList=service.save_name_list;
    service.params.dataBuf=&service.payload;
    service.params.dataBufSize=sizeof(service.payload);
    service.params.dataSize=mode==PSP_UTILITY_SAVEDATA_LISTSAVE ? sizeof(service.payload) : 0;
    if (mode==PSP_UTILITY_SAVEDATA_LISTSAVE) {
        snprintf(service.title,sizeof(service.title),"EMBERWAKE SAVE");
        service.params.icon0FileData.buf=save_icon0;
        service.params.icon0FileData.bufSize=sizeof(save_icon0);
        service.params.icon0FileData.size=sizeof(save_icon0);
        service.new_data.icon0.buf=save_icon0;
        service.new_data.icon0.bufSize=sizeof(save_icon0);
        service.new_data.icon0.size=sizeof(save_icon0);
        service.new_data.title=service.title;
        service.params.newData=&service.new_data;
        snprintf(service.params.sfoParam.title,sizeof(service.params.sfoParam.title),"EMBERWAKE");
        snprintf(service.params.sfoParam.savedataTitle,sizeof(service.params.sfoParam.savedataTitle),"%s",SAVE_NAME);
        snprintf(service.params.sfoParam.detail,sizeof(service.params.sfoParam.detail),"EMBERWAKE PHASE 8 SESSION");
        service.params.sfoParam.parentalLevel=1;
    }
}

static int begin(int mode)
{
    if (service.status==SAVE_STATUS_BUSY) return -1;
    savedata_prepare(mode);
    service.shutdown_requested=0;
    service.loaded_valid=0;
    if (sceUtilitySavedataInitStart(&service.params)<0) {
        service.status=SAVE_STATUS_FAILED;
        return -1;
    }
    service.status=SAVE_STATUS_BUSY;
    return 0;
}

int save_data_begin_save(const SavePayload *payload)
{
    if (!payload || service.status==SAVE_STATUS_BUSY) return -1;
    service.payload=*payload;
    service.loading=0;
    return begin(PSP_UTILITY_SAVEDATA_LISTSAVE);
}

int save_data_begin_load(void)
{
    if (service.status==SAVE_STATUS_BUSY) return -1;
    memset(&service.payload,0,sizeof(service.payload));
    service.loading=1;
    return begin(PSP_UTILITY_SAVEDATA_LISTLOAD);
}

void save_data_update(void)
{
    int state;
    if (service.status!=SAVE_STATUS_BUSY) return;
    state=sceUtilitySavedataGetStatus();
    if (state==PSP_UTILITY_DIALOG_VISIBLE) {
        sceUtilitySavedataUpdate(1);
    } else if (state==PSP_UTILITY_DIALOG_QUIT) {
        if (!service.shutdown_requested && sceUtilitySavedataShutdownStart()>=0)
            service.shutdown_requested=1;
    } else if (state==PSP_UTILITY_DIALOG_FINISHED) {
        /* Shutdown is already underway; do not request it a second time. */
        service.shutdown_requested=1;
    } else if (state==PSP_UTILITY_DIALOG_NONE && service.shutdown_requested) {
        /* FINISHED is a shutdown state, not the save result. Evaluate the
           final result even when polling observes QUIT followed by NONE. */
        if (service.params.base.result==0 && service.loading &&
            service.params.dataSize==sizeof(service.payload)) {
            service.loaded=service.payload;
            service.loaded_valid=service.loaded.magic==SAVE_DATA_MAGIC &&
                service.loaded.version==SAVE_DATA_VERSION;
        }
        service.status=service.params.base.result!=0 ||
            (service.loading && !service.loaded_valid) ?
            SAVE_STATUS_FAILED : SAVE_STATUS_SUCCEEDED;
    }
}

SaveStatus save_data_status(void)
{
    return service.status;
}

int save_data_take_loaded(SavePayload *payload)
{
    if (!payload || service.status!=SAVE_STATUS_SUCCEEDED || !service.loading || !service.loaded_valid)
        return 0;
    *payload=service.loaded;
    service.loaded_valid=0;
    return 1;
}
