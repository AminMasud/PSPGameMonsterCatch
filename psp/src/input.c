#include <pspctrl.h>
#include "input.h"

int input_init(void)
{
    int result = sceCtrlSetSamplingCycle(0);
    if (result < 0) {
        return result;
    }
    return sceCtrlSetSamplingMode(PSP_CTRL_MODE_DIGITAL);
}

void input_poll(Input *input)
{
    static unsigned int previous;
    SceCtrlData pad = {0};
    input->horizontal = 0;
    input->vertical = 0;
    input->confirm = input->cancel = 0;
    if (sceCtrlPeekBufferPositive(&pad, 1) <= 0) {
        return;
    }
    input->horizontal = ((pad.Buttons & PSP_CTRL_RIGHT) != 0)
                      - ((pad.Buttons & PSP_CTRL_LEFT) != 0);
    input->vertical = ((pad.Buttons & PSP_CTRL_DOWN) != 0)
                    - ((pad.Buttons & PSP_CTRL_UP) != 0);
    unsigned int pressed = pad.Buttons & ~previous;
    previous = pad.Buttons;
    input->confirm = (pressed & PSP_CTRL_CROSS) != 0;
    input->cancel = (pressed & PSP_CTRL_CIRCLE) != 0;
}
