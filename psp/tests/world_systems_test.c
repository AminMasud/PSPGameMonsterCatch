#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "game.h"
#include "graphics.h"
#include "pet_draw.h"
#include "text.h"

static unsigned char pixels[272][480][3];
static unsigned int rectangles;
static unsigned int textures;
static unsigned int texture_tints[7];
void graphics_texture_tinted(int x,int y,int w,int h,const uint16_t *texture,int flip,unsigned int tint)
{
    assert(textures<7);texture_tints[textures]=tint;
    ++textures;
    assert(w>0 && h>0 && texture);
    for(int py=0;py<h;++py) for(int px=0;px<w;++px) {
        int tx=px*96/w,ty=py*96/h;
        if(flip) tx=95-tx;
        uint16_t value=texture[ty*128+tx];
        int alpha=(value>>12)*17;
        if(x+px<0 || x+px>=480 || y+py<0 || y+py>=272) continue;
        for(int channel=0;channel<3;++channel) {
            int source=((value>>(channel*4))&15)*17;
            source=source*(int)((tint>>(channel*8))&255)/255;
            unsigned char *dest=&pixels[y+py][x+px][channel];
            *dest=(unsigned char)((source*alpha+*dest*(255-alpha)+127)/255);
        }
    }
}
void graphics_texture(int x,int y,int w,int h,const uint16_t *texture,int flip)
{
    graphics_texture_tinted(x,y,w,h,texture,flip,0xffffffffu);
}
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
    memset(pixels,0,sizeof(pixels));memset(texture_tints,0,sizeof(texture_tints));
    rectangles=0;textures=0;
    game_draw(g);
    assert(rectangles < 6000); /* Conservative <576 KiB GU command estimate. */
    assert(textures<=7); /* At most six list portraits and one detail portrait. */
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
    int lines=text_wrap(0,0,444,0,text,0xffffffff,1);
    assert(lines==0 || (lines-1)*9+7<=53);
}
static const NpcBattleData framework_challenger={
    .id=3,.name="ARCHIVIST",
    .before={"SHOW ME HOW YOUR TEAM MOVES.","WE WILL USE TWO VEYLINGS."},
    .victory={"YOUR TEAM WORKED AS ONE.","TAKE THESE EMBERMARKS."},
    .defeat={"REST YOUR TEAM AND RETURN.",0},
    .party={{SPECIES_MOSSPRIG,4},{SPECIES_ZAPPIP,5}},.party_count=2,
    .ai_profile=NPC_AI_STANDARD,.reward_embermarks=75,
    .progression_flag=PROGRESSION_CAVE_UNLOCKED
};
static const NpcBattleData framework_defeat={
    .id=4,.name="WARDEN",
    .before={"THIS IS A DEFEAT-FLOW TEST.",0},
    .victory={"YOU PREVAILED.",0},.defeat={"RETURN WHEN YOU ARE READY.",0},
    .party={{SPECIES_GRUBBL,5}},.party_count=1,
    .ai_profile=NPC_AI_BOSS,.reward_embermarks=200,
    .progression_flag=PROGRESSION_NORTH_FOREST_BOSS_DEFEATED
};
int main(void)
{
    int portals=0,locked_gates=0;
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
            if(n.people[i].battle) assert(npc_battle_data_valid(n.people[i].battle));
        }
        for(int y=0;y<m->height;++y) {
            assert(strlen(m->rows[y])==(size_t)m->width);
            for(int x=0;x<m->width;++x) {
                const Portal *p=map_portal(id,x,y);
                if(!p) continue;
                ++portals;
                assert(gate_valid(p));
                if(p->required_flag==PROGRESSION_NONE)
                    assert(gate_can_enter(p,0));
                else {
                    ++locked_gates;
                    assert(gate_is_locked(p,0) && !gate_can_enter(p,0));
                    assert(p->gatekeeper && gate_current_dialogue(p,0));
                }
                assert(seen[y][x]);
                assert(map_walkable(m,x,y));
                assert(map_walkable(map_get(p->to),p->arrival_x,p->arrival_y));
                assert(!map_portal(p->to,p->arrival_x,p->arrival_y));
                Npcs destination; npc_load(&destination,p->to);
                assert(!npc_blocks(&destination,p->arrival_x,p->arrival_y));
            }
        }
    }
    assert(portals==10 && locked_gates==1);
    Game g;
    place(&g,MAP_CLEARING,5,10);
    update(&g,(Input){0,-1,0,0,0,0},10);
    assert(g.map_id==MAP_LODGE);
    update(&g,(Input){0,1,0,0,0,0},10);
    assert(g.map_id==MAP_CLEARING && g.player.tile_y==10);

    /* Phase 16: Ren guards the east portal until the first easy NPC victory. */
    place(&g,MAP_CLEARING,37,11);
    const Gate *east_gate=map_gate(MAP_CLEARING,38,11);
    assert(east_gate && east_gate->to==MAP_FOREST && east_gate->arrival_x==2 &&
           east_gate->arrival_y==11 && east_gate->required_flag==PROGRESSION_FIRST_CHALLENGER_DEFEATED);
    assert(gate_is_locked(east_gate,&g.progression) && !gate_can_enter(east_gate,&g.progression));
    const GateDialogue *gate_words=gate_current_dialogue(east_gate,&g.progression);
    assert(gate_words && !strcmp(gate_words->first,"THE EAST PATH LEADS INTO FERNVEIL."));
    assert(g.npcs.count==3 && g.npcs.people[2].battle);
    assert(g.npcs.people[2].gate==east_gate);
    assert(g.npcs.people[2].battle->id==NPC_BATTLE_EAST_CHALLENGER);
    assert(g.npcs.people[2].actor.tile_x==38 && g.npcs.people[2].actor.tile_y==11);
    render(&g,"previews/east-challenger.ppm");
    Npc east_guard=g.npcs.people[2];g.npcs.count=2;
    update(&g,(Input){.horizontal=1},10);
    assert(g.player.tile_x==37 && g.map_id==MAP_CLEARING); /* The gate itself is solid while locked. */
    g.npcs.people[2]=east_guard;g.npcs.count=3;
    update(&g,(Input){1,0,0,0,0,0},10);
    assert(g.map_id==MAP_CLEARING && g.player.tile_x==37); /* Ren blocks the portal. */
    int east_marks=g.inventory.embermarks;
    update(&g,(Input){.confirm=1},1);
    assert(g.dialogue.active && !strcmp(g.dialogue.title,"REN"));
    assert(!strcmp(g.dialogue.pages[0],east_gate->locked_dialogue.first));
    update(&g,(Input){.confirm=1},1);
    update(&g,(Input){.confirm=1},1);
    assert(g.ready_prompt.active && g.npc_battle.flow==NPC_BATTLE_FLOW_READY);
    render(&g,"previews/east-challenger-ready.ppm");
    update(&g,(Input){.confirm=1},1);
    assert(g.in_battle && g.battle.npc_battle && g.battle.enemy_count==1);
    assert(g.battle.enemy.species==SPECIES_MOSSPRIG && g.battle.enemy.level==3);
    assert(g.battle.ai_profile==NPC_AI_EASY);
    g.transition=0;
    render(&g,"previews/east-challenger-battle.ppm");
    g.battle.phase=BATTLE_DONE;g.battle.result=BATTLE_WIN;
    update(&g,(Input){0},1);
    assert(!g.in_battle && npc_battle_is_defeated(&g.npc_battle_progress,
           NPC_BATTLE_EAST_CHALLENGER));
    assert(progression_has(&g.progression,PROGRESSION_FIRST_CHALLENGER_DEFEATED));
    assert(!gate_is_locked(east_gate,&g.progression) && gate_can_enter(east_gate,&g.progression));
    assert(g.inventory.embermarks==east_marks+50 && g.dialogue.active);
    assert(g.npcs.people[2].actor.tile_x==37 && g.npcs.people[2].actor.tile_y==10);
    g.transition=0;
    render(&g,"previews/east-challenger-victory.ppm");
    update(&g,(Input){.cancel=1},1);
    g.player.facing=FACE_UP;
    update(&g,(Input){.confirm=1},1);
    assert(g.dialogue.active && g.npc_battle.flow==NPC_BATTLE_FLOW_NONE &&
           !g.ready_prompt.active); /* A defeated Ren only repeats victory dialogue. */
    assert(!strcmp(g.dialogue.pages[0],east_gate->unlocked_dialogue.first));
    update(&g,(Input){.cancel=1},1);
    update(&g,(Input){.horizontal=1},10);
    assert(g.map_id==MAP_FOREST);

    place(&g,MAP_FOREST,29,11);
    update(&g,(Input){.horizontal=1},10);
    assert(g.map_id==MAP_MARSH);
    update(&g,(Input){.horizontal=-1},10);
    assert(g.map_id==MAP_FOREST);
    place(&g,MAP_MARSH,24,5);
    update(&g,(Input){.vertical=-1},10);
    assert(g.map_id==MAP_REST);
    update(&g,(Input){.vertical=1},10);
    assert(g.map_id==MAP_MARSH);
    place(&g,MAP_FOREST,2,11);
    update(&g,(Input){-1,0,0,0,0,0},10);
    assert(g.map_id==MAP_CLEARING);
    place(&g,MAP_FOREST,28,6);
    update(&g,(Input){0,-1,0,0,0,0},10);
    assert(g.map_id==MAP_CAVE);
    update(&g,(Input){0,1,0,0,0,0},10);
    assert(g.map_id==MAP_FOREST);

    place(&g,MAP_CLEARING,7,11);
    update(&g,(Input){0,-1,0,0,0,0},20);
    assert(g.player.tile_y==11); /* NPC is solid. */
    update(&g,(Input){0,0,1,0,0,0},1);
    assert(g.dialogue.active && !strcmp(g.dialogue.title,"MIRA"));
    float npc_x=g.npcs.people[1].actor.x;
    update(&g,(Input){1,0,0,0,0,0},100);
    assert(g.player.tile_x==7 && g.npcs.people[1].actor.x==npc_x);
    render(&g,"previews/dialogue.ppm");
    update(&g,(Input){0,0,1,0,0,0},1);
    assert(g.dialogue.page==1);
    update(&g,(Input){0,0,0,1,0,0},1);
    assert(!g.dialogue.active);
    update(&g,(Input){0,0,0,0,0,0},100);
    assert(g.npcs.people[1].actor.x!=npc_x);
    for(int i=0;i<1000;++i) {
        update(&g,(Input){0},1);
        const Player *p=&g.npcs.people[1].actor;
        assert(p->x>=9*32 && p->x<=13*32);
    }
    place(&g,MAP_CLEARING,10,13);
    update(&g,(Input){0},200);
    assert(g.npcs.people[1].actor.tile_x==9); /* Patrol cannot enter player. */

    /* Important battles use a reusable ready prompt; declining preserves the world. */
    assert(!game_offer_important_battle(&g,"INVALID",SPECIES_COUNT,6,1234));
    assert(!game_offer_important_battle(&g,"INVALID",SPECIES_MOSSPRIG,0,1234));
    int ready_x=g.player.tile_x,ready_y=g.player.tile_y;
    npc_x=g.npcs.people[1].actor.x;
    assert(game_offer_important_battle(&g,"MIRA'S CHALLENGE",SPECIES_MOSSPRIG,6,1234));
    assert(g.ready_prompt.active && g.ready_prompt.cursor==0 && !g.in_battle);
    assert(g.pending_battle.species==SPECIES_MOSSPRIG && g.pending_battle.level==6);
    assert(!game_offer_important_battle(&g,"SECOND CHALLENGE",SPECIES_ZAPPIP,7,2));
    render(&g,"previews/ready-prompt.ppm");
    update(&g,(Input){.horizontal=1},40);
    assert(g.player.tile_x==ready_x && g.player.tile_y==ready_y);
    assert(g.npcs.people[1].actor.x==npc_x);
    update(&g,(Input){.vertical=1},10);
    assert(g.ready_prompt.cursor==1); /* A held direction moves only once. */
    render(&g,"previews/ready-prompt-no.ppm");
    update(&g,(Input){0},1);
    update(&g,(Input){.confirm=1},1);
    assert(!g.ready_prompt.active && !g.in_battle && g.pending_battle.level==0);
    assert(g.player.tile_x==ready_x && g.player.tile_y==ready_y);
    assert(game_offer_important_battle(&g,"MIRA'S CHALLENGE",SPECIES_ZAPPIP,7,99));
    update(&g,(Input){.cancel=1},1);
    assert(!g.ready_prompt.active && !g.in_battle);
    assert(game_offer_important_battle(&g,"MIRA'S CHALLENGE",SPECIES_MOSSPRIG,6,1234));
    update(&g,(Input){.confirm=1},1);
    assert(!g.ready_prompt.active && g.in_battle);
    assert(g.battle.enemy.species==SPECIES_MOSSPRIG && g.battle.enemy.level==6);

    /* Phase 14 framework: intro -> ready prompt -> NPC party -> persistent outcome. */
    place(&g,MAP_CLEARING,10,13);
    assert(npc_battle_data_valid(&framework_challenger));
    fits(framework_challenger.before.first);fits(framework_challenger.before.second);
    fits(framework_challenger.victory.first);fits(framework_challenger.victory.second);
    int marks_before=g.inventory.embermarks;
    assert(game_offer_npc_battle(&g,&framework_challenger,77));
    assert(g.dialogue.active && g.npc_battle.flow==NPC_BATTLE_FLOW_INTRO);
    update(&g,(Input){.cancel=1},1);
    assert(!g.dialogue.active && g.npc_battle.flow==NPC_BATTLE_FLOW_NONE);
    assert(game_offer_npc_battle(&g,&framework_challenger,77));
    update(&g,(Input){.confirm=1},1);
    assert(g.dialogue.active && g.dialogue.page==1);
    update(&g,(Input){.confirm=1},1);
    assert(!g.dialogue.active && g.ready_prompt.active &&
           g.npc_battle.flow==NPC_BATTLE_FLOW_READY);
    update(&g,(Input){.confirm=1},1);
    assert(g.in_battle && g.npc_battle.flow==NPC_BATTLE_FLOW_ACTIVE);
    assert(g.battle.npc_battle && g.battle.enemy_count==2 &&
           g.battle.ai_profile==NPC_AI_STANDARD);
    g.transition=0;
    render(&g,"previews/npc-battle.ppm");
    g.battle.phase=BATTLE_DONE;g.battle.result=BATTLE_WIN;
    update(&g,(Input){0},1);
    assert(!g.in_battle && npc_battle_is_defeated(&g.npc_battle_progress,framework_challenger.id));
    assert(progression_has(&g.progression,framework_challenger.progression_flag));
    assert(g.inventory.embermarks==marks_before+framework_challenger.reward_embermarks);
    assert(g.dialogue.active && !strcmp(g.dialogue.title,framework_challenger.name));
    update(&g,(Input){.cancel=1},1);
    assert(game_offer_npc_battle(&g,&framework_challenger,99));
    assert(g.dialogue.active && g.npc_battle.flow==NPC_BATTLE_FLOW_NONE && !g.ready_prompt.active);
    assert(g.inventory.embermarks==marks_before+framework_challenger.reward_embermarks);
    update(&g,(Input){.cancel=1},1);

    assert(game_offer_npc_battle(&g,&framework_defeat,88));
    update(&g,(Input){.confirm=1},1);
    assert(g.ready_prompt.active);
    update(&g,(Input){.confirm=1},1);
    assert(g.in_battle && g.battle.ai_profile==NPC_AI_BOSS);
    g.battle.phase=BATTLE_DONE;g.battle.result=BATTLE_LOSS;
    update(&g,(Input){0},1);
    assert(!g.in_battle && g.map_id==MAP_CLEARING && g.player.tile_x==5 && g.player.tile_y==11);
    assert(!npc_battle_is_defeated(&g.npc_battle_progress,framework_defeat.id));
    assert(g.dialogue.active && strstr(g.dialogue.pages[0],"RETURN WHEN"));
    update(&g,(Input){.cancel=1},1);

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
            assert(result.level>=(area==1?2:3) && result.level<=18);
            ++count;
            for(int j=0;j<4;++j) assert(!encounter_step(&e,area,&result));
        }
        assert(count>50);
    }
    place(&g,MAP_FOREST,8,12);
    for(int i=0;i<2000 && !g.in_battle;++i)
        update(&g,(Input){g.player.tile_x>=15?-1:1,0,0,0,0,0},1);
    assert(g.in_battle && !g.ready_prompt.active); /* Wild encounters skip the ready prompt. */
    fits(g.battle.message);
    render(&g,"previews/encounter.ppm");
    float before_x=g.player.x,before_y=g.player.y;
    update(&g,(Input){1,0,0,0,0,0},40);
    assert(g.player.x==before_x && g.player.y==before_y);
    update(&g,(Input){0,0,1,0,0,0},1);
    assert(g.battle.phase==BATTLE_MENU);
    render(&g,"previews/battle-menu.ppm");
    g.battle.ally.hp-=10;
    g.battle.cursor=3;update(&g,(Input){.confirm=1},1);
    assert(g.battle.phase==BATTLE_ITEMS);
    render(&g,"previews/items.ppm");
    update(&g,(Input){.cancel=1},1);
    assert(g.battle.phase==BATTLE_MENU);
    g.battle.cursor=0;
    update(&g,(Input){0,0,1,0,0,0},1);
    render(&g,"previews/battle-moves.ppm");
    update(&g,(Input){0,0,0,1,0,0},1);
    g.battle.cursor=4;g.battle.escape_attempts=2;
    update(&g,(Input){0,0,1,0,0,0},2);
    assert(!g.in_battle && g.map_id==MAP_FOREST);
    assert(g.player.x==before_x && g.player.y==before_y);
    assert(g.party.members[g.party.lead].hp==g.party.members[g.party.lead].max_hp && g.encounter.safe_steps==4);
    battle_begin(&g.battle,&g.party.members[g.party.lead],SPECIES_TITANOCERA,7,99);g.in_battle=1;
    g.battle.ally.hp=1;g.battle.enemy.speed=999;
    for(int i=0;i<4;++i) g.battle.enemy.moves[i]=MOVE_NUDGE;
    update(&g,(Input){0,0,1,0,0,0},3);
    assert(g.battle.result==BATTLE_LOSS);
    update(&g,(Input){0,0,1,0,0,0},2);
    assert(!g.in_battle && g.map_id==MAP_CLEARING && g.player.tile_x==5);
    assert(g.party.members[g.party.lead].hp==g.party.members[g.party.lead].max_hp);
    /* Real game integration: victory -> learning choice -> persistent partner. */
    creature_create(&g.party.members[g.party.lead],SPECIES_CINDLET,5);
    g.party.members[g.party.lead].experience=creature_xp_for_level(6)-1;
    battle_begin(&g.battle,&g.party.members[g.party.lead],SPECIES_MOSSPRIG,3,42);g.in_battle=1;
    g.battle.enemy.hp=1;g.battle.ally.speed=999;
    update(&g,(Input){0,0,1,0,0,0},4);
    assert(g.battle.ally.level==6);
    update(&g,(Input){0,0,1,0,0,0},2);
    assert(g.battle.phase==BATTLE_LEARN);
    render(&g,"previews/learn-move.ppm");
    g.battle.learn_cursor=1;update(&g,(Input){0,0,1,0,0,0},1);
    for(int i=0;i<20 && g.in_battle;++i) update(&g,(Input){0,0,1,0,0,0},1);
    assert(!g.in_battle && g.party.members[g.party.lead].level==6 && g.party.members[g.party.lead].moves[1]==MOVE_HEAT);
    int saved_xp=g.party.members[g.party.lead].experience;
    update(&g,(Input){0},5);assert(g.party.members[g.party.lead].experience==saved_xp);

    creature_create(&g.party.members[g.party.lead],SPECIES_CINDLET,7);
    g.party.members[g.party.lead].experience=creature_xp_for_level(8)-1;
    battle_begin(&g.battle,&g.party.members[g.party.lead],SPECIES_MOSSPRIG,3,42);g.in_battle=1;
    g.battle.enemy.hp=1;g.battle.ally.speed=999;g.battle.ally.moves[0]=MOVE_NUDGE;
    update(&g,(Input){0,0,1,0,0,0},6);
    assert(g.battle.ally.species==SPECIES_EMBERYN && strstr(g.battle.message,"EVOLUTION"));
    render(&g,"previews/evolution.ppm");
    for(int i=0;i<25 && g.in_battle;++i) {
        if(g.battle.phase==BATTLE_LEARN) update(&g,(Input){0,0,0,1,0,0},1);
        else update(&g,(Input){0,0,1,0,0,0},1);
    }
    assert(!g.in_battle && g.party.members[g.party.lead].species==SPECIES_EMBERYN);
    saved_xp=g.party.members[g.party.lead].experience;
    g.player.tile_x=g.player.target_x=5;g.player.tile_y=g.player.target_y=10;
    g.player.x=160;g.player.y=320;g.player.moving=0;
    update(&g,(Input){0,-1,0,0,0,0},10);
    assert(g.map_id==MAP_LODGE && g.party.members[g.party.lead].species==SPECIES_EMBERYN && g.party.members[g.party.lead].experience==saved_xp);
    update(&g,(Input){0,0,0,0,1,0},1);
    assert(g.dialogue.active);fits(g.dialogue.pages[0]);fits(g.dialogue.pages[1]);
    render(&g,"previews/partner.ppm");
    battle_begin(&g.battle,&g.party.members[g.party.lead],SPECIES_MOSSPRIG,3,42);
    assert(g.battle.ally.species==SPECIES_EMBERYN && strstr(g.battle.message,"EMBERYN IS READY"));
    /* Capture into a full party flows through battle into collection management. */
    place(&g,MAP_CLEARING,5,11);
    Creature extra;
    for(int i=0;i<3;++i) {
        creature_create(&extra,SPECIES_MOSSPRIG+i,4+i);
        assert(party_add(&g.party,&extra)==1);
    }
    battle_begin_party(&g.battle,&g.party,SPECIES_MOSSPRIG,3,1);g.in_battle=1;
    update(&g,(Input){.confirm=1},1);
    g.battle.cursor=1;update(&g,(Input){.confirm=1},1);
    assert(g.battle.phase==BATTLE_CAPTURE);
    render(&g,"previews/capture.ppm");
    g.battle.enemy.hp=1;
    g.battle.random=3; /* Known successful roll after round-start AI selection. */
    update(&g,(Input){.confirm=1},1);
    assert(g.battle.result==BATTLE_CAUGHT);
    render(&g,"previews/captured.ppm");
    update(&g,(Input){.confirm=1},1);
    assert(!g.in_battle && g.party.count==4 && g.party.stored==1);
    assert(g.party.collection[0].species==SPECIES_MOSSPRIG && g.party.collection[0].level==3);
    assert(g.party.collection[0].hp==g.party.collection[0].max_hp);
    update(&g,(Input){.menu=1},1);
    assert(g.menu_open && !g.roster_open && g.transition==0);
    render(&g,"previews/player-menu.ppm");
    update(&g,(Input){.confirm=1},1);
    assert(g.roster_open);
    render(&g,"previews/party.ppm");
    before_x=g.player.x;
    update(&g,(Input){.horizontal=1},1);
    assert(g.roster.tab==1 && g.player.x==before_x);
    render(&g,"previews/collection.ppm");
    update(&g,(Input){.confirm=1},1);
    assert(g.roster.mode==PARTY_MENU_SWAP);
    render(&g,"previews/collection-swap.ppm");
    update(&g,(Input){.confirm=1},1);
    assert(g.party.members[0].species==SPECIES_MOSSPRIG && g.party.collection[0].species==SPECIES_CINDLET);
    update(&g,(Input){.horizontal=-1},1);
    update(&g,(Input){.vertical=1},1);
    update(&g,(Input){.confirm=1},2);
    assert(g.party.lead==1);
    update(&g,(Input){.menu=1},1);
    assert(!g.roster_open);
    battle_begin_party(&g.battle,&g.party,SPECIES_GRUBBL,5,2);g.in_battle=1;
    assert(g.battle.active==1 && g.battle.ally.species==g.party.members[1].species);
    g.battle.enemy.speed=0;
    update(&g,(Input){.confirm=1},1);
    g.battle.cursor=2;update(&g,(Input){.confirm=1},1);
    render(&g,"previews/battle-switch.ppm");
    g.battle.switch_cursor=0;update(&g,(Input){.confirm=1},2);
    assert(g.battle.active==0 && g.battle.phase==BATTLE_MENU);
    g.battle.cursor=4;g.battle.escape_attempts=2;update(&g,(Input){.confirm=1},2);
    assert(!g.in_battle && g.party.lead==0 && g.party.stored==1);

    /* Final-member deposit protection, withdrawal, empty state, and full scrolling. */
    party_init(&g.party);party_menu_open(&g.roster);g.roster_open=1;
    update(&g,(Input){.confirm=1},1);update(&g,(Input){.vertical=1},1);
    update(&g,(Input){.confirm=1},1);
    assert(g.party.count==1 && !g.party.stored);
    update(&g,(Input){.cancel=1},1);update(&g,(Input){.horizontal=1},1);
    render(&g,"previews/collection-empty.ppm");
    creature_create(&extra,SPECIES_GRUBBL,5);party_add(&g.party,&extra);
    assert(party_deposit(&g.party,1));
    update(&g,(Input){.confirm=1},1);
    assert(g.party.count==2 && g.party.stored==0);
    while(party_has_space(&g.party)) assert(party_add(&g.party,&extra));
    for(int i=0;i<23;++i) { update(&g,(Input){0},1);update(&g,(Input){.vertical=1},1); }
    assert(g.roster.cursor[1]==23 && g.roster.scroll[1]==18);
    render(&g,"previews/collection-full.ppm");
    for(int id=0;id<MAP_COUNT;++id) {
        place(&g,id,map_get(id)->spawn_x,map_get(id)->spawn_y);
        render(&g,0);
    }
    place(&g,MAP_LODGE,2,2);
    g.player.facing=FACE_UP;
    g.party.members[0].hp=1;g.party.members[0].uses[0]=0;
    update(&g,(Input){.confirm=1},1);
    assert(g.dialogue.active && g.party.members[0].hp==g.party.members[0].max_hp &&
           g.party.members[0].uses[0]==attack_get(g.party.members[0].moves[0])->uses);
    place(&g,MAP_LODGE,7,4);
    g.player.facing=FACE_UP;
    update(&g,(Input){.confirm=1},1);
    assert(g.dialogue.active);
    update(&g,(Input){.confirm=1},1);
    update(&g,(Input){.confirm=1},1);
    assert(g.shop_open);
    render(&g,"previews/shop.ppm");
    int marks=g.inventory.embermarks, tonics=g.inventory.quantities[ITEM_PULSE_TONIC];
    update(&g,(Input){.confirm=1},1);
    assert(g.inventory.embermarks==marks-inventory_item_price(ITEM_PULSE_TONIC) &&
           g.inventory.quantities[ITEM_PULSE_TONIC]==tonics+1);
    update(&g,(Input){.cancel=1},1);
    assert(!g.shop_open);
    /* New area content and menu editing are rendered with the real game code. */
    place(&g,MAP_MARSH,19,10);
    render(&g,"previews/marsh.ppm");
    place(&g,MAP_REST,7,6);
    render(&g,"previews/lantern-rest.ppm");
    place(&g,MAP_CLEARING,5,11);
    update(&g,(Input){.menu=INPUT_MENU_OPEN},1);
    g.menu.cursor=1;update(&g,(Input){.confirm=1},1);
    assert(g.menu.page==MENU_BAG);
    g.party.members[0].hp-=10;
    update(&g,(Input){.confirm=1},1);
    assert(g.party.members[0].hp==g.party.members[0].max_hp && g.inventory.quantities[0]==2);
    render(&g,"previews/field-items.ppm");
    update(&g,(Input){.cancel=1},1);g.menu.cursor=5;
    update(&g,(Input){.confirm=1},1);
    assert(g.menu.page==MENU_OPTIONS);
    update(&g,(Input){.confirm=1},1);
    assert(!g.options.music);
    render(&g,"previews/options.ppm");
    int px=g.player.tile_x;
    update(&g,(Input){.horizontal=1},40);
    assert(g.player.tile_x==px);
    update(&g,(Input){.menu=INPUT_MENU_OPEN},1);
    assert(!g.menu_open);
    int species_seen[SPECIES_COUNT]={0};
    for(int area=1;area<=3;++area) {
        encounter_init(&e,456);
        for(int i=0;i<200000;++i) if(encounter_step(&e,area,&result)) {
            int family=result.species/3,stage=result.species%3;
            assert(result.species>=0 && result.species<SPECIES_COUNT);
            if(area==1) assert(family==2 || family==7 || family==8 || family==9);
            if(area==2) assert(family==0 || family==4 || family==5);
            if(area==3) assert(family==1 || family==3 || family==6);
            int minimum=stage==2?16:stage==1?8:area==1?2:area==2?3:5;
            assert(result.level>=minimum && result.level<=minimum+2);
            ++species_seen[result.species];
        }
    }
    for(int i=0;i<SPECIES_COUNT;++i) assert(species_seen[i]>0);
    battle_begin_party(&g.battle,&g.party,SPECIES_ZAPPIP,7,42);g.in_battle=1;
    render(&g,"previews/zappip.ppm");
    battle_begin_party(&g.battle,&g.party,SPECIES_BUBFIN,6,42);
    render(&g,"previews/bubfin.ppm");
    g.battle.enemy.hp-=10;g.battle.hit_time=.2f;g.battle.hit_side=1;
    battle_animate(&g.battle,.05f,1);
    assert(g.battle.enemy_hp_shown>g.battle.enemy.hp && g.battle.enemy_hp_shown<g.battle.enemy.max_hp);
    render(&g,"previews/battle-impact.ppm");
    battle_animate(&g.battle,.05f,0);
    assert(g.battle.hit_time==0 && g.battle.enemy_hp_shown==g.battle.enemy.hp);
    /* Phase 11: action messages spotlight only the actor; command selection
       restores both sprites to their normal brightness. */
    Party spotlight_party={0};spotlight_party.count=1;
    creature_create(&spotlight_party.members[0],SPECIES_CINDLET,5);
    battle_begin_party(&g.battle,&spotlight_party,SPECIES_MOSSPRIG,3,42);g.in_battle=1;
    g.battle.ally.hp=g.battle.ally.max_hp=1000;g.battle.ally.speed=30;
    g.battle.enemy.hp=g.battle.enemy.max_hp=1000;g.battle.enemy.speed=20;
    battle_update(&g.battle,&(Input){.confirm=1});
    assert(g.battle.phase==BATTLE_MENU && g.battle.acting_side==-1);
    render(&g,"previews/spotlight-idle.ppm");
    assert(texture_tints[0]==0xffffffffu && texture_tints[1]==0xffffffffu);
    g.battle.cursor=0;battle_update(&g.battle,&(Input){.confirm=1});
    g.battle.move_cursor=0;battle_update(&g.battle,&(Input){.confirm=1});
    assert(g.battle.phase==BATTLE_MESSAGE && g.battle.acting_side==0);
    render(&g,"previews/spotlight-ally.ppm");
    assert(texture_tints[0]==0xff7e7c74u && texture_tints[1]==0xffffffffu);
    battle_update(&g.battle,&(Input){.confirm=1});
    assert(g.battle.phase==BATTLE_MESSAGE && g.battle.acting_side==1);
    render(&g,"previews/spotlight-enemy.ppm");
    assert(texture_tints[0]==0xffffffffu && texture_tints[1]==0xff7e7c74u);
    battle_update(&g.battle,&(Input){.confirm=1});
    assert(g.battle.phase==BATTLE_MENU && g.battle.acting_side==-1);
    for(int id=0;id<SPECIES_COUNT;++id) {
        char path[80];
        battle_begin_party(&g.battle,&g.party,id,id%3==2?16:id%3==1?8:3,42);
        snprintf(path,sizeof(path),"previews/pet-%03d.ppm",id+1);
        render(&g,path);
        assert(textures==2);
        int opaque=0,transparent=0;
        for(int p=0;p<128*128;++p) {
            if(pet_pixels[id][p]>>12) ++opaque;else ++transparent;
        }
        assert(opaque>100 && transparent>128*32);
    }
    g.in_battle=0;
    dialogue_open(&g.dialogue,"SESSION SAVED","YOUR PROGRESS IS SAFE ON THE MEMORY STICK.","KEEP MOVING.");
    render(&g,"previews/saved-dialogue.ppm");
    /* Round-trip all 30 forms and the expanded full collection through the
       real game snapshot/application paths using an in-memory utility. */
    place(&g,MAP_MARSH,2,10);
    for(int i=1;i<PARTY_MAX+COLLECTION_MAX;++i) {
        Creature pet;creature_create(&pet,i%SPECIES_COUNT,20+i);
        pet.hp-=i%5;pet.experience+=3;
        assert(party_add(&g.party,&pet));
    }
    assert(g.party.count==4 && g.party.stored==32);
    g.party.lead=2;g.inventory.embermarks=4242;
    g.npc_battle_progress.defeated=(1u<<NPC_BATTLE_EAST_CHALLENGER)|(1u<<3)|(1u<<11);
    progression_set(&g.progression,PROGRESSION_FIRST_CHALLENGER_DEFEATED);
    progression_set(&g.progression,PROGRESSION_CAVE_UNLOCKED);
    Party snapshot=g.party;
    NpcBattleProgress npc_snapshot=g.npc_battle_progress;
    ProgressionState progression_snapshot=g.progression;
    update(&g,(Input){.menu=INPUT_MENU_SAVE},1);
    assert(save_data_status()==SAVE_STATUS_BUSY);
    update(&g,(Input){0},1);
    assert(g.dialogue.active && !strcmp(g.dialogue.title,"SESSION SAVED"));
    party_init(&g.party);g.inventory.embermarks=0;g.npc_battle_progress=(NpcBattleProgress){0};
    progression_init(&g.progression);
    update(&g,(Input){.cancel=1},1);
    update(&g,(Input){.menu=INPUT_MENU_LOAD},1);
    update(&g,(Input){0},1);
    assert(g.dialogue.active && !strcmp(g.dialogue.title,"SESSION LOADED"));
    assert(g.map_id==MAP_MARSH && g.player.tile_x==2 && g.player.tile_y==10);
    assert(g.party.count==4 && g.party.stored==32 && g.party.lead==2 && g.inventory.embermarks==4242);
    assert(g.npc_battle_progress.defeated==npc_snapshot.defeated &&
           progression_save_bits(&g.progression)==progression_save_bits(&progression_snapshot));
    Npcs restored_npcs;npc_load(&restored_npcs,MAP_CLEARING);
    npc_apply_progress(&restored_npcs,g.npc_battle_progress.defeated);
    assert(restored_npcs.people[2].actor.tile_x==37 && restored_npcs.people[2].actor.tile_y==10);
    ProgressionState phase16_save;progression_init(&phase16_save);
    npc_reconcile_progression(1u<<NPC_BATTLE_EAST_CHALLENGER,&phase16_save);
    assert(progression_has(&phase16_save,PROGRESSION_FIRST_CHALLENGER_DEFEATED));
    for(int i=0;i<PARTY_MAX+COLLECTION_MAX;++i) {
        const Creature *a=i<4?&snapshot.members[i]:&snapshot.collection[i-4];
        const Creature *b=i<4?&g.party.members[i]:&g.party.collection[i-4];
        assert(a->species==b->species && a->level==b->level && a->hp==b->hp && a->experience==b->experience);
        assert(!memcmp(a->moves,b->moves,sizeof(a->moves)) && !memcmp(a->uses,b->uses,sizeof(a->uses)));
    }
    puts("PASS: Phase 18 locked gate, entrance challenger, world systems, NPC persistence, party/inventory, drawing budget, actor spotlight states");
    return 0;
}
