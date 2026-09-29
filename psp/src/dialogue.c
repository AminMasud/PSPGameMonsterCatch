#include <stdio.h>
#include "dialogue.h"
void dialogue_open(Dialogue *d, const char *title, const char *first, const char *second)
{
    const char *const pages[]={first,second};
    dialogue_open_pages(d,title,pages,second?2:1);
}
void dialogue_open_pages(Dialogue *d, const char *title, const char *const pages[], int count)
{
    if(!d) return;
    *d = (Dialogue){0};
    if(!pages || count<1) return;
    if(count>DIALOGUE_PAGES) count=DIALOGUE_PAGES;
    d->active = 1; d->count = count;
    snprintf(d->title,sizeof(d->title),"%s",title?title:"");
    for(int i=0;i<count;++i)
        snprintf(d->pages[i],sizeof(d->pages[i]),"%s",pages[i]?pages[i]:"");
}
void dialogue_advance(Dialogue *d)
{
    if (!d->active) return;
    if (++d->page >= d->count) d->active = 0;
}
