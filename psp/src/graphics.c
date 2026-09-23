#include <stddef.h>
#include <pspdisplay.h>
#include <pspgu.h>
#include "graphics.h"

#define BUFFER_WIDTH 512
#define FRAME_BYTES (BUFFER_WIDTH * SCREEN_HEIGHT * 4)

/* Aligned command buffer, reused only after the previous list completes. */
/* Reserve 1 MiB for visible terrain, actors, and the bitmap dialogue font.
   Host drawing tests enforce a conservative 6000-rectangle frame budget. */
static unsigned int command_list[262144] __attribute__((aligned(16)));

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
