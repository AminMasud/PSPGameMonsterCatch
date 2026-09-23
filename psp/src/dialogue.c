#include <stdio.h>
#include "dialogue.h"
void dialogue_open(Dialogue *d, const char *title, const char *first, const char *second)
{
    *d = (Dialogue){0};
    d->active = 1;
    d->count = second ? 2 : 1;
    snprintf(d->title,sizeof(d->title),"%s",title);
    snprintf(d->pages[0],sizeof(d->pages[0]),"%s",first);
    if (second) snprintf(d->pages[1],sizeof(d->pages[1]),"%s",second);
}
void dialogue_advance(Dialogue *d)
{
    if (!d->active) return;
    if (++d->page >= d->count) d->active = 0;
}
