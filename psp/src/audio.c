#include <pspaudiolib.h>
#include "audio_synth.h"

static AudioSynth synth;
/* Single aligned word mailboxes: the main thread owns writes, the callback
   owns the synthesizer. No mutable sample buffers are shared across threads. */
static volatile unsigned int control=0x300;
static volatile unsigned int effect_command;
static unsigned int last_effect;
static int ready;

static void callback(void *buffer,unsigned int frames,void *context)
{
    (void)context;
    unsigned int settings=control, command=effect_command;
    audio_synth_scene(&synth,(MusicScene)(settings&255));
    if (command!=last_effect) {
        audio_synth_effect(&synth,(Sound)(command&255));last_effect=command;
    }
    audio_synth_render(&synth,buffer,frames,(settings&256)!=0,(settings&512)!=0);
}
int audio_init(void)
{
    audio_synth_init(&synth);
    control=0x300;effect_command=last_effect=0;
    if (pspAudioInit()<0) return 0;
    ready=1;
    pspAudioSetVolume(0,PSP_VOLUME_MAX/2,PSP_VOLUME_MAX/2);
    pspAudioSetChannelCallback(0,callback,0);
    return 1;
}
void audio_shutdown(void)
{
    if (!ready) return;
    pspAudioEnd();ready=0;
}
void audio_scene(MusicScene scene)
{
    if (scene>=0 && scene<MUSIC_COUNT) control=(control&0x300)|(unsigned int)scene;
}
void audio_play(Sound sound)
{
    if (sound>SOUND_NONE && sound<SOUND_COUNT && (control&512))
        effect_command=((effect_command+256)&~255u)|(unsigned int)sound;
}
void audio_settings(int music,int effects)
{
    control=(control&255)|(music?256u:0)|(effects?512u:0);
}
