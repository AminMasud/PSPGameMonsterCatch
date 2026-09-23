#ifndef EMBERWAKE_DIALOGUE_H
#define EMBERWAKE_DIALOGUE_H
#define DIALOGUE_PAGES 3
typedef struct {
    int active, page, count;
    char title[40];
    char pages[DIALOGUE_PAGES][160];
} Dialogue;
void dialogue_open(Dialogue *d, const char *title, const char *first, const char *second);
void dialogue_advance(Dialogue *d);
#endif
