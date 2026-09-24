#ifndef TEST_PSPUTILITY_SAVEDATA_H
#define TEST_PSPUTILITY_SAVEDATA_H
#include "psputility.h"
typedef enum {
    PSP_UTILITY_SAVEDATA_LISTLOAD=4, PSP_UTILITY_SAVEDATA_LISTSAVE=5
} PspUtilitySavedataMode;
enum { PSP_UTILITY_SAVEDATA_FOCUS_LATEST=3, PSP_UTILITY_SAVEDATA_FOCUS_FIRSTEMPTY=7 };
typedef struct {
    void *buf;
    size_t bufSize, size;
} PspUtilitySavedataFileData;
typedef struct {
    PspUtilitySavedataFileData icon0;
    char *title;
} PspUtilitySavedataListSaveNewData;
typedef struct {
    pspUtilityDialogCommon base;
    PspUtilitySavedataMode mode;
    int overwrite, focus;
    char gameName[13], saveName[20], fileName[13];
    char (*saveNameList)[20];
    void *dataBuf;
    size_t dataBufSize, dataSize;
    PspUtilitySavedataFileData icon0FileData;
    PspUtilitySavedataListSaveNewData *newData;
    struct {
        char title[128], savedataTitle[128], detail[1024];
        unsigned char parentalLevel;
    } sfoParam;
} SceUtilitySavedataParam;
int sceUtilitySavedataInitStart(SceUtilitySavedataParam *params);
int sceUtilitySavedataGetStatus(void);
int sceUtilitySavedataShutdownStart(void);
void sceUtilitySavedataUpdate(int step);
#endif
