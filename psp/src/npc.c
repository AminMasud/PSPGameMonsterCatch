#include "npc.h"
static void add(Npcs *n, int x, int y, const char *name, const char *a, const char *b, int end)
{
    Npc *p = &n->people[n->count++];
    *p = (Npc){0};
    p->actor.tile_x = p->actor.target_x = x;
    p->actor.tile_y = p->actor.target_y = y;
    p->actor.x = (float)(x*TILE_SIZE); p->actor.y = (float)(y*TILE_SIZE);
    p->actor.facing = FACE_DOWN;
    p->name = name; p->first = a; p->second = b;
    p->patrol_start = x; p->patrol_end = end; p->direction = 1; p->wait = 1;
}
void npc_load(Npcs *n, int map_id)
{
    *n = (Npcs){0};
    if (map_id == 0) {
        add(n,7,10,"MIRA","WELCOME TO HEARTH CLEARING.\nTHE LODGE IS JUST NORTHWEST.","FOLLOW THE EAST PATH TO THE WOODS.\nPRESS X WHILE FACING SOMEONE.",7);
        add(n,9,13,"ORIN","I KEEP THIS PATH CLEAR.","THE WOODS ARE HOME TO VEYLINGS.\nLOOK FOR THEM IN THE TALL GRASS.",13);
    } else if (map_id == 1) {
        add(n,4,9,"SEN","THE DARK GRASS HIDES VEYLINGS.","THE LIT OPENING NORTHEAST LEADS\nINTO HOLLOWSTONE CAVE.",4);
    } else if (map_id == 2) {
        add(n,7,3,"TAVI","WELCOME TO THE WAYFARER LODGE.","REST A MOMENT. THE SOUTH DOOR\nLEADS BACK TO THE CLEARING.",7);
    } else if (map_id == MAP_CAVE) {
        add(n,5,10,"NEL","FOOTSTEPS ECHO IN THIS CAVE.","VEYLINGS STIR ON THE ROUGH FLOOR.\nTHE SOUTHWEST EXIT LEADS OUT.",5);
    } else if (map_id == MAP_MARSH) {
        add(n,4,8,"ELA","BUBFIN AND ZAPPIP LIVE IN THE REEDS.\nPEBCHICK LIKES THE COOL BANKS.","THE BOARDWALK IS SAFE TO FOLLOW.\nLANTERN REST LIES NORTHEAST.",4);
    } else if (map_id == MAP_REST) {
        add(n,7,3,"ILSEN","WELCOME TO LANTERN REST.\nTHE GREEN DAIS RESTORES YOUR TEAM.","SAVE BEFORE YOUR NEXT ADVENTURE.\nTHE MARSH IS WAITING OUTSIDE.",7);
    }
}
int npc_blocks(void *context, int x, int y)
{
    const Npcs *n = context;
    for (int i = 0; i < n->count; ++i) {
        const Player *p = &n->people[i].actor;
        if ((p->tile_x == x && p->tile_y == y) ||
            (p->target_x == x && p->target_y == y)) return 1;
    }
    return 0;
}
typedef struct { Npcs *n; const Player *player; } Occupancy;
static int occupied(void *context, int x, int y)
{
    Occupancy *o = context;
    const Player *p = o->player;
    return npc_blocks(o->n,x,y) ||
           (p->tile_x == x && p->tile_y == y) ||
           (p->target_x == x && p->target_y == y);
}
void npc_update(Npcs *n, const Map *map, const Player *player, float seconds)
{
    Occupancy occupancy = {n,player};
    for (int i = 0; i < n->count; ++i) {
        Npc *p = &n->people[i];
        Input input = {0};
        if (!p->actor.moving && p->patrol_start != p->patrol_end) {
            p->wait -= seconds;
            if (p->wait <= 0) {
                if (p->actor.tile_x >= p->patrol_end) p->direction = -1;
                if (p->actor.tile_x <= p->patrol_start) p->direction = 1;
                input.horizontal = p->direction;
                p->wait = 0.8f;
            }
        }
        player_update_blocked(&p->actor,map,&input,seconds,occupied,&occupancy);
    }
}
Npc *npc_facing(Npcs *n, const Player *p)
{
    int x = p->tile_x, y = p->tile_y;
    if (p->facing == FACE_LEFT) --x;
    if (p->facing == FACE_RIGHT) ++x;
    if (p->facing == FACE_UP) --y;
    if (p->facing == FACE_DOWN) ++y;
    for (int i = 0; i < n->count; ++i)
        if (!n->people[i].actor.moving && n->people[i].actor.tile_x == x &&
            n->people[i].actor.tile_y == y) return &n->people[i];
    return 0;
}
