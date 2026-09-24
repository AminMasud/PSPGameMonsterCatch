#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "audio_synth.h"
#include "player_menu.h"
#include "text.h"

static int bounds, left, top, right, bottom, rectangles;
void graphics_rectangle(int x,int y,int width,int height,unsigned int color)
{
    (void)color;
    assert(width>0 && height>0);
    if (bounds) assert(x>=left && y>=top && x+width<=right && y+height<=bottom);
    ++rectangles;
}
static void le32(FILE *file,unsigned int value)
{
    for(int i=0;i<4;++i) fputc((value>>(i*8))&255,file);
}
static void audio_checks(void)
{
    AudioSynth synth;
    int16_t frames[1024*2];
    const char *const names[]={"hearthlight","fernwind","hollow-echo","loom-duel"};
    for(int scene=0;scene<MUSIC_COUNT;++scene) {
        audio_synth_init(&synth);audio_synth_scene(&synth,(MusicScene)scene);
        char path[120];snprintf(path,sizeof(path),"previews/%s.wav",names[scene]);
        FILE *f=fopen(path,"wb");assert(f);
        unsigned int remaining=AUDIO_RATE*8, samples=0, nonzero=0;
        fputs("RIFF",f);le32(f,36+remaining*4);fputs("WAVEfmt ",f);le32(f,16);
        fputc(1,f);fputc(0,f);fputc(2,f);fputc(0,f);
        le32(f,AUDIO_RATE);le32(f,AUDIO_RATE*4);
        fputc(4,f);fputc(0,f);fputc(16,f);fputc(0,f);
        fputs("data",f);le32(f,remaining*4);
        while(remaining) {
            unsigned int count=remaining>1024?1024:remaining;
            audio_synth_render(&synth,frames,count,1,1);
            for(unsigned int i=0;i<count*2;++i) {
                assert(abs(frames[i])<6000);
                if(frames[i]) ++nonzero;
                unsigned int value=(uint16_t)frames[i];
                fputc(value&255,f);fputc(value>>8,f);
            }
            remaining-=count;samples+=count;
        }
        assert(samples==AUDIO_RATE*8 && nonzero>AUDIO_RATE);
        fclose(f);
        audio_synth_render(&synth,frames,1024,0,0);
        for(int i=512;i<2048;++i) assert(frames[i]==0);
    }
    for(int effect=SOUND_CURSOR;effect<SOUND_COUNT;++effect) {
        audio_synth_init(&synth);audio_synth_effect(&synth,(Sound)effect);
        int nonzero=0;
        for(int block=0;block<40;++block) {
            audio_synth_render(&synth,frames,1024,0,1);
            for(int i=0;i<2048;++i) { assert(abs(frames[i])<6000);nonzero+=frames[i]!=0; }
        }
        assert(nonzero && synth.effect==SOUND_NONE);
    }
    /* Callback buffer size must not alter the generated stream. */
    AudioSynth a,b;int16_t whole[4096],pieces[4096];
    audio_synth_init(&a);audio_synth_init(&b);
    audio_synth_effect(&a,SOUND_HEAL);audio_synth_effect(&b,SOUND_HEAL);
    audio_synth_render(&a,whole,2048,1,1);
    audio_synth_render(&b,pieces,777,1,1);
    audio_synth_render(&b,pieces+1554,1271,1,1);
    assert(!memcmp(whole,pieces,sizeof(whole)));
}
int main(void)
{
    Party party;Inventory bag;PlayerMenu m;GameOptions options={1,1,1};
    party_init(&party);inventory_init(&bag);player_menu_open(&m);
    assert(player_menu_update(&m,&party,&bag,&options,&(Input){.confirm=1})==MENU_PARTY);
    m.cursor=2;
    assert(player_menu_update(&m,&party,&bag,&options,&(Input){.confirm=1})==MENU_COLLECTION);
    m.cursor=1;
    player_menu_update(&m,&party,&bag,&options,&(Input){.confirm=1});
    assert(m.page==MENU_BAG);
    player_menu_update(&m,&party,&bag,&options,&(Input){.confirm=1});
    assert(bag.quantities[0]==3); /* Full HP: no item consumed. */
    party.members[0].hp-=5;
    player_menu_update(&m,&party,&bag,&options,&(Input){.confirm=1});
    assert(bag.quantities[0]==2 && party.members[0].hp==party.members[0].max_hp);
    party.members[0].hp=0;
    player_menu_update(&m,&party,&bag,&options,&(Input){.confirm=1});
    assert(bag.quantities[0]==2 && party.members[0].hp==0);
    player_menu_update(&m,&party,&bag,&options,&(Input){.cancel=1});
    m.cursor=5;player_menu_update(&m,&party,&bag,&options,&(Input){.confirm=1});
    for(int i=0;i<3;++i) { m.cursor=i;player_menu_update(&m,&party,&bag,&options,&(Input){.confirm=1}); }
    assert(!options.music && !options.effects && !options.motion);
    assert(player_menu_update(&m,&party,&bag,&options,&(Input){.menu=INPUT_MENU_SAVE})==MENU_SAVE);
    assert(player_menu_update(&m,&party,&bag,&options,&(Input){.menu=INPUT_MENU_LOAD})==MENU_LOAD);
    assert(player_menu_update(&m,&party,&bag,&options,&(Input){.menu=INPUT_MENU_OPEN})==MENU_CLOSE);
    left=0;top=0;right=480;bottom=272;bounds=1;
    for(int page=MENU_HOME;page<=MENU_OPTIONS;++page) {
        player_menu_open(&m);m.page=(PlayerMenuPage)page;
        player_menu_draw(&m,&party,&bag,&options,4);
    }
    left=18;top=191;right=462;bottom=244;
    text_box(18,191,444,53,"YOUR PROGRESS IS SAFE ON THE MEMORY STICK.",0xffffffff);
    text_box(18,191,444,53,"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA",0xffffffff);
    text_box(18,191,444,53,"ONE\nTWO\nTHREE\nFOUR\nFIVE",0xffffffff);
    assert(text_wrap(18,191,444,53,"",0xffffffff,2)==0);
    int before=rectangles;
    assert(text_wrap(18,191,444,0,"A SHORT MESSAGE",0xffffffff,2)==1);
    assert(rectangles==before);
    audio_checks();
    puts("PASS: player menu, item validation, independent options, bounded text, original PCM tracks/effects and mute");
    return 0;
}
