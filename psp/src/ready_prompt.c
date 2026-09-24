#include <stdio.h>
#include <pspgu.h>
#include "ready_prompt.h"
#include "graphics.h"
#include "text.h"

#define C(r,g,b) GU_RGBA(r,g,b,255)

void ready_prompt_open(ReadyPrompt *prompt,const char *opponent)
{
    *prompt=(ReadyPrompt){0};
    prompt->active=1;
    snprintf(prompt->opponent,sizeof(prompt->opponent),"%s",opponent && opponent[0]?opponent:"IMPORTANT BATTLE");
}

ReadyPromptResult ready_prompt_update(ReadyPrompt *prompt,const Input *input)
{
    if(!prompt->active) return READY_WAITING;
    int direction=input->vertical;
    if(direction && direction!=prompt->previous_direction)
        prompt->cursor=1-prompt->cursor;
    prompt->previous_direction=direction;
    if(input->cancel) {
        prompt->active=0;
        return READY_DECLINED;
    }
    if(!input->confirm) return READY_WAITING;
    prompt->active=0;
    return prompt->cursor==0?READY_ACCEPTED:READY_DECLINED;
}

void ready_prompt_draw(const ReadyPrompt *prompt)
{
    if(!prompt->active) return;
    graphics_rectangle(72,38,336,196,C(179,145,91));
    graphics_rectangle(75,41,330,190,C(18,27,34));
    text_draw(143,61,"ARE YOU READY?",C(245,217,166),2);
    text_draw(159,91,prompt->opponent,C(165,195,186),1);
    for(int i=0;i<2;++i) {
        int y=119+i*39;
        if(i==prompt->cursor) {
            graphics_rectangle(132,y,216,31,C(65,83,78));
            graphics_rectangle(132,y,4,31,C(242,177,92));
        }
        text_draw(220,y+11,i==0?"YES":"NO",i==prompt->cursor?C(255,220,162):C(174,190,186),1);
    }
    text_draw(133,211,"UP/DOWN SELECT   X CONFIRM   O NO",C(158,187,179),1);
}
