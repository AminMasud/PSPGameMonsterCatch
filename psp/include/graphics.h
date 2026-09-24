#ifndef EMBERWAKE_GRAPHICS_H
#define EMBERWAKE_GRAPHICS_H
#include <stdint.h>

#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 272

void graphics_init(void);
void graphics_begin(void);
void graphics_rectangle(int x, int y, int width, int height,
                        unsigned int color);
/* 96x96 canvas in a 128x128 RGBA4444 texture; flip mirrors horizontally. */
void graphics_texture(int x,int y,int width,int height,const uint16_t *pixels,int flip);
/* RGB tint is multiplied with the texture; alpha continues to come from it. */
void graphics_texture_tinted(int x,int y,int width,int height,
                             const uint16_t *pixels,int flip,unsigned int tint);
void graphics_end(void);
void graphics_shutdown(void);

#endif
