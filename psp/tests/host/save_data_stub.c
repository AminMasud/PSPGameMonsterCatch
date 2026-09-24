#include "save_data.h"

int save_data_begin_save(const SavePayload *payload)
{
    (void)payload;
    return -1;
}

int save_data_begin_load(void) { return -1; }
void save_data_update(void) { }
SaveStatus save_data_status(void) { return SAVE_STATUS_IDLE; }
int save_data_take_loaded(SavePayload *payload)
{
    (void)payload;
    return 0;
}
