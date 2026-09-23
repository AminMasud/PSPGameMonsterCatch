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
    player_update_blocked(p,map,input,seconds,0,0);
}
void player_update_blocked(Player *p, const Map *map, const Input *input,
                           float seconds, PlayerBlocker blocker, void *context)
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
            if (!map_walkable(map, p->tile_x + dx, p->tile_y + dy) ||
                (blocker && blocker(context,p->tile_x+dx,p->tile_y+dy))) {
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
