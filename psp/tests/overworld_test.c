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
            assert(strchr(".#~=O,WD>gc_", map_tile(map,x,y)) != NULL);
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
    tick(&p,map,(Input){1,0,0,0,0},1,0.025f);
    assert(p.moving && p.x > 160 && p.x < 192);
    /* Release completes exactly one step. */
    tick(&p,map,(Input){0,0,0,0,0},20,0.025f);
    assert(!p.moving && p.tile_x == 6 && p.x == 192);

    player_init(&p,map);
    tick(&p,map,(Input){1,0,0,0,0},1,0.025f);
    tick(&p,map,(Input){0,-1,0,0,0},40,0.025f);
    assert(p.tile_x == 6 && p.tile_y < 11 && p.facing == FACE_UP);

    /* Held motion covers the same distance at 30, 60 and 120 Hz. */
    const int rates[] = {30,60,120};
    for (int i = 0; i < 3; ++i) {
        player_init(&p,map);
        tick(&p,map,(Input){1,0,0,0,0},rates[i],1.0f/rates[i]);
        assert(fabsf(p.x - 288.0f) < 0.01f);
    }
    player_init(&p,map);
    tick(&p,map,(Input){1,-1,0,0,0},20,0.025f);
    assert(p.y == 352); /* Horizontal priority; no diagonal corner cutting. */
    player_init(&p,map);
    tick(&p,map,(Input){-1,0,0,0,0},100,0.05f);
    assert(p.tile_x == 1 && p.x == 32 && !p.moving);
    tick(&p,map,(Input){0,-1,0,0,0},200,0.05f);
    assert(p.tile_y == 1 && p.y == 32 && !p.moving);

    /* Collision from all four sides of a blocked tile. */
    const char *const rows[] = {".....",".....","..O..",".....","....."};
    const int starts[][2] = {{1,2},{3,2},{2,1},{2,3}};
    const Input directions[] = {{1,0,0,0,0},{-1,0,0,0,0},{0,1,0,0,0},{0,-1,0,0,0}};
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
    tick(&p,map,(Input){1,0,0,0,0},1,10.0f);
    assert(p.x <= 166.401f); /* Long pauses are clamped. */
    puts("PASS: map, collision, release, turning, frame rates, camera, pause cap");
    return 0;
}
