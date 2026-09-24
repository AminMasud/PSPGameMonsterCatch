#include <string.h>
#include "graphics.h"
#include "text.h"
/* Original 5x7 bitmap alphabet. Each row uses its low five bits. */
static const unsigned char glyphs[36][7] = {
 {14,17,17,31,17,17,17},{30,17,17,30,17,17,30},{14,17,16,16,16,17,14},
 {30,17,17,17,17,17,30},{31,16,16,30,16,16,31},{31,16,16,30,16,16,16},
 {14,17,16,23,17,17,15},{17,17,17,31,17,17,17},{14,4,4,4,4,4,14},
 {7,2,2,2,18,18,12},{17,18,20,24,20,18,17},{16,16,16,16,16,16,31},
 {17,27,21,21,17,17,17},{17,25,25,21,19,19,17},{14,17,17,17,17,17,14},
 {30,17,17,30,16,16,16},{14,17,17,17,21,18,13},{30,17,17,30,20,18,17},
 {15,16,16,14,1,1,30},{31,4,4,4,4,4,4},{17,17,17,17,17,17,14},
 {17,17,17,17,17,10,4},{17,17,17,21,21,21,10},{17,17,10,4,10,17,17},
 {17,17,10,4,4,4,4},{31,1,2,4,8,16,31},
 {14,17,19,21,25,17,14},{4,12,4,4,4,4,14},{14,17,1,2,4,8,31},
 {30,1,1,14,1,1,30},{2,6,10,18,31,2,2},{31,16,16,30,1,1,30},
 {14,16,16,30,17,17,14},{31,1,2,4,8,8,8},{14,17,17,14,17,17,14},
 {14,17,17,15,1,1,14}
};
void text_draw(int x, int y, const char *text, unsigned int color, int scale)
{
    if (!text || scale<1) return;
    int origin = x;
    for (; *text; ++text) {
        char ch = *text;
        if (ch == '\n') { x = origin; y += 9*scale; continue; }
        if (ch >= 'a' && ch <= 'z') ch -= 'a'-'A';
        int index = ch >= 'A' && ch <= 'Z' ? ch-'A' :
                    ch >= '0' && ch <= '9' ? 26+ch-'0' : -1;
        for (int row = 0; row < 7; ++row) {
            unsigned int bits = index >= 0 ? glyphs[index][row] : 0;
            if (ch == '.' && row == 6) bits = 4;
            if (ch == '-' && row == 3) bits = 14;
            if (ch == ':' && (row == 2 || row == 5)) bits = 4;
            if (ch == '\'' && row < 2) bits = 4;
            if (ch == '+' && row >= 1 && row <= 5) bits = row==3?31:4;
            if (ch == '/' && row < 5) bits = 1u << row;
            if (ch == '?' && row < 7) { const int q[] = {14,17,1,2,4,0,4}; bits=q[row]; }
            for (int col = 0; col < 5;) {
                if (!(bits & (16u >> col))) { ++col; continue; }
                int start = col++;
                while (col < 5 && (bits & (16u >> col))) ++col;
                graphics_rectangle(x+start*scale,y+row*scale,(col-start)*scale,scale,color);
            }
        }
        x += 6*scale;
    }
}

int text_wrap(int x,int y,int width,int height,const char *text,unsigned int color,int scale)
{
    char line[81];
    int row=0, columns=scale>0?width/(6*scale):0;
    if (!text || columns<1) return 0;
    if (columns>80) columns=80;
    while (*text) {
        int n=0, space=-1, take;
        while (text[n] && text[n]!='\n' && n<columns) {
            if (text[n]==' ') space=n;
            ++n;
        }
        take=n;
        if (text[n] && text[n]!='\n' && text[n]!=' ' && space>0) take=space;
        memcpy(line,text,(size_t)take);line[take]='\0';
        if (row*9*scale+7*scale<=height)
            text_draw(x,y+row*9*scale,line,color,scale);
        ++row;text+=take;
        if (*text=='\n') ++text;
        else while (*text==' ') ++text;
    }
    return row;
}

void text_box(int x,int y,int width,int height,const char *text,unsigned int color)
{
    /* Measure without drawing; prefer the larger font where the whole message fits. */
    int lines=text_wrap(0,0,width,0,text,color,2);
    int scale=lines*18-4<=height?2:1;
    text_wrap(x,y,width,height,text,color,scale);
}
