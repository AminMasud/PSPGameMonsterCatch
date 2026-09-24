#ifndef EMBERWAKE_AUDIO_SYNTH_H
#define EMBERWAKE_AUDIO_SYNTH_H
#include <stdint.h>
#include "audio.h"
#define AUDIO_RATE 44100
typedef struct {
    MusicScene scene;
    unsigned int clock, phase, bass_phase, effect_phase, effect_clock;
    unsigned int step, bass_step, effect_step;
    int gain, effect_gain;
    Sound effect;
} AudioSynth;
void audio_synth_init(AudioSynth *s);
void audio_synth_scene(AudioSynth *s,MusicScene scene);
void audio_synth_effect(AudioSynth *s,Sound effect);
void audio_synth_render(AudioSynth *s,int16_t *stereo,unsigned int frames,int music,int effects);
#endif
