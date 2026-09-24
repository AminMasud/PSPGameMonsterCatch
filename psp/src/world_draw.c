#include <pspgu.h>
#include "graphics.h"
#include "world_draw.h"

#define COLOR(r,g,b) GU_RGBA(r,g,b,255)

static void tile_draw(char tile, int x, int y, int variant,int phase)
{
    graphics_rectangle(x,y,32,32,variant ? COLOR(62,93,66) : COLOR(59,89,63));
    switch (tile) {
    case 'W':
        graphics_rectangle(x,y,32,32,COLOR(110,69,52));
        graphics_rectangle(x,y,32,3,COLOR(174,113,72));
        graphics_rectangle(x+15,y+3,2,29,COLOR(78,50,44));
        break;
    case 'D':
        graphics_rectangle(x,y,32,32,COLOR(154,121,78));
        graphics_rectangle(x+5,y+2,22,30,COLOR(24,30,37));
        graphics_rectangle(x+9,y+27,14,3,COLOR(238,188,96));
        break;
    case '>':
    case '<':
        graphics_rectangle(x,y,32,32,COLOR(169,148,104));
        graphics_rectangle(x+5,y+13,22,6,COLOR(246,219,147));
        graphics_rectangle(x+(tile=='<'?9:19),y+8,4,16,COLOR(246,219,147));
        break;
    case 'r':
        graphics_rectangle(x,y,32,32,variant?COLOR(69,100,83):COLOR(61,92,78));
        for (int i=0;i<3;++i) {
            int sway=(phase+i)%3-1;
            graphics_rectangle(x+5+i*9,y+12,2,17,COLOR(115,151,101));
            graphics_rectangle(x+4+i*9+sway,y+5,4,12,COLOR(207,183,114));
        }
        break;
    case '_':
        graphics_rectangle(x,y,32,32,COLOR(116,101,79));
        graphics_rectangle(x,y+30,32,2,COLOR(74,68,58));
        break;
    case 'c':
        graphics_rectangle(x,y,32,32,COLOR(66,68,81));
        graphics_rectangle(x+5,y+8,8,3,COLOR(91,89,106));
        graphics_rectangle(x+22,y+23,5,3,COLOR(45,48,63));
        break;
    case 'g':
        graphics_rectangle(x,y,32,32,COLOR(36,71,52));
        for (int i=0;i<4;++i) {
            graphics_rectangle(x+3+i*7,y+7,3,10,COLOR(81,122,64));
            graphics_rectangle(x+1+i*7,y+23,3,7,COLOR(62,101,52));
        }
        break;
    case '#': /* Trees fit inside their solid tile. */
        graphics_rectangle(x+12,y+18,8,12,COLOR(89,63,45));
        graphics_rectangle(x+3,y+8,26,15,COLOR(27,58,47));
        graphics_rectangle(x+7,y+2,18,15,COLOR(39,76,53));
        graphics_rectangle(x+10,y+4,8,3,COLOR(58,98,64));
        break;
    case '~':
        graphics_rectangle(x,y,32,32,COLOR(44,91,113));
        graphics_rectangle(x+3+phase,y+9,12,2,COLOR(86,139,151));
        graphics_rectangle(x+18-phase,y+23,10,2,COLOR(63,115,138));
        break;
    case 'O':
        graphics_rectangle(x+3,y+9,26,20,COLOR(44,55,55));
        graphics_rectangle(x+5,y+5,21,20,COLOR(111,118,106));
        graphics_rectangle(x+8,y+5,15,5,COLOR(147,149,126));
        break;
    case 'H':
        graphics_rectangle(x+2,y+3,28,26,COLOR(45,71,68));
        graphics_rectangle(x+6,y+7,20,18,COLOR(86,168,151));
        graphics_rectangle(x+10,y+11,12,10,COLOR(190,234,183));
        graphics_rectangle(x+14,y+9,4,14,COLOR(86,168,151));
        graphics_rectangle(x+9,y+14,14,4,COLOR(86,168,151));
        break;
    case '=':
        graphics_rectangle(x,y,32,32,COLOR(169,148,104));
        graphics_rectangle(x+5,y+8,3,2,COLOR(143,124,86));
        graphics_rectangle(x+22,y+23,4,2,COLOR(190,169,120));
        break;
    case ',': /* Decorative flowers; no encounters in Phase 2. */
        graphics_rectangle(x+8,y+13,2,6,COLOR(33,64,43));
        graphics_rectangle(x+6,y+10,6,4,COLOR(226,183,105));
        graphics_rectangle(x+22,y+22,5,4,COLOR(192,144,130));
        break;
    default:
        graphics_rectangle(x+7,y+10,2,4,COLOR(76,109,70));
        graphics_rectangle(x+23,y+22,3,2,COLOR(47,79,56));
        break;
    }
}

void world_actor_draw(const Player *p, const Camera *camera, int npc)
{
    int x = (int)p->x - camera->x + 8;
    int y = (int)p->y - camera->y + 7;
    if (x < -32 || y < -32 || x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT) return;
    int stride = p->moving ? ((int)(p->animation / 0.1f) % 4) : 0;
    int left = stride == 1 ? 2 : 0;
    int right = stride == 3 ? 2 : 0;
    graphics_rectangle(x,y+19,16,4,COLOR(36,57,43));
    graphics_rectangle(x+3,y+16-left,4,6,COLOR(34,42,57));
    graphics_rectangle(x+9,y+16-right,4,6,COLOR(34,42,57));
    graphics_rectangle(x,y+8,16,9,npc ? COLOR(184,133,181) : COLOR(76,174,161));
    graphics_rectangle(x+3,y+1,10,8,COLOR(238,203,154));
    graphics_rectangle(x+1,y,14,3,COLOR(218,119,65));
    if (p->facing == FACE_UP) {
        graphics_rectangle(x+3,y+3,10,4,COLOR(98,66,47));
        graphics_rectangle(x+4,y+10,8,7,COLOR(163,107,62));
    } else if (p->facing == FACE_DOWN) {
        graphics_rectangle(x+5,y+5,2,2,COLOR(35,43,48));
        graphics_rectangle(x+10,y+5,2,2,COLOR(35,43,48));
    } else {
        int eye = p->facing == FACE_LEFT ? 3 : 11;
        graphics_rectangle(x+eye,y+5,2,2,COLOR(35,43,48));
        graphics_rectangle(x+(p->facing == FACE_LEFT ? 12 : 0),y+9,4,8,
                           COLOR(163,107,62));
    }
}

void world_draw(const Map *map, const Player *player, const Camera *camera,float animation)
{
    /* At most 16 columns x 10 rows, independent of total map size. */
    int first_x = camera->x / TILE_SIZE;
    int first_y = camera->y / TILE_SIZE;
    int last_x = (camera->x + SCREEN_WIDTH - 1) / TILE_SIZE;
    int last_y = (camera->y + SCREEN_HEIGHT - 1) / TILE_SIZE;
    for (int y = first_y; y <= last_y && y < map->height; ++y) {
        for (int x = first_x; x <= last_x && x < map->width; ++x) {
            tile_draw(map_tile(map,x,y), x*TILE_SIZE-camera->x,
                      y*TILE_SIZE-camera->y, (x+y)%2,(int)(animation*4)%4);
        }
    }
    world_actor_draw(player, camera, 0);
}
