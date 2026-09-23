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
