#include <stdio.h>
#include <pspgu.h>
#include "game.h"
#include "world_draw.h"
#include "graphics.h"
#include "text.h"

static void enter_map(Game *g, int id, int x, int y)
{
    g->map_id = id;
    g->map = map_get(id);
    player_init(&g->player,g->map);
    g->player.tile_x = g->player.target_x = x;
    g->player.tile_y = g->player.target_y = y;
    g->player.x = (float)(x*TILE_SIZE);
    g->player.y = (float)(y*TILE_SIZE);
    npc_load(&g->npcs,id);
    g->encounter.safe_steps = 4;
    camera_update(&g->camera,&g->player,g->map);
}
void game_init(Game *g)
{
    *g = (Game){0};
    encounter_init(&g->encounter,0x19236u);
    battler_starter(&g->partner);
    enter_map(g,MAP_CLEARING,5,11);
}
void game_update(Game *g, const Input *input, float seconds)
{
    if(g->in_battle) {
        battle_update(&g->battle,input);
        if(g->battle.phase==BATTLE_DONE) {
            g->in_battle=0;
            /* Phase 4 testing rule: every battle restores HP and move uses.
               Persistent attrition and healing locations arrive with RPG systems. */
            battler_restore(&g->partner);
            if(g->battle.result==BATTLE_LOSS) enter_map(g,MAP_CLEARING,5,11);
            g->encounter.safe_steps=4;
        }
        return;
    }
    if (g->dialogue.active) {
        if (input->cancel) g->dialogue.active = 0;
        else if (input->confirm) dialogue_advance(&g->dialogue);
        return;
    }
    if (input->confirm && !g->player.moving) {
        Npc *npc = npc_facing(&g->npcs,&g->player);
        if (npc) {
            npc->actor.facing = g->player.facing == FACE_UP ? FACE_DOWN :
                g->player.facing == FACE_DOWN ? FACE_UP :
                g->player.facing == FACE_LEFT ? FACE_RIGHT : FACE_LEFT;
            dialogue_open(&g->dialogue,npc->name,npc->first,npc->second);
            return;
        }
    }
    int old_x = g->player.tile_x, old_y = g->player.tile_y;
    player_update_blocked(&g->player,g->map,input,seconds,npc_blocks,&g->npcs);
    if (old_x != g->player.tile_x || old_y != g->player.tile_y) {
        const Portal *portal = map_portal(g->map_id,g->player.tile_x,g->player.tile_y);
        if (portal) {
            enter_map(g,portal->to,portal->arrival_x,portal->arrival_y);
            return;
        }
        EncounterResult result;
        int area = map_encounter_area(g->map_id,g->player.tile_x,g->player.tile_y);
        if (encounter_step(&g->encounter,area,&result)) {
            battle_begin(&g->battle,&g->partner,result.name,result.level,g->encounter.random);
            g->in_battle=1;
        }
    }
    if (!g->dialogue.active && !g->in_battle) npc_update(&g->npcs,g->map,&g->player,seconds);
    camera_update(&g->camera,&g->player,g->map);
}
void game_draw(const Game *g)
{
    if(g->in_battle) { battle_draw(&g->battle); return; }
    world_draw(g->map,&g->player,&g->camera);
    for (int i=0;i<g->npcs.count;++i)
        world_actor_draw(&g->npcs.people[i].actor,&g->camera,1);
    graphics_rectangle(0,0,480,15,GU_RGBA(18,27,30,255));
    text_draw(6,4,map_name(g->map_id),GU_RGBA(241,212,150,255),1);
    text_draw(290,4,"X TALK   O CLOSE",GU_RGBA(210,221,211,255),1);
    if (g->dialogue.active) {
        graphics_rectangle(6,167,468,99,GU_RGBA(184,150,96,255));
        graphics_rectangle(8,169,464,95,GU_RGBA(21,30,36,255));
        text_draw(18,176,g->dialogue.title,GU_RGBA(242,198,117,255),1);
        text_draw(18,191,g->dialogue.pages[g->dialogue.page],GU_RGBA(235,237,220,255),2);
        text_draw(18,251,g->dialogue.page+1 < g->dialogue.count ? "X NEXT   O CLOSE" : "X CLOSE   O CLOSE",GU_RGBA(163,189,178,255),1);
    }
}
