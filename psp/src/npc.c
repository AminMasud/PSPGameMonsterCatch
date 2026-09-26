#include "npc.h"

static const NpcBattleData east_challenger = {
    .id=NPC_BATTLE_EAST_CHALLENGER,.name="REN",
    .before={"THE EAST PATH LEADS INTO FERNVEIL.","SHOW ME ONE CALM BATTLE FIRST."},
    .victory={"YOU ARE READY FOR THE EAST WOODS.","THE PATH IS OPEN. TRAVEL SAFELY."},
    .defeat={"REST AT THE LODGE, THEN TRY AGAIN.",0},
    .party={{SPECIES_MOSSPRIG,3}},.party_count=1,
    .ai_profile=NPC_AI_EASY,.reward_embermarks=50,
    .progression_flag=PROGRESSION_FIRST_CHALLENGER_DEFEATED
};

static const NpcBattleData *battle_registry[] = {&east_challenger};

static void add(Npcs *n, int x, int y, const char *name, const char *a, const char *b, int end)
{
    Npc *p = &n->people[n->count++];
    *p = (Npc){0};
    p->actor.tile_x = p->actor.target_x = x;
    p->actor.tile_y = p->actor.target_y = y;
    p->actor.x = (float)(x*TILE_SIZE); p->actor.y = (float)(y*TILE_SIZE);
    p->actor.facing = FACE_DOWN;
    p->name = name; p->first = a; p->second = b;
    p->open_x = p->open_y = -1;
    p->patrol_start = x; p->patrol_end = end; p->direction = 1; p->wait = 1;
}
void npc_load(Npcs *n, int map_id)
{
    *n = (Npcs){0};
    if (map_id == 0) {
        add(n,7,10,"MIRA","WELCOME TO HEARTH CLEARING.\nTHE LODGE IS JUST NORTHWEST.","FOLLOW THE EAST PATH TO THE WOODS.\nPRESS X WHILE FACING SOMEONE.",7);
        add(n,9,13,"ORIN","I KEEP THIS PATH CLEAR.","THE WOODS ARE HOME TO VEYLINGS.\nLOOK FOR THEM IN THE TALL GRASS.",13);
        add(n,38,11,east_challenger.name,east_challenger.before.first,
            east_challenger.before.second,38);
        n->people[n->count-1].actor.facing=FACE_LEFT;
        n->people[n->count-1].battle=&east_challenger;
        n->people[n->count-1].gate=map_gate(MAP_CLEARING,38,11);
        n->people[n->count-1].open_x=37;
        n->people[n->count-1].open_y=10;
    } else if (map_id == 1) {
        const BossData *east_forest_guardian=boss_get(BOSS_EAST_FOREST_GUARDIAN);
        add(n,4,9,"SEN","THE DARK GRASS HIDES VEYLINGS.","THE LIT OPENING NORTHEAST LEADS\nINTO HOLLOWSTONE CAVE.",4);
        const Gate *sunthread=map_gate(MAP_FOREST,30,11);
        add(n,30,11,sunthread->gatekeeper,sunthread->locked_dialogue.first,
            sunthread->locked_dialogue.second,30);
        n->people[n->count-1].actor.facing=FACE_LEFT;
        n->people[n->count-1].gate=sunthread;
        n->people[n->count-1].open_x=29;
        n->people[n->count-1].open_y=10;
        const Gate *hollowstone=map_gate(MAP_FOREST,28,5);
        add(n,28,5,hollowstone->gatekeeper,hollowstone->locked_dialogue.first,
            hollowstone->locked_dialogue.second,28);
        n->people[n->count-1].actor.facing=FACE_DOWN;
        n->people[n->count-1].gate=hollowstone;
        n->people[n->count-1].open_x=29;
        n->people[n->count-1].open_y=6;
        add(n,26,11,east_forest_guardian->name,east_forest_guardian->intro.first,
            east_forest_guardian->intro.second,26);
        n->people[n->count-1].actor.facing=FACE_LEFT;
        n->people[n->count-1].boss=east_forest_guardian;
        n->people[n->count-1].open_x=26;
        n->people[n->count-1].open_y=10;
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
void npc_apply_progress(Npcs *n,uint32_t defeated,const ProgressionState *progression)
{
    for (int i=0;i<n->count;++i) {
        Npc *p=&n->people[i];
        int battle_cleared=p->battle && (defeated&(1u<<p->battle->id));
        int boss_cleared=p->boss && boss_is_defeated(progression,p->boss);
        int gate_open=p->gate && !gate_is_locked(p->gate,progression);
        if (p->open_x<0 || p->open_y<0 || (!battle_cleared && !boss_cleared && !gate_open)) continue;
        p->actor.tile_x=p->actor.target_x=p->open_x;
        p->actor.tile_y=p->actor.target_y=p->open_y;
        p->actor.x=(float)(p->open_x*TILE_SIZE);
        p->actor.y=(float)(p->open_y*TILE_SIZE);
        p->actor.moving=0;
        p->actor.facing=FACE_DOWN;
        p->patrol_start=p->patrol_end=p->open_x;
    }
}
void npc_reconcile_progression(uint32_t defeated,ProgressionState *progression)
{
    int count=(int)(sizeof(battle_registry)/sizeof(battle_registry[0]));
    for(int i=0;i<count;++i) {
        const NpcBattleData *battle=battle_registry[i];
        if((defeated&(1u<<(unsigned int)battle->id)) &&
           battle->progression_flag!=PROGRESSION_NONE)
            progression_set(progression,battle->progression_flag);
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
