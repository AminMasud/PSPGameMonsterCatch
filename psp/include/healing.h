#ifndef EMBERWAKE_HEALING_H
#define EMBERWAKE_HEALING_H

#include "input.h"

#define HEALING_POINT_MAX 8

typedef struct {
    int map_id, x, y;
    const char *name;
    const char *prompt;
} HealingPoint;

typedef enum { HEALING_WAITING, HEALING_ACCEPTED, HEALING_DECLINED } HealingPromptResult;

typedef struct {
    int active, cursor, previous_direction;
    const HealingPoint *point;
} HealingPrompt;

int healing_point_valid(const HealingPoint *point);
int healing_point_count(void);
const HealingPoint *healing_point_at(int map_id,int x,int y);
void healing_prompt_open(HealingPrompt *prompt,const HealingPoint *point);
HealingPromptResult healing_prompt_update(HealingPrompt *prompt,const Input *input);
void healing_prompt_draw(const HealingPrompt *prompt);

#endif
