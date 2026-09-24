#ifndef EMBERWAKE_GATE_H
#define EMBERWAKE_GATE_H

#include "progression.h"

typedef struct {
    const char *first;
    const char *second;
} GateDialogue;

typedef struct {
    int from, x, y;
    int to, arrival_x, arrival_y;
    ProgressionFlag required_flag;
    const char *gatekeeper;
    GateDialogue locked_dialogue;
    GateDialogue unlocked_dialogue;
} Gate;

int gate_valid(const Gate *gate);
int gate_is_locked(const Gate *gate,const ProgressionState *progression);
int gate_can_enter(const Gate *gate,const ProgressionState *progression);
const GateDialogue *gate_current_dialogue(const Gate *gate,
                                          const ProgressionState *progression);

#endif
