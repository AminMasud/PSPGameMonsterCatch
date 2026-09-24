#include "save_data.h"
static SavePayload slot;
static SaveStatus status=SAVE_STATUS_IDLE;
static int loading,has_slot;
int save_data_begin_save(const SavePayload *payload)
{
    if(!payload || status==SAVE_STATUS_BUSY) return -1;
    slot=*payload;loading=0;has_slot=1;status=SAVE_STATUS_BUSY;return 0;
}
int save_data_begin_load(void)
{
    if(status==SAVE_STATUS_BUSY) return -1;
    loading=1;status=SAVE_STATUS_BUSY;return 0;
}
void save_data_update(void)
{
    if(status==SAVE_STATUS_BUSY) status=has_slot?SAVE_STATUS_SUCCEEDED:SAVE_STATUS_FAILED;
}
SaveStatus save_data_status(void) { return status; }
int save_data_take_loaded(SavePayload *payload)
{
    if(!payload || status!=SAVE_STATUS_SUCCEEDED || !loading) return 0;
    *payload=slot;loading=0;return 1;
}
