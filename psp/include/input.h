#ifndef EMBERWAKE_INPUT_H
#define EMBERWAKE_INPUT_H

typedef struct {
    int horizontal;
    int vertical;
    int confirm, cancel;
} Input;

int input_init(void);
void input_poll(Input *input);

#endif
