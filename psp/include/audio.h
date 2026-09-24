#ifndef EMBERWAKE_AUDIO_H
#define EMBERWAKE_AUDIO_H
typedef enum { MUSIC_HEARTH, MUSIC_WILD, MUSIC_CAVE, MUSIC_BATTLE, MUSIC_COUNT } MusicScene;
typedef enum { SOUND_NONE, SOUND_CURSOR, SOUND_CONFIRM, SOUND_HIT, SOUND_HEAL,
               SOUND_BOND, SOUND_VICTORY, SOUND_ERROR, SOUND_COUNT } Sound;
int audio_init(void);
void audio_shutdown(void);
void audio_scene(MusicScene scene);
void audio_play(Sound sound);
void audio_settings(int music,int effects);
#endif
