#include "audio.h"
int audio_init(void) { return 1; }
void audio_shutdown(void) { }
void audio_scene(MusicScene scene) { (void)scene; }
void audio_play(Sound sound) { (void)sound; }
void audio_settings(int music,int effects) { (void)music;(void)effects; }
