#include "audio_synth.h"

/* Original 32-step phrases: Hearthlight, Fernwind, Hollow Echo, and Loom Duel.
   Frequency tables and integer oscillators keep audio independent of assets. */
static const unsigned short phrases[MUSIC_COUNT][32]={
    {330,392,440,0,392,330,294,0,262,330,392,0,330,294,262,0,
     330,392,523,0,494,440,392,0,440,392,330,294,330,262,0,0},
    {392,440,523,440,587,523,440,0,392,330,392,440,330,294,0,0,
     440,523,659,587,523,440,392,0,330,392,440,523,392,330,0,0},
    {220,0,262,0,294,0,330,0,247,0,220,0,196,0,0,0,
     220,0,330,0,349,0,294,0,262,0,247,0,220,0,0,0},
    {330,330,392,440,330,494,440,392,294,294,349,392,440,392,349,294,
     330,392,494,523,494,440,392,330,294,349,440,494,392,330,294,0}
};
static const unsigned short bass[MUSIC_COUNT][4]={
    {131,110,98,131},{131,165,110,98},{110,87,98,110},{165,147,131,147}
};
static unsigned int increment(unsigned int hz)
{
    return (unsigned int)(((uint64_t)hz<<32)/AUDIO_RATE);
}
static int triangle(unsigned int phase)
{
    int v=(int)(phase>>20);
    return v<2048?v*2-2048:6143-v*2;
}
void audio_synth_init(AudioSynth *s) { *s=(AudioSynth){0}; }
void audio_synth_scene(AudioSynth *s,MusicScene scene)
{
    if (scene<0 || scene>=MUSIC_COUNT || scene==s->scene) return;
    s->scene=scene;s->clock=0;s->gain=0;
}
void audio_synth_effect(AudioSynth *s,Sound effect)
{
    if (effect<=SOUND_NONE || effect>=SOUND_COUNT) return;
    s->effect=effect;s->effect_clock=0;s->effect_phase=0;
}
void audio_synth_render(AudioSynth *s,int16_t *stereo,unsigned int frames,int music,int effects)
{
    unsigned int beat=s->scene==MUSIC_BATTLE?6615:s->scene==MUSIC_CAVE?13230:11025;
    for (unsigned int i=0;i<frames;++i) {
        unsigned int local=s->clock%beat;
        if (!local) {
            unsigned int note=(s->clock/beat)%32;
            s->step=increment(phrases[s->scene][note]);
            s->bass_step=increment(bass[s->scene][note/8]);
        }
        int target=music?128:0;
        if (s->gain<target) ++s->gain;
        if (s->gain>target) --s->gain;
        s->phase+=s->step;s->bass_phase+=s->bass_step;
        int envelope=local<256?(int)local:local>beat-256?(int)(beat-local):256;
        int melody=s->step?triangle(s->phase)*envelope/256:0;
        int sample=(melody+triangle(s->bass_phase)/2)*s->gain/128;
        if (s->effect!=SOUND_NONE) {
            unsigned int t=s->effect_clock++, duration=4410, hz=660;
            switch(s->effect) {
            case SOUND_CURSOR: hz=700;duration=1764;break;
            case SOUND_CONFIRM: hz=t<2205?660:880;break;
            case SOUND_HIT: hz=100+(4410-(t<4410?t:4410))/12;break;
            case SOUND_HEAL: hz=t<6615?523:t<13230?659:784;duration=19845;break;
            case SOUND_BOND: hz=392+(t/2205)*65;duration=17640;break;
            case SOUND_VICTORY: hz=t<6615?523:t<13230?659:t<19845?784:1047;duration=26460;break;
            case SOUND_ERROR: hz=t<4410?220:165;duration=8820;break;
            default: break;
            }
            int effect_target=effects && t<duration?128:0;
            if (s->effect_gain<effect_target) ++s->effect_gain;
            if (s->effect_gain>effect_target) --s->effect_gain;
            if (!(t&63)) s->effect_step=increment(hz);
            s->effect_phase+=s->effect_step;
            int attack=t<128?(int)t:128;
            sample+=triangle(s->effect_phase)*s->effect_gain/128*attack/128;
            if (t>=duration && !s->effect_gain) s->effect=SOUND_NONE;
        }
        stereo[i*2]=(int16_t)sample;stereo[i*2+1]=(int16_t)sample;
        if (++s->clock>=beat*32) s->clock=0;
    }
}
