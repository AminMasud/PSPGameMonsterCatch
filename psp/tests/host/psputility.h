#ifndef TEST_PSPUTILITY_H
#define TEST_PSPUTILITY_H
/* Host declarations for exercising savedata control flow, not PSP ABI layout. */
#include <stddef.h>
enum {
    PSP_UTILITY_DIALOG_NONE, PSP_UTILITY_DIALOG_INIT, PSP_UTILITY_DIALOG_VISIBLE,
    PSP_UTILITY_DIALOG_QUIT, PSP_UTILITY_DIALOG_FINISHED
};
#define PSP_UTILITY_ACCEPT_CROSS 1
typedef struct {
    unsigned int size;
    int language, buttonSwap, graphicsThread, accessThread, fontThread, soundThread;
    int result;
} pspUtilityDialogCommon;
#endif
