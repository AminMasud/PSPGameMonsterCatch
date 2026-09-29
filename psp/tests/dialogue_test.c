#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "dialogue.h"

int main(void)
{
    Dialogue d;
    const char *const story[]={"FIRST","SECOND","THIRD","IGNORED"};
    dialogue_open_pages(&d,"STORY",story,4);
    assert(d.active && d.count==DIALOGUE_PAGES && !strcmp(d.title,"STORY"));
    for(int i=0;i<DIALOGUE_PAGES;++i) {
        assert(!strcmp(d.pages[i],story[i]));
        dialogue_advance(&d);
    }
    assert(!d.active);
    dialogue_open(&d,"NPC","HELLO",0);
    assert(d.active && d.count==1 && !strcmp(d.pages[0],"HELLO"));
    dialogue_open_pages(&d,"BAD",0,0);assert(!d.active);
    puts("PASS: dialogue pages, legacy NPC dialogue, and safe story-page bounds");
    return 0;
}
