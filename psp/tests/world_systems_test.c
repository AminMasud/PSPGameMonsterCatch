#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "game.h"
#include "graphics.h"

static unsigned char pixels[272][480][3];
static unsigned int rectangles;
void graphics_rectangle(int x,int y,int w,int h,unsigned int color)
{
    ++rectangles;
    assert(w > 0 && h > 0);
    for (int py=y;py<y+h;++py) for(int px=x;px<x+w;++px)
        if (px>=0 && px<480 && py>=0 && py<272) {
            pixels[py][px][0] = color & 255;
            pixels[py][px][1] = (color>>8)&255;
            pixels[py][px][2] = (color>>16)&255;
        }
}
static void render(const Game *g,const char *path)
{
    memset(pixels,0,sizeof(pixels)); rectangles=0;
    game_draw(g);
    assert(rectangles < 6000); /* Conservative <576 KiB GU command estimate. */
    if (path) {
        FILE *f=fopen(path,"wb"); assert(f);
        fprintf(f,"P6\n480 272\n255\n");
        assert(fwrite(pixels,1,sizeof(pixels),f)==sizeof(pixels));
        fclose(f);
    }
}
static void place(Game *g,int id,int x,int y)
{
    game_init(g); g->map_id=id; g->map=map_get(id);
    player_init(&g->player,g->map);
    g->player.tile_x=g->player.target_x=x;
    g->player.tile_y=g->player.target_y=y;
    g->player.x=(float)(x*32);g->player.y=(float)(y*32);
    npc_load(&g->npcs,id);
    camera_update(&g->camera,&g->player,g->map);
}
static void update(Game *g,Input input,int frames)
{
    for(int i=0;i<frames;++i) game_update(g,&input,0.025f);
}
static void fits(const char *text)
{
    int width=0,lines=1;
    for(;*text;++text) {
        if (*text=='\n') { assert(width<=37);width=0;++lines; }
        else ++width;
    }
    assert(width<=37 && lines<=3);
}
int main(void)
{
    int portals=0;
    for(int id=0;id<MAP_COUNT;++id) {
        const Map *m=map_get(id);
        int seen[24][40]={{0}}, queue_x[960],queue_y[960],head=0,tail=1;
        queue_x[0]=m->spawn_x;queue_y[0]=m->spawn_y;
        seen[m->spawn_y][m->spawn_x]=1;
        while(head<tail) {
            int x=queue_x[head],y=queue_y[head++];
            const int dx[]={1,-1,0,0},dy[]={0,0,1,-1};
            for(int d=0;d<4;++d) {
                int nx=x+dx[d],ny=y+dy[d];
                if(nx<0 || ny<0 || nx>=m->width || ny>=m->height ||
                   seen[ny][nx] || !map_walkable(m,nx,ny)) continue;
                assert(tail<960);
                seen[ny][nx]=1;queue_x[tail]=nx;queue_y[tail++]=ny;
            }
        }
        Npcs n; npc_load(&n,id);
        assert(map_walkable(m,m->spawn_x,m->spawn_y));
        for(int i=0;i<n.count;++i) {
            assert(map_walkable(m,n.people[i].actor.tile_x,n.people[i].actor.tile_y));
            fits(n.people[i].first); fits(n.people[i].second);
        }
        for(int y=0;y<m->height;++y) {
            assert(strlen(m->rows[y])==(size_t)m->width);
            for(int x=0;x<m->width;++x) {
                const Portal *p=map_portal(id,x,y);
                if(!p) continue;
                ++portals;
                assert(seen[y][x]);
                assert(map_walkable(m,x,y));
                assert(map_walkable(map_get(p->to),p->arrival_x,p->arrival_y));
                assert(!map_portal(p->to,p->arrival_x,p->arrival_y));
                Npcs destination; npc_load(&destination,p->to);
                assert(!npc_blocks(&destination,p->arrival_x,p->arrival_y));
            }
        }
    }
    assert(portals==6);
    Game g;
    place(&g,MAP_CLEARING,5,10);
    update(&g,(Input){0,-1,0,0,0},10);
    assert(g.map_id==MAP_LODGE);
    update(&g,(Input){0,1,0,0,0},10);
    assert(g.map_id==MAP_CLEARING && g.player.tile_y==10);
    place(&g,MAP_CLEARING,37,11);
    update(&g,(Input){1,0,0,0,0},10);
    assert(g.map_id==MAP_FOREST);
    update(&g,(Input){-1,0,0,0,0},10);
    assert(g.map_id==MAP_CLEARING);
    place(&g,MAP_FOREST,28,6);
    update(&g,(Input){0,-1,0,0,0},10);
    assert(g.map_id==MAP_CAVE);
    update(&g,(Input){0,1,0,0,0},10);
    assert(g.map_id==MAP_FOREST);

    place(&g,MAP_CLEARING,7,11);
    update(&g,(Input){0,-1,0,0,0},20);
    assert(g.player.tile_y==11); /* NPC is solid. */
    update(&g,(Input){0,0,1,0,0},1);
    assert(g.dialogue.active && !strcmp(g.dialogue.title,"MIRA"));
    float npc_x=g.npcs.people[1].actor.x;
    update(&g,(Input){1,0,0,0,0},100);
    assert(g.player.tile_x==7 && g.npcs.people[1].actor.x==npc_x);
    render(&g,"previews/dialogue.ppm");
    update(&g,(Input){0,0,1,0,0},1);
    assert(g.dialogue.page==1);
    update(&g,(Input){0,0,0,1,0},1);
    assert(!g.dialogue.active);
    update(&g,(Input){0,0,0,0,0},100);
    assert(g.npcs.people[1].actor.x!=npc_x);
    for(int i=0;i<1000;++i) {
        update(&g,(Input){0},1);
        const Player *p=&g.npcs.people[1].actor;
        assert(p->x>=9*32 && p->x<=13*32);
    }
    place(&g,MAP_CLEARING,10,13);
    update(&g,(Input){0},200);
    assert(g.npcs.people[1].actor.tile_x==9); /* Patrol cannot enter player. */
    place(&g,MAP_FOREST,8,12);
    update(&g,(Input){0},1000);
    assert(!g.dialogue.active && !g.in_battle); /* Only completed steps roll. */
    Encounter e; EncounterResult result;
    encounter_init(&e,123);
    for(int i=0;i<1000;++i) assert(!encounter_step(&e,0,&result));
    for(int area=1;area<=2;++area) {
        int count=0;
        encounter_init(&e,123);
        for(int i=0;i<1000;++i) if(encounter_step(&e,area,&result)) {
            assert(result.level>=(area==1?2:3) && result.level<=(area==1?6:7));
            ++count;
            for(int j=0;j<4;++j) assert(!encounter_step(&e,area,&result));
        }
        assert(count>50);
    }
    place(&g,MAP_FOREST,8,12);
    for(int i=0;i<2000 && !g.in_battle;++i)
        update(&g,(Input){g.player.tile_x>=15?-1:1,0,0,0,0},1);
    assert(g.in_battle);
    fits(g.battle.message);
    render(&g,"previews/encounter.ppm");
    float before_x=g.player.x,before_y=g.player.y;
    update(&g,(Input){1,0,0,0,0},40);
    assert(g.player.x==before_x && g.player.y==before_y);
    update(&g,(Input){0,0,1,0,0},1);
    assert(g.battle.phase==BATTLE_MENU);
    render(&g,"previews/battle-menu.ppm");
    update(&g,(Input){0,0,1,0,0},1);
    render(&g,"previews/battle-moves.ppm");
    update(&g,(Input){0,0,0,1,0},1);
    g.battle.cursor=4;g.battle.escape_attempts=2;
    update(&g,(Input){0,0,1,0,0},2);
    assert(!g.in_battle && g.map_id==MAP_FOREST);
    assert(g.player.x==before_x && g.player.y==before_y);
    assert(g.partner.hp==g.partner.max_hp && g.encounter.safe_steps==4);
    battle_begin(&g.battle,&g.partner,SPECIES_ECHOCRAG,7,99);g.in_battle=1;
    g.battle.ally.hp=1;g.battle.enemy.speed=999;
    for(int i=0;i<4;++i) g.battle.enemy.moves[i]=MOVE_NUDGE;
    update(&g,(Input){0,0,1,0,0},3);
    assert(g.battle.result==BATTLE_LOSS);
    update(&g,(Input){0,0,1,0,0},2);
    assert(!g.in_battle && g.map_id==MAP_CLEARING && g.player.tile_x==5);
    assert(g.partner.hp==g.partner.max_hp);
    /* Real game integration: victory -> learning choice -> persistent partner. */
    creature_create(&g.partner,SPECIES_CINDLET,5);
    g.partner.experience=creature_xp_for_level(6)-1;
    battle_begin(&g.battle,&g.partner,SPECIES_MOSSLET,3,42);g.in_battle=1;
    g.battle.enemy.hp=1;g.battle.ally.speed=999;
    update(&g,(Input){0,0,1,0,0},4);
    assert(g.battle.ally.level==6);
    update(&g,(Input){0,0,1,0,0},2);
    assert(g.battle.phase==BATTLE_LEARN);
    render(&g,"previews/learn-move.ppm");
    g.battle.learn_cursor=1;update(&g,(Input){0,0,1,0,0},1);
    for(int i=0;i<20 && g.in_battle;++i) update(&g,(Input){0,0,1,0,0},1);
    assert(!g.in_battle && g.partner.level==6 && g.partner.moves[1]==MOVE_HEAT);
    int saved_xp=g.partner.experience;
    update(&g,(Input){0},5);assert(g.partner.experience==saved_xp);

    creature_create(&g.partner,SPECIES_CINDLET,7);
    g.partner.experience=creature_xp_for_level(8)-1;
    battle_begin(&g.battle,&g.partner,SPECIES_MOSSLET,3,42);g.in_battle=1;
    g.battle.enemy.hp=1;g.battle.ally.speed=999;g.battle.ally.moves[0]=MOVE_NUDGE;
    update(&g,(Input){0,0,1,0,0},6);
    assert(g.battle.ally.species==SPECIES_EMBERLYN && strstr(g.battle.message,"EVOLUTION"));
    render(&g,"previews/evolution.ppm");
    for(int i=0;i<25 && g.in_battle;++i) {
        if(g.battle.phase==BATTLE_LEARN) update(&g,(Input){0,0,0,1,0},1);
        else update(&g,(Input){0,0,1,0,0},1);
    }
    assert(!g.in_battle && g.partner.species==SPECIES_EMBERLYN);
    saved_xp=g.partner.experience;
    g.player.tile_x=g.player.target_x=5;g.player.tile_y=g.player.target_y=10;
    g.player.x=160;g.player.y=320;g.player.moving=0;
    update(&g,(Input){0,-1,0,0,0},10);
    assert(g.map_id==MAP_LODGE && g.partner.species==SPECIES_EMBERLYN && g.partner.experience==saved_xp);
    update(&g,(Input){0,0,0,0,1},1);
    assert(g.dialogue.active);fits(g.dialogue.pages[0]);fits(g.dialogue.pages[1]);
    render(&g,"previews/partner.ppm");
    battle_begin(&g.battle,&g.partner,SPECIES_MOSSLET,3,42);
    assert(g.battle.ally.species==SPECIES_EMBERLYN && strstr(g.battle.message,"EMBERLYN IS READY"));
    for(int id=0;id<MAP_COUNT;++id) {
        place(&g,id,map_get(id)->spawn_x,map_get(id)->spawn_y);
        render(&g,0);
    }
    puts("PASS: world systems, battle return, learning/evolution persistence, partner summary, drawing budget");
    return 0;
}
