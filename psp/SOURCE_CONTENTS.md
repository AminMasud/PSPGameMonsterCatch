# Complete Phase 2 source contents

## .gitignore

````text
*.o
*.elf
*.prx
*.PBP
*.SFO
*.d
````

## Makefile

````text
TARGET = emberwake
OBJS = src/main.o src/game.o src/input.o src/graphics.o src/map.o src/player.o src/camera.o src/world_draw.o

INCDIR = include
CFLAGS = -O2 -G0 -std=c99 -Wall -Wextra -Werror -MMD -MP
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)
LIBDIR =
LDFLAGS =
LIBS = -lpspgu -lpspge -lpspdisplay -lpspctrl

BUILD_PRX = 1
PSP_FW_VERSION = 660
EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = Emberwake - Phase 2

PSPSDK = $(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak

src/main.o: include/game.h include/graphics.h include/input.h
src/game.o: include/game.h include/graphics.h include/input.h
src/input.o: include/input.h
src/graphics.o: include/graphics.h

src/map.o: assets/maps/clearing.inc
PARAM.SFO: Makefile
-include $(OBJS:.o=.d)
````

## README.md

````text
# Emberwake — Phase 2

An original PSP RPG prototype, built incrementally on the hardware-tested Phase 1.
Future creatures are called Veylings. This phase adds only the overworld foundation.

## What changed

- 40 x 24 tiles, each 32 pixels: a 1280 x 768 clearing.
- Reusable grass, path, flower, tree, rock, and pond tile graphics.
- Four-direction, smooth tile-to-tile movement at 128 pixels/second.
- Four facing poses and alternating foot animation.
- Trees, rocks, water, and map boundaries block movement.
- A pixel-aligned camera follows the player and clamps at world edges.
- Rendering visits only visible tiles (at most 160), with no per-frame heap allocation.
- The GU command buffer is now 256 KiB to accommodate tile drawing.
- Existing HOME callback handling, controller polling, double buffering, and
  elapsed-time cap remain in place.

Release the D-pad to finish the current step. Direction changes happen at tile
boundaries. If two directions are held, horizontal takes priority. This is now
four-direction grid movement, rather than Phase 1's free diagonal movement.

## Folder structure

psp/
  .gitignore
  Makefile
  README.md
  SOURCE_CONTENTS.md
  assets/maps/clearing.inc
  include/
    camera.h
    game.h
    graphics.h
    input.h
    map.h
    player.h
    world_draw.h
  src/
    main.c
    camera.c
    game.c
    graphics.c
    input.c
    map.c
    player.c
    world_draw.c
  tests/overworld_test.c

SOURCE_CONTENTS.md contains the complete contents of all authored project files,
excluding the listing itself. Generated output includes EBOOT.PBP, PARAM.SFO,
emberwake.elf, emberwake.prx, and src/*.o / src/*.d.

## Architecture

main.c manages startup, HOME exit, and the frame loop. game.c coordinates the
world. map.c exposes tile lookup and collision. player.c handles step timing and
facing independently of rendering. camera.c computes the viewport. world_draw.c
draws reusable tiles and the traveler using graphics.c's existing rectangle API.

Only the original PSPSDK GU/GE, display, controller, and kernel facilities are
required. No new libraries or external art assets are needed.

## Map format

assets/maps/clearing.inc contains 24 quoted C strings, each exactly 40 characters,
followed by commas. The map is compiled into the EBOOT; no files need to be copied
alongside it. Coordinates start at zero in the top-left corner.

| Character | Tile | Walkable |
| --- | --- | --- |
| . | Grass | Yes |
| = | Path | Yes |
| , | Flowers | Yes |
| # | Tree | No |
| O | Rock | No |
| ~ | Water | No |

Unknown tile codes and coordinates outside the map are blocked. Map dimensions
and spawn coordinates are declared in src/map.c. Spawn is (5, 11). When editing,
keep all row lengths consistent, the dimensions accurate, and the spawn walkable.
Run the logic test after map edits. Changing the map include triggers a rebuild.

There are no entrances, transitions, NPCs, dialogue, or encounters yet.
Those belong to Phase 3.

## Exact build commands

From PowerShell on this computer (existing Ubuntu PSPDEV environment):

    wsl -d Ubuntu -- bash -lc 'cd /mnt/c/Users/polo1/OneDrive/Documents/app/psp && make'

For a clean build:

    wsl -d Ubuntu -- bash -lc 'cd /mnt/c/Users/polo1/OneDrive/Documents/app/psp && make clean && make'

From Linux/WSL in this project's psp folder, with PSPDEV configured:

    make

The build targets a user-mode PRX for 6.60/6.61 custom firmware and packages
EBOOT.PBP. No toolchain installation or system configuration was changed.

## Logic checks

In Linux/WSL, from the psp folder:

    cc -std=c99 -Wall -Wextra -Werror -fsanitize=address,undefined -Iinclude tests/overworld_test.c src/map.c src/player.c src/camera.c -lm -o /tmp/emberwake-overworld-test
    /tmp/emberwake-overworld-test

Checks cover map row dimensions and tile codes, spawn validity, world bounds,
solid tiles, partial movement, release-to-complete behavior, turning, diagonal
priority, speed at 30/60/120 updates per second, collision from four directions,
camera centering and edge clamps, small maps, and long-frame clamping.

## Install and hardware test

Replace the previous file with this build:

    ms0:/PSP/GAME/EMBERWAKE/EBOOT.PBP

Open Game > Memory Stick > Emberwake - Phase 2.

1. Expect the traveler on a sandy horizontal path in a green clearing.
2. Walk in all four directions; check facing and stepping animation.
3. Release mid-step: the traveler should settle onto the next tile.
4. Follow the path right: the camera should scroll. Walk toward all world edges:
   the camera should stop without showing outside the map.
5. Try walking into trees, rocks, and the pond. The player should remain outside.
   The pond is northeast of the starting area, trees north, and rocks south.
6. Try turns and two held directions near corners; do not pass through obstacles.
7. Confirm HOME > Cancel resumes play and HOME > Quit returns to the system menu.
8. Suspend/resume and check that there is no large movement jump.

## Validation status

The host logic checks passed with AddressSanitizer and UndefinedBehaviorSanitizer.
The PSP build uses -Wall -Wextra -Werror. Hardware visuals, actual frame rate,
camera feel, suspend/resume, and HOME behavior need your Phase 2 PSP test.

Phase 1 movement/rendering/boundaries/HOME were confirmed by you on PSP hardware.
Development stops at Phase 2 pending your next test.

## Official references

- [PSPDEV installation](https://pspdev.github.io/installation.html)
- [PSPSDK GU reference](https://pspdev.github.io/pspsdk/group__GU.html)
- [PSPSDK build rules](https://github.com/pspdev/pspsdk/blob/master/src/base/build.mak)
````

## assets/maps/clearing.inc

````text
"########################################",
"#...........,.......==.........,.......#",
"#....,..............==..,..............#",
"#.........#####..,..==.~~~~~~~~~....,..#",
"#.........#####.....==.~~~~~~~~~.......#",
"#..,......#####.....==,~~~~~~~~~.......#",
"#.........#####,....==.~~~~~~~~~..,....#",
"#.......,.#####.....==.~~~~~~~~~.......#",
"#,........#####.....==.~~~~~~~~~.......#",
"#............,......==..........,......#",
"#.....,.............==...,.............#",
"#======================================#",
"#======================================#",
"#...,...............==.,...............#",
"#...............,O..==.............,...#",
"#........,.......O..==......,.######...#",
"#.,..............O..==........######...#",
"#.....OOOO....,..O..==........######...#",
"#......,.........O..==....,...######...#",
"#................O.,==........######..,#",
"#...........,.......==.........,.......#",
"#....,..............==..,..............#",
"#................,..==..............,..#",
"########################################",
````

## include/camera.h

````text
#ifndef EMBERWAKE_CAMERA_H
#define EMBERWAKE_CAMERA_H
#include "player.h"
typedef struct { int x, y; } Camera;
void camera_update(Camera *camera, const Player *player, const Map *map);
#endif
````

## include/game.h

````text
#ifndef EMBERWAKE_GAME_H
#define EMBERWAKE_GAME_H
#include "camera.h"
typedef struct {
    const Map *map;
    Player player;
    Camera camera;
} Game;
void game_init(Game *game);
void game_update(Game *game, const Input *input, float seconds);
void game_draw(const Game *game);
#endif
````

## include/graphics.h

````text
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
````

## include/input.h

````text
#ifndef EMBERWAKE_INPUT_H
#define EMBERWAKE_INPUT_H

typedef struct {
    int horizontal;
    int vertical;
} Input;

int input_init(void);
void input_poll(Input *input);

#endif
````

## include/map.h

````text
#ifndef EMBERWAKE_MAP_H
#define EMBERWAKE_MAP_H
#define TILE_SIZE 32
typedef struct {
    int width, height;
    const char *const *rows;
    int spawn_x, spawn_y;
} Map;
const Map *map_test(void);
char map_tile(const Map *map, int x, int y);
int map_walkable(const Map *map, int x, int y);
#endif
````

## include/player.h

````text
#ifndef EMBERWAKE_PLAYER_H
#define EMBERWAKE_PLAYER_H
#include "input.h"
#include "map.h"
typedef enum { FACE_DOWN, FACE_LEFT, FACE_RIGHT, FACE_UP } Direction;
typedef struct {
    int tile_x, tile_y, target_x, target_y;
    float x, y, progress, animation;
    int moving;
    Direction facing;
} Player;
void player_init(Player *player, const Map *map);
void player_update(Player *player, const Map *map, const Input *input, float seconds);
#endif
````

## include/world_draw.h

````text
#ifndef EMBERWAKE_WORLD_DRAW_H
#define EMBERWAKE_WORLD_DRAW_H
#include "camera.h"
void world_draw(const Map *map, const Player *player, const Camera *camera);
#endif
````

## src/camera.c

````text
#include "camera.h"
#include "graphics.h"
static int clamp_camera(int value, int maximum)
{
    if (maximum < 0 || value < 0) return 0;
    return value > maximum ? maximum : value;
}
void camera_update(Camera *camera, const Player *player, const Map *map)
{
    camera->x = clamp_camera((int)player->x + TILE_SIZE / 2 - SCREEN_WIDTH / 2,
                            map->width * TILE_SIZE - SCREEN_WIDTH);
    camera->y = clamp_camera((int)player->y + TILE_SIZE / 2 - SCREEN_HEIGHT / 2,
                            map->height * TILE_SIZE - SCREEN_HEIGHT);
}
````

## src/game.c

````text
#include "game.h"
#include "world_draw.h"
void game_init(Game *game)
{
    game->map = map_test();
    player_init(&game->player, game->map);
    camera_update(&game->camera, &game->player, game->map);
}
void game_update(Game *game, const Input *input, float seconds)
{
    player_update(&game->player, game->map, input, seconds);
    camera_update(&game->camera, &game->player, game->map);
}
void game_draw(const Game *game)
{
    world_draw(game->map, &game->player, &game->camera);
}
````

## src/graphics.c

````text
#include <stddef.h>
#include <pspdisplay.h>
#include <pspgu.h>
#include "graphics.h"

#define BUFFER_WIDTH 512
#define FRAME_BYTES (BUFFER_WIDTH * SCREEN_HEIGHT * 4)

/* Aligned command buffer, reused only after the previous list completes. */
/* Visible tiles need at most 800 rectangles plus the player. Reserve 256 KiB
   for their vertex storage and commands; do not draw the entire world. */
static unsigned int command_list[65536] __attribute__((aligned(16)));

typedef struct {
    unsigned int color;
    float x, y, z;
} Vertex;

void graphics_init(void)
{
    sceGuInit();
    sceGuStart(GU_DIRECT, command_list);
    /* GU buffer pointers are offsets into VRAM, not ordinary RAM pointers.
       Two 512 x 272 RGBA buffers consume 1,114,112 bytes of 2 MiB VRAM. */
    sceGuDrawBuffer(GU_PSM_8888, (void *)0, BUFFER_WIDTH);
    sceGuDispBuffer(SCREEN_WIDTH, SCREEN_HEIGHT,
                    (void *)FRAME_BYTES, BUFFER_WIDTH);
    sceGuOffset(2048 - SCREEN_WIDTH / 2, 2048 - SCREEN_HEIGHT / 2);
    sceGuViewport(2048, 2048, SCREEN_WIDTH, SCREEN_HEIGHT);
    sceGuScissor(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    sceGuEnable(GU_SCISSOR_TEST);
    sceGuDisable(GU_DEPTH_TEST);
    sceGuDisable(GU_TEXTURE_2D);
    sceGuDisable(GU_CULL_FACE);
    sceGuDisable(GU_BLEND);
    sceGuShadeModel(GU_SMOOTH);
    sceGuFinish();
    sceGuSync(0, 0);
    sceDisplayWaitVblankStart();
    sceGuDisplay(GU_TRUE);
}

void graphics_begin(void)
{
    sceGuStart(GU_DIRECT, command_list);
    sceGuClearColor(GU_RGBA(42, 65, 61, 255));
    sceGuClear(GU_COLOR_BUFFER_BIT);
}

void graphics_rectangle(int x, int y, int width, int height,
                        unsigned int color)
{
    /* Allocate vertices inside the GU list; no per-frame heap allocation. */
    Vertex *vertices = sceGuGetMemory(2 * sizeof(Vertex));
    vertices[0] = (Vertex){color, (float)x, (float)y, 0.0f};
    vertices[1] = (Vertex){color, (float)(x + width),
                         (float)(y + height), 0.0f};
    sceGuDrawArray(GU_SPRITES,
                  GU_COLOR_8888 | GU_VERTEX_32BITF | GU_TRANSFORM_2D,
                  2, NULL, vertices);
}

void graphics_end(void)
{
    sceGuFinish();
    sceGuSync(0, 0);
    sceDisplayWaitVblankStart();
    sceGuSwapBuffers();
}

void graphics_shutdown(void)
{
    sceGuDisplay(GU_FALSE);
    sceGuTerm();
}
````

## src/input.c

````text
#include <pspctrl.h>
#include "input.h"

int input_init(void)
{
    int result = sceCtrlSetSamplingCycle(0);
    if (result < 0) {
        return result;
    }
    return sceCtrlSetSamplingMode(PSP_CTRL_MODE_DIGITAL);
}

void input_poll(Input *input)
{
    SceCtrlData pad = {0};
    input->horizontal = 0;
    input->vertical = 0;
    if (sceCtrlPeekBufferPositive(&pad, 1) <= 0) {
        return;
    }
    input->horizontal = ((pad.Buttons & PSP_CTRL_RIGHT) != 0)
                      - ((pad.Buttons & PSP_CTRL_LEFT) != 0);
    input->vertical = ((pad.Buttons & PSP_CTRL_DOWN) != 0)
                    - ((pad.Buttons & PSP_CTRL_UP) != 0);
}
````

## src/main.c

````text
#include <pspkernel.h>
#include "game.h"
#include "graphics.h"
#include "input.h"

/* User-mode PRX: no kernel access or extra PSP-3000 RAM required. */
PSP_MODULE_INFO("Emberwake", 0, 0, 1);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER);
PSP_HEAP_SIZE_KB(1024);

static volatile int running = 1;

static int exit_callback(int arg1, int arg2, void *common)
{
    (void)arg1;
    (void)arg2;
    (void)common;
    running = 0;
    return 0;
}

static SceUID setup_callbacks(void)
{
    /* Callbacks belong to the creating thread. Main services this callback
       once per frame, so a separate callback thread is unnecessary. */
    SceUID callback = sceKernelCreateCallback("home_exit", exit_callback, NULL);
    if (callback < 0) {
        return callback;
    }
    if (sceKernelRegisterExitCallback(callback) < 0) {
        sceKernelDeleteCallback(callback);
        return -1;
    }
    return callback;
}

int main(void)
{
    Game game;
    Input input;
    SceUID callback = setup_callbacks();
    if (callback < 0 || input_init() < 0) {
        sceKernelExitGame();
        return 1;
    }

    graphics_init();
    game_init(&game);
    unsigned int previous = sceKernelGetSystemTimeLow();

    while (running) {
        sceKernelCheckCallback();
        if (!running) {
            break;
        }
        unsigned int now = sceKernelGetSystemTimeLow();
        /* Unsigned subtraction handles the microsecond counter wrapping. */
        float seconds = (float)(now - previous) / 1000000.0f;
        previous = now;
        if (seconds > 0.05f) {
            seconds = 0.05f;
        }
        input_poll(&input);
        game_update(&game, &input, seconds);
        graphics_begin();
        game_draw(&game);
        graphics_end();
    }

    graphics_shutdown();
    sceKernelDeleteCallback(callback);
    sceKernelExitGame();
    return 0;
}
````

## src/map.c

````text
#include "map.h"
static const char *const clearing_rows[] = {
#include "../assets/maps/clearing.inc"
};
static const Map clearing = {40, 24, clearing_rows, 5, 11};
const Map *map_test(void) { return &clearing; }
char map_tile(const Map *map, int x, int y)
{
    if (x < 0 || y < 0 || x >= map->width || y >= map->height) return '#';
    return map->rows[y][x];
}
int map_walkable(const Map *map, int x, int y)
{
    char tile = map_tile(map, x, y);
    return tile == '.' || tile == '=' || tile == ',';
}
````

## src/player.c

````text
#include "player.h"
#define WALK_SPEED 128.0f
void player_init(Player *p, const Map *map)
{
    *p = (Player){0};
    p->tile_x = p->target_x = map->spawn_x;
    p->tile_y = p->target_y = map->spawn_y;
    p->x = (float)(map->spawn_x * TILE_SIZE);
    p->y = (float)(map->spawn_y * TILE_SIZE);
    p->facing = FACE_DOWN;
}
void player_update(Player *p, const Map *map, const Input *input, float seconds)
{
    if (seconds <= 0.0f) return;
    if (seconds > 0.05f) seconds = 0.05f;
    /* Finish a tile after release; turns happen at tile boundaries.
       Carry leftover frame time into the next step to avoid movement pauses. */
    while (seconds > 0.0f) {
        if (!p->moving) {
            int dx = input->horizontal;
            int dy = dx != 0 ? 0 : input->vertical;
            if (dx == 0 && dy == 0) { p->animation = 0.0f; break; }
            p->facing = dx < 0 ? FACE_LEFT : dx > 0 ? FACE_RIGHT
                       : dy < 0 ? FACE_UP : FACE_DOWN;
            if (!map_walkable(map, p->tile_x + dx, p->tile_y + dy)) {
                p->animation = 0.0f;
                break;
            }
            p->target_x = p->tile_x + dx;
            p->target_y = p->tile_y + dy;
            p->progress = 0.0f;
            p->moving = 1;
        }
        float remaining = (TILE_SIZE - p->progress) / WALK_SPEED;
        float used = seconds < remaining ? seconds : remaining;
        p->progress += used * WALK_SPEED;
        p->animation += used;
        if (p->animation >= 0.4f) p->animation -= 0.4f;
        seconds -= used;
        float fraction = p->progress / TILE_SIZE;
        p->x = (p->tile_x + (p->target_x - p->tile_x) * fraction) * TILE_SIZE;
        p->y = (p->tile_y + (p->target_y - p->tile_y) * fraction) * TILE_SIZE;
        if (used >= remaining) {
            p->tile_x = p->target_x;
            p->tile_y = p->target_y;
            p->x = (float)(p->tile_x * TILE_SIZE);
            p->y = (float)(p->tile_y * TILE_SIZE);
            p->moving = 0;
        }
    }
}
````

## src/world_draw.c

````text
#include <pspgu.h>
#include "graphics.h"
#include "world_draw.h"

#define COLOR(r,g,b) GU_RGBA(r,g,b,255)

static void tile_draw(char tile, int x, int y, int variant)
{
    graphics_rectangle(x,y,32,32,variant ? COLOR(62,93,66) : COLOR(59,89,63));
    switch (tile) {
    case '#': /* Trees fit inside their solid tile. */
        graphics_rectangle(x+12,y+18,8,12,COLOR(89,63,45));
        graphics_rectangle(x+3,y+8,26,15,COLOR(27,58,47));
        graphics_rectangle(x+7,y+2,18,15,COLOR(39,76,53));
        graphics_rectangle(x+10,y+4,8,3,COLOR(58,98,64));
        break;
    case '~':
        graphics_rectangle(x,y,32,32,COLOR(44,91,113));
        graphics_rectangle(x+4,y+9,12,2,COLOR(86,139,151));
        graphics_rectangle(x+17,y+23,10,2,COLOR(63,115,138));
        break;
    case 'O':
        graphics_rectangle(x+3,y+9,26,20,COLOR(44,55,55));
        graphics_rectangle(x+5,y+5,21,20,COLOR(111,118,106));
        graphics_rectangle(x+8,y+5,15,5,COLOR(147,149,126));
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

static void player_draw(const Player *p, const Camera *camera)
{
    int x = (int)p->x - camera->x + 8;
    int y = (int)p->y - camera->y + 7;
    int stride = p->moving ? ((int)(p->animation / 0.1f) % 4) : 0;
    int left = stride == 1 ? 2 : 0;
    int right = stride == 3 ? 2 : 0;
    graphics_rectangle(x,y+19,16,4,COLOR(36,57,43));
    graphics_rectangle(x+3,y+16-left,4,6,COLOR(34,42,57));
    graphics_rectangle(x+9,y+16-right,4,6,COLOR(34,42,57));
    graphics_rectangle(x,y+8,16,9,COLOR(76,174,161));
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

void world_draw(const Map *map, const Player *player, const Camera *camera)
{
    /* At most 16 columns x 10 rows, independent of total map size. */
    int first_x = camera->x / TILE_SIZE;
    int first_y = camera->y / TILE_SIZE;
    int last_x = (camera->x + SCREEN_WIDTH - 1) / TILE_SIZE;
    int last_y = (camera->y + SCREEN_HEIGHT - 1) / TILE_SIZE;
    for (int y = first_y; y <= last_y && y < map->height; ++y) {
        for (int x = first_x; x <= last_x && x < map->width; ++x) {
            tile_draw(map_tile(map,x,y), x*TILE_SIZE-camera->x,
                      y*TILE_SIZE-camera->y, (x+y)%2);
        }
    }
    player_draw(player, camera);
}
````

## tests/overworld_test.c

````text
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "camera.h"
#include "graphics.h"

static void tick(Player *p, const Map *map, Input input, int frames, float dt)
{
    for (int i = 0; i < frames; ++i) player_update(p,map,&input,dt);
}

int main(void)
{
    const Map *map = map_test();
    for (int y = 0; y < map->height; ++y) {
        assert(strlen(map->rows[y]) == (size_t)map->width);
        for (int x = 0; x < map->width; ++x)
            assert(strchr(".#~=O,", map_tile(map,x,y)) != NULL);
    }
    assert(map_walkable(map,map->spawn_x,map->spawn_y));
    assert(!map_walkable(map,-1,5));
    assert(!map_walkable(map,map->width,5));
    assert(!map_walkable(map,5,-1));
    assert(!map_walkable(map,5,map->height));
    assert(!map_walkable(map,10,3)); /* trees */
    assert(!map_walkable(map,23,3)); /* water */
    assert(!map_walkable(map,17,14)); /* rocks */

    Player p;
    player_init(&p,map);
    tick(&p,map,(Input){1,0},1,0.025f);
    assert(p.moving && p.x > 160 && p.x < 192);
    /* Release completes exactly one step. */
    tick(&p,map,(Input){0,0},20,0.025f);
    assert(!p.moving && p.tile_x == 6 && p.x == 192);

    player_init(&p,map);
    tick(&p,map,(Input){1,0},1,0.025f);
    tick(&p,map,(Input){0,-1},40,0.025f);
    assert(p.tile_x == 6 && p.tile_y < 11 && p.facing == FACE_UP);

    /* Held motion covers the same distance at 30, 60 and 120 Hz. */
    const int rates[] = {30,60,120};
    for (int i = 0; i < 3; ++i) {
        player_init(&p,map);
        tick(&p,map,(Input){1,0},rates[i],1.0f/rates[i]);
        assert(fabsf(p.x - 288.0f) < 0.01f);
    }
    player_init(&p,map);
    tick(&p,map,(Input){1,-1},20,0.025f);
    assert(p.y == 352); /* Horizontal priority; no diagonal corner cutting. */
    player_init(&p,map);
    tick(&p,map,(Input){-1,0},100,0.05f);
    assert(p.tile_x == 1 && p.x == 32 && !p.moving);
    tick(&p,map,(Input){0,-1},200,0.05f);
    assert(p.tile_y == 1 && p.y == 32 && !p.moving);

    /* Collision from all four sides of a blocked tile. */
    const char *const rows[] = {".....",".....","..O..",".....","....."};
    const int starts[][2] = {{1,2},{3,2},{2,1},{2,3}};
    const Input directions[] = {{1,0},{-1,0},{0,1},{0,-1}};
    for (int i = 0; i < 4; ++i) {
        Map small = {5,5,rows,starts[i][0],starts[i][1]};
        player_init(&p,&small);
        tick(&p,&small,directions[i],20,0.05f);
        assert(p.tile_x == starts[i][0] && p.tile_y == starts[i][1]);
        assert(!p.moving);
        Camera camera;
        camera_update(&camera,&p,&small);
        assert(camera.x == 0 && camera.y == 0);
    }
    Camera camera;
    p.x = p.y = 0;
    camera_update(&camera,&p,map);
    assert(camera.x == 0 && camera.y == 0);
    p.x = 640; p.y = 384;
    camera_update(&camera,&p,map);
    assert(camera.x == 416 && camera.y == 264);
    p.x = 1248; p.y = 736;
    camera_update(&camera,&p,map);
    assert(camera.x == map->width*TILE_SIZE-SCREEN_WIDTH);
    assert(camera.y == map->height*TILE_SIZE-SCREEN_HEIGHT);
    player_init(&p,map);
    tick(&p,map,(Input){1,0},1,10.0f);
    assert(p.x <= 166.401f); /* Long pauses are clamped. */
    puts("PASS: map, collision, release, turning, frame rates, camera, pause cap");
    return 0;
}
````
