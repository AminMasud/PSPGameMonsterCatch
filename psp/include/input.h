#ifndef EMBERWAKE_INPUT_H
#define EMBERWAKE_INPUT_H

typedef struct {
    int horizontal;
    int vertical;
    int confirm, cancel;
    int details;
    int menu;
} Input;

enum {
    INPUT_MENU_OPEN = 1,
    INPUT_MENU_SAVE = 2,
    INPUT_MENU_LOAD = 4
};

int input_init(void);
void input_poll(Input *input);

#endif
