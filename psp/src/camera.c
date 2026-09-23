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
