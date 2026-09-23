#ifndef EMBERWAKE_GRAPHICS_H
#define EMBERWAKE_GRAPHICS_H

#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 272

void graphics_init(void);
void graphics_begin(void);
void graphics_rectangle(int x, int y, int width, int height,
                        unsigned int color);
void graphics_end(void);
void graphics_shutdown(void);

#endif
