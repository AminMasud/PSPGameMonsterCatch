#ifndef EMBERWAKE_READY_PROMPT_H
#define EMBERWAKE_READY_PROMPT_H

#include "input.h"

typedef enum { READY_WAITING, READY_ACCEPTED, READY_DECLINED } ReadyPromptResult;

typedef struct {
    int active, cursor, previous_direction;
    char opponent[40];
} ReadyPrompt;

void ready_prompt_open(ReadyPrompt *prompt,const char *opponent);
ReadyPromptResult ready_prompt_update(ReadyPrompt *prompt,const Input *input);
void ready_prompt_draw(const ReadyPrompt *prompt);

#endif
