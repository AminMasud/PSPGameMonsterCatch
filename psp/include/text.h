#ifndef EMBERWAKE_TEXT_H
#define EMBERWAKE_TEXT_H
void text_draw(int x, int y, const char *text, unsigned int color, int scale);
/* Word wrap inside a pixel rectangle. Returns the number of lines required. */
int text_wrap(int x,int y,int width,int height,const char *text,unsigned int color,int scale);
void text_box(int x,int y,int width,int height,const char *text,unsigned int color);
#endif
