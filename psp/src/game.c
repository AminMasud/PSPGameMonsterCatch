#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <pspgu.h>
#include "game.h"
#include "world_draw.h"
#include "graphics.h"
#include "text.h"
#include "audio.h"

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
    npc_apply_progress(&g->npcs,g->npc_battle_progress.defeated,&g->progression);
    g->encounter.safe_steps = 4;
    camera_update(&g->camera,&g->player,g->map);
    g->transition=0.22f;g->area_label=2.0f;
}
static char facing_tile(const Game *g)
{
    int x=g->player.tile_x, y=g->player.tile_y;
    if(g->player.facing==FACE_UP) --y;
    else if(g->player.facing==FACE_DOWN) ++y;
    else if(g->player.facing==FACE_LEFT) --x;
    else ++x;
    return map_tile(g->map,x,y);
}
static int world_blocks(void *context,int x,int y)
{
    Game *g=context;
    const Gate *gate=map_gate(g->map_id,x,y);
    return npc_blocks(&g->npcs,x,y) ||
           (gate && !gate_can_enter(gate,&g->progression));
}
static void shop_feedback(Game *g, const char *text)
{
    snprintf(g->shop_message,sizeof(g->shop_message),"%s",text);
}
static void shop_update(Game *g, const Input *input)
{
    int direction=input->vertical?input->vertical:input->horizontal;
    if (direction && direction!=g->shop_previous_direction)
        g->shop_cursor=(g->shop_cursor+direction+ITEM_COUNT)%ITEM_COUNT;
    g->shop_previous_direction=direction;
    if (input->cancel || (input->menu & INPUT_MENU_OPEN)) { g->shop_open=0;return; }
    if (!input->confirm) return;
    if (inventory_buy(&g->inventory,(ItemId)g->shop_cursor))
        shop_feedback(g,"PURCHASED. THE ITEM IS IN YOUR BAG.");
    else if (g->inventory.quantities[g->shop_cursor]>=inventory_item_max((ItemId)g->shop_cursor))
        shop_feedback(g,"YOUR BAG IS FULL FOR THAT ITEM.");
    else shop_feedback(g,"YOU NEED MORE EMBERMARKS.");
}
static void save_creature(SaveCreature *saved, const Creature *creature)
{
    saved->species=creature->species;
    saved->level=creature->level;
    saved->experience=creature->experience;
    saved->hp=creature->hp;
    snprintf(saved->nickname,sizeof(saved->nickname),"%s",creature->nickname);
    for (int i=0;i<CREATURE_MOVES;++i) {
        saved->moves[i]=creature->moves[i];
        saved->uses[i]=creature->uses[i];
    }
}
static void load_creature(Creature *creature, const SaveCreature *saved)
{
    creature_create(creature,saved->species,saved->level);
    creature->experience=saved->experience;
    creature->hp=saved->hp;
    snprintf(creature->nickname,sizeof(creature->nickname),"%s",saved->nickname);
    for (int i=0;i<CREATURE_MOVES;++i) {
        creature->moves[i]=saved->moves[i];
        creature->uses[i]=saved->uses[i];
    }
}
static void save_snapshot(const Game *g, SavePayload *saved)
{
    *saved=(SavePayload){0};
    saved->magic=SAVE_DATA_MAGIC;
    saved->version=SAVE_DATA_VERSION;
    saved->map_id=g->map_id;
    saved->tile_x=g->player.tile_x;
    saved->tile_y=g->player.tile_y;
    saved->facing=g->player.facing;
    saved->encounter_random=g->encounter.random;
    saved->encounter_safe_steps=g->encounter.safe_steps;
    saved->party.count=g->party.count;
    saved->party.lead=g->party.lead;
    saved->party.stored=g->party.stored;
    for (int i=0;i<g->party.count;++i) save_creature(&saved->party.members[i],&g->party.members[i]);
    for (int i=0;i<g->party.stored;++i) save_creature(&saved->party.collection[i],&g->party.collection[i]);
    for (int i=0;i<ITEM_COUNT;++i) saved->item_quantities[i]=g->inventory.quantities[i];
    saved->embermarks=g->inventory.embermarks;
    saved->npc_defeated=g->npc_battle_progress.defeated;
    saved->progression_flags=progression_save_bits(&g->progression);
}
static int valid_saved_creature(const SaveCreature *saved)
{
    int max_hp;
    if (!saved || saved->species<0 || saved->species>=SPECIES_COUNT || saved->level<1 ||
        saved->level>CREATURE_MAX_LEVEL || saved->experience<0 ||
        saved->experience>creature_xp_for_level(CREATURE_MAX_LEVEL) ||
        !memchr(saved->nickname,'\0',sizeof(saved->nickname))) return 0;
    max_hp=species_get(saved->species)->base_hp+saved->level*5;
    if (saved->experience<creature_xp_for_level(saved->level) || saved->hp<0 || saved->hp>max_hp) return 0;
    for (int i=0;i<CREATURE_MOVES;++i) {
        if (saved->moves[i]<-1 || saved->moves[i]>=MOVE_COUNT || saved->uses[i]<0) return 0;
        if (saved->moves[i]>=0 && saved->uses[i]>attack_get(saved->moves[i])->uses) return 0;
    }
    return 1;
}
static int load_party(Party *party, const SaveParty *saved)
{
    if (!party || !saved || saved->count<1 || saved->count>PARTY_MAX ||
        saved->stored<0 || saved->stored>COLLECTION_MAX || saved->lead<0 || saved->lead>=saved->count)
        return 0;
    *party=(Party){0};
    party->count=saved->count;party->lead=saved->lead;party->stored=saved->stored;
    for (int i=0;i<saved->count;++i) {
        if (!valid_saved_creature(&saved->members[i])) return 0;
        load_creature(&party->members[i],&saved->members[i]);
    }
    for (int i=0;i<saved->stored;++i) {
        if (!valid_saved_creature(&saved->collection[i])) return 0;
        load_creature(&party->collection[i],&saved->collection[i]);
    }
    return 1;
}
static int apply_snapshot(Game *g, const SavePayload *saved)
{
    Party party;
    Inventory inventory;
    if (!g || !saved || saved->map_id<0 || saved->map_id>=MAP_COUNT ||
        !map_walkable(map_get(saved->map_id),saved->tile_x,saved->tile_y) ||
        saved->encounter_safe_steps<0 || saved->encounter_safe_steps>4 ||
        !load_party(&party,&saved->party) || saved->embermarks<0) return 0;
    inventory_init(&inventory);
    for (int i=0;i<ITEM_COUNT;++i) {
        if (saved->item_quantities[i]<0 || saved->item_quantities[i]>inventory_item_max((ItemId)i)) return 0;
        inventory.quantities[i]=saved->item_quantities[i];
    }
    inventory.embermarks=saved->embermarks;
    g->npc_battle_progress.defeated=saved->npc_defeated;
    progression_load_bits(&g->progression,saved->progression_flags);
    npc_reconcile_progression(g->npc_battle_progress.defeated,&g->progression);
    enter_map(g,saved->map_id,saved->tile_x,saved->tile_y);
    g->player.facing=(Direction)(saved->facing>=FACE_DOWN && saved->facing<=FACE_UP ? saved->facing : FACE_DOWN);
    g->party=party;g->inventory=inventory;
    g->encounter.random=saved->encounter_random?saved->encounter_random:0x712a9u;
    g->encounter.safe_steps=saved->encounter_safe_steps;
    g->dialogue=(Dialogue){0};g->roster_open=0;g->menu_open=0;g->shop_open=0;g->tavi_shop_pending=0;
    g->ready_prompt=(ReadyPrompt){0};g->pending_battle=(PendingBattle){0};
    g->npc_battle=(PendingNpcBattle){0};
    g->boss_battle=(PendingBossBattle){0};
    return 1;
}
static void save_status_update(Game *g)
{
    SaveStatus status=save_data_status();
    if (status==g->save_seen_status) return;
    if (status==SAVE_STATUS_SUCCEEDED) {
        audio_play(SOUND_HEAL);
        SavePayload saved;
        if (save_data_take_loaded(&saved)) {
            if (apply_snapshot(g,&saved))
                dialogue_open(&g->dialogue,"SESSION LOADED","YOUR EMBERWAKE SESSION IS RESTORED.","KEEP MOVING.");
            else dialogue_open(&g->dialogue,"LOAD FAILED","THE SAVE DATA IS NOT VALID.","YOUR CURRENT SESSION IS UNCHANGED.");
        } else dialogue_open(&g->dialogue,"SESSION SAVED","YOUR PROGRESS IS SAFE ON THE MEMORY STICK.","KEEP MOVING.");
    } else if (status==SAVE_STATUS_FAILED) {
        audio_play(SOUND_ERROR);
        dialogue_open(&g->dialogue,"SAVE UNAVAILABLE","THE MEMORY STICK SAVE DID NOT COMPLETE.","YOUR CURRENT SESSION IS STILL ACTIVE.");
    }
    g->save_seen_status=status;
}
static void start_save(Game *g)
{
    SavePayload saved;
    save_snapshot(g,&saved);g->menu_open=0;
    g->save_seen_status=SAVE_STATUS_BUSY;
    save_data_begin_save(&saved);
}
static void start_load(Game *g)
{
    g->menu_open=0;g->save_seen_status=SAVE_STATUS_BUSY;
    save_data_begin_load();
}
void game_init(Game *g)
{
    *g = (Game){0};
    encounter_init(&g->encounter,0x19236u);
    party_init(&g->party);
    inventory_init(&g->inventory);
    g->options=(GameOptions){1,1,1};
    g->save_seen_status=SAVE_STATUS_IDLE;
    enter_map(g,MAP_CLEARING,5,11);
    g->transition=0;g->area_label=0;
}
int game_offer_important_battle(Game *g,const char *opponent,
                                SpeciesId species,int level,uint32_t seed)
{
    if(!g || species<0 || species>=SPECIES_COUNT || level<1 || level>CREATURE_MAX_LEVEL ||
       g->in_battle || g->ready_prompt.active || g->menu_open || g->roster_open ||
       g->shop_open || g->dialogue.active || g->npc_battle.flow!=NPC_BATTLE_FLOW_NONE ||
       g->boss_battle.flow!=NPC_BATTLE_FLOW_NONE ||
       save_data_status()==SAVE_STATUS_BUSY) return 0;
    g->pending_battle=(PendingBattle){species,level,seed};
    ready_prompt_open(&g->ready_prompt,opponent);
    g->transition=0;
    return 1;
}
int game_offer_npc_battle(Game *g,const NpcBattleData *data,uint32_t seed)
{
    if(!g || !npc_battle_data_valid(data) || g->in_battle || g->ready_prompt.active ||
       g->menu_open || g->roster_open || g->shop_open || g->dialogue.active ||
       g->npc_battle.flow!=NPC_BATTLE_FLOW_NONE ||
       g->boss_battle.flow!=NPC_BATTLE_FLOW_NONE || save_data_status()==SAVE_STATUS_BUSY) return 0;
    if(npc_battle_is_defeated(&g->npc_battle_progress,data->id)) {
        dialogue_open(&g->dialogue,data->name,data->victory.first,data->victory.second);
        return 1;
    }
    g->npc_battle=(PendingNpcBattle){data,seed,NPC_BATTLE_FLOW_INTRO};
    g->pending_battle=(PendingBattle){0};
    dialogue_open(&g->dialogue,data->name,data->before.first,data->before.second);
    g->transition=0;
    return 1;
}
int game_offer_boss_battle(Game *g,const BossData *data,uint32_t seed)
{
    if(!g || !boss_data_valid(data) || !boss_is_available(&g->progression,data) ||
       g->in_battle || g->ready_prompt.active ||
       g->menu_open || g->roster_open || g->shop_open || g->dialogue.active ||
       g->npc_battle.flow!=NPC_BATTLE_FLOW_NONE ||
       g->boss_battle.flow!=NPC_BATTLE_FLOW_NONE || save_data_status()==SAVE_STATUS_BUSY) return 0;
    if(boss_is_defeated(&g->progression,data)) {
        dialogue_open(&g->dialogue,data->name,data->victory.first,data->victory.second);
        return 1;
    }
    g->boss_battle=(PendingBossBattle){data,seed,NPC_BATTLE_FLOW_INTRO};
    g->pending_battle=(PendingBattle){0};
    dialogue_open(&g->dialogue,data->name,data->intro.first,data->intro.second);
    g->transition=0;
    return 1;
}
static void game_step(Game *g, const Input *input, float seconds)
{
    save_data_update();
    save_status_update(g);
    if (save_data_status()==SAVE_STATUS_BUSY) return;
    if(g->in_battle) {
        battle_update(&g->battle,input);
        if(g->battle.phase==BATTLE_DONE) {
            const NpcBattleData *npc_data=g->npc_battle.flow==NPC_BATTLE_FLOW_ACTIVE?
                g->npc_battle.data:0;
            const BossData *boss_data=g->boss_battle.flow==NPC_BATTLE_FLOW_ACTIVE?
                g->boss_battle.data:0;
            BattleResult result=g->battle.result;
            g->in_battle=0;
            g->party=g->battle.party; /* Includes captures and every switched creature. */
            g->inventory=g->battle.inventory;
            /* The prototype still restores the complete team after battle so
               encounter and progression testing stays repeatable. The lodge
               dais gives the player an explicit refill while exploring. */
            party_restore(&g->party);
            if(result==BATTLE_LOSS) enter_map(g,MAP_CLEARING,5,11);
            g->encounter.safe_steps=4;
            g->transition=0.22f;
            if(npc_data) {
                if(result==BATTLE_WIN) {
                    if(npc_battle_mark_defeated(&g->npc_battle_progress,&g->progression,npc_data)) {
                        int reward=npc_data->reward_embermarks;
                        g->inventory.embermarks=reward>INT_MAX-g->inventory.embermarks?
                            INT_MAX:g->inventory.embermarks+reward;
                    }
                    npc_apply_progress(&g->npcs,g->npc_battle_progress.defeated,&g->progression);
                    dialogue_open(&g->dialogue,npc_data->name,
                                  npc_data->victory.first,npc_data->victory.second);
                } else if(result==BATTLE_LOSS && npc_data->defeat.first) {
                    dialogue_open(&g->dialogue,npc_data->name,
                                  npc_data->defeat.first,npc_data->defeat.second);
                }
                g->npc_battle=(PendingNpcBattle){0};
            } else if(boss_data) {
                if(result==BATTLE_WIN) {
                    if(boss_mark_victory(&g->progression,boss_data)) {
                        int reward=boss_data->reward_embermarks;
                        g->inventory.embermarks=reward>INT_MAX-g->inventory.embermarks?
                            INT_MAX:g->inventory.embermarks+reward;
                    }
                    npc_apply_progress(&g->npcs,g->npc_battle_progress.defeated,&g->progression);
                    dialogue_open(&g->dialogue,boss_data->name,
                                  boss_data->victory.first,boss_data->victory.second);
                } else if(result==BATTLE_LOSS && boss_data->defeat.first) {
                    dialogue_open(&g->dialogue,boss_data->name,
                                  boss_data->defeat.first,boss_data->defeat.second);
                }
                g->boss_battle=(PendingBossBattle){0};
            }
        }
        return;
    }
    if(g->ready_prompt.active) {
        ReadyPromptResult choice=ready_prompt_update(&g->ready_prompt,input);
        if(choice==READY_ACCEPTED) {
            if(g->boss_battle.flow==NPC_BATTLE_FLOW_READY && g->boss_battle.data) {
                const BossData *data=g->boss_battle.data;
                if(battle_begin_npc_party_with_inventory(&g->battle,&g->party,&g->inventory,
                    data->title,data->party,data->party_count,data->ai_profile,g->boss_battle.seed)) {
                    g->battle.presentation=data->presentation;
                    g->boss_battle.flow=NPC_BATTLE_FLOW_ACTIVE;
                    g->in_battle=1;g->transition=0.3f;audio_play(SOUND_BOND);
                } else g->boss_battle=(PendingBossBattle){0};
            } else if(g->npc_battle.flow==NPC_BATTLE_FLOW_READY && g->npc_battle.data) {
                const NpcBattleData *data=g->npc_battle.data;
                if(battle_begin_npc_party_with_inventory(&g->battle,&g->party,&g->inventory,
                    data->name,data->party,data->party_count,data->ai_profile,g->npc_battle.seed)) {
                    g->npc_battle.flow=NPC_BATTLE_FLOW_ACTIVE;
                    g->in_battle=1;g->transition=0.3f;audio_play(SOUND_BOND);
                } else g->npc_battle=(PendingNpcBattle){0};
            } else {
                PendingBattle request=g->pending_battle;
                g->pending_battle=(PendingBattle){0};
                battle_begin_party_with_inventory(&g->battle,&g->party,&g->inventory,
                                                   request.species,request.level,request.seed);
                g->in_battle=1;g->transition=0.3f;audio_play(SOUND_BOND);
            }
        } else if(choice==READY_DECLINED) {
            g->pending_battle=(PendingBattle){0};g->npc_battle=(PendingNpcBattle){0};
            g->boss_battle=(PendingBossBattle){0};
        }
        return;
    }
    if(g->shop_open) { shop_update(g,input);return; }
    if(g->roster_open) {
        g->roster_open=party_menu_update(&g->roster,&g->party,input);
        if (!g->roster_open && (input->menu&INPUT_MENU_OPEN)) g->menu_open=0;
        return;
    }
    if(g->menu_open) {
        PlayerMenuAction action=player_menu_update(&g->menu,&g->party,&g->inventory,&g->options,input);
        if (action==MENU_CLOSE) g->menu_open=0;
        else if (action==MENU_PARTY || action==MENU_COLLECTION) {
            party_menu_open(&g->roster);g->roster.tab=action==MENU_COLLECTION;g->roster_open=1;
        } else if (action==MENU_SAVE) start_save(g);
        else if (action==MENU_LOAD) start_load(g);
        return;
    }
    if (g->dialogue.active) {
        if (input->cancel) {
            g->dialogue.active=0;g->tavi_shop_pending=0;
            if(g->npc_battle.flow==NPC_BATTLE_FLOW_INTRO) g->npc_battle=(PendingNpcBattle){0};
            if(g->boss_battle.flow==NPC_BATTLE_FLOW_INTRO) g->boss_battle=(PendingBossBattle){0};
        }
        else if (input->confirm) {
            int last_page=g->dialogue.page+1>=g->dialogue.count;
            dialogue_advance(&g->dialogue);
            if(last_page && g->boss_battle.flow==NPC_BATTLE_FLOW_INTRO) {
                g->boss_battle.flow=NPC_BATTLE_FLOW_READY;
                ready_prompt_open(&g->ready_prompt,g->boss_battle.data->title);
            } else if(last_page && g->npc_battle.flow==NPC_BATTLE_FLOW_INTRO) {
                g->npc_battle.flow=NPC_BATTLE_FLOW_READY;
                ready_prompt_open(&g->ready_prompt,g->npc_battle.data->name);
            } else if (last_page && g->tavi_shop_pending) {
                g->tavi_shop_pending=0;g->shop_open=1;g->shop_cursor=0;
                g->shop_previous_direction=0;
                shop_feedback(g,"WELCOME. EMBERMARKS BUY SIMPLE SUPPLIES.");
            }
        }
        return;
    }
    if((input->menu & INPUT_MENU_OPEN) && !g->player.moving) {
        player_menu_open(&g->menu);g->menu_open=1;g->transition=0;return;
    }
    if(input->details && !g->player.moving) {
        const Creature *partner=&g->party.members[g->party.lead];
        char summary[160];
        snprintf(summary,sizeof(summary),"LEVEL %d %s - HP %d\nATK %d DEF %d SPEED %d\nXP %d - NEXT IN %d",partner->level,
                 element_name(partner->element),partner->hp,partner->attack,
                 partner->defense,partner->speed,partner->experience,creature_xp_remaining(partner));
        dialogue_open(&g->dialogue,creature_name(partner),summary,species_get(partner->species)->description);
        return;
    }
    if((input->menu & INPUT_MENU_SAVE) && !g->player.moving) {
        start_save(g);
        return;
    }
    if((input->menu & INPUT_MENU_LOAD) && !g->player.moving) {
        start_load(g);
        return;
    }
    if (input->confirm && !g->player.moving) {
        if (facing_tile(g)=='H') {
            party_restore(&g->party);
            dialogue_open(&g->dialogue,map_name(g->map_id),"YOUR TEAM IS RESTORED.\nHP AND ATTACK USES ARE READY.","TAKE A BREATH. THEN KEEP MOVING.");
            audio_play(SOUND_HEAL);
            return;
        }
        Npc *npc = npc_facing(&g->npcs,&g->player);
        if (npc) {
            npc->actor.facing = g->player.facing == FACE_UP ? FACE_DOWN :
                g->player.facing == FACE_DOWN ? FACE_UP :
                g->player.facing == FACE_LEFT ? FACE_RIGHT : FACE_LEFT;
            if (npc->boss) {
                game_offer_boss_battle(g,npc->boss,
                    g->encounter.random^(uint32_t)(npc->boss->id+1)*0x85ebca6bu);
            } else if (npc->gate && gate_is_locked(npc->gate,&g->progression) && npc->battle) {
                game_offer_npc_battle(g,npc->battle,
                    g->encounter.random^(uint32_t)(npc->battle->id+1)*0x9e3779b9u);
            } else if (npc->gate) {
                const GateDialogue *words=gate_current_dialogue(npc->gate,&g->progression);
                dialogue_open(&g->dialogue,npc->gate->gatekeeper,words->first,words->second);
            } else if (!strcmp(npc->name,"TAVI")) {
                g->tavi_shop_pending=1;
                dialogue_open(&g->dialogue,npc->name,npc->first,npc->second);
            } else dialogue_open(&g->dialogue,npc->name,npc->first,npc->second);
            return;
        }
    }
    int old_x = g->player.tile_x, old_y = g->player.tile_y;
    player_update_blocked(&g->player,g->map,input,seconds,world_blocks,g);
    if (old_x != g->player.tile_x || old_y != g->player.tile_y) {
        const Portal *portal = map_portal(g->map_id,g->player.tile_x,g->player.tile_y);
        if (portal && gate_can_enter(portal,&g->progression)) {
            enter_map(g,portal->to,portal->arrival_x,portal->arrival_y);
            return;
        }
        EncounterResult result;
        int area = map_encounter_area(g->map_id,g->player.tile_x,g->player.tile_y);
        if (encounter_step(&g->encounter,area,&result)) {
            battle_begin_party_with_inventory(&g->battle,&g->party,&g->inventory,result.species,result.level,g->encounter.random);
            g->in_battle=1;
            g->transition=0.3f;audio_play(SOUND_BOND);
        }
    }
    if (!g->dialogue.active && !g->in_battle) npc_update(&g->npcs,g->map,&g->player,seconds);
    camera_update(&g->camera,&g->player,g->map);
}
void game_update(Game *g,const Input *input,float seconds)
{
    if (seconds<0) seconds=0;
    if (seconds>0.05f) seconds=0.05f;
    int busy=save_data_status()==SAVE_STATUS_BUSY;
    int modal=g->menu_open || g->roster_open || g->shop_open || g->dialogue.active ||
              g->ready_prompt.active || g->in_battle;
    int direction=input->vertical?input->vertical:input->horizontal;
    if (!busy && modal && direction && direction!=g->previous_ui_direction) audio_play(SOUND_CURSOR);
    if (!busy && (input->confirm || input->cancel || (input->menu&INPUT_MENU_OPEN))) audio_play(SOUND_CONFIRM);
    g->previous_ui_direction=direction;
    int fighting=g->in_battle, ally_hp=g->battle.ally.hp, enemy_hp=g->battle.enemy.hp;
    BattleResult result=g->battle.result;
    int reward=g->battle.reward_given;
    if (!busy) {
        if (g->transition>0) g->transition-=seconds;
        if (g->area_label>0) g->area_label-=seconds;
        if (g->options.motion) {
            g->animation+=seconds;
            if (g->animation>=16) g->animation-=16;
        }
    }
    game_step(g,input,seconds);
    if (g->in_battle && fighting) {
        if (g->battle.ally.hp<ally_hp || g->battle.enemy.hp<enemy_hp) audio_play(SOUND_HIT);
        if ((!reward && g->battle.reward_given) || (result!=BATTLE_CAUGHT && g->battle.result==BATTLE_CAUGHT))
            audio_play(SOUND_VICTORY);
    }
    if (g->in_battle) battle_animate(&g->battle,seconds,g->options.motion);
    busy=save_data_status()==SAVE_STATUS_BUSY;
    audio_settings(g->options.music && !busy,g->options.effects && !busy);
    audio_scene(g->in_battle?MUSIC_BATTLE:g->map_id==MAP_CAVE?MUSIC_CAVE:
                (g->map_id==MAP_FOREST || g->map_id==MAP_MARSH)?MUSIC_WILD:MUSIC_HEARTH);
}
static void draw_scene(const Game *g)
{
    if(g->in_battle) { battle_draw(&g->battle); return; }
    if(g->roster_open) { party_menu_draw(&g->roster,&g->party);return; }
    if(g->menu_open) { player_menu_draw(&g->menu,&g->party,&g->inventory,&g->options,g->map_id);return; }
    world_draw(g->map,&g->player,&g->camera,g->options.motion?g->animation:0);
    for (int i=0;i<g->npcs.count;++i)
        world_actor_draw(&g->npcs.people[i].actor,&g->camera,1);
    graphics_rectangle(0,0,480,15,GU_RGBA(18,27,30,255));
    text_draw(6,4,map_name(g->map_id),GU_RGBA(241,212,150,255),1);
    text_draw(192,4,"X TALK  TRI MENU  START SAVE  L LOAD",GU_RGBA(210,221,211,255),1);
    if (g->area_label>0 && !g->dialogue.active && !g->shop_open) {
        graphics_rectangle(10,25,236,24,GU_RGBA(21,30,36,255));
        graphics_rectangle(10,25,3,24,GU_RGBA(242,198,117,255));
        text_draw(22,34,map_name(g->map_id),GU_RGBA(239,218,173,255),1);
    }
    if(g->ready_prompt.active) { ready_prompt_draw(&g->ready_prompt);return; }
    if (g->shop_open) {
        graphics_rectangle(38,37,404,205,GU_RGBA(184,150,96,255));
        graphics_rectangle(40,39,400,201,GU_RGBA(21,30,36,255));
        text_draw(56,52,"WAYFARER SUPPLIES",GU_RGBA(244,198,117,255),2);
        char line[80];
        snprintf(line,sizeof(line),"EMBERMARKS %d",g->inventory.embermarks);
        text_draw(56,73,line,GU_RGBA(180,211,193,255),1);
        for (int i=0;i<ITEM_COUNT;++i) {
            if (i==g->shop_cursor) graphics_rectangle(53,92+i*32,220,27,GU_RGBA(67,85,80,255));
            snprintf(line,sizeof(line),"%s  %dM  x%d",inventory_item_name((ItemId)i),
                     inventory_item_price((ItemId)i),g->inventory.quantities[i]);
            text_draw(61,100+i*32,line,i==g->shop_cursor?GU_RGBA(255,213,147,255):GU_RGBA(220,225,215,255),1);
        }
        text_wrap(286,101,144,36,g->shop_cursor==ITEM_PULSE_TONIC?"UP TO 25 HP RESTORED.":"FULL HP RESTORE.",GU_RGBA(220,225,215,255),1);
        text_wrap(56,177,365,36,g->shop_message,GU_RGBA(220,213,190,255),1);
        text_draw(56,224,"UP/DOWN SELECT   X BUY   O CLOSE",GU_RGBA(163,189,178,255),1);
        return;
    }
    if (g->dialogue.active) {
        graphics_rectangle(6,167,468,99,GU_RGBA(184,150,96,255));
        graphics_rectangle(8,169,464,95,GU_RGBA(21,30,36,255));
        text_draw(18,176,g->dialogue.title,GU_RGBA(242,198,117,255),1);
        text_box(18,191,444,53,g->dialogue.pages[g->dialogue.page],GU_RGBA(235,237,220,255));
        text_draw(18,251,g->dialogue.page+1 < g->dialogue.count ? "X NEXT   O CLOSE" : "X CLOSE   O CLOSE",GU_RGBA(163,189,178,255),1);
    }
}
void game_draw(const Game *g)
{
    draw_scene(g);
    if (g->options.motion && g->transition>0 && save_data_status()!=SAVE_STATUS_BUSY) {
        /* A short shutter reveal uses only two opaque rectangles. */
        int height=(int)(136*g->transition/(g->in_battle?0.3f:0.22f));
        if (height>136) height=136;
        if (height>0) {
            graphics_rectangle(0,0,480,height,GU_RGBA(16,25,31,255));
            graphics_rectangle(0,272-height,480,height,GU_RGBA(16,25,31,255));
        }
    }
}
