# Complete Phase 9 source contents

## .gitattributes

````text
* text=auto eol=lf
````

## .gitignore

````text
*.o
*.elf
*.prx
*.PBP
*.SFO
*.d
previews/
````

## assets/maps/cave.inc

````text
"OOOOOOOOOOOOOOOOOOOO",
"OccccccccccccccccccO",
"OccccccccccccccccccO",
"OcccccccOccccccccccO",
"OcccccccOccccccccccO",
"OcccccccOccccOOOOccO",
"OcccccccOccccccccccO",
"OcccccccOccccccccccO",
"OcccccccOccccccccccO",
"OccccccccccccccccccO",
"O______ccccccccccccO",
"O_D____ccccccccccccO",
"O______ccccccccccccO",
"OOOOOOOOOOOOOOOOOOOO",
````

## assets/maps/clearing.inc

````text
"########################################",
"#...........,.......==.........,.......#",
"#....,..............==..,..............#",
"#.........#####..,..==.~~~~~~~~~....,..#",
"#.........#####.....==.~~~~~~~~~.......#",
"#..,......#####.....==,~~~~~~~~~.......#",
"#..WWWWW..#####,....==.~~~~~~~~~..,....#",
"#..WWWWW,.#####.....==.~~~~~~~~~.......#",
"#,.WWWWW..#####.....==.~~~~~~~~~.......#",
"#..WWDWW.....,......==..........,......#",
"#.....,.............==...,.............#",
"#=====================================>#",
"#======================================#",
"#...,...............==.,...............#",
"#...............,O..==.............,...#",
"#........,.......O..==......,.######...#",
"#.,..............O..==........######...#",
"#.....OOOO....,..O..==........######...#",
"#......,.........O..==....,...######...#",
"#................O.,==........######..,#",
"#...........,.......==.........,.......#",
"#....,..............==..,..............#",
"#................,..==..............,..#",
"########################################",
````

## assets/maps/forest.inc

````text
"################################",
"#...........................=..#",
"#...........................=..#",
"#......ggggggggggggggggggg.OOO.#",
"#......ggggggggggggggggggg.OOO.#",
"#......ggg#gggggggg#gggggg.ODO.#",
"#......ggg#gggggggg#gggggg..=..#",
"#......ggg#gggggggg#gggggg..=..#",
"#......ggg#gggggggg#gggggg..=..#",
"#......ggggggggggggggggggg..=..#",
"#......ggggggggggggggggggg..=..#",
"#>============================>#",
"#......ggggggggggggggggggg..=..#",
"#......ggggggggggggggggggg..=..#",
"#......ggggggggggggggggggg..=..#",
"#......ggggggggggggggggggg..=..#",
"#......ggggggggggggggggggg..=..#",
"#...........................=..#",
"#...........................=..#",
"################################",
````

## assets/maps/lodge.inc

````text
"WWWWWWWWWWWWWWW",
"W_H___________W",
"W_OOO_____OOO_W",
"W_____________W",
"W_____________W",
"W_____________W",
"W_____________W",
"W______D______W",
"WWWWWWWWWWWWWWW",
````

## assets/maps/marsh.inc

````text
"############################",
"#..........................#",
"#.......rrrrrrrrrrrrrrWWWWW#",
"#.......rr~~~~~~rrrrrrWWWWW#",
"#.......rr~~~~~~rrrrrrWWDWW#",
"#.......rr~~~~~~rrrrrrrr=r.#",
"#.......rr~~~~~~rrrrrrrr=r.#",
"#..,....rr~~~~~~rrrrrrrr=r.#",
"#.......rr~~~~~~rrrrrrr,=r.#",
"#.......rr~~~~~~rrrrrrrr=r.#",
"#<=========================#",
"#.......rr~~~~~~rrrrrrrrrr.#",
"#..~~~..rr~~~~~~rrr,rrrrrr.#",
"#..~~~..rr~~~~~~rrrrrrrrrr.#",
"#..~~~..rr~~~~~~rrrrrrrrrr.#",
"#..~~~..rrrrrrrrrrrrrrrrrr.#",
"#..........................#",
"############################",
````

## assets/maps/rest.inc

````text
"WWWWWWWWWWWWWWW",
"W_____________W",
"W_H_______OO__W",
"W_________OO__W",
"W_____________W",
"W_____________W",
"W_____________W",
"W______D______W",
"WWWWWWWWWWWWWWW",
````

## include/attacks.h

````text
#ifndef EMBERWAKE_ATTACKS_H
#define EMBERWAKE_ATTACKS_H
typedef enum { ELEMENT_PLAIN, ELEMENT_GROVE, ELEMENT_EMBER, ELEMENT_STONE, ELEMENT_WIND } Element;
typedef enum { MOVE_NUDGE, MOVE_CINDER, MOVE_LEAF, MOVE_PEBBLE, MOVE_GUST, MOVE_LUNGE,
               MOVE_HEAT, MOVE_FLARE, MOVE_VINE, MOVE_FAULT, MOVE_TEMPEST, MOVE_COUNT } MoveId;
typedef struct { const char *name; int power, accuracy; Element element; int uses; } Attack;
const Attack *attack_get(int id);
const char *element_name(Element element);
int attack_effectiveness(Element attack, Element defender); /* 1 half, 2 normal, 4 double */
#endif
````

## include/audio_synth.h

````text
#ifndef EMBERWAKE_AUDIO_SYNTH_H
#define EMBERWAKE_AUDIO_SYNTH_H
#include <stdint.h>
#include "audio.h"
#define AUDIO_RATE 44100
typedef struct {
    MusicScene scene;
    unsigned int clock, phase, bass_phase, effect_phase, effect_clock;
    unsigned int step, bass_step, effect_step;
    int gain, effect_gain;
    Sound effect;
} AudioSynth;
void audio_synth_init(AudioSynth *s);
void audio_synth_scene(AudioSynth *s,MusicScene scene);
void audio_synth_effect(AudioSynth *s,Sound effect);
void audio_synth_render(AudioSynth *s,int16_t *stereo,unsigned int frames,int music,int effects);
#endif
````

## include/audio.h

````text
#ifndef EMBERWAKE_AUDIO_H
#define EMBERWAKE_AUDIO_H
typedef enum { MUSIC_HEARTH, MUSIC_WILD, MUSIC_CAVE, MUSIC_BATTLE, MUSIC_COUNT } MusicScene;
typedef enum { SOUND_NONE, SOUND_CURSOR, SOUND_CONFIRM, SOUND_HIT, SOUND_HEAL,
               SOUND_BOND, SOUND_VICTORY, SOUND_ERROR, SOUND_COUNT } Sound;
int audio_init(void);
void audio_shutdown(void);
void audio_scene(MusicScene scene);
void audio_play(Sound sound);
void audio_settings(int music,int effects);
#endif
````

## include/battle.h

````text
#ifndef EMBERWAKE_BATTLE_H
#define EMBERWAKE_BATTLE_H
#include <stdint.h>
#include "input.h"
#include "attacks.h"
#include "creature.h"
#include "party.h"
#include "inventory.h"
#define BATTLE_MOVES 4
typedef Creature Battler;
typedef enum { BATTLE_MESSAGE, BATTLE_MENU, BATTLE_ATTACKS, BATTLE_LEARN, BATTLE_SWITCH, BATTLE_CAPTURE, BATTLE_ITEMS, BATTLE_DONE } BattlePhase;
typedef enum { BATTLE_ONGOING, BATTLE_WIN, BATTLE_LOSS, BATTLE_ESCAPED, BATTLE_CAUGHT } BattleResult;
typedef enum { AFTER_MENU, AFTER_TURN, AFTER_GROWTH, AFTER_SWITCH, AFTER_DONE } BattleAfter;
typedef struct {
    Battler ally, enemy;
    Party party;
    Inventory inventory;
    int active, switch_cursor, forced_switch, capture_charges;
    BattlePhase phase;
    BattleResult result;
    BattleAfter after;
    uint32_t random;
    int cursor, move_cursor, item_cursor, previous_direction;
    int choices[2], order[2], turn_index;
    int escape_attempts;
    CreatureGrowth growth;
    int growth_stage, growth_move, learn_cursor, reward_given, experience_reward;
    float animation, hit_time, ally_hp_shown, enemy_hp_shown;
    int hit_side;
    char message[160];
} Battle;
void battler_starter(Battler *b);
void battler_restore(Battler *b);
void battle_begin(Battle *b,const Battler *ally,SpeciesId species,int level,uint32_t seed);
void battle_begin_party(Battle *b,const Party *party,SpeciesId species,int level,uint32_t seed);
void battle_begin_party_with_inventory(Battle *b,const Party *party,const Inventory *inventory,SpeciesId species,int level,uint32_t seed);
void battle_update(Battle *b,const Input *input);
int battle_damage(const Battler *attacker,const Battler *defender,const Attack *attack,int variation);
void battle_draw(const Battle *b);
void battle_animate(Battle *b,float seconds,int motion);
#endif
````

## include/camera.h

````text
#ifndef EMBERWAKE_CAMERA_H
#define EMBERWAKE_CAMERA_H
#include "player.h"
typedef struct { int x, y; } Camera;
void camera_update(Camera *camera, const Player *player, const Map *map);
#endif
````

## include/capture.h

````text
#ifndef EMBERWAKE_CAPTURE_H
#define EMBERWAKE_CAPTURE_H
#include "creature.h"

/* 0 common, 1 uncommon, 2 rare. Unknown species are conservatively treated as rare. */
int capture_rarity(SpeciesId species);
/* Strength is 1 (standard Resonance Loom) through 5, clamped to this range.
   Returns percent 5..95, or zero for an invalid/fainted creature. */
int capture_chance(const Creature *creature,int strength);
/* Caller supplies a uniformly distributed roll 0..99; invalid rolls always fail. */
int capture_attempt(const Creature *creature,int strength,unsigned int roll);
#endif
````

## include/creature.h

````text
#ifndef EMBERWAKE_CREATURE_H
#define EMBERWAKE_CREATURE_H
#include "attacks.h"
#define CREATURE_MOVES 4
#define CREATURE_MAX_LEVEL 100
typedef enum {
    SPECIES_CINDLET, SPECIES_EMBERLYN, SPECIES_MOSSLET, SPECIES_MOSSHORN,
    SPECIES_TWIGLINT, SPECIES_GLOWMOTH, SPECIES_FLINTLING, SPECIES_FLINTAUR,
    SPECIES_DUSKWISP, SPECIES_ECHOCRAG, SPECIES_REEDSKIP, SPECIES_SUNFINCH, SPECIES_COUNT
} SpeciesId;
typedef struct { int level, move; } LearnMove;
typedef struct {
    SpeciesId id;
    const char *name, *description;
    Element element;
    int base_hp, base_attack, base_defense, base_speed, experience_yield;
    int evolution_level, evolved_species;
    int overworld_sprite, battle_sprite;
    LearnMove learnset[8];
    int learn_count;
} Species;
typedef struct {
    SpeciesId species;
    char nickname[20];
    const char *name; /* Derived cache of immutable species data, not save data. */
    Element element;
    int level, experience, max_hp, hp, attack, defense, speed;
    int moves[CREATURE_MOVES], uses[CREATURE_MOVES];
} Creature;
typedef struct {
    int old_level;
    SpeciesId old_species;
    int moves[MOVE_COUNT], move_count;
} CreatureGrowth;
const Species *species_get(int id);
const char *creature_name(const Creature *c);
void creature_create(Creature *c,int species,int level);
void creature_restore(Creature *c);
int creature_xp_for_level(int level);
int creature_xp_remaining(const Creature *c);
void creature_gain_xp(Creature *c,int amount,CreatureGrowth *growth);
int creature_learn(Creature *c,int move,int slot);
#endif
````

## include/dialogue.h

````text
#ifndef EMBERWAKE_DIALOGUE_H
#define EMBERWAKE_DIALOGUE_H
#define DIALOGUE_PAGES 3
typedef struct {
    int active, page, count;
    char title[40];
    char pages[DIALOGUE_PAGES][160];
} Dialogue;
void dialogue_open(Dialogue *d, const char *title, const char *first, const char *second);
void dialogue_advance(Dialogue *d);
#endif
````

## include/encounter.h

````text
#ifndef EMBERWAKE_ENCOUNTER_H
#define EMBERWAKE_ENCOUNTER_H
#include <stdint.h>
#include "creature.h"
typedef struct { uint32_t random; int safe_steps; } Encounter;
typedef struct { const char *name; int level; SpeciesId species; } EncounterResult;
void encounter_init(Encounter *e, uint32_t seed);
int encounter_step(Encounter *e, int area, EncounterResult *result);
#endif
````

## include/game.h

````text
#ifndef EMBERWAKE_GAME_H
#define EMBERWAKE_GAME_H
#include "camera.h"
#include "npc.h"
#include "dialogue.h"
#include "encounter.h"
#include "battle.h"
#include "party_menu.h"
#include "inventory.h"
#include "save_data.h"
#include "player_menu.h"
typedef struct {
    const Map *map;
    Player player;
    Camera camera;
    int map_id;
    Npcs npcs;
    Dialogue dialogue;
    Encounter encounter;
    Party party;
    Inventory inventory;
    PartyMenu roster;
    int roster_open;
    PlayerMenu menu;
    GameOptions options;
    int menu_open, previous_ui_direction;
    float transition, area_label, animation;
    int shop_open, shop_cursor, shop_previous_direction;
    int tavi_shop_pending;
    char shop_message[112];
    SaveStatus save_seen_status;
    Battle battle;
    int in_battle;
} Game;
void game_init(Game *game);
void game_update(Game *game, const Input *input, float seconds);
void game_draw(const Game *game);
#endif
````

## include/graphics.h

````text
#ifndef EMBERWAKE_GRAPHICS_H
#define EMBERWAKE_GRAPHICS_H

#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 272

void graphics_init(void);
void graphics_begin(void);
void graphics_rectangle(int x, int y, int width, int height,
                        unsigned int color);
void graphics_end(void);
void graphics_shutdown(void);

#endif
````

## include/input.h

````text
#ifndef EMBERWAKE_INPUT_H
#define EMBERWAKE_INPUT_H

typedef struct {
    int horizontal;
    int vertical;
    int confirm, cancel;
    int details;
    int menu;
} Input;

enum {
    INPUT_MENU_OPEN = 1,
    INPUT_MENU_SAVE = 2,
    INPUT_MENU_LOAD = 4
};

int input_init(void);
void input_poll(Input *input);

#endif
````

## include/inventory.h

````text
#ifndef EMBERWAKE_INVENTORY_H
#define EMBERWAKE_INVENTORY_H

#include "creature.h"

typedef enum {
    ITEM_PULSE_TONIC,
    ITEM_HEARTH_RESTORE,
    ITEM_COUNT
} ItemId;

typedef struct {
    int quantities[ITEM_COUNT];
    int embermarks;
} Inventory;

void inventory_init(Inventory *inventory);
const char *inventory_item_name(ItemId item);
const char *inventory_item_description(ItemId item);
int inventory_item_max(ItemId item);
int inventory_item_price(ItemId item);
int inventory_buy(Inventory *inventory, ItemId item);
int inventory_use_healing(Inventory *inventory, ItemId item, Creature *target);

#endif
````

## include/map.h

````text
#ifndef EMBERWAKE_MAP_H
#define EMBERWAKE_MAP_H
#define TILE_SIZE 32
typedef struct {
    int width, height;
    const char *const *rows;
    int spawn_x, spawn_y;
} Map;
const Map *map_test(void);
char map_tile(const Map *map, int x, int y);
int map_walkable(const Map *map, int x, int y);
/* Append new IDs to preserve existing saves. */
enum { MAP_CLEARING, MAP_FOREST, MAP_LODGE, MAP_CAVE, MAP_MARSH, MAP_REST, MAP_COUNT };
typedef struct { int from, x, y, to, arrival_x, arrival_y; } Portal;
const Map *map_get(int id);
const char *map_name(int id);
const Portal *map_portal(int id, int x, int y);
int map_encounter_area(int id, int x, int y);
#endif
````

## include/npc.h

````text
#ifndef EMBERWAKE_NPC_H
#define EMBERWAKE_NPC_H
#include "player.h"
#define NPC_MAX 4
typedef struct {
    Player actor;
    const char *name, *first, *second;
    int patrol_start, patrol_end, direction;
    float wait;
} Npc;
typedef struct { Npc people[NPC_MAX]; int count; } Npcs;
void npc_load(Npcs *npcs, int map_id);
int npc_blocks(void *context, int x, int y);
void npc_update(Npcs *npcs, const Map *map, const Player *player, float seconds);
Npc *npc_facing(Npcs *npcs, const Player *player);
#endif
````

## include/party_menu.h

````text
#ifndef EMBERWAKE_PARTY_MENU_H
#define EMBERWAKE_PARTY_MENU_H

#include "input.h"
#include "party.h"

typedef enum { PARTY_MENU_BROWSE, PARTY_MENU_ACTION, PARTY_MENU_SWAP } PartyMenuMode;

typedef struct {
    int tab;
    int cursor[2], scroll[2];
    int action, swap;
    int previous_horizontal, previous_vertical;
    PartyMenuMode mode;
    char message[112];
} PartyMenu;

void party_menu_open(PartyMenu *menu);
/* Returns one while open, zero when Circle/Triangle closes the menu. */
int party_menu_update(PartyMenu *menu, Party *party, const Input *input);
void party_menu_draw(const PartyMenu *menu, const Party *party);

#endif
````

## include/party.h

````text
#ifndef EMBERWAKE_PARTY_H
#define EMBERWAKE_PARTY_H
#include "creature.h"

#define PARTY_MAX 4
#define COLLECTION_MAX 24

typedef struct {
    Creature members[PARTY_MAX];
    int count, lead;
    Creature collection[COLLECTION_MAX];
    int stored;
} Party;

void party_init(Party *party);
/* Copies the complete creature unchanged; 1 = party, 2 = collection, 0 = full/invalid. */
int party_add(Party *party,const Creature *creature);
int party_has_space(const Party *party);
void party_restore(Party *party);
int party_set_lead(Party *party,int index);
/* Swap with a party slot, or withdraw when party_index == count and count < PARTY_MAX. */
int party_exchange(Party *party,int stored_index,int party_index);
/* Keep at least one party member; no operation below heals or replenishes moves. */
int party_deposit(Party *party,int index);
#endif
````

## include/player_menu.h

````text
#ifndef EMBERWAKE_PLAYER_MENU_H
#define EMBERWAKE_PLAYER_MENU_H
#include "input.h"
#include "party.h"
#include "inventory.h"
typedef enum { MENU_HOME, MENU_BAG, MENU_PLAYER, MENU_OPTIONS } PlayerMenuPage;
typedef enum { MENU_STAY, MENU_CLOSE, MENU_PARTY, MENU_COLLECTION, MENU_SAVE, MENU_LOAD } PlayerMenuAction;
typedef struct {
    int music, effects, motion;
} GameOptions;
typedef struct {
    PlayerMenuPage page;
    int cursor, previous_direction;
    char message[120];
} PlayerMenu;
void player_menu_open(PlayerMenu *m);
PlayerMenuAction player_menu_update(PlayerMenu *m,Party *party,Inventory *bag,GameOptions *options,const Input *input);
void player_menu_draw(const PlayerMenu *m,const Party *party,const Inventory *bag,const GameOptions *options,int map_id);
#endif
````

## include/player.h

````text
#ifndef EMBERWAKE_PLAYER_H
#define EMBERWAKE_PLAYER_H
#include "input.h"
#include "map.h"
typedef enum { FACE_DOWN, FACE_LEFT, FACE_RIGHT, FACE_UP } Direction;
typedef struct {
    int tile_x, tile_y, target_x, target_y;
    float x, y, progress, animation;
    int moving;
    Direction facing;
} Player;
void player_init(Player *player, const Map *map);
void player_update(Player *player, const Map *map, const Input *input, float seconds);
typedef int (*PlayerBlocker)(void *context, int x, int y);
void player_update_blocked(Player *p, const Map *map, const Input *input,
                           float seconds, PlayerBlocker blocker, void *context);
#endif
````

## include/save_data.h

````text
#ifndef EMBERWAKE_SAVE_DATA_H
#define EMBERWAKE_SAVE_DATA_H

#include <stdint.h>
#include "creature.h"
#include "party.h"
#include "inventory.h"

#define SAVE_DATA_MAGIC 0x454D4252u
#define SAVE_DATA_VERSION 1u

typedef struct {
    int species;
    int level;
    int experience;
    int hp;
    char nickname[20];
    int moves[CREATURE_MOVES];
    int uses[CREATURE_MOVES];
} SaveCreature;

typedef struct {
    SaveCreature members[PARTY_MAX];
    SaveCreature collection[COLLECTION_MAX];
    int count;
    int lead;
    int stored;
} SaveParty;

typedef struct {
    uint32_t magic;
    uint32_t version;
    int map_id;
    int tile_x;
    int tile_y;
    int facing;
    uint32_t encounter_random;
    int encounter_safe_steps;
    SaveParty party;
    int item_quantities[ITEM_COUNT];
    int embermarks;
} SavePayload;

typedef enum {
    SAVE_STATUS_IDLE,
    SAVE_STATUS_BUSY,
    SAVE_STATUS_SUCCEEDED,
    SAVE_STATUS_FAILED
} SaveStatus;

int save_data_begin_save(const SavePayload *payload);
int save_data_begin_load(void);
void save_data_update(void);
SaveStatus save_data_status(void);
int save_data_take_loaded(SavePayload *payload);

#endif
````

## include/text.h

````text
#ifndef EMBERWAKE_TEXT_H
#define EMBERWAKE_TEXT_H
void text_draw(int x, int y, const char *text, unsigned int color, int scale);
/* Word wrap inside a pixel rectangle. Returns the number of lines required. */
int text_wrap(int x,int y,int width,int height,const char *text,unsigned int color,int scale);
void text_box(int x,int y,int width,int height,const char *text,unsigned int color);
#endif
````

## include/world_draw.h

````text
#ifndef EMBERWAKE_WORLD_DRAW_H
#define EMBERWAKE_WORLD_DRAW_H
#include "camera.h"
void world_draw(const Map *map, const Player *player, const Camera *camera,float animation);
void world_actor_draw(const Player *p, const Camera *camera, int npc);
#endif
````

## Makefile

````text
TARGET = emberwake
OBJS = src/main.o src/game.o src/input.o src/graphics.o src/map.o src/player.o src/camera.o src/world_draw.o src/npc.o src/dialogue.o src/encounter.o src/text.o src/attacks.o src/battle.o src/battle_draw.o src/creature.o src/party.o src/capture.o src/party_menu.o src/inventory.o src/save_data.o src/player_menu.o src/audio.o src/audio_synth.o

INCDIR = include
CFLAGS = -O2 -G0 -std=c99 -Wall -Wextra -Werror -MMD -MP
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)
LIBDIR =
LDFLAGS =
LIBS = -lpspaudiolib -lpspgu -lpspge -lpspdisplay -lpspctrl -lpspaudio

BUILD_PRX = 1
PSP_FW_VERSION = 660
EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = Emberwake - Phase 9

PSPSDK = $(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak
# libc glue also imports sceUtility. Keep its stubs together after other SDK
# libraries, including the defaults added by build.mak.
LIBS += -lpsputility

$(OBJS): Makefile

src/main.o: include/game.h include/graphics.h include/input.h
src/game.o: include/game.h include/graphics.h include/input.h
src/input.o: include/input.h
src/graphics.o: include/graphics.h
src/inventory.o: include/inventory.h
src/save_data.o: include/save_data.h

src/map.o: assets/maps/clearing.inc
PARAM.SFO: Makefile
-include $(OBJS:.o=.d)
````

## README.md

````text
# Emberwake — Phase 9

Original PSP homebrew RPG prototype in C / PSPSDK. The creatures are called
**Veylings**. Phase 9 adds original synthesized music and sound effects, animated
battles and terrain, screen transitions, a complete player menu, Sunthread Marsh,
Lantern Rest, and two new Veylings. Existing captures, party/collection management,
progression, inventory, shop, healing, and Memory Stick saves remain available.
Phase 8 saves are compatible: the version-1 payload remains 1960 bytes, and old
species/map IDs retain their meanings. Load manually with L after launching.

## Play this build

The Phase 9 build is **EBOOT-PHASE9.PBP**. Older EBOOT files are retained;
use the explicitly named Phase 9 artifact.

Copy EBOOT-PHASE9.PBP to the Memory Stick, naming the destination:

    ms0:/PSP/GAME/EMBERWAKE/EBOOT.PBP

Launch **Emberwake - Phase 9** from Game > Memory Stick.

- D-pad: smooth four-direction tile movement; horizontal wins when two directions
  are held. Release finishes the current tile.
- X: speak to a stationary NPC in the tile you face; advance/close dialogue.
- Circle: close dialogue immediately.
- Triangle: open the player menu while stationary; Triangle closes it.
- Select: view the lead partner's stats, experience, and species description while stationary.
- Start: open the PSP save-data dialog and save the current session.
- L: open the PSP save-data dialog and load the latest session.
- HOME: system exit menu, as in earlier phases.

X and Circle use new-press detection, so holding a button does not skip pages.
Dialogue and player menus pause player movement and NPC patrols. Moving NPCs must finish their
current step before you can speak to them.

## Phase 9 polish

Triangle opens the Wayfarer's Field Kit: CREATURES, ITEMS, COLLECTION, PLAYER,
SAVE, and OPTIONS. Choose with the D-pad and X; Circle returns one page. Party
and collection browsing return to the Field Kit with Circle; Triangle closes
the entire menu. Start saves and L loads from the Field Kit as well.

ITEMS applies a healing item to the current lead, consuming stock only when it
restores HP. Set another lead through CREATURES to use an item on that partner.
PLAYER shows location, currency, roster counts, and a hint toward the new route.

OPTIONS independently toggles music, sound effects, and animation. These are
session settings and reset on launch; they are not added to the save payload.
Four original looping phrases cover settlements, wilderness, cave, and battles.
Menu, impact, healing, encounter, victory, and error sounds are generated by the
same integer synthesizer. No external audio files are needed. Sound is muted
while the PSP savedata utility is active, and audio initialization failure allows
silent play.

Battle sprites bob, impacts shake the target, and HP bars ease toward the new
value. Water ripples and marsh reeds move. Brief shutters reveal map changes and
battles, with a location label after entering a map. Disabling animation removes
these effects and makes HP bars update immediately. Dialogue and battle messages
now wrap within their panels and use smaller text when needed.

Phase 9 PSP test route:
1. Load your Phase 8 save with L and check your party, collection, items, and map.
2. Press Triangle and try all six menu entries. Toggle music, effects, and
   animation independently; verify gameplay is paused while browsing.
3. Travel east through Fernveil Woods to the new exit at the end of the path.
4. Cross the Sunthread Marsh boardwalk. Walk in reeds to find Reedskip, Glowmoth,
   and the rarer Sunfinch. Fight and capture the new species.
5. Follow the eastern path north to Lantern Rest. Talk to Ilsen, then face the
   green healing dais and press X. Return through the south door.
6. Save in the new area with a new creature, quit, relaunch, load, and verify both.
7. Check the music changes in the cave and in battle, HP animation, readable
   dialogue, HOME > Cancel/Quit, and suspend/resume with sound enabled.

## World and test route

| Area | Contents | Connections |
| --- | --- | --- |
| Hearth Clearing | Original map, lodge exterior, Mira, patrolling Orin | Lodge door northwest of spawn; woods at east end of horizontal path |
| Fernveil Woods | Sen, dark tall grass, northern cave entrance | West to clearing; north to cave; east to marsh |
| Wayfarer Lodge | Indoor floor, furniture, Tavi | South door returns to clearing |
| Hollowstone Cave | Rock formations, rough encounter floor, Nel | Southwest doorway returns to woods |
| Sunthread Marsh | Ela, reed encounters, ponds, boardwalk | West to woods; northeastern doorway to Lantern Rest |
| Lantern Rest | Ilsen, safe floor, healing dais | South door returns to marsh |

Starting at the clearing:
1. Walk north from the starting tile to enter the lodge's gold-lit doorway.
2. Walk toward Tavi and press X while facing them. X advances; Circle closes.
3. Return through the lodge's south doorway.
4. Find Mira northeast of spawn and Orin patrolling just south of the path.
   Verify you cannot walk through them and Orin cannot walk through you.
5. Follow the horizontal sandy path east to the marked exit to the woods.
6. Walk through dark tall grass until a battle begins. Press X through the
   introduction, select FIGHT, then select an attack. Standing still never
   triggers an encounter.
7. Follow the right-hand path north into the gold-lit cave entrance.
8. Walk on the rough cave floor for its different encounter table.
9. Return through both exits; verify no immediate teleport loop.
10. Check scrolling, collision, HOME > Cancel/Quit, and suspend/resume on PSP.

Leaving and reentering an area resets its NPCs to their starting positions.
Saving records the current map, tile, facing, encounter seed/safety steps, party,
collection, progression, item quantities, and Embermarks. NPC patrol positions,
open menus, active battles, and dialogue are intentionally not saved.

## Encounters

Only completed steps roll for encounters. Paths, flowers, ordinary grass, and
lodge floors are safe. An encounter has an 18% chance on eligible terrain after
a four-step grace period following map entry or the previous encounter.
The grace counter counts completed steps on any terrain.

| Terrain | Common 60% | Uncommon 30% | Rare 10% |
| --- | --- | --- | --- |
| Woods tall grass | Mosslet, level 2-4 | Twiglint, level 3-5 | Glowmoth, level 4-6 |
| Cave rough floor | Flintling, level 3-5 | Duskwisp, level 4-6 | Echocrag, level 5-7 |
| Marsh reeds | Reedskip, level 5-7 | Glowmoth, level 5-7 | Sunfinch, level 6-8 |

Encounters reference stable species IDs in the creature database. Wild creatures
use species-specific stats and up to four attacks unlocked at their level.
All eight encounter species can be captured. The item bag is initialized locally
at the start of a new session and is included in Memory Stick saves.

## Battle prototype

Your partner starts as **Cindlet**, a level 5 Ember Veyling with four attacks. Wild
opponents come from the woods, cave, and marsh tables. Battles display both
creatures, names, levels, elements, numeric HP, and HP bars.

Controls:
- Tap D-pad up/down to select. Left/right also moves one menu entry.
- X confirms an option or advances the current battle message.
- Circle returns from attack selection, capture confirmation, or voluntary team
  selection to the main battle menu without spending a turn.
- Holding X does not skip messages. Circle cannot dismiss combat results.

FIGHT opens the four attacks with remaining uses, power, accuracy, and element.
Faster creatures act first; equal speed uses a random tie-break. Each attack
checks accuracy and consumes one use even if it misses. A defeated creature
cannot retaliate. Each action has a separate message, followed by the outcome.

Damage is:

    base = ((2 * level / 5 + 2) * power * attack / defense) / 20 + 2
    damage = max(1, base * elemental_multiplier * random(90..100) / 100)

Integer arithmetic is used, with HP clamped at zero. Ember beats Grove, Grove
beats Stone, Stone beats Wind, and Wind beats Ember (2x damage). Reverse matchups
and same-element attacks deal half damage. Plain attacks are neutral.

NUDGE is accurate and neutral; CINDER ARC is strong against Grove; BOLD LUNGE is
powerful but less accurate; WHIRL CUT is a Wind option. Selecting an exhausted
attack does not spend a turn. If all four moves are exhausted, FIGHT offers the
weak, unlimited PRESS ON attack so combat cannot become stuck.

RUN has a 70% escape chance and is guaranteed on the third attempt. A failed
escape allows exactly one enemy action. CAPTURE opens the Resonance Loom, and
CREATURES opens party selection. ITEMS opens the item bag. A successful healing
item uses the turn and allows one enemy response; an invalid use spends neither.

**Temporary prototype rule:** every party and collection member's HP and attack
uses refill after every battle, including escape, capture, and defeat. The Loom
starts each new battle with three charges. Victory, capture, or escape returns
to the same exploration position. Defeat returns you to Hearth Clearing at the original starting tile.
All outcomes grant four safe steps before another encounter can roll. This rule
keeps the battle loop repeatable while the new item and lodge-healing loop is
being tested.
Captured creatures, team/collection membership, lead choice, experience, levels,
evolution, and chosen attacks are retained through subsequent battles and map
transitions in this running session. The last active battler becomes your lead
for the next battle. Quitting or restarting now preserves progress after the
player saves from the overworld.

Additional PSP checks:
1. Enter woods tall grass and try all four attacks across multiple battles.
2. Check that uses decrease, HP bars change, and each action requires X.
3. Cancel attack selection with Circle without spending a turn.
4. Try RUN; a failure should produce only one enemy response.
5. Win a battle and confirm you return to the same place with your whole team restored.
6. Lose all party members against a stronger cave opponent and confirm return to the clearing.
7. Test HOME > Cancel/Quit during both menus and battle messages.

## Creature progression

Each immutable species definition has a stable ID, name, description, element,
base HP/attack/defense/speed, experience yield, evolution threshold and target,
learnset, and sprite references. Individual creatures track species, nickname,
level, cumulative experience, current HP, derived stats, four learned attack
slots, and remaining uses. Nicknames are supported in the data model; a rename
interface is not part of this phase. Graphics are still original placeholders.

| ID | Species | Element | Evolution |
| --- | --- | --- | --- |
| 0 | Cindlet | Ember | Emberlyn at level 8 |
| 1 | Emberlyn | Ember | Final form |
| 2 | Mosslet | Grove | Mosshorn at level 8 |
| 3 | Mosshorn | Grove | Final form |
| 4 | Twiglint | Grove | No evolution |
| 5 | Glowmoth | Wind | No evolution |
| 6 | Flintling | Stone | Flintaur at level 9 |
| 7 | Flintaur | Stone | Final form |
| 8 | Duskwisp | Wind | No evolution |
| 9 | Echocrag | Stone | No evolution |
| 10 | Reedskip | Grove | No evolution |
| 11 | Sunfinch | Wind | No evolution |

The encounter tables offer eight wild species. Cindlet is the starter, and
three evolved forms complete the twelve-species database. Captured Mosslet and
Flintling can evolve through the same progression system as the starter.

A victory awards enemy species experience-yield multiplied by enemy level to
the active battler at the moment of victory. Earlier participants and stored
creatures receive no share. Capturing, escaping, or losing grants no XP. XP is
awarded once per battle. The cumulative
threshold for level L is 20 * (L - 1)^2, with a level cap of 100. Excess XP carries
toward the next level; multiple level gains from one award are supported.
Stat formulas are base HP + 5*level, base attack + 3*level, base defense + 2*level,
and base speed + 2*level. Evolution uses the new species' bases.

Cindlet begins at level 5 with 320 cumulative XP:
- Level 6 (500 XP): offers HEAT SPIRAL.
- Level 8 (980 XP): evolves into Emberlyn and offers eligible attacks for its new form.
- Level 9 (1280 XP): offers FLARE CREST.

Empty attack slots fill automatically. If all four are occupied, D-pad selects
an existing attack to replace; X confirms. KEEP CURRENT MOVES is selected by
default, and Circle declines. Each newly eligible attack is offered separately.
Evolution can offer unknown attacks from the evolved learnset, including moves
you previously declined or replaced. Existing attacks are never silently removed.

Level-up, evolution, and learned/replaced attacks each receive a message.
Evolution updates stats, displayed species name, and the placeholder silhouette.
The battle menu reports XP remaining. Select in the overworld shows your
lead partner's stats/total XP, followed by its species description. Progress survives
battles and changing maps, and is retained across quitting when saved first.

Progression regression test on PSP:
1. Press Select while standing still; check Cindlet's level, XP, and description.
2. Win forest battles and verify total XP increases; escape once and confirm no XP.
3. Reach level 6, choose a move to replace with HEAT SPIRAL, and use it next battle.
4. Reach level 8 and check the Emberlyn name, new silhouette, and increased stats.
5. Try declining an offered attack without losing existing moves.
6. Enter/leave the lodge and check that level, species, XP, and attacks are retained.
7. Save, confirm HOME exit still works, relaunch, and load with L to restore progress.

## Capture, party, and collection

CAPTURE uses an original reusable **Resonance Loom**, with three charges for each
battle. The confirmation
screen shows the current success percentage and remaining charges; X attempts
a capture, and Circle cancels. Lowering the wild creature's HP improves its odds,
but a knocked-out creature cannot be captured. A successful capture ends the
battle, adds the creature to the first available party slot or to the collection
when the party is full, and grants no XP. A failed attempt spends one charge and
allows exactly one enemy response. Canceling, having no charges, or having both
party and collection full spends neither a charge nor a turn.

The pure capture calculation is:

    chance = 35 - 12 * rarity
             + floor(50 * (maximum_HP - current_HP) / maximum_HP)
             + 10 * (strength - 1)

The result is clamped to 5-95 percent. Strength is clamped to 1-5; the current
Loom always uses strength 1. Invalid or fainted targets have a zero chance.
Common Mosslet/Flintling/Reedskip have rarity 0; Cindlet/Twiglint/Duskwisp have
rarity 1; Glowmoth/Echocrag/Sunfinch/evolved species have rarity 2. At full HP the
current Loom's chances are 35%, 23%, and 11% respectively. Each attempt draws a
0-99 roll and succeeds when the roll is below the percentage.

Press Triangle while stationary outside battle, then select CREATURES or COLLECTION:

- Left/right changes between PARTY and COLLECTION; up/down selects a creature.
- In PARTY, X opens SET AS LEAD, MOVE TO COLLECTION, and CANCEL. The lead starts
  the next battle and appears in the Select summary. A fainted member cannot be
  selected as lead.
- In COLLECTION, X withdraws the selected creature when a party slot is free.
  With a full party, X opens a choice of which party member to swap with it.
- Circle backs out of the current action, or returns to the Field Kit while browsing.
  Triangle closes the entire menu from any view.

The party holds four members; the collection holds 24. Depositing the final
party member or depositing into a full collection is blocked. There is no
release/delete action. Deposits, withdrawals, and swaps preserve each creature's
species, nickname, level, experience, stats, HP, learned moves, and remaining
uses. They do not heal or reset a creature. The separate end-of-battle restoration
rule is the only automatic refill.

In battle, CREATURES lists the party's current HP. Switching voluntarily to a
different healthy member spends a turn and gives the opponent one action.
Selecting the active member or a fainted member makes no change and spends no
turn. If the active member is knocked out and a healthy reserve remains, select
a replacement; this forced switch is free and cannot be canceled. Defeat occurs
only when no healthy party reserve remains. Collection members cannot battle
until withdrawn or swapped into the party outside battle.

## Items and healing

The item bag starts each new session with three **Pulse Tonics** and one
**Hearth Restore**. Pulse Tonics restore up to 25 HP; a Hearth Restore fills the
active Veyling's HP. Items cannot revive a fainted Veyling, cannot be used at
full HP, and are consumed only after a valid use.

In battle, choose ITEMS, move through the bag with the D-pad, and press X. A
valid healing item uses the player's turn and permits exactly one enemy response.
Circle returns to the battle menu without spending a turn. The inventory is
copied into the battle state and returned to exploration when the battle ends,
so quantities remain consistent for the whole session.

The Wayfarer Lodge now has a green Hearth Dais in its northwest room. Face the
dais and press X to restore every party member's HP and attack uses. The lodge
interaction pauses movement and confirms the refill in dialogue. The dais does
not add items or change party membership.

Tavi's existing two-page lodge dialogue remains available. Finish the dialogue
while facing Tavi to open the Wayfarer supply counter. Embermarks are session
currency; buy a selected item with X, then close the counter with Circle.

Phase 8 PSP test route:

1. Press Triangle at the clearing, then choose CREATURES or COLLECTION to inspect the starter and the empty collection.
   Try depositing the sole party member and verify the explanatory message.
2. Enter woods tall grass. Open CAPTURE and cancel once; check that charges and
   HP are unchanged. Damage a wild Veyling and check that its displayed odds rise.
3. Attempt captures. A failed attempt should spend one charge and produce one
   enemy response. After all three charges are used, another attempt must not
   spend a turn. Enter a new battle and verify the Loom is recharged.
4. Capture a Veyling, inspect it with Triangle, and confirm that catching grants
   no XP. Capture enough to fill the party; the next catch must go to COLLECTION.
5. Select a caught creature in PARTY and SET AS LEAD. Close the menu and press
   Select; the summary and next battle's first battler must use that creature.
6. Use CREATURES during battle to switch voluntarily and observe one enemy reply.
   Let a member be knocked out with healthy reserves remaining; choose a free
   replacement. Win with the caught creature and check that only the final
   battler receives XP and can learn attacks/evolve through normal progression.
7. Outside battle, deposit a nonfinal party member, withdraw it into the free
   slot, then choose a stored creature with a full party to swap. Check each
   creature's identity, XP, and attacks, and verify the lead indicator remains valid.
8. Walk through map transitions and reopen the party/collection menu. Check that
   membership and progression remain intact. After any battle, check restoration
   of the entire party and collection. Lose the whole party and verify the return
   to the clearing; the last battler remains the lead after restoration.
9. Test HOME > Cancel/Quit while browsing collection, confirming captures, and
   switching.
10. Enter a battle with reduced HP, choose ITEMS, use a Pulse Tonic, and verify
    the item count drops and the enemy receives one response. Try a full-HP and
    out-of-stock use; both should be rejected without an enemy response.
11. Enter the lodge, face the green Hearth Dais, and press X. Confirm that all
    party HP and attack uses refill and that the dialogue can be closed normally.
12. Press START while stationary, complete the PSP save dialog, then change
    maps and buy or use an item. Press L, load the latest save, and confirm the
    saved map, party, collection, progression, quantities, and Embermarks return.
    Cancel a save/load dialog and confirm the current session remains usable.

The automated tests also fill all 28 available creature slots and verify safe
rejection of another capture, collection scrolling, and blocked deposits into a
full collection; filling the entire collection manually is optional for PSP testing.

## Architecture and exact source tree

    psp/
      .gitattributes
      .gitignore
      Makefile
      README.md
      SOURCE_CONTENTS.md
      assets/maps/
        clearing.inc
        forest.inc
        lodge.inc
        cave.inc
        marsh.inc
        rest.inc
      include/
        attacks.h
        audio.h
        audio_synth.h
        battle.h
        camera.h
        capture.h
        creature.h
        dialogue.h
        encounter.h
        game.h
        graphics.h
        input.h
        map.h
        npc.h
        party.h
        party_menu.h
        inventory.h
        save_data.h
        player.h
        player_menu.h
        text.h
        world_draw.h
      src/
        main.c
        attacks.c
        audio.c
        audio_synth.c
        battle.c
        battle_draw.c
        camera.c
        capture.c
        creature.c
        dialogue.c
        encounter.c
        game.c
        graphics.c
        input.c
        map.c
        npc.c
        party.c
        party_menu.c
        inventory.c
        save_data.c
        player.c
        player_menu.c
        text.c
        world_draw.c
      tests/
        host/audio_stub.c
        host/pspgu.h
        host/psputility.h
        host/psputility_savedata.h
        host/psputility_sysparam.h
        host/save_data_stub.c
        battle_test.c
        battle_party_test.c
        inventory_test.c
        creature_test.c
        overworld_test.c
        party_capture_test.c
        polish_test.c
        save_data_test.c
        world_systems_test.c
        preview.py
        run.sh

SOURCE_CONTENTS.md contains the full text of all authored PSP project files
except itself. Generated binaries and previews are ignored by Git.

main.c initializes PSP services, seeds encounter randomness, and runs the existing
vblank-paced loop. game.c coordinates map entry, step events, NPC interaction,
and battle entry/return. Player movement accepts an optional occupancy callback;
NPC collision reserves both current and destination tiles to avoid overlap.
npc.c owns dialogue data and patrol behavior. dialogue.c stores bounded text
pages. encounter.c owns random selection and grace steps. map.c owns map lookup,
terrain collision, encounter areas, and explicit portal destinations.

world_draw.c draws tiles and actors. text.c renders an original 5x7 bitmap font.
graphics.c retains GU rectangles, double buffering, and vblank synchronization.
The build links PSPSDK GU, savedata utility, and audio libraries; no external
engines, art, or audio assets are required.

player_menu.c owns the Field Kit and session options. audio.c manages the PSP
audio callback and passes control changes through aligned word mailboxes.
audio_synth.c owns deterministic stereo PCM synthesis without platform calls,
heap allocation, or floating-point work in the callback. pspaudiolib is linked
before the PSP import libraries, with the utility library last, to keep import stubs ordered.

## Map format

Each assets/maps/*.inc file contains quoted, comma-terminated C strings: one
character per tile, one row per string. Rows must match the dimensions in map.c.

| Code | Meaning | Walkable |
| --- | --- | --- |
| . | Ordinary grass | Yes |
| = | Path | Yes |
| , | Decorative flowers | Yes |
| # | Tree | No |
| O | Rock/furniture | No |
| ~ | Water | No |
| W | Building wall | No |
| D | Doorway | Yes |
| > | Area exit marker | Yes |
| < | West exit marker | Yes |
| _ | Safe floor | Yes |
| H | Hearth healing dais | Yes |
| g | Woods encounter grass | Yes |
| c | Cave encounter floor | Yes |
| r | Marsh encounter reeds | Yes |

Portal behavior is defined by map ID and coordinates in map.c's portals table,
not by the visual tile alone. Each portal declares an explicit walkable arrival
tile away from the return trigger. Unknown codes/out-of-bounds locations block
movement. All maps and graphics are embedded in the EBOOT.

attacks.c defines attack power, accuracy, type, and uses. battle.c owns battle
turn sequencing, results, escape logic, and damage. battle_draw.c owns the
battle screen and original placeholder creature graphics. creature.c owns the
immutable species database and reusable individual-creature data, stat calculation,
experience thresholds, evolution, and attack learning.
party.c owns the four-member party and 24-slot collection, lossless transfers,
lead selection, and restoration. capture.c owns the pure rarity/chance/roll
calculations. party_menu.c owns the overworld roster controls and drawing.
battle.c coordinates battle inventory uses and item turns. inventory.c owns item
definitions, quantities, descriptions, and capped healing rules. game.c owns
the lodge Hearth Dais interaction and transfers the inventory back to exploration.
save_data.c owns the versioned PSP savedata payload, save/load dialog lifecycle,
and validation boundary; game.c snapshots and restores only safe overworld state.
battle.c coordinates capture charges, caught-creature placement, switching,
forced replacements, and rewards; game.c transfers the complete roster back to
exploration and applies the temporary end-of-battle restoration.

To add a map, add its rows, ID, dimensions, name, portal connections, and optional
NPC definitions / encounter terrain. Keep coordinates within the declared map
and extend tests for new dimensions if they exceed the current test grid.

## Build commands

This machine uses the existing Ubuntu/WSL PSPDEV environment.

From PowerShell, build the Phase 9 artifact:

    wsl -d Ubuntu -- bash -lc 'cd /mnt/c/Users/polo1/OneDrive/Documents/app/psp && make PSP_EBOOT=EBOOT-PHASE9.PBP EXTRA_TARGETS=EBOOT-PHASE9.PBP'

In a configured Linux/WSL shell, from the psp directory:

    make PSP_EBOOT=EBOOT-PHASE9.PBP EXTRA_TARGETS=EBOOT-PHASE9.PBP

For the conventional EBOOT.PBP output when that file is not open elsewhere:

    make

The user-mode PRX targets 6.60/6.61 custom firmware. The Makefile generates header
dependencies automatically and repackages the title when its settings change.

## Validation and rendering budget

From the psp directory in Linux/WSL:

    sh tests/run.sh

All nine test executables use AddressSanitizer and UndefinedBehaviorSanitizer.
Checks include Phase 2 regression coverage plus portal reachability and all ten
transitions, valid/non-trigger arrival tiles, NPC collisions in both directions,
patrol limits, dialogue pause/advance/cancel, no stationary encounters, weighted
table level ranges, encounter grace steps, text bounds, and drawing budget.
Battle checks additionally cover turn order, accuracy, elemental damage, spent
moves, fallback attacks, win/loss, no retaliation after knockout, menu controls,
escape success/failure, battle freezing the world, and defeat returning home.
Creature checks cover all twelve species at levels 1-100, distinct known attacks,
XP boundaries, large awards, the level cap, evolution, nickname preservation,
replacement/decline controls, and rewards being applied only once. Integration
checks carry an evolved partner and chosen attacks through world transitions.
party_capture_test.c verifies all party/collection limits, complete state
preservation across transfers, lead adjustments, restoration, rarity mapping,
health/strength effects, and exact capture roll boundaries. battle_party_test.c
covers successful/failed captures, charge exhaustion, full capacity, single
enemy responses, voluntary/forced switches, whole-team defeat, no capture XP,
and correct reward ownership. World integration checks exercise party menu
controls, empty/full collection states, scrolling, transfers, map persistence,
capture returns, and whole-roster restoration.
inventory_test.c verifies default quantities, capped tonic healing, full restores,
and invalid-use rejection. The PSP build links PSPSDK savedata utility support;
the host world test uses a no-op savedata stub so rendering and gameplay checks
remain platform independent.
save_data_test.c exercises the real savedata service with simulated utility
states. It covers success with and without observing FINISHED, cancellation,
errors, load size/header validation, overlapping requests, and shutdown retries.
The service checks the final utility result after shutdown instead of requiring
the FINISHED state to be observed. Actual Memory Stick persistence still needs
the PSP save/load test route above.
polish_test.c checks menu actions, field-item validation, independent options,
long-text wrapping, audio amplitude bounds, silence after muting, all four
tracks and seven effects, and deterministic playback across buffer sizes.
World integration adds the new route, encounters, menu movement blocking, and
animated/immediate HP updates. Save tests lock the version-1 payload size and
original IDs. The host test renders the real draw functions into software pixel buffers;
tests/preview.py converts those buffers to PNG without third-party dependencies.

Software previews are saved in previews/dialogue.png, previews/encounter.png,
previews/battle-menu.png, previews/battle-moves.png, previews/learn-move.png,
previews/evolution.png, previews/partner.png, previews/capture.png,
previews/captured.png, previews/party.png, previews/collection.png,
previews/collection-swap.png, previews/battle-switch.png,
previews/collection-empty.png, and previews/collection-full.png.
Additional Phase 9 previews include player-menu.png, marsh.png, lantern-rest.png,
field-items.png, options.png, reedskip.png, sunfinch.png, battle-impact.png, and
saved-dialogue.png. Four eight-second PCM WAV samples are also written under
previews/: hearthlight.wav, fernwind.wav, hollow-echo.wav, and loom-duel.wav.
These previews are not emulator or hardware captures.

Only visible terrain is drawn (at most 160 tiles). The GU list reserves 1 MiB for
tile, actor, and bitmap text commands. The tested scenes stay below a conservative
6000-rectangle budget. Two color buffers still occupy 1,114,112 bytes of VRAM.
The main loop retains its 50 ms elapsed-time cap and HOME callback service.

The Phase 9 release checks are the nine host regression/integration suites and
PSP compilation/linking with -Wall -Wextra -Werror, followed by packaging
EBOOT-PHASE9.PBP. Software previews exercise the real drawing functions but do
not replace hardware testing. Phase 8 save/load was confirmed working by the
player. Phase 9 sound playback, performance, suspend/resume, and the new route
still require the PSP test route above.

Phase 9 is ready for PSP testing.

## Official references

- [PSPDEV installation](https://pspdev.github.io/installation.html)
- [PSPSDK GU reference](https://pspdev.github.io/pspsdk/group__GU.html)
- [PSPSDK build rules](https://github.com/pspdev/pspsdk/blob/master/src/base/build.mak)
````

## src/attacks.c

````text
#include "attacks.h"
static const Attack attacks[MOVE_COUNT] = {
    {"NUDGE",28,100,ELEMENT_PLAIN,24},
    {"CINDER ARC",40,95,ELEMENT_EMBER,12},
    {"LEAF LASH",40,95,ELEMENT_GROVE,12},
    {"PEBBLE BURST",42,90,ELEMENT_STONE,12},
    {"WHIRL CUT",38,100,ELEMENT_WIND,12},
    {"BOLD LUNGE",55,75,ELEMENT_PLAIN,8},
    {"HEAT SPIRAL",55,95,ELEMENT_EMBER,10},
    {"FLARE CREST",70,90,ELEMENT_EMBER,8},
    {"VINE SWEEP",58,95,ELEMENT_GROVE,10},
    {"FAULT DRUM",62,90,ELEMENT_STONE,10},
    {"TEMPEST THREAD",56,100,ELEMENT_WIND,10}
};
const Attack *attack_get(int id)
{
    return &attacks[id>=0 && id<MOVE_COUNT ? id : MOVE_NUDGE];
}
const char *element_name(Element element)
{
    const char *const names[]={"PLAIN","GROVE","EMBER","STONE","WIND"};
    return names[element>=ELEMENT_PLAIN && element<=ELEMENT_WIND ? element : ELEMENT_PLAIN];
}
int attack_effectiveness(Element a,Element d)
{
    if(a==ELEMENT_PLAIN || d==ELEMENT_PLAIN) return 2;
    if((a==ELEMENT_EMBER && d==ELEMENT_GROVE) ||
       (a==ELEMENT_GROVE && d==ELEMENT_STONE) ||
       (a==ELEMENT_STONE && d==ELEMENT_WIND) ||
       (a==ELEMENT_WIND && d==ELEMENT_EMBER)) return 4;
    if(a==d || (d==ELEMENT_EMBER && a==ELEMENT_GROVE) ||
       (d==ELEMENT_GROVE && a==ELEMENT_STONE) ||
       (d==ELEMENT_STONE && a==ELEMENT_WIND) ||
       (d==ELEMENT_WIND && a==ELEMENT_EMBER)) return 1;
    return 2;
}
````

## src/audio_synth.c

````text
#include "audio_synth.h"

/* Original 32-step phrases: Hearthlight, Fernwind, Hollow Echo, and Loom Duel.
   Frequency tables and integer oscillators keep audio independent of assets. */
static const unsigned short phrases[MUSIC_COUNT][32]={
    {330,392,440,0,392,330,294,0,262,330,392,0,330,294,262,0,
     330,392,523,0,494,440,392,0,440,392,330,294,330,262,0,0},
    {392,440,523,440,587,523,440,0,392,330,392,440,330,294,0,0,
     440,523,659,587,523,440,392,0,330,392,440,523,392,330,0,0},
    {220,0,262,0,294,0,330,0,247,0,220,0,196,0,0,0,
     220,0,330,0,349,0,294,0,262,0,247,0,220,0,0,0},
    {330,330,392,440,330,494,440,392,294,294,349,392,440,392,349,294,
     330,392,494,523,494,440,392,330,294,349,440,494,392,330,294,0}
};
static const unsigned short bass[MUSIC_COUNT][4]={
    {131,110,98,131},{131,165,110,98},{110,87,98,110},{165,147,131,147}
};
static unsigned int increment(unsigned int hz)
{
    return (unsigned int)(((uint64_t)hz<<32)/AUDIO_RATE);
}
static int triangle(unsigned int phase)
{
    int v=(int)(phase>>20);
    return v<2048?v*2-2048:6143-v*2;
}
void audio_synth_init(AudioSynth *s) { *s=(AudioSynth){0}; }
void audio_synth_scene(AudioSynth *s,MusicScene scene)
{
    if (scene<0 || scene>=MUSIC_COUNT || scene==s->scene) return;
    s->scene=scene;s->clock=0;s->gain=0;
}
void audio_synth_effect(AudioSynth *s,Sound effect)
{
    if (effect<=SOUND_NONE || effect>=SOUND_COUNT) return;
    s->effect=effect;s->effect_clock=0;s->effect_phase=0;
}
void audio_synth_render(AudioSynth *s,int16_t *stereo,unsigned int frames,int music,int effects)
{
    unsigned int beat=s->scene==MUSIC_BATTLE?6615:s->scene==MUSIC_CAVE?13230:11025;
    for (unsigned int i=0;i<frames;++i) {
        unsigned int local=s->clock%beat;
        if (!local) {
            unsigned int note=(s->clock/beat)%32;
            s->step=increment(phrases[s->scene][note]);
            s->bass_step=increment(bass[s->scene][note/8]);
        }
        int target=music?128:0;
        if (s->gain<target) ++s->gain;
        if (s->gain>target) --s->gain;
        s->phase+=s->step;s->bass_phase+=s->bass_step;
        int envelope=local<256?(int)local:local>beat-256?(int)(beat-local):256;
        int melody=s->step?triangle(s->phase)*envelope/256:0;
        int sample=(melody+triangle(s->bass_phase)/2)*s->gain/128;
        if (s->effect!=SOUND_NONE) {
            unsigned int t=s->effect_clock++, duration=4410, hz=660;
            switch(s->effect) {
            case SOUND_CURSOR: hz=700;duration=1764;break;
            case SOUND_CONFIRM: hz=t<2205?660:880;break;
            case SOUND_HIT: hz=100+(4410-(t<4410?t:4410))/12;break;
            case SOUND_HEAL: hz=t<6615?523:t<13230?659:784;duration=19845;break;
            case SOUND_BOND: hz=392+(t/2205)*65;duration=17640;break;
            case SOUND_VICTORY: hz=t<6615?523:t<13230?659:t<19845?784:1047;duration=26460;break;
            case SOUND_ERROR: hz=t<4410?220:165;duration=8820;break;
            default: break;
            }
            int effect_target=effects && t<duration?128:0;
            if (s->effect_gain<effect_target) ++s->effect_gain;
            if (s->effect_gain>effect_target) --s->effect_gain;
            if (!(t&63)) s->effect_step=increment(hz);
            s->effect_phase+=s->effect_step;
            int attack=t<128?(int)t:128;
            sample+=triangle(s->effect_phase)*s->effect_gain/128*attack/128;
            if (t>=duration && !s->effect_gain) s->effect=SOUND_NONE;
        }
        stereo[i*2]=(int16_t)sample;stereo[i*2+1]=(int16_t)sample;
        if (++s->clock>=beat*32) s->clock=0;
    }
}
````

## src/audio.c

````text
#include <pspaudiolib.h>
#include "audio_synth.h"

static AudioSynth synth;
/* Single aligned word mailboxes: the main thread owns writes, the callback
   owns the synthesizer. No mutable sample buffers are shared across threads. */
static volatile unsigned int control=0x300;
static volatile unsigned int effect_command;
static unsigned int last_effect;
static int ready;

static void callback(void *buffer,unsigned int frames,void *context)
{
    (void)context;
    unsigned int settings=control, command=effect_command;
    audio_synth_scene(&synth,(MusicScene)(settings&255));
    if (command!=last_effect) {
        audio_synth_effect(&synth,(Sound)(command&255));last_effect=command;
    }
    audio_synth_render(&synth,buffer,frames,(settings&256)!=0,(settings&512)!=0);
}
int audio_init(void)
{
    audio_synth_init(&synth);
    control=0x300;effect_command=last_effect=0;
    if (pspAudioInit()<0) return 0;
    ready=1;
    pspAudioSetVolume(0,PSP_VOLUME_MAX/2,PSP_VOLUME_MAX/2);
    pspAudioSetChannelCallback(0,callback,0);
    return 1;
}
void audio_shutdown(void)
{
    if (!ready) return;
    pspAudioEnd();ready=0;
}
void audio_scene(MusicScene scene)
{
    if (scene>=0 && scene<MUSIC_COUNT) control=(control&0x300)|(unsigned int)scene;
}
void audio_play(Sound sound)
{
    if (sound>SOUND_NONE && sound<SOUND_COUNT && (control&512))
        effect_command=((effect_command+256)&~255u)|(unsigned int)sound;
}
void audio_settings(int music,int effects)
{
    control=(control&255)|(music?256u:0)|(effects?512u:0);
}
````

## src/battle_draw.c

````text
#include <stdio.h>
#include <pspgu.h>
#include "battle.h"
#include "capture.h"
#include "graphics.h"
#include "text.h"
#define C(r,g,b) GU_RGBA(r,g,b,255)

static void creature_draw(int x,int y,const Battler *unit,int back)
{
    Element element=unit->element;
    int sprite=species_get(unit->species)->battle_sprite;
    unsigned int body=element==ELEMENT_EMBER?C(224,123,67):
        element==ELEMENT_GROVE?C(114,167,92):element==ELEMENT_STONE?C(141,149,174):C(155,140,208);
    graphics_rectangle(x-6,y+48,78,8,C(33,45,53));
    graphics_rectangle(x+4,y+18,52,32,body);
    graphics_rectangle(x+12,y+8,36,30,body);
    graphics_rectangle(x+8,y+45,12,10,body);
    graphics_rectangle(x+40,y+45,12,10,body);
    if(element==ELEMENT_EMBER) {
        graphics_rectangle(x+16,y,8,12,C(255,188,91));
        graphics_rectangle(x+33,y-5,8,17,C(255,188,91));
        graphics_rectangle(x+55,y+28,12,8,C(236,162,76));
    } else if(element==ELEMENT_GROVE) {
        graphics_rectangle(x+2,y+1,23,8,C(77,121,74));
        graphics_rectangle(x+34,y-4,22,8,C(168,194,96));
    } else if(element==ELEMENT_STONE) {
        graphics_rectangle(x+7,y+4,15,16,C(190,192,197));
        graphics_rectangle(x+39,y+3,17,19,C(108,115,145));
    } else {
        graphics_rectangle(x-14,y+15,24,12,C(185,169,226));
        graphics_rectangle(x+52,y+15,24,12,C(185,169,226));
    }
    /* Species references select small original silhouette details. */
    if(sprite==1) { /* Emberlyn: large mane and crown. */
        graphics_rectangle(x-2,y+18,8,25,C(245,171,80));
        graphics_rectangle(x+56,y+18,8,25,C(245,171,80));
        graphics_rectangle(x+24,y-9,9,17,C(255,210,104));
    } else if(sprite==3) { /* Mosshorn */
        graphics_rectangle(x+2,y-8,7,24,C(191,177,112));
        graphics_rectangle(x+49,y-8,7,24,C(191,177,112));
    } else if(sprite==7) { /* Flintaur */
        graphics_rectangle(x-6,y+16,14,30,C(107,121,151));
        graphics_rectangle(x+52,y+16,14,30,C(107,121,151));
    } else if(sprite==4) {
        graphics_rectangle(x+23,y-10,5,19,C(162,185,96));
    } else if(sprite==8) {
        graphics_rectangle(x+21,y-8,20,10,C(208,192,234));
    } else if(sprite==9) {
        graphics_rectangle(x+23,y-9,15,17,C(158,174,197));
    } else if(sprite==10) { /* Reedskip: reed feelers and long springing feet. */
        graphics_rectangle(x+13,y-13,3,24,C(223,196,112));
        graphics_rectangle(x+40,y-18,3,28,C(223,196,112));
        graphics_rectangle(x-2,y+46,22,8,C(153,187,104));
        graphics_rectangle(x+40,y+46,22,8,C(153,187,104));
    } else if(sprite==11) { /* Sunfinch: gold crest, beak, and forked tail. */
        graphics_rectangle(x+17,y-11,8,18,C(249,207,104));
        graphics_rectangle(x+28,y-17,8,24,C(239,169,76));
        graphics_rectangle(x+25,y+30,12,7,C(245,189,87));
        graphics_rectangle(x-13,y+40,17,5,C(249,207,104));
        graphics_rectangle(x-17,y+49,21,5,C(239,169,76));
    }
    if(!back) {
        graphics_rectangle(x+17,y+23,6,7,C(23,31,41));
        graphics_rectangle(x+38,y+23,6,7,C(23,31,41));
        graphics_rectangle(x+27,y+35,8,3,C(58,54,57));
    } else {
        graphics_rectangle(x+21,y+20,23,5,C(255,179,94));
    }
}
static void status(const Battler *unit,int x,int y,int width,float shown_hp)
{
    char line[64];
    graphics_rectangle(x,y,width,58,C(25,35,45));
    snprintf(line,sizeof(line),"%s  LV %d",creature_name(unit),unit->level);
    text_draw(x+8,y+7,line,C(241,232,207),1);
    text_draw(x+8,y+20,element_name(unit->element),C(174,192,188),1);
    int bar_width=width-16;
    graphics_rectangle(x+8,y+32,bar_width,6,C(66,72,78));
    int filled=(int)(bar_width*shown_hp/unit->max_hp);
    if(filled>bar_width) filled=bar_width;
    if(filled>0) graphics_rectangle(x+8,y+32,filled,6,
        unit->hp*4<=unit->max_hp?C(228,121,99):C(125,201,154));
    snprintf(line,sizeof(line),"HP %d / %d",unit->hp,unit->max_hp);
    text_draw(x+8,y+44,line,C(224,227,218),1);
}
void battle_draw(const Battle *b)
{
    graphics_rectangle(0,0,480,272,C(48,65,74));
    graphics_rectangle(0,88,480,88,C(65,81,77));
    graphics_rectangle(0,0,480,15,C(19,28,36));
    text_draw(10,4,"WILD VEYLING ENCOUNTER",C(241,204,145),1);
    static const int bob[]={0,-1,-2,-1,0,1,2,1};
    int frame=(int)(b->animation*6)%8;
    int shake=b->hit_time>0?((int)(b->hit_time*40)%2?3:-3):0;
    creature_draw(325+(b->hit_side==1?shake:0),38+bob[frame],&b->enemy,0);
    creature_draw(65+(b->hit_side==0?shake:0),104+bob[(frame+4)%8],&b->ally,1);
    if (b->hit_time>0) {
        int x=b->hit_side?355:95,y=b->hit_side?60:127;
        graphics_rectangle(x-12,y,28,3,C(255,221,139));
        graphics_rectangle(x,y-12,3,28,C(255,221,139));
    }
    status(&b->enemy,18,24,202,b->enemy_hp_shown);
    status(&b->ally,253,109,210,b->ally_hp_shown);
    graphics_rectangle(6,176,468,90,C(177,144,94));
    graphics_rectangle(8,178,464,86,C(21,30,38));
    if(b->phase==BATTLE_SWITCH) {
        text_draw(18,188,b->forced_switch?"CHOOSE A READY VEYLING.":"SWITCH YOUR ACTIVE VEYLING.",C(244,198,118),1);
        for(int i=0;i<b->party.count;++i) {
            const Creature *member=i==b->active?&b->ally:&b->party.members[i];
            char line[80];
            if(i==b->switch_cursor) graphics_rectangle(14,200+i*13,450,12,C(79,92,86));
            snprintf(line,sizeof(line),"%s  LV %d  HP %d/%d  %s",creature_name(member),member->level,member->hp,member->max_hp,
                     i==b->active?"ACTIVE":member->hp<=0?"NEEDS REST":"READY");
            text_draw(20,203+i*13,line,C(236,236,218),1);
        }
        text_draw(18,255,b->forced_switch?"X SEND OUT - A REPLACEMENT IS REQUIRED":"X SWITCH   O BACK - SWITCHING USES A TURN",C(167,194,180),1);
    } else if(b->phase==BATTLE_CAPTURE) {
        char line[80];
        text_draw(18,188,"RESONANCE LOOM",C(244,198,118),2);
        snprintf(line,sizeof(line),"CHARGES %d/3   BOND CHANCE %d/100",b->capture_charges,capture_chance(&b->enemy,1));
        text_draw(18,210,line,C(236,236,218),1);
        text_draw(18,226,"WEAKEN THE VEYLING FOR A STRONGER BOND.",C(167,194,180),1);
        text_draw(18,247,"X WEAVE A BOND   O BACK",C(236,236,218),1);
        /* Original angular tuning frame; no sphere-shaped capture device. */
        graphics_rectangle(414,195,4,41,C(125,205,199));
        graphics_rectangle(454,195,4,41,C(125,205,199));
        graphics_rectangle(414,195,44,4,C(125,205,199));
        graphics_rectangle(414,232,44,4,C(125,205,199));
        graphics_rectangle(424,209,24,3,C(238,193,106));
        graphics_rectangle(424,222,24,3,C(238,193,106));
    } else if(b->phase==BATTLE_ITEMS) {
        text_draw(18,188,"ITEM BAG",C(244,198,118),2);
        text_draw(18,207,"USE AN ITEM ON YOUR ACTIVE VEYLING.",C(167,194,180),1);
        for(int i=0;i<ITEM_COUNT;++i) {
            char line[80];
            if(i==b->item_cursor) graphics_rectangle(14,220+i*15,270,14,C(79,92,86));
            snprintf(line,sizeof(line),"%s  x%d",inventory_item_name((ItemId)i),b->inventory.quantities[i]);
            text_draw(20,224+i*15,line,i==b->item_cursor?C(255,213,147):C(187,193,193),1);
        }
        text_draw(300,224,b->item_cursor==ITEM_PULSE_TONIC?"UP TO 25 HP RESTORED.":"FULL HP RESTORE.",C(220,225,215),1);
        text_draw(18,258,"X USE   O BACK",C(236,236,218),1);
    } else if(b->phase==BATTLE_LEARN) {
        const Attack *move=attack_get(b->growth.moves[b->growth_move]);
        text_draw(18,188,"LEARN A NEW ATTACK?",C(244,198,118),1);
        text_draw(18,205,move->name,C(236,236,218),1);
        char detail[64];
        snprintf(detail,sizeof(detail),"POWER %d  ACCURACY %d",move->power,move->accuracy);
        text_draw(18,220,detail,C(188,204,190),1);
        text_draw(18,239,"X REPLACE   O DECLINE",C(188,204,190),1);
        for(int i=0;i<5;++i) {
            if(i==b->learn_cursor) graphics_rectangle(290,182+i*15,174,14,C(79,92,86));
            text_draw(296,186+i*15,i==4?"KEEP CURRENT MOVES":attack_get(b->ally.moves[i])->name,C(239,227,200),1);
        }
    } else if(b->phase==BATTLE_MESSAGE || b->phase==BATTLE_DONE) {
        text_box(18,188,444,56,b->message,C(236,236,218));
        text_draw(18,251,"X CONTINUE",C(167,194,180),1);
    } else if(b->phase==BATTLE_MENU) {
        text_draw(18,191,"CHOOSE YOUR NEXT MOVE.",C(236,236,218),1);
        text_draw(18,210,"D-PAD SELECT   X CONFIRM",C(167,194,180),1);
        char growth[64];
        snprintf(growth,sizeof(growth),"%s - NEXT LEVEL IN %d XP",creature_name(&b->ally),creature_xp_remaining(&b->ally));
        text_wrap(18,230,274,19,growth,C(233,173,115),1);
        const char *const options[]={"FIGHT","CAPTURE","CREATURES","ITEMS","RUN"};
        for(int i=0;i<5;++i) {
            if(i==b->cursor) graphics_rectangle(302,182+i*15,158,14,C(79,92,86));
            text_draw(310,186+i*15,options[i],i==b->cursor?C(255,213,147):C(187,193,193),1);
        }
    } else {
        int available=0;
        for(int i=0;i<4;++i) available+=b->ally.uses[i];
        if(!available) {
            text_draw(18,190,"ALL ATTACKS ARE SPENT.",C(236,236,218),1);
            text_draw(18,211,"X PRESS ON - WEAK BUT UNLIMITED",C(236,236,218),1);
        } else {
            for(int i=0;i<4;++i) {
                char line[64];
                const Attack *move=attack_get(b->ally.moves[i]);
                if(i==b->move_cursor) graphics_rectangle(14,183+i*15,261,14,C(79,92,86));
                snprintf(line,sizeof(line),"%s  %d/%d",b->ally.moves[i]<0?"EMPTY":move->name,b->ally.uses[i],b->ally.moves[i]<0?0:move->uses);
                text_draw(20,187+i*15,line,b->ally.uses[i]?C(239,227,200):C(155,155,155),1);
            }
            const Attack *selected=attack_get(b->ally.moves[b->move_cursor]);
            char line[40];
            text_draw(295,190,element_name(selected->element),C(245,185,112),1);
            snprintf(line,sizeof(line),"POWER %d",selected->power);
            text_draw(295,208,line,C(220,225,215),1);
            snprintf(line,sizeof(line),"ACCURACY %d/100",selected->accuracy);
            text_draw(295,226,line,C(220,225,215),1);
        }
        text_draw(18,251,"X ATTACK   O BACK",C(167,194,180),1);
    }
}
````

## src/battle.c

````text
#include <stdio.h>
#include <string.h>
#include "battle.h"
#include "capture.h"

static uint32_t random_next(Battle *b)
{
    uint32_t x=b->random;
    x^=x<<13; x^=x>>17; x^=x<<5;
    return b->random=x;
}
void battler_restore(Battler *b) { creature_restore(b); }
void battler_starter(Battler *b) { creature_create(b,SPECIES_CINDLET,5); }
void battle_begin(Battle *b,const Battler *ally,SpeciesId species,int level,uint32_t seed)
{
    Party party={0};party.count=1;party.members[0]=*ally;
    battle_begin_party_with_inventory(b,&party,0,species,level,seed);
}
void battle_begin_party(Battle *b,const Party *party,SpeciesId species,int level,uint32_t seed)
{
    battle_begin_party_with_inventory(b,party,0,species,level,seed);
}
void battle_begin_party_with_inventory(Battle *b,const Party *party,const Inventory *inventory,SpeciesId species,int level,uint32_t seed)
{
    *b=(Battle){0};
    b->party=*party;b->active=party->lead;
    if (inventory) b->inventory=*inventory; else inventory_init(&b->inventory);
    b->ally=party->members[b->active]; b->random=seed?seed:0x3291u;
    b->capture_charges=3;
    creature_create(&b->enemy,species,level);
    b->ally_hp_shown=(float)b->ally.hp;b->enemy_hp_shown=(float)b->enemy.hp;
    b->phase=BATTLE_MESSAGE; b->after=AFTER_MENU;
    snprintf(b->message,sizeof(b->message),"A WILD %s APPEARS.\n%s IS READY.",creature_name(&b->enemy),creature_name(&b->ally));
}
int battle_damage(const Battler *a,const Battler *d,const Attack *move,int variation)
{
    int defense=d->defense>0?d->defense:1;
    if(variation<90) variation=90;
    if(variation>100) variation=100;
    int base=((2*a->level/5+2)*move->power*a->attack/defense)/20+2;
    int damage=base*attack_effectiveness(move->element,d->element)*variation/200;
    return damage<1?1:damage;
}
static void message(Battle *b,const char *text,BattleAfter after)
{
    snprintf(b->message,sizeof(b->message),"%s",text);
    b->phase=BATTLE_MESSAGE; b->after=after;
}
static int choose_enemy(Battle *b)
{
    int slots[4],count=0;
    for(int i=0;i<4;++i) if(b->enemy.uses[i]>0) slots[count++]=i;
    return count?slots[random_next(b)%(unsigned int)count]:-1;
}
static void sync_active(Battle *b)
{
    b->party.members[b->active]=b->ally;
    b->party.lead=b->active;
}
static int reserve_available(const Battle *b)
{
    for(int i=0;i<b->party.count;++i)
        if(i!=b->active && b->party.members[i].hp>0) return 1;
    return 0;
}
static void enemy_response(Battle *b)
{
    b->choices[1]=choose_enemy(b);b->order[1]=1;b->turn_index=1;
}
static void next_action(Battle *b)
{
    if(b->turn_index>=2) { b->phase=BATTLE_MENU; return; }
    int side=b->order[b->turn_index++];
    Battler *a=side==0?&b->ally:&b->enemy;
    Battler *d=side==0?&b->enemy:&b->ally;
    int slot=b->choices[side];
    /* PRESS ON is an unlimited weak fallback, only when every move is spent. */
    const Attack fallback={"PRESS ON",15,100,ELEMENT_PLAIN,0};
    const Attack *move=slot<0?&fallback:attack_get(a->moves[slot]);
    if(slot>=0) --a->uses[slot];
    int hit=(int)(random_next(b)%100)<move->accuracy;
    if(!hit) {
        snprintf(b->message,sizeof(b->message),"%s USED %s.\nTHE ATTACK MISSED.",creature_name(a),move->name);
    } else {
        int damage=battle_damage(a,d,move,90+(int)(random_next(b)%11));
        if(damage>d->hp) damage=d->hp;
        d->hp-=damage;
        b->hit_side=1-side;b->hit_time=0.24f;
        int effect=attack_effectiveness(move->element,d->element);
        snprintf(b->message,sizeof(b->message),"%s USED %s.\n%d DAMAGE. %s",creature_name(a),move->name,damage,
                 effect==4?"STRONG MATCH.":effect==1?"RESISTED.":"");
    }
    b->phase=BATTLE_MESSAGE; b->after=AFTER_TURN;
    if(d->hp==0) {
        if(side==0) b->result=BATTLE_WIN;
        else if(reserve_available(b)) b->forced_switch=1;
        else b->result=BATTLE_LOSS;
        /* Damage is shown first; the next confirmation shows the outcome. */
    }
}
static float approach_hp(float shown,int hp,float step)
{
    if (shown<hp) return shown+step>hp?(float)hp:shown+step;
    return shown-step<hp?(float)hp:shown-step;
}
void battle_animate(Battle *b,float seconds,int motion)
{
    if (!motion) {
        b->animation=0;b->hit_time=0;
        b->ally_hp_shown=(float)b->ally.hp;b->enemy_hp_shown=(float)b->enemy.hp;
        return;
    }
    if (seconds<0) seconds=0;
    if (seconds>0.05f) seconds=0.05f;
    b->animation+=seconds;
    if (b->animation>=8) b->animation-=8;
    if (b->hit_time>0) b->hit_time-=seconds;
    b->ally_hp_shown=approach_hp(b->ally_hp_shown,b->ally.hp,b->ally.max_hp*seconds*3);
    b->enemy_hp_shown=approach_hp(b->enemy_hp_shown,b->enemy.hp,b->enemy.max_hp*seconds*3);
}
static void begin_turn(Battle *b,int slot)
{
    b->choices[0]=slot; b->choices[1]=choose_enemy(b);
    int enemy_first=b->enemy.speed>b->ally.speed;
    if(b->enemy.speed==b->ally.speed) enemy_first=(int)(random_next(b)%2);
    b->order[0]=enemy_first; b->order[1]=1-enemy_first; b->turn_index=0;
    next_action(b);
}
static int navigation(Battle *b,const Input *input)
{
    int direction=input->vertical?input->vertical:input->horizontal;
    int edge=direction && direction!=b->previous_direction;
    b->previous_direction=direction;
    return edge?direction:0;
}
static void growth_next(Battle *b)
{
    if(b->growth_stage==0) {
        b->growth_stage=1;
        if(b->growth.old_level!=b->ally.level) {
            snprintf(b->message,sizeof(b->message),"LEVEL UP. %d TO %d\nHP %d  ATTACK %d\nDEFENSE %d  SPEED %d",b->growth.old_level,b->ally.level,
                     b->ally.max_hp,b->ally.attack,b->ally.defense,b->ally.speed);
            b->phase=BATTLE_MESSAGE;b->after=AFTER_GROWTH;return;
        }
    }
    if(b->growth_stage==1) {
        b->growth_stage=2;
        if(b->growth.old_species!=b->ally.species) {
            snprintf(b->message,sizeof(b->message),"EVOLUTION.\n%s BECAME %s.\nA NEW FORM. A STRONGER PARTNER.",
                     species_get(b->growth.old_species)->name,b->ally.name);
            b->phase=BATTLE_MESSAGE;b->after=AFTER_GROWTH;return;
        }
    }
    while(b->growth_move<b->growth.move_count) {
        int move=b->growth.moves[b->growth_move];
        int empty=-1,known=0;
        for(int i=0;i<4;++i) {
            if(b->ally.moves[i]==move) known=1;
            if(b->ally.moves[i]<0 && empty<0) empty=i;
        }
        if(known) { ++b->growth_move;continue; }
        if(empty>=0) {
            creature_learn(&b->ally,move,empty);++b->growth_move;
            snprintf(b->message,sizeof(b->message),"%s LEARNED\n%s.",creature_name(&b->ally),attack_get(move)->name);
            b->phase=BATTLE_MESSAGE;b->after=AFTER_GROWTH;return;
        }
        b->learn_cursor=4; /* Default to KEEP CURRENT MOVES; no silent replacement. */
        b->phase=BATTLE_LEARN;return;
    }
    message(b,"TEAM RESTORED AFTER BATTLE.\nX RETURN TO EXPLORING",AFTER_DONE);
}
void battle_update(Battle *b,const Input *input)
{
    int nav=navigation(b,input);
    if(b->phase==BATTLE_DONE) return;
    if(b->phase==BATTLE_CAPTURE) {
        if(input->cancel) { b->phase=BATTLE_MENU;return; }
        if(!input->confirm) return;
        if(!party_has_space(&b->party)) {
            message(b,"PARTY AND COLLECTION ARE FULL.\nNO MORE SPACE FOR NEW VEYLINGS.",AFTER_MENU);return;
        }
        if(b->capture_charges<=0) {
            message(b,"THE RESONANCE LOOM IS EMPTY.\nIT RECHARGES AFTER THIS BATTLE.",AFTER_MENU);return;
        }
        --b->capture_charges;
        if(capture_attempt(&b->enemy,1,random_next(b)%100)) {
            sync_active(b);
            int destination=party_add(&b->party,&b->enemy);
            b->result=BATTLE_CAUGHT;
            snprintf(b->message,sizeof(b->message),"%s JOINS YOU.\n%s\nTEAM RESTORED AFTER BATTLE.",creature_name(&b->enemy),
                     destination==1?"ADDED TO YOUR PARTY.":"SENT TO YOUR COLLECTION.");
            b->phase=BATTLE_MESSAGE;b->after=AFTER_DONE;
        } else {
            enemy_response(b);
            message(b,"THE RESONANCE THREAD FADES.\nTHE WILD VEYLING STAYS ALERT.",AFTER_TURN);
        }
        return;
    }
    if(b->phase==BATTLE_SWITCH) {
        if(input->cancel && !b->forced_switch) { b->phase=BATTLE_MENU;return; }
        if(nav) b->switch_cursor=(b->switch_cursor+nav+b->party.count)%b->party.count;
        if(!input->confirm) return;
        int selected=b->switch_cursor;
        if(selected==b->active || b->party.members[selected].hp<=0) {
            message(b,selected==b->active?"THAT VEYLING IS ALREADY ACTIVE.":"THAT VEYLING NEEDS A REST.",AFTER_SWITCH);return;
        }
        sync_active(b);b->active=selected;b->ally=b->party.members[selected];
        int forced=b->forced_switch;b->forced_switch=0;
        b->move_cursor=0;
        snprintf(b->message,sizeof(b->message),"%s TAKES THE FIELD.",creature_name(&b->ally));
        b->phase=BATTLE_MESSAGE;b->after=forced?AFTER_MENU:AFTER_TURN;
        if(!forced) enemy_response(b);
        return;
    }
    if(b->phase==BATTLE_ITEMS) {
        if(input->cancel) { b->phase=BATTLE_MENU;return; }
        if(nav) b->item_cursor=(b->item_cursor+nav+ITEM_COUNT)%ITEM_COUNT;
        if(!input->confirm) return;
        int restored=inventory_use_healing(&b->inventory,(ItemId)b->item_cursor,&b->ally);
        if(!restored) {
            message(b,b->inventory.quantities[b->item_cursor]<=0 ? "THAT ITEM IS OUT OF STOCK." :
                    b->ally.hp>=b->ally.max_hp ? "YOUR ACTIVE VEYLING IS ALREADY AT FULL HP." :
                    "THAT ITEM CANNOT BE USED HERE.",AFTER_MENU);
            return;
        }
        sync_active(b);enemy_response(b);
        snprintf(b->message,sizeof(b->message),"%s USED %s.\n%d HP RESTORED.",creature_name(&b->ally),
                 inventory_item_name((ItemId)b->item_cursor),restored);
        b->phase=BATTLE_MESSAGE;b->after=AFTER_TURN;
        return;
    }
    if(b->phase==BATTLE_LEARN) {
        if(nav) b->learn_cursor=(b->learn_cursor+nav+5)%5;
        if(input->cancel || (input->confirm && b->learn_cursor==4)) {
            ++b->growth_move;growth_next(b);return;
        }
        if(input->confirm) {
            int move=b->growth.moves[b->growth_move++];
            const char *old=attack_get(b->ally.moves[b->learn_cursor])->name;
            creature_learn(&b->ally,move,b->learn_cursor);
            snprintf(b->message,sizeof(b->message),"%s LEARNED %s.\nREPLACED %s.",creature_name(&b->ally),attack_get(move)->name,old);
            b->phase=BATTLE_MESSAGE;b->after=AFTER_GROWTH;
        }
        return;
    }
    if(b->phase==BATTLE_MESSAGE) {
        if(!input->confirm) return; /* Results cannot be accidentally canceled. */
        if(b->after==AFTER_DONE) { sync_active(b);b->phase=BATTLE_DONE; return; }
        if(b->after==AFTER_MENU) { b->phase=BATTLE_MENU; return; }
        if(b->after==AFTER_GROWTH) { growth_next(b);return; }
        if(b->after==AFTER_SWITCH || b->forced_switch) {
            sync_active(b);b->phase=BATTLE_SWITCH;b->switch_cursor=b->active;return;
        }
        if(b->result==BATTLE_WIN) {
            if(!b->reward_given) {
                int old_xp=b->ally.experience;
                b->experience_reward=b->ally.level>=100?0:species_get(b->enemy.species)->experience_yield*b->enemy.level;
                creature_gain_xp(&b->ally,b->experience_reward,&b->growth);
                b->experience_reward=b->ally.experience-old_xp;
                b->reward_given=1;
            }
            snprintf(b->message,sizeof(b->message),"VICTORY. %d XP EARNED.\n%s - LEVEL %d\n%d XP TO NEXT LEVEL",b->experience_reward,
                     creature_name(&b->ally),b->ally.level,creature_xp_remaining(&b->ally));
            if(b->ally.level==100)
                snprintf(b->message,sizeof(b->message),"VICTORY. %d XP EARNED.\n%s - LEVEL 100\nMAX LEVEL REACHED",b->experience_reward,creature_name(&b->ally));
            b->phase=BATTLE_MESSAGE;b->after=AFTER_GROWTH;
        } else if(b->result==BATTLE_LOSS) {
            message(b,"YOUR TEAM NEEDS A REST.\nRETURNING TO HEARTH CLEARING.\nTEAM RESTORED AFTER BATTLE.",AFTER_DONE);
        } else next_action(b);
        return;
    }
    if(b->phase==BATTLE_ATTACKS) {
        if(input->cancel) { b->phase=BATTLE_MENU; return; }
        if(nav) b->move_cursor=(b->move_cursor+nav+4)%4;
        if(!input->confirm) return;
        int available=0;
        for(int i=0;i<4;++i) available+=b->ally.uses[i];
        if(!available) { begin_turn(b,-1); return; }
        if(b->ally.uses[b->move_cursor]<=0) {
            message(b,"THAT ATTACK HAS NO USES LEFT.\nCHOOSE ANOTHER ATTACK.",AFTER_MENU);
            return;
        }
        begin_turn(b,b->move_cursor);
        return;
    }
    if(nav) b->cursor=(b->cursor+nav+5)%5;
    if(!input->confirm) return;
    switch(b->cursor) {
    case 0: b->phase=BATTLE_ATTACKS; break;
    case 1:
        b->phase=BATTLE_CAPTURE;break;
    case 2:
        sync_active(b);b->switch_cursor=b->active;b->phase=BATTLE_SWITCH;break;
    case 3:
        b->item_cursor=0;b->phase=BATTLE_ITEMS;break;
    default:
        ++b->escape_attempts;
        if(b->escape_attempts>=3 || random_next(b)%100<70) {
            b->result=BATTLE_ESCAPED;
            message(b,"YOU GOT AWAY SAFELY.\nTEAM RESTORED AFTER BATTLE.",AFTER_DONE);
        } else {
            enemy_response(b);
            message(b,"THE WAY OUT IS BLOCKED.\nTHE WILD VEYLING MOVES CLOSER.",AFTER_TURN);
        }
        break;
    }
}
````

## src/camera.c

````text
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
````

## src/capture.c

````text
#include <stdint.h>
#include "capture.h"

int capture_rarity(SpeciesId species)
{
    switch(species) {
    case SPECIES_MOSSLET:case SPECIES_FLINTLING:case SPECIES_REEDSKIP:return 0;
    case SPECIES_CINDLET:case SPECIES_TWIGLINT:case SPECIES_DUSKWISP:return 1;
    default:return 2;
    }
}

int capture_chance(const Creature *c,int strength)
{
    if(!c || c->species<0 || c->species>=SPECIES_COUNT ||
       c->max_hp<=0 || c->hp<=0 || c->hp>c->max_hp) return 0;
    if(strength<1) strength=1;
    if(strength>5) strength=5;
    /* A common creature starts at 35%; each rarity tier subtracts 12 points.
       Lost health adds up to 50 points, stronger looms add 10 per tier.
       Integer rounding is deterministic and 64-bit multiplication avoids overflow. */
    int wounded=(int)((int64_t)(c->max_hp-c->hp)*50/c->max_hp);
    int chance=35-12*capture_rarity(c->species)+wounded+10*(strength-1);
    return chance<5?5:chance>95?95:chance;
}

int capture_attempt(const Creature *c,int strength,unsigned int roll)
{
    return roll<100 && roll<(unsigned int)capture_chance(c,strength);
}
````

## src/creature.c

````text
#include <limits.h>
#include <string.h>
#include "creature.h"
#define L(l,m) {l,m}
static const Species species[SPECIES_COUNT] = {
 {SPECIES_CINDLET,"CINDLET","A SMALL HEARTH DWELLER.\nITS EARS GLOW LIKE WARM COALS.",ELEMENT_EMBER,
  24,10,10,8,24,8,SPECIES_EMBERLYN,0,0,
  {L(1,MOVE_NUDGE),L(2,MOVE_CINDER),L(4,MOVE_LUNGE),L(5,MOVE_GUST),L(6,MOVE_HEAT),L(9,MOVE_FLARE)},6},
 {SPECIES_EMBERLYN,"EMBERLYN","ITS COAL MANE KEEPS TRAVELERS\nWARM THROUGH LONG WINTER NIGHTS.",ELEMENT_EMBER,
  35,18,16,14,45,0,-1,1,1,
  {L(1,MOVE_NUDGE),L(2,MOVE_CINDER),L(4,MOVE_LUNGE),L(5,MOVE_GUST),L(6,MOVE_HEAT),L(8,MOVE_PEBBLE),L(9,MOVE_FLARE)},7},
 {SPECIES_MOSSLET,"MOSSLET","IT GATHERS DEW IN A SOFT MOSS COAT.\nIT SLEEPS BENEATH FALLEN LEAVES.",ELEMENT_GROVE,
  25,8,12,6,22,8,SPECIES_MOSSHORN,2,2,
  {L(1,MOVE_NUDGE),L(2,MOVE_LEAF),L(4,MOVE_LUNGE),L(6,MOVE_VINE)},4},
 {SPECIES_MOSSHORN,"MOSSHORN","BRANCHING HORNS SHELTER SEEDLINGS.\nIT WALKS SOFTLY THROUGH THE WOODS.",ELEMENT_GROVE,
  39,15,22,8,44,0,-1,3,3,
  {L(1,MOVE_NUDGE),L(2,MOVE_LEAF),L(4,MOVE_LUNGE),L(6,MOVE_VINE),L(9,MOVE_PEBBLE)},5},
 {SPECIES_TWIGLINT,"TWIGLINT","THIS NIMBLE BRANCH DWELLER HIDES\nIN PATCHES OF SHIFTING SUNLIGHT.",ELEMENT_GROVE,
  20,12,8,14,27,0,-1,4,4,
  {L(1,MOVE_NUDGE),L(2,MOVE_LEAF),L(4,MOVE_GUST),L(7,MOVE_VINE)},4},
 {SPECIES_GLOWMOTH,"GLOWMOTH","ITS DUSTY WINGS SHIMMER WHEN\nNIGHT BREEZES CROSS THE CANOPY.",ELEMENT_WIND,
  22,11,9,16,34,0,-1,5,5,
  {L(1,MOVE_NUDGE),L(2,MOVE_GUST),L(4,MOVE_LUNGE),L(7,MOVE_TEMPEST)},4},
 {SPECIES_FLINTLING,"FLINTLING","IT TAPS STONES TO LEAVE SPARKING\nTRAILS THROUGH THE DARK.",ELEMENT_STONE,
  27,12,15,5,28,9,SPECIES_FLINTAUR,6,6,
  {L(1,MOVE_NUDGE),L(2,MOVE_PEBBLE),L(4,MOVE_LUNGE),L(7,MOVE_FAULT)},4},
 {SPECIES_FLINTAUR,"FLINTAUR","A HEAVY CAVE WANDERER WITH\nRIDGES OF POLISHED FLINT.",ELEMENT_STONE,
  43,22,25,7,48,0,-1,7,7,
  {L(1,MOVE_NUDGE),L(2,MOVE_PEBBLE),L(4,MOVE_LUNGE),L(7,MOVE_FAULT),L(9,MOVE_CINDER)},5},
 {SPECIES_DUSKWISP,"DUSKWISP","A PALE DRIFTER THAT FOLLOWS\nTHE QUIET AIR INSIDE CAVERNS.",ELEMENT_WIND,
  20,14,8,17,31,0,-1,8,8,
  {L(1,MOVE_NUDGE),L(2,MOVE_GUST),L(4,MOVE_CINDER),L(7,MOVE_TEMPEST)},4},
 {SPECIES_ECHOCRAG,"ECHOCRAG","ITS HOLLOW CREST CARRIES SOUND\nACROSS UNDERGROUND CHAMBERS.",ELEMENT_STONE,
  33,14,18,4,38,0,-1,9,9,
  {L(1,MOVE_NUDGE),L(2,MOVE_PEBBLE),L(4,MOVE_LUNGE),L(6,MOVE_FAULT)},4},
 {SPECIES_REEDSKIP,"REEDSKIP","ITS REED ANTENNAE FEEL THE BREEZE.\nIT HOPS BETWEEN SUNLIT RIVERBANKS.",ELEMENT_GROVE,
  26,12,10,15,33,0,-1,10,10,
  {L(1,MOVE_NUDGE),L(2,MOVE_LEAF),L(4,MOVE_GUST),L(6,MOVE_VINE),L(9,MOVE_LUNGE)},5},
 {SPECIES_SUNFINCH,"SUNFINCH","GOLDEN FEATHERS CATCH THE DAWN.\nITS SONG GUIDES LOST WAYFARERS.",ELEMENT_WIND,
  25,15,10,18,40,0,-1,11,11,
  {L(1,MOVE_NUDGE),L(2,MOVE_GUST),L(4,MOVE_CINDER),L(6,MOVE_LUNGE),L(8,MOVE_TEMPEST)},5}
};
const Species *species_get(int id)
{
    return &species[id>=0 && id<SPECIES_COUNT?id:SPECIES_CINDLET];
}
const char *creature_name(const Creature *c) { return c->nickname[0]?c->nickname:c->name; }
int creature_xp_for_level(int level)
{
    if(level<1) level=1;
    if(level>CREATURE_MAX_LEVEL) level=CREATURE_MAX_LEVEL;
    return 20*(level-1)*(level-1);
}
int creature_xp_remaining(const Creature *c)
{
    return c->level>=CREATURE_MAX_LEVEL?0:creature_xp_for_level(c->level+1)-c->experience;
}
static void recalculate(Creature *c)
{
    const Species *s=species_get(c->species);
    int old_max=c->max_hp,old_hp=c->hp;
    c->name=s->name; c->element=s->element;
    c->max_hp=s->base_hp+c->level*5;
    c->attack=s->base_attack+c->level*3;
    c->defense=s->base_defense+c->level*2;
    c->speed=s->base_speed+c->level*2;
    c->hp=old_hp>0?old_hp+c->max_hp-old_max:0;
    if(c->hp>c->max_hp) c->hp=c->max_hp;
}
void creature_restore(Creature *c)
{
    c->hp=c->max_hp;
    for(int i=0;i<4;++i) c->uses[i]=c->moves[i]<0?0:attack_get(c->moves[i])->uses;
}
static int knows(const Creature *c,int move)
{
    for(int i=0;i<4;++i) if(c->moves[i]==move) return 1;
    return 0;
}
int creature_learn(Creature *c,int move,int slot)
{
    if(move<0 || move>=MOVE_COUNT || slot<0 || slot>=4 || knows(c,move)) return 0;
    c->moves[slot]=move; c->uses[slot]=attack_get(move)->uses;
    return 1;
}
void creature_create(Creature *c,int id,int level)
{
    *c=(Creature){0};
    c->species=species_get(id)->id;
    c->level=level<1?1:level>100?100:level;
    c->experience=creature_xp_for_level(c->level);
    for(int i=0;i<4;++i) c->moves[i]=-1;
    recalculate(c);
    const Species *s=species_get(c->species);
    int slot=0;
    for(int i=0;i<s->learn_count;++i)
        if(s->learnset[i].level<=c->level && !knows(c,s->learnset[i].move)) {
            /* Fresh wild creatures use their latest four unlocked attacks. */
            if(slot==4) {
                for(int j=0;j<3;++j) c->moves[j]=c->moves[j+1];
                slot=3;
            }
            c->moves[slot++]=s->learnset[i].move;
        }
    creature_restore(c);
}
static void queue_moves(const Creature *c,const Species *s,int exact,CreatureGrowth *g)
{
    for(int i=0;i<s->learn_count;++i) {
        LearnMove learned=s->learnset[i];
        if((exact?learned.level!=c->level:learned.level>c->level) || knows(c,learned.move)) continue;
        int duplicate=0;
        for(int j=0;j<g->move_count;++j) if(g->moves[j]==learned.move) duplicate=1;
        if(!duplicate && g->move_count<MOVE_COUNT) g->moves[g->move_count++]=learned.move;
    }
}
void creature_gain_xp(Creature *c,int amount,CreatureGrowth *g)
{
    *g=(CreatureGrowth){0};
    g->old_level=c->level;g->old_species=c->species;
    if(amount<=0 || c->level>=100) return;
    int cap=creature_xp_for_level(100);
    c->experience=amount>=cap-c->experience?cap:c->experience+amount;
    while(c->level<100 && c->experience>=creature_xp_for_level(c->level+1)) {
        ++c->level;
        const Species *s=species_get(c->species);
        queue_moves(c,s,1,g);
        if(s->evolution_level>0 && c->level>=s->evolution_level) {
            c->species=(SpeciesId)s->evolved_species;
            queue_moves(c,species_get(c->species),0,g);
        }
        recalculate(c);
    }
}
````

## src/dialogue.c

````text
#include <stdio.h>
#include "dialogue.h"
void dialogue_open(Dialogue *d, const char *title, const char *first, const char *second)
{
    *d = (Dialogue){0};
    d->active = 1;
    d->count = second ? 2 : 1;
    snprintf(d->title,sizeof(d->title),"%s",title);
    snprintf(d->pages[0],sizeof(d->pages[0]),"%s",first);
    if (second) snprintf(d->pages[1],sizeof(d->pages[1]),"%s",second);
}
void dialogue_advance(Dialogue *d)
{
    if (!d->active) return;
    if (++d->page >= d->count) d->active = 0;
}
````

## src/encounter.c

````text
#include "encounter.h"
typedef struct { SpeciesId species; int weight, minimum, maximum; } Entry;
static const Entry forest[] = {{SPECIES_MOSSLET,60,2,4},{SPECIES_TWIGLINT,30,3,5},{SPECIES_GLOWMOTH,10,4,6}};
static const Entry cave[] = {{SPECIES_FLINTLING,60,3,5},{SPECIES_DUSKWISP,30,4,6},{SPECIES_ECHOCRAG,10,5,7}};
static const Entry marsh[] = {{SPECIES_REEDSKIP,60,5,7},{SPECIES_GLOWMOTH,30,5,7},{SPECIES_SUNFINCH,10,6,8}};
static uint32_t next(Encounter *e)
{
    uint32_t x = e->random;
    x ^= x << 13; x ^= x >> 17; x ^= x << 5;
    return e->random = x;
}
void encounter_init(Encounter *e, uint32_t seed)
{
    e->random = seed ? seed : 0x712a9u;
    e->safe_steps = 4;
}
int encounter_step(Encounter *e, int area, EncounterResult *result)
{
    if (e->safe_steps > 0) { --e->safe_steps; return 0; }
    if (area < 1 || area > 3 || next(e)%100 >= 18) return 0;
    const Entry *table = area == 1 ? forest : area == 2 ? cave : marsh;
    int roll = (int)(next(e)%100);
    int index = 0;
    while (index < 2 && roll >= table[index].weight) {
        roll -= table[index].weight;
        ++index;
    }
    result->species = table[index].species;
    result->name = species_get(result->species)->name;
    result->level = table[index].minimum +
        (int)(next(e)%(table[index].maximum-table[index].minimum+1));
    e->safe_steps = 4;
    return 1;
}
````

## src/game.c

````text
#include <stdio.h>
#include <string.h>
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
    enter_map(g,saved->map_id,saved->tile_x,saved->tile_y);
    g->player.facing=(Direction)(saved->facing>=FACE_DOWN && saved->facing<=FACE_UP ? saved->facing : FACE_DOWN);
    g->party=party;g->inventory=inventory;
    g->encounter.random=saved->encounter_random?saved->encounter_random:0x712a9u;
    g->encounter.safe_steps=saved->encounter_safe_steps;
    g->dialogue=(Dialogue){0};g->roster_open=0;g->menu_open=0;g->shop_open=0;g->tavi_shop_pending=0;
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
static void game_step(Game *g, const Input *input, float seconds)
{
    save_data_update();
    save_status_update(g);
    if (save_data_status()==SAVE_STATUS_BUSY) return;
    if(g->in_battle) {
        battle_update(&g->battle,input);
        if(g->battle.phase==BATTLE_DONE) {
            g->in_battle=0;
            g->party=g->battle.party; /* Includes captures and every switched creature. */
            g->inventory=g->battle.inventory;
            /* The prototype still restores the complete team after battle so
               encounter and progression testing stays repeatable. The lodge
               dais gives the player an explicit refill while exploring. */
            party_restore(&g->party);
            if(g->battle.result==BATTLE_LOSS) enter_map(g,MAP_CLEARING,5,11);
            g->encounter.safe_steps=4;
            g->transition=0.22f;
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
        if (input->cancel) { g->dialogue.active = 0;g->tavi_shop_pending=0; }
        else if (input->confirm) {
            int last_page=g->dialogue.page+1>=g->dialogue.count;
            dialogue_advance(&g->dialogue);
            if (last_page && g->tavi_shop_pending) {
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
            if (!strcmp(npc->name,"TAVI")) {
                g->tavi_shop_pending=1;
                dialogue_open(&g->dialogue,npc->name,npc->first,npc->second);
            } else dialogue_open(&g->dialogue,npc->name,npc->first,npc->second);
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
    int modal=g->menu_open || g->roster_open || g->shop_open || g->dialogue.active || g->in_battle;
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
````

## src/graphics.c

````text
#include <stddef.h>
#include <pspdisplay.h>
#include <pspgu.h>
#include "graphics.h"

#define BUFFER_WIDTH 512
#define FRAME_BYTES (BUFFER_WIDTH * SCREEN_HEIGHT * 4)

/* Aligned command buffer, reused only after the previous list completes. */
/* Reserve 1 MiB for visible terrain, actors, and the bitmap dialogue font.
   Host drawing tests enforce a conservative 6000-rectangle frame budget. */
static unsigned int command_list[262144] __attribute__((aligned(16)));

typedef struct {
    unsigned int color;
    float x, y, z;
} Vertex;

void graphics_init(void)
{
    sceGuInit();
    sceGuStart(GU_DIRECT, command_list);
    /* GU buffer pointers are offsets into VRAM, not ordinary RAM pointers.
       Two 512 x 272 RGBA buffers consume 1,114,112 bytes of 2 MiB VRAM. */
    sceGuDrawBuffer(GU_PSM_8888, (void *)0, BUFFER_WIDTH);
    sceGuDispBuffer(SCREEN_WIDTH, SCREEN_HEIGHT,
                    (void *)FRAME_BYTES, BUFFER_WIDTH);
    sceGuOffset(2048 - SCREEN_WIDTH / 2, 2048 - SCREEN_HEIGHT / 2);
    sceGuViewport(2048, 2048, SCREEN_WIDTH, SCREEN_HEIGHT);
    sceGuScissor(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    sceGuEnable(GU_SCISSOR_TEST);
    sceGuDisable(GU_DEPTH_TEST);
    sceGuDisable(GU_TEXTURE_2D);
    sceGuDisable(GU_CULL_FACE);
    sceGuDisable(GU_BLEND);
    sceGuShadeModel(GU_SMOOTH);
    sceGuFinish();
    sceGuSync(0, 0);
    sceDisplayWaitVblankStart();
    sceGuDisplay(GU_TRUE);
}

void graphics_begin(void)
{
    sceGuStart(GU_DIRECT, command_list);
    sceGuClearColor(GU_RGBA(42, 65, 61, 255));
    sceGuClear(GU_COLOR_BUFFER_BIT);
}

void graphics_rectangle(int x, int y, int width, int height,
                        unsigned int color)
{
    /* Allocate vertices inside the GU list; no per-frame heap allocation. */
    Vertex *vertices = sceGuGetMemory(2 * sizeof(Vertex));
    vertices[0] = (Vertex){color, (float)x, (float)y, 0.0f};
    vertices[1] = (Vertex){color, (float)(x + width),
                         (float)(y + height), 0.0f};
    sceGuDrawArray(GU_SPRITES,
                  GU_COLOR_8888 | GU_VERTEX_32BITF | GU_TRANSFORM_2D,
                  2, NULL, vertices);
}

void graphics_end(void)
{
    sceGuFinish();
    sceGuSync(0, 0);
    sceDisplayWaitVblankStart();
    sceGuSwapBuffers();
}

void graphics_shutdown(void)
{
    sceGuDisplay(GU_FALSE);
    sceGuTerm();
}
````

## src/input.c

````text
#include <pspctrl.h>
#include "input.h"

int input_init(void)
{
    int result = sceCtrlSetSamplingCycle(0);
    if (result < 0) {
        return result;
    }
    return sceCtrlSetSamplingMode(PSP_CTRL_MODE_DIGITAL);
}

void input_poll(Input *input)
{
    static unsigned int previous;
    SceCtrlData pad = {0};
    input->horizontal = 0;
    input->vertical = 0;
    input->confirm = input->cancel = input->details = input->menu = 0;
    if (sceCtrlPeekBufferPositive(&pad, 1) <= 0) {
        return;
    }
    input->horizontal = ((pad.Buttons & PSP_CTRL_RIGHT) != 0)
                      - ((pad.Buttons & PSP_CTRL_LEFT) != 0);
    input->vertical = ((pad.Buttons & PSP_CTRL_DOWN) != 0)
                    - ((pad.Buttons & PSP_CTRL_UP) != 0);
    unsigned int pressed = pad.Buttons & ~previous;
    previous = pad.Buttons;
    input->confirm = (pressed & PSP_CTRL_CROSS) != 0;
    input->cancel = (pressed & PSP_CTRL_CIRCLE) != 0;
    input->details = (pressed & PSP_CTRL_SELECT) != 0;
    if (pressed & PSP_CTRL_TRIANGLE) input->menu |= INPUT_MENU_OPEN;
    if (pressed & PSP_CTRL_START) input->menu |= INPUT_MENU_SAVE;
    if (pressed & PSP_CTRL_LTRIGGER) input->menu |= INPUT_MENU_LOAD;
}
````

## src/inventory.c

````text
#include "inventory.h"

void inventory_init(Inventory *inventory)
{
    if (!inventory) return;
    *inventory = (Inventory){{3, 1}, 250};
}

const char *inventory_item_name(ItemId item)
{
    switch (item) {
    case ITEM_PULSE_TONIC: return "PULSE TONIC";
    case ITEM_HEARTH_RESTORE: return "HEARTH RESTORE";
    default: return "UNKNOWN ITEM";
    }
}

const char *inventory_item_description(ItemId item)
{
    switch (item) {
    case ITEM_PULSE_TONIC: return "RESTORES 25 HP.";
    case ITEM_HEARTH_RESTORE: return "RESTORES THE FULL HP OF A VEYLING.";
    default: return "NO EFFECT.";
    }
}

int inventory_item_max(ItemId item)
{
    switch (item) {
    case ITEM_PULSE_TONIC: return 99;
    case ITEM_HEARTH_RESTORE: return 20;
    default: return 0;
    }
}

int inventory_item_price(ItemId item)
{
    switch (item) {
    case ITEM_PULSE_TONIC: return 50;
    case ITEM_HEARTH_RESTORE: return 120;
    default: return 0;
    }
}

int inventory_buy(Inventory *inventory, ItemId item)
{
    int price=inventory_item_price(item), maximum=inventory_item_max(item);
    if (!inventory || item < 0 || item >= ITEM_COUNT || !price ||
        inventory->quantities[item] >= maximum || inventory->embermarks < price)
        return 0;
    inventory->embermarks-=price;
    ++inventory->quantities[item];
    return 1;
}

int inventory_use_healing(Inventory *inventory, ItemId item, Creature *target)
{
    int before;
    if (!inventory || !target || item < 0 || item >= ITEM_COUNT ||
        inventory->quantities[item] <= 0 || target->hp <= 0 || target->hp >= target->max_hp)
        return 0;
    before = target->hp;
    if (item == ITEM_PULSE_TONIC) target->hp += 25;
    else if (item == ITEM_HEARTH_RESTORE) target->hp = target->max_hp;
    else return 0;
    if (target->hp > target->max_hp) target->hp = target->max_hp;
    if (target->hp == before) return 0;
    --inventory->quantities[item];
    return target->hp - before;
}
````

## src/main.c

````text
#include <pspkernel.h>
#include "game.h"
#include "graphics.h"
#include "input.h"
#include "audio.h"

/* User-mode PRX: no kernel access or extra PSP-3000 RAM required. */
PSP_MODULE_INFO("Emberwake", 0, 0, 1);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER);
/* Savedata dialogs allocate their own UI buffers; keep enough user heap for
   the dialog without requiring extra hardware memory. */
PSP_HEAP_SIZE_KB(4096);

static volatile int running = 1;

static int exit_callback(int arg1, int arg2, void *common)
{
    (void)arg1;
    (void)arg2;
    (void)common;
    running = 0;
    return 0;
}

static SceUID setup_callbacks(void)
{
    /* Callbacks belong to the creating thread. Main services this callback
       once per frame, so a separate callback thread is unnecessary. */
    SceUID callback = sceKernelCreateCallback("home_exit", exit_callback, NULL);
    if (callback < 0) {
        return callback;
    }
    if (sceKernelRegisterExitCallback(callback) < 0) {
        sceKernelDeleteCallback(callback);
        return -1;
    }
    return callback;
}

int main(void)
{
    Game game;
    Input input;
    SceUID callback = setup_callbacks();
    if (callback < 0 || input_init() < 0) {
        sceKernelExitGame();
        return 1;
    }

    graphics_init();
    audio_init(); /* A missing audio channel leaves the game playable silently. */
    game_init(&game);
    encounter_init(&game.encounter,sceKernelGetSystemTimeLow());
    unsigned int previous = sceKernelGetSystemTimeLow();

    while (running) {
        sceKernelCheckCallback();
        if (!running) {
            break;
        }
        unsigned int now = sceKernelGetSystemTimeLow();
        /* Unsigned subtraction handles the microsecond counter wrapping. */
        float seconds = (float)(now - previous) / 1000000.0f;
        previous = now;
        if (seconds > 0.05f) {
            seconds = 0.05f;
        }
        input_poll(&input);
        game_update(&game, &input, seconds);
        graphics_begin();
        game_draw(&game);
        graphics_end();
    }

    audio_shutdown();
    graphics_shutdown();
    sceKernelDeleteCallback(callback);
    sceKernelExitGame();
    return 0;
}
````

## src/map.c

````text
#include "map.h"
static const char *const clearing_rows[] = {
#include "../assets/maps/clearing.inc"
};
static const Map clearing = {40, 24, clearing_rows, 5, 11};
static const char *const forest_rows[] = {
#include "../assets/maps/forest.inc"
};
static const char *const lodge_rows[] = {
#include "../assets/maps/lodge.inc"
};
static const char *const cave_rows[] = {
#include "../assets/maps/cave.inc"
};
static const Map forest = {32,20,forest_rows,2,11};
static const Map lodge = {15,9,lodge_rows,7,6};
static const Map cave = {20,14,cave_rows,2,10};
static const char *const marsh_rows[] = {
#include "../assets/maps/marsh.inc"
};
static const char *const rest_rows[] = {
#include "../assets/maps/rest.inc"
};
static const Map marsh = {28,18,marsh_rows,2,10};
static const Map rest = {15,9,rest_rows,7,6};
static const Portal portals[] = {
    {MAP_CLEARING,38,11,MAP_FOREST,2,11},
    {MAP_FOREST,1,11,MAP_CLEARING,37,11},
    {MAP_CLEARING,5,9,MAP_LODGE,7,6},
    {MAP_LODGE,7,7,MAP_CLEARING,5,10},
    {MAP_FOREST,28,5,MAP_CAVE,2,10},
    {MAP_CAVE,2,11,MAP_FOREST,28,6},
    {MAP_FOREST,30,11,MAP_MARSH,2,10},
    {MAP_MARSH,1,10,MAP_FOREST,29,11},
    {MAP_MARSH,24,4,MAP_REST,7,6},
    {MAP_REST,7,7,MAP_MARSH,24,5}
};
const Map *map_get(int id)
{
    const Map *const maps[] = {&clearing,&forest,&lodge,&cave,&marsh,&rest};
    return id >= 0 && id < MAP_COUNT ? maps[id] : &clearing;
}
const char *map_name(int id)
{
    const char *const names[] = {"HEARTH CLEARING","FERNVEIL WOODS","WAYFARER LODGE","HOLLOWSTONE CAVE","SUNTHREAD MARSH","LANTERN REST"};
    return id >= 0 && id < MAP_COUNT ? names[id] : names[0];
}
const Portal *map_portal(int id, int x, int y)
{
    for (unsigned int i = 0; i < sizeof(portals)/sizeof(portals[0]); ++i)
        if (portals[i].from == id && portals[i].x == x && portals[i].y == y)
            return &portals[i];
    return 0;
}
int map_encounter_area(int id, int x, int y)
{
    char tile = map_tile(map_get(id),x,y);
    return tile == 'g' && id == MAP_FOREST ? 1 : tile == 'c' && id == MAP_CAVE ? 2 :
           tile == 'r' && id == MAP_MARSH ? 3 : 0;
}
const Map *map_test(void) { return &clearing; }
char map_tile(const Map *map, int x, int y)
{
    if (x < 0 || y < 0 || x >= map->width || y >= map->height) return '#';
    return map->rows[y][x];
}
int map_walkable(const Map *map, int x, int y)
{
    char tile = map_tile(map, x, y);
    return tile == '.' || tile == '=' || tile == ',' || tile == 'D' ||
           tile == '>' || tile == '<' || tile == 'g' || tile == 'r' || tile == 'c' || tile == '_' || tile == 'H';
}
````

## src/npc.c

````text
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
        add(n,4,8,"ELA","REEDSKIP HOPS AMONG THESE REEDS.\nSUNFINCH IS A RARER SIGHT.","THE BOARDWALK IS SAFE TO FOLLOW.\nLANTERN REST LIES NORTHEAST.",4);
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
````

## src/party_menu.c

````text
#include <stdio.h>
#include "party_menu.h"
#include "graphics.h"
#include "text.h"

#define C(r,g,b) (0xff000000u | ((unsigned int)(b)<<16) | ((unsigned int)(g)<<8) | (unsigned int)(r))
#define LIST_ROWS 6

static void feedback(PartyMenu *menu, const char *message)
{
    snprintf(menu->message,sizeof(menu->message),"%s",message);
}

static void normalize(PartyMenu *menu, const Party *party)
{
    const int counts[2]={party->count,party->stored};
    for(int tab=0;tab<2;++tab) {
        if(menu->cursor[tab]>=counts[tab]) menu->cursor[tab]=counts[tab]-1;
        if(menu->cursor[tab]<0) menu->cursor[tab]=0;
        if(menu->scroll[tab]>menu->cursor[tab]) menu->scroll[tab]=menu->cursor[tab];
        if(menu->cursor[tab]>=menu->scroll[tab]+LIST_ROWS)
            menu->scroll[tab]=menu->cursor[tab]-LIST_ROWS+1;
        int last=counts[tab]>LIST_ROWS?counts[tab]-LIST_ROWS:0;
        if(menu->scroll[tab]>last) menu->scroll[tab]=last;
    }
}

void party_menu_open(PartyMenu *menu)
{
    *menu=(PartyMenu){0};
    feedback(menu,"TRAVEL WITH UP TO FOUR VEYLINGS.\nMOVE OTHERS TO YOUR COLLECTION.");
}

int party_menu_update(PartyMenu *menu, Party *party, const Input *input)
{
    int horizontal=input->horizontal!=menu->previous_horizontal?input->horizontal:0;
    int vertical=input->vertical!=menu->previous_vertical?input->vertical:0;
    menu->previous_horizontal=input->horizontal;
    menu->previous_vertical=input->vertical;
    normalize(menu,party);
    if(input->menu & INPUT_MENU_OPEN) return 0;
    if(input->cancel) {
        if(menu->mode==PARTY_MENU_BROWSE) return 0;
        menu->mode=PARTY_MENU_BROWSE;
        feedback(menu,"NO CHANGE MADE.\nX CHOOSE A VEYLING.");
        return 1;
    }
    if(menu->mode==PARTY_MENU_ACTION) {
        if(vertical) menu->action=(menu->action+vertical+3)%3;
        if(!input->confirm) return 1;
        if(menu->action==0) {
            if(party_set_lead(party,menu->cursor[0]))
                feedback(menu,"LEAD PARTNER UPDATED.\nTHIS VEYLING STARTS THE NEXT BATTLE.");
            else feedback(menu,"THAT VEYLING CANNOT LEAD.\nCHOOSE A PARTNER WITH HP.");
        } else if(menu->action==1) {
            if(party->count<=1) {
                feedback(menu,"KEEP AT LEAST ONE VEYLING IN YOUR PARTY.\nCATCH ANOTHER BEFORE DEPOSITING THIS ONE.");
                return 1;
            }
            if(party->stored>=COLLECTION_MAX) {
                feedback(menu,"YOUR COLLECTION IS FULL.\nWITHDRAW A VEYLING TO MAKE ROOM.");
                return 1;
            }
            if(party_deposit(party,menu->cursor[0]))
                feedback(menu,"VEYLING MOVED TO YOUR COLLECTION.\nUSE LEFT OR RIGHT TO VIEW IT.");
            else feedback(menu,"THIS VEYLING MUST STAY IN YOUR PARTY.\nCHOOSE ANOTHER PARTNER.");
        } else feedback(menu,"NO CHANGE MADE.\nX CHOOSE A VEYLING.");
        menu->mode=PARTY_MENU_BROWSE;
        normalize(menu,party);
        return 1;
    }
    if(menu->mode==PARTY_MENU_SWAP) {
        if(vertical && party->count>0) menu->swap=(menu->swap+vertical+party->count)%party->count;
        if(input->confirm) {
            if(party_exchange(party,menu->cursor[1],menu->swap))
                feedback(menu,"VEYLINGS EXCHANGED.\nYOUR NEW PARTNER IS READY TO TRAVEL.");
            else feedback(menu,"EXCHANGE COULD NOT BE COMPLETED.\nCHOOSE ANOTHER PARTNER.");
            menu->mode=PARTY_MENU_BROWSE;
            normalize(menu,party);
        }
        return 1;
    }
    if(horizontal) {
        menu->tab=1-menu->tab;
        feedback(menu,menu->tab?"X WITHDRAW A VEYLING.\nIF THE PARTY IS FULL, CHOOSE ONE TO SWAP.":
                 "X CHOOSE A VEYLING.\nSET YOUR LEAD OR MOVE IT TO COLLECTION.");
    }
    int count=menu->tab?party->stored:party->count;
    if(vertical && count>0) {
        menu->cursor[menu->tab]=(menu->cursor[menu->tab]+vertical+count)%count;
        normalize(menu,party);
    }
    if(!input->confirm) return 1;
    if(count==0) {
        feedback(menu,"YOUR COLLECTION IS EMPTY.\nCATCH A VEYLING OR DEPOSIT A PARTY MEMBER.");
    } else if(menu->tab==0) {
        menu->mode=PARTY_MENU_ACTION;
        menu->action=0;
        feedback(menu,"SET LEAD TO CHOOSE YOUR FIRST BATTLER.\nDEPOSIT MOVES THIS VEYLING TO COLLECTION.");
    } else if(party->count<PARTY_MAX) {
        if(party_exchange(party,menu->cursor[1],party->count))
            feedback(menu,"VEYLING ADDED TO YOUR PARTY.\nSWITCH TO PARTY TO SET YOUR LEAD.");
        else feedback(menu,"WITHDRAWAL COULD NOT BE COMPLETED.\nCHOOSE ANOTHER VEYLING.");
        normalize(menu,party);
    } else {
        menu->mode=PARTY_MENU_SWAP;
        menu->swap=0;
        feedback(menu,"YOUR PARTY IS FULL. CHOOSE WHO TO SWAP.\nTHE OTHER VEYLING MOVES TO COLLECTION.");
    }
    return 1;
}

static void details_draw(const Creature *creature)
{
    char line[80];
    snprintf(line,sizeof(line),"%.19s  LV %d",creature_name(creature),creature->level);
    text_draw(244,72,line,C(244,217,169),1);
    text_draw(244,87,element_name(creature->element),C(166,196,188),1);
    snprintf(line,sizeof(line),"HP %d / %d",creature->hp,creature->max_hp);
    text_draw(244,100,line,C(229,233,220),1);
    graphics_rectangle(244,112,210,5,C(57,70,77));
    if(creature->max_hp>0 && creature->hp>0) {
        int filled=210*creature->hp/creature->max_hp;
        if(filled>210) filled=210;
        if(filled>0) graphics_rectangle(244,112,filled,5,C(125,201,154));
    }
    snprintf(line,sizeof(line),"ATK %d  DEF %d  SPD %d",creature->attack,creature->defense,creature->speed);
    text_draw(244,125,line,C(207,216,209),1);
    if(creature->level>=CREATURE_MAX_LEVEL) snprintf(line,sizeof(line),"XP %d - MAX LEVEL",creature->experience);
    else snprintf(line,sizeof(line),"XP %d - NEXT %d",creature->experience,creature_xp_remaining(creature));
    text_draw(244,139,line,C(233,173,115),1);
    for(int i=0;i<CREATURE_MOVES;++i) {
        if(creature->moves[i]<0) snprintf(line,sizeof(line),"- EMPTY -");
        else {
            const Attack *move=attack_get(creature->moves[i]);
            snprintf(line,sizeof(line),"%.22s %d/%d",move->name,creature->uses[i],move->uses);
        }
        text_draw(244,154+i*13,line,C(188,205,197),1);
    }
}

static void actions_draw(const PartyMenu *menu, const Party *party)
{
    graphics_rectangle(238,64,230,145,C(174,143,94));
    graphics_rectangle(240,66,226,141,C(25,35,45));
    if(menu->mode==PARTY_MENU_ACTION) {
        const char *const options[]={"SET AS LEAD","MOVE TO COLLECTION","CANCEL"};
        text_draw(250,76,"PARTY ACTION",C(246,206,146),1);
        for(int i=0;i<3;++i) {
            if(i==menu->action) graphics_rectangle(246,97+i*30,214,24,C(67,85,80));
            text_draw(254,105+i*30,options[i],C(238,229,207),1);
        }
    } else {
        text_draw(250,76,"SWAP WITH PARTY MEMBER",C(246,206,146),1);
        for(int i=0;i<party->count;++i) {
            char line[64];
            if(i==menu->swap) graphics_rectangle(246,93+i*27,214,24,C(67,85,80));
            snprintf(line,sizeof(line),"%.19s LV %d%s",creature_name(&party->members[i]),party->members[i].level,i==party->lead?" LEAD":"");
            text_draw(251,102+i*27,line,C(238,229,207),1);
        }
    }
}

void party_menu_draw(const PartyMenu *menu, const Party *party)
{
    char line[80];
    int count=menu->tab?party->stored:party->count;
    graphics_rectangle(0,0,480,272,C(17,26,33));
    text_draw(14,12,"YOUR VEYLINGS",C(241,221,184),2);
    text_draw(302,18,"PARTNERS FOR THE JOURNEY",C(165,188,181),1);
    for(int tab=0;tab<2;++tab) {
        int x=12+tab*230;
        graphics_rectangle(x,39,224,20,tab==menu->tab?C(76,86,78):C(29,41,49));
        snprintf(line,sizeof(line),tab==0?"PARTY  %d / %d":"COLLECTION  %d / %d",
                 tab==0?party->count:party->stored,tab==0?PARTY_MAX:COLLECTION_MAX);
        text_draw(x+10,46,line,tab==menu->tab?C(255,214,146):C(177,194,188),1);
    }
    graphics_rectangle(12,64,216,145,C(25,35,45));
    graphics_rectangle(238,64,230,145,C(25,35,45));
    if(count==0) {
        text_draw(22,80,"NO VEYLINGS STORED",C(234,219,191),1);
        text_draw(22,100,"CATCH NEW PARTNERS OR\nMOVE ONE HERE FROM\nYOUR PARTY.",C(171,194,183),1);
        text_draw(249,85,"ROOM FOR FUTURE PARTNERS.",C(238,214,176),1);
        text_draw(249,108,"EXTRA CAPTURES GO HERE\nWHEN YOUR PARTY IS FULL.\n\nYOU CAN WITHDRAW OR SWAP\nTHEM INTO YOUR PARTY.",C(171,194,183),1);
    } else {
        for(int row=0;row<LIST_ROWS;++row) {
            int index=menu->scroll[menu->tab]+row;
            if(index>=count) break;
            int y=67+row*23;
            const Creature *creature=menu->tab?&party->collection[index]:&party->members[index];
            if(index==menu->cursor[menu->tab]) graphics_rectangle(16,y,208,22,C(59,78,76));
            snprintf(line,sizeof(line),"%02d %.19s",index+1,creature_name(creature));
            text_draw(22,y+3,line,C(235,230,209),1);
            if(menu->tab==0 && index==party->lead) text_draw(193,y+3,"LEAD",C(255,201,132),1);
            snprintf(line,sizeof(line),"LV %d  HP %d/%d",creature->level,creature->hp,creature->max_hp);
            text_draw(40,y+13,line,C(168,199,183),1);
        }
        const Creature *selected=menu->tab?&party->collection[menu->cursor[1]]:&party->members[menu->cursor[0]];
        details_draw(selected);
        if(menu->mode!=PARTY_MENU_BROWSE) actions_draw(menu,party);
    }
    graphics_rectangle(12,214,456,32,C(32,44,49));
    text_draw(21,220,menu->message,C(220,213,190),1);
    text_draw(12,255,menu->mode==PARTY_MENU_BROWSE?
              "L/R TAB   UP/DOWN SELECT   X ACTION   O BACK   TRIANGLE CLOSE":
              "UP/DOWN SELECT   X CONFIRM   O CANCEL   TRIANGLE CLOSE",C(161,189,181),1);
}
````

## src/party.c

````text
#include <string.h>
#include "party.h"

static int valid(const Party *p)
{
    return p && p->count>=1 && p->count<=PARTY_MAX &&
        p->stored>=0 && p->stored<=COLLECTION_MAX;
}

/* Retain the selected slot when usable. An all-fainted party still has a valid lead. */
static void repair_lead(Party *p)
{
    if(p->lead<0 || p->lead>=p->count) p->lead=0;
    if(p->members[p->lead].hp>0) return;
    for(int i=0;i<p->count;++i) {
        if(p->members[i].hp>0) { p->lead=i;return; }
    }
}

void party_init(Party *p)
{
    if(!p) return;
    memset(p,0,sizeof(*p));
    creature_create(&p->members[0],SPECIES_CINDLET,5);
    p->count=1;
}

int party_has_space(const Party *p)
{
    return valid(p) && (p->count<PARTY_MAX || p->stored<COLLECTION_MAX);
}

int party_add(Party *p,const Creature *c)
{
    if(!c || !party_has_space(p)) return 0;
    if(p->count<PARTY_MAX) {
        p->members[p->count]=*c;
        ++p->count;
        repair_lead(p);
        return 1;
    }
    p->collection[p->stored]=*c;
    ++p->stored;
    return 2;
}

void party_restore(Party *p)
{
    if(!valid(p)) return;
    for(int i=0;i<p->count;++i) creature_restore(&p->members[i]);
    for(int i=0;i<p->stored;++i) creature_restore(&p->collection[i]);
    repair_lead(p);
}

int party_set_lead(Party *p,int index)
{
    if(!valid(p) || index<0 || index>=p->count || p->members[index].hp<=0) return 0;
    p->lead=index;
    return 1;
}

int party_exchange(Party *p,int stored_index,int party_index)
{
    if(!valid(p) || stored_index<0 || stored_index>=p->stored ||
       party_index<0 || party_index>p->count || party_index>=PARTY_MAX) return 0;
    Creature withdrawn=p->collection[stored_index];
    if(party_index<p->count) {
        p->collection[stored_index]=p->members[party_index];
        p->members[party_index]=withdrawn;
    } else {
        p->members[p->count++]=withdrawn;
        --p->stored;
        for(int i=stored_index;i<p->stored;++i) p->collection[i]=p->collection[i+1];
        memset(&p->collection[p->stored],0,sizeof(Creature));
    }
    repair_lead(p);
    return 1;
}

int party_deposit(Party *p,int index)
{
    if(!valid(p) || p->count<=1 || p->stored>=COLLECTION_MAX || index<0 || index>=p->count) return 0;
    p->collection[p->stored++]=p->members[index];
    --p->count;
    for(int i=index;i<p->count;++i) p->members[i]=p->members[i+1];
    memset(&p->members[p->count],0,sizeof(Creature));
    if(p->lead>index) --p->lead;
    else if(p->lead==index) p->lead=0;
    repair_lead(p);
    return 1;
}
````

## src/player_menu.c

````text
#include <stdio.h>
#include "player_menu.h"
#include "map.h"
#include "graphics.h"
#include "text.h"
#define C(r,g,b) (0xff000000u|((unsigned int)(b)<<16)|((unsigned int)(g)<<8)|(r))

void player_menu_open(PlayerMenu *m) { *m=(PlayerMenu){0}; }
PlayerMenuAction player_menu_update(PlayerMenu *m,Party *party,Inventory *bag,GameOptions *options,const Input *input)
{
    int direction=input->vertical?input->vertical:input->horizontal;
    int edge=direction && direction!=m->previous_direction;
    m->previous_direction=direction;
    if (input->menu&INPUT_MENU_OPEN) return MENU_CLOSE;
    if (input->cancel) {
        if (m->page==MENU_HOME) return MENU_CLOSE;
        player_menu_open(m);return MENU_STAY;
    }
    if (input->menu&INPUT_MENU_LOAD) return MENU_LOAD;
    if (input->menu&INPUT_MENU_SAVE) return MENU_SAVE;
    int count=m->page==MENU_HOME?6:m->page==MENU_BAG?ITEM_COUNT:m->page==MENU_OPTIONS?3:1;
    if (edge) m->cursor=(m->cursor+direction+count)%count;
    if (!input->confirm) return MENU_STAY;
    if (m->page==MENU_HOME) {
        if (m->cursor==0) return MENU_PARTY;
        if (m->cursor==2) return MENU_COLLECTION;
        if (m->cursor==4) return MENU_SAVE;
        m->page=m->cursor==1?MENU_BAG:m->cursor==3?MENU_PLAYER:MENU_OPTIONS;
        m->cursor=0;
    } else if (m->page==MENU_BAG) {
        int healed=inventory_use_healing(bag,(ItemId)m->cursor,&party->members[party->lead]);
        if (healed) snprintf(m->message,sizeof(m->message),"%s RECOVERED %d HP.",creature_name(&party->members[party->lead]),healed);
        else snprintf(m->message,sizeof(m->message),"NO ITEM USED. CHECK STOCK AND YOUR LEAD'S HP. THE LODGE CAN REVIVE YOUR TEAM.");
    } else if (m->page==MENU_OPTIONS) {
        if (m->cursor==0) options->music=!options->music;
        else if (m->cursor==1) options->effects=!options->effects;
        else options->motion=!options->motion;
    }
    return MENU_STAY;
}
static void row(int y,const char *label,int selected)
{
    graphics_rectangle(252,y,208,25,selected?C(52,80,76):C(25,37,43));
    if (selected) graphics_rectangle(252,y,3,25,C(240,179,97));
    text_wrap(264,y+9,188,9,label,selected?C(255,216,155):C(180,199,193),1);
}
void player_menu_draw(const PlayerMenu *m,const Party *party,const Inventory *bag,const GameOptions *options,int map_id)
{
    char line[100];
    const Creature *lead=&party->members[party->lead];
    graphics_rectangle(0,0,480,272,C(16,26,32));
    graphics_rectangle(0,0,5,272,C(214,137,75));
    text_draw(20,15,"EMBERWAKE",C(246,213,158),2);
    text_draw(20,37,"WAYFARER'S FIELD KIT",C(145,182,174),1);
    graphics_rectangle(20,61,216,149,C(24,39,46));
    text_wrap(32,75,192,18,map_name(map_id),C(232,220,183),1);
    text_draw(32,105,"LEAD PARTNER",C(143,181,172),1);
    text_box(32,121,188,18,creature_name(lead),C(239,215,169));
    snprintf(line,sizeof(line),"LEVEL %d  HP %d/%d",lead->level,lead->hp,lead->max_hp);
    text_draw(32,148,line,C(191,212,199),1);
    snprintf(line,sizeof(line),"%d EMBERMARKS",bag->embermarks);
    text_draw(32,174,line,C(241,181,108),1);
    snprintf(line,sizeof(line),"TEAM %d/4   STORED %d/24",party->count,party->stored);
    text_draw(32,191,line,C(160,190,181),1);
    if (m->page==MENU_HOME) {
        const char *const names[]={"CREATURES","ITEMS","COLLECTION","PLAYER","SAVE","OPTIONS"};
        for (int i=0;i<6;++i) row(51+i*29,names[i],i==m->cursor);
        text_draw(20,233,"TAKE A MOMENT. YOUR JOURNEY IS PAUSED.",C(170,191,181),1);
    } else if (m->page==MENU_BAG) {
        text_draw(256,57,"ITEMS",C(244,198,118),2);
        for (int i=0;i<ITEM_COUNT;++i) {
            snprintf(line,sizeof(line),"%s  X%d",inventory_item_name((ItemId)i),bag->quantities[i]);
            row(86+i*31,line,i==m->cursor);
        }
        text_wrap(258,158,195,44,inventory_item_description((ItemId)m->cursor),C(171,202,188),1);
        text_wrap(20,221,438,23,m->message[0]?m->message:"X USE ON LEAD PARTNER. SET YOUR LEAD IN CREATURES TO HEAL SOMEONE ELSE.",C(226,211,175),1);
    } else if (m->page==MENU_OPTIONS) {
        text_draw(256,57,"OPTIONS",C(244,198,118),2);
        const int values[]={options->music,options->effects,options->motion};
        const char *const labels[]={"MUSIC","SOUND EFFECTS","ANIMATION"};
        for (int i=0;i<3;++i) {
            snprintf(line,sizeof(line),"%s  %s",labels[i],values[i]?"ON":"OFF");
            row(86+i*32,line,i==m->cursor);
        }
        text_draw(20,226,"X TOGGLE. OPTIONS APPLY TO THIS PLAY SESSION.",C(170,191,181),1);
    } else {
        text_draw(256,57,"PLAYER",C(244,198,118),2);
        text_wrap(258,90,195,100,"THE FERNVEIL JOURNEY\n\nTRAVEL EAST THROUGH THE WOODS TO REACH SUNTHREAD MARSH. SEEK TWO NEW VEYLINGS AMONG ITS REEDS.",C(191,212,199),1);
        text_draw(20,226,"START SAVE   L LOAD   O RETURN TO FIELD KIT",C(170,191,181),1);
    }
    graphics_rectangle(20,247,440,1,C(66,81,81));
    text_draw(20,256,"D-PAD SELECT   X CONFIRM   O BACK   TRI CLOSE",C(169,195,182),1);
}
````

## src/player.c

````text
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
````

## src/save_data.c

````text
#include <psputility.h>
#include <psputility_savedata.h>
#include <psputility_sysparam.h>
#include <stdio.h>
#include <string.h>
#include "save_data.h"

#define SAVE_GAME_NAME "EMBRWAKE"
#define SAVE_NAME "0000"
#define SAVE_FILE "DATA.BIN"

/* 144 x 80 PNG used for the new-slot preview and saved entry. */
static unsigned char save_icon0[] __attribute__((aligned(16))) = {
    0x89,0x50,0x4E,0x47,0x0D,0x0A,0x1A,0x0A,0x00,0x00,0x00,0x0D,0x49,0x48,0x44,0x52,
    0x00,0x00,0x00,0x90,0x00,0x00,0x00,0x50,0x08,0x02,0x00,0x00,0x00,0x79,0xCC,0x6B,
    0x5B,0x00,0x00,0x00,0x8F,0x49,0x44,0x41,0x54,0x78,0xDA,0xED,0xD1,0x01,0x09,0x00,
    0x00,0x08,0x04,0xB1,0x0F,0x61,0x3A,0xFB,0xF7,0xD0,0x1A,0x0A,0x83,0x4B,0x70,0xCB,
    0x74,0xE9,0x51,0xB1,0x00,0x98,0x80,0x09,0x18,0x30,0x01,0x13,0x30,0x60,0x02,0x26,
    0x60,0xC0,0x04,0x4C,0xC0,0x80,0x09,0x98,0x80,0x01,0x13,0x30,0x01,0x03,0x26,0x60,
    0x02,0x06,0x4C,0xC0,0x04,0x0C,0x98,0x80,0x09,0x18,0x30,0x17,0x80,0x09,0x98,0x80,
    0x01,0x13,0x30,0x01,0x03,0x26,0x60,0x02,0x06,0x4C,0xC0,0x04,0x0C,0x98,0x80,0x09,
    0x18,0x30,0x01,0x13,0x30,0x60,0x02,0x26,0x60,0xC0,0x04,0x4C,0xC0,0x80,0x09,0x98,
    0x80,0x01,0x13,0x30,0x01,0x13,0x30,0x60,0x02,0x26,0x60,0xC0,0x04,0x4C,0xC0,0x80,
    0x09,0x98,0x80,0x01,0x13,0x30,0x01,0x03,0x26,0x60,0x02,0x06,0x4C,0xC0,0x04,0x0C,
    0x98,0x6E,0x82,0x2D,0x28,0x32,0x2C,0xC1,0x09,0x5E,0x91,0xC2,0x00,0x00,0x00,0x00,
    0x49,0x45,0x4E,0x44,0xAE,0x42,0x60,0x82
};

typedef struct {
    SceUtilitySavedataParam params;
    PspUtilitySavedataListSaveNewData new_data;
    char save_name_list[2][20];
    char title[32];
    SavePayload payload;
    SavePayload loaded;
    SaveStatus status;
    int loading;
    int shutdown_requested;
    int loaded_valid;
} SaveService;

static SaveService service __attribute__((aligned(16)));

static void savedata_prepare(int mode)
{
    memset(&service.new_data,0,sizeof(service.new_data));
    memset(&service.params,0,sizeof(service.params));
    service.params.base.size=sizeof(service.params);
    service.params.base.language=PSP_SYSTEMPARAM_LANGUAGE_ENGLISH;
    service.params.base.buttonSwap=PSP_UTILITY_ACCEPT_CROSS;
    service.params.base.graphicsThread=0x11;
    service.params.base.accessThread=0x13;
    service.params.base.fontThread=0x12;
    service.params.base.soundThread=0x10;
    /* Only an explicit success result from the utility completes a save. */
    service.params.base.result=-1;
    service.params.mode=(PspUtilitySavedataMode)mode;
    service.params.overwrite=1;
    service.params.focus=mode==PSP_UTILITY_SAVEDATA_LISTSAVE ?
        PSP_UTILITY_SAVEDATA_FOCUS_FIRSTEMPTY : PSP_UTILITY_SAVEDATA_FOCUS_LATEST;
    snprintf(service.params.gameName,sizeof(service.params.gameName),"%s",SAVE_GAME_NAME);
    snprintf(service.params.saveName,sizeof(service.params.saveName),"%s",SAVE_NAME);
    snprintf(service.params.fileName,sizeof(service.params.fileName),"%s",SAVE_FILE);
    snprintf(service.save_name_list[0],sizeof(service.save_name_list[0]),"%s",SAVE_NAME);
    service.save_name_list[1][0]='\0';
    service.params.saveNameList=service.save_name_list;
    service.params.dataBuf=&service.payload;
    service.params.dataBufSize=sizeof(service.payload);
    service.params.dataSize=mode==PSP_UTILITY_SAVEDATA_LISTSAVE ? sizeof(service.payload) : 0;
    if (mode==PSP_UTILITY_SAVEDATA_LISTSAVE) {
        snprintf(service.title,sizeof(service.title),"EMBERWAKE SAVE");
        service.params.icon0FileData.buf=save_icon0;
        service.params.icon0FileData.bufSize=sizeof(save_icon0);
        service.params.icon0FileData.size=sizeof(save_icon0);
        service.new_data.icon0.buf=save_icon0;
        service.new_data.icon0.bufSize=sizeof(save_icon0);
        service.new_data.icon0.size=sizeof(save_icon0);
        service.new_data.title=service.title;
        service.params.newData=&service.new_data;
        snprintf(service.params.sfoParam.title,sizeof(service.params.sfoParam.title),"EMBERWAKE");
        snprintf(service.params.sfoParam.savedataTitle,sizeof(service.params.sfoParam.savedataTitle),"%s",SAVE_NAME);
        snprintf(service.params.sfoParam.detail,sizeof(service.params.sfoParam.detail),"EMBERWAKE PHASE 8 SESSION");
        service.params.sfoParam.parentalLevel=1;
    }
}

static int begin(int mode)
{
    if (service.status==SAVE_STATUS_BUSY) return -1;
    savedata_prepare(mode);
    service.shutdown_requested=0;
    service.loaded_valid=0;
    if (sceUtilitySavedataInitStart(&service.params)<0) {
        service.status=SAVE_STATUS_FAILED;
        return -1;
    }
    service.status=SAVE_STATUS_BUSY;
    return 0;
}

int save_data_begin_save(const SavePayload *payload)
{
    if (!payload || service.status==SAVE_STATUS_BUSY) return -1;
    service.payload=*payload;
    service.loading=0;
    return begin(PSP_UTILITY_SAVEDATA_LISTSAVE);
}

int save_data_begin_load(void)
{
    if (service.status==SAVE_STATUS_BUSY) return -1;
    memset(&service.payload,0,sizeof(service.payload));
    service.loading=1;
    return begin(PSP_UTILITY_SAVEDATA_LISTLOAD);
}

void save_data_update(void)
{
    int state;
    if (service.status!=SAVE_STATUS_BUSY) return;
    state=sceUtilitySavedataGetStatus();
    if (state==PSP_UTILITY_DIALOG_VISIBLE) {
        sceUtilitySavedataUpdate(1);
    } else if (state==PSP_UTILITY_DIALOG_QUIT) {
        if (!service.shutdown_requested && sceUtilitySavedataShutdownStart()>=0)
            service.shutdown_requested=1;
    } else if (state==PSP_UTILITY_DIALOG_FINISHED) {
        /* Shutdown is already underway; do not request it a second time. */
        service.shutdown_requested=1;
    } else if (state==PSP_UTILITY_DIALOG_NONE && service.shutdown_requested) {
        /* FINISHED is a shutdown state, not the save result. Evaluate the
           final result even when polling observes QUIT followed by NONE. */
        if (service.params.base.result==0 && service.loading &&
            service.params.dataSize==sizeof(service.payload)) {
            service.loaded=service.payload;
            service.loaded_valid=service.loaded.magic==SAVE_DATA_MAGIC &&
                service.loaded.version==SAVE_DATA_VERSION;
        }
        service.status=service.params.base.result!=0 ||
            (service.loading && !service.loaded_valid) ?
            SAVE_STATUS_FAILED : SAVE_STATUS_SUCCEEDED;
    }
}

SaveStatus save_data_status(void)
{
    return service.status;
}

int save_data_take_loaded(SavePayload *payload)
{
    if (!payload || service.status!=SAVE_STATUS_SUCCEEDED || !service.loading || !service.loaded_valid)
        return 0;
    *payload=service.loaded;
    service.loaded_valid=0;
    return 1;
}
````

## src/text.c

````text
#include <string.h>
#include "graphics.h"
#include "text.h"
/* Original 5x7 bitmap alphabet. Each row uses its low five bits. */
static const unsigned char glyphs[36][7] = {
 {14,17,17,31,17,17,17},{30,17,17,30,17,17,30},{14,17,16,16,16,17,14},
 {30,17,17,17,17,17,30},{31,16,16,30,16,16,31},{31,16,16,30,16,16,16},
 {14,17,16,23,17,17,15},{17,17,17,31,17,17,17},{14,4,4,4,4,4,14},
 {7,2,2,2,18,18,12},{17,18,20,24,20,18,17},{16,16,16,16,16,16,31},
 {17,27,21,21,17,17,17},{17,25,25,21,19,19,17},{14,17,17,17,17,17,14},
 {30,17,17,30,16,16,16},{14,17,17,17,21,18,13},{30,17,17,30,20,18,17},
 {15,16,16,14,1,1,30},{31,4,4,4,4,4,4},{17,17,17,17,17,17,14},
 {17,17,17,17,17,10,4},{17,17,17,21,21,21,10},{17,17,10,4,10,17,17},
 {17,17,10,4,4,4,4},{31,1,2,4,8,16,31},
 {14,17,19,21,25,17,14},{4,12,4,4,4,4,14},{14,17,1,2,4,8,31},
 {30,1,1,14,1,1,30},{2,6,10,18,31,2,2},{31,16,16,30,1,1,30},
 {14,16,16,30,17,17,14},{31,1,2,4,8,8,8},{14,17,17,14,17,17,14},
 {14,17,17,15,1,1,14}
};
void text_draw(int x, int y, const char *text, unsigned int color, int scale)
{
    if (!text || scale<1) return;
    int origin = x;
    for (; *text; ++text) {
        char ch = *text;
        if (ch == '\n') { x = origin; y += 9*scale; continue; }
        if (ch >= 'a' && ch <= 'z') ch -= 'a'-'A';
        int index = ch >= 'A' && ch <= 'Z' ? ch-'A' :
                    ch >= '0' && ch <= '9' ? 26+ch-'0' : -1;
        for (int row = 0; row < 7; ++row) {
            unsigned int bits = index >= 0 ? glyphs[index][row] : 0;
            if (ch == '.' && row == 6) bits = 4;
            if (ch == '-' && row == 3) bits = 14;
            if (ch == ':' && (row == 2 || row == 5)) bits = 4;
            if (ch == '\'' && row < 2) bits = 4;
            if (ch == '+' && row >= 1 && row <= 5) bits = row==3?31:4;
            if (ch == '/' && row < 5) bits = 1u << row;
            if (ch == '?' && row < 7) { const int q[] = {14,17,1,2,4,0,4}; bits=q[row]; }
            for (int col = 0; col < 5;) {
                if (!(bits & (16u >> col))) { ++col; continue; }
                int start = col++;
                while (col < 5 && (bits & (16u >> col))) ++col;
                graphics_rectangle(x+start*scale,y+row*scale,(col-start)*scale,scale,color);
            }
        }
        x += 6*scale;
    }
}

int text_wrap(int x,int y,int width,int height,const char *text,unsigned int color,int scale)
{
    char line[81];
    int row=0, columns=scale>0?width/(6*scale):0;
    if (!text || columns<1) return 0;
    if (columns>80) columns=80;
    while (*text) {
        int n=0, space=-1, take;
        while (text[n] && text[n]!='\n' && n<columns) {
            if (text[n]==' ') space=n;
            ++n;
        }
        take=n;
        if (text[n] && text[n]!='\n' && text[n]!=' ' && space>0) take=space;
        memcpy(line,text,(size_t)take);line[take]='\0';
        if (row*9*scale+7*scale<=height)
            text_draw(x,y+row*9*scale,line,color,scale);
        ++row;text+=take;
        if (*text=='\n') ++text;
        else while (*text==' ') ++text;
    }
    return row;
}

void text_box(int x,int y,int width,int height,const char *text,unsigned int color)
{
    /* Measure without drawing; prefer the larger font where the whole message fits. */
    int lines=text_wrap(0,0,width,0,text,color,2);
    int scale=lines*18-4<=height?2:1;
    text_wrap(x,y,width,height,text,color,scale);
}
````

## src/world_draw.c

````text
#include <pspgu.h>
#include "graphics.h"
#include "world_draw.h"

#define COLOR(r,g,b) GU_RGBA(r,g,b,255)

static void tile_draw(char tile, int x, int y, int variant,int phase)
{
    graphics_rectangle(x,y,32,32,variant ? COLOR(62,93,66) : COLOR(59,89,63));
    switch (tile) {
    case 'W':
        graphics_rectangle(x,y,32,32,COLOR(110,69,52));
        graphics_rectangle(x,y,32,3,COLOR(174,113,72));
        graphics_rectangle(x+15,y+3,2,29,COLOR(78,50,44));
        break;
    case 'D':
        graphics_rectangle(x,y,32,32,COLOR(154,121,78));
        graphics_rectangle(x+5,y+2,22,30,COLOR(24,30,37));
        graphics_rectangle(x+9,y+27,14,3,COLOR(238,188,96));
        break;
    case '>':
    case '<':
        graphics_rectangle(x,y,32,32,COLOR(169,148,104));
        graphics_rectangle(x+5,y+13,22,6,COLOR(246,219,147));
        graphics_rectangle(x+(tile=='<'?9:19),y+8,4,16,COLOR(246,219,147));
        break;
    case 'r':
        graphics_rectangle(x,y,32,32,variant?COLOR(69,100,83):COLOR(61,92,78));
        for (int i=0;i<3;++i) {
            int sway=(phase+i)%3-1;
            graphics_rectangle(x+5+i*9,y+12,2,17,COLOR(115,151,101));
            graphics_rectangle(x+4+i*9+sway,y+5,4,12,COLOR(207,183,114));
        }
        break;
    case '_':
        graphics_rectangle(x,y,32,32,COLOR(116,101,79));
        graphics_rectangle(x,y+30,32,2,COLOR(74,68,58));
        break;
    case 'c':
        graphics_rectangle(x,y,32,32,COLOR(66,68,81));
        graphics_rectangle(x+5,y+8,8,3,COLOR(91,89,106));
        graphics_rectangle(x+22,y+23,5,3,COLOR(45,48,63));
        break;
    case 'g':
        graphics_rectangle(x,y,32,32,COLOR(36,71,52));
        for (int i=0;i<4;++i) {
            graphics_rectangle(x+3+i*7,y+7,3,10,COLOR(81,122,64));
            graphics_rectangle(x+1+i*7,y+23,3,7,COLOR(62,101,52));
        }
        break;
    case '#': /* Trees fit inside their solid tile. */
        graphics_rectangle(x+12,y+18,8,12,COLOR(89,63,45));
        graphics_rectangle(x+3,y+8,26,15,COLOR(27,58,47));
        graphics_rectangle(x+7,y+2,18,15,COLOR(39,76,53));
        graphics_rectangle(x+10,y+4,8,3,COLOR(58,98,64));
        break;
    case '~':
        graphics_rectangle(x,y,32,32,COLOR(44,91,113));
        graphics_rectangle(x+3+phase,y+9,12,2,COLOR(86,139,151));
        graphics_rectangle(x+18-phase,y+23,10,2,COLOR(63,115,138));
        break;
    case 'O':
        graphics_rectangle(x+3,y+9,26,20,COLOR(44,55,55));
        graphics_rectangle(x+5,y+5,21,20,COLOR(111,118,106));
        graphics_rectangle(x+8,y+5,15,5,COLOR(147,149,126));
        break;
    case 'H':
        graphics_rectangle(x+2,y+3,28,26,COLOR(45,71,68));
        graphics_rectangle(x+6,y+7,20,18,COLOR(86,168,151));
        graphics_rectangle(x+10,y+11,12,10,COLOR(190,234,183));
        graphics_rectangle(x+14,y+9,4,14,COLOR(86,168,151));
        graphics_rectangle(x+9,y+14,14,4,COLOR(86,168,151));
        break;
    case '=':
        graphics_rectangle(x,y,32,32,COLOR(169,148,104));
        graphics_rectangle(x+5,y+8,3,2,COLOR(143,124,86));
        graphics_rectangle(x+22,y+23,4,2,COLOR(190,169,120));
        break;
    case ',': /* Decorative flowers; no encounters in Phase 2. */
        graphics_rectangle(x+8,y+13,2,6,COLOR(33,64,43));
        graphics_rectangle(x+6,y+10,6,4,COLOR(226,183,105));
        graphics_rectangle(x+22,y+22,5,4,COLOR(192,144,130));
        break;
    default:
        graphics_rectangle(x+7,y+10,2,4,COLOR(76,109,70));
        graphics_rectangle(x+23,y+22,3,2,COLOR(47,79,56));
        break;
    }
}

void world_actor_draw(const Player *p, const Camera *camera, int npc)
{
    int x = (int)p->x - camera->x + 8;
    int y = (int)p->y - camera->y + 7;
    if (x < -32 || y < -32 || x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT) return;
    int stride = p->moving ? ((int)(p->animation / 0.1f) % 4) : 0;
    int left = stride == 1 ? 2 : 0;
    int right = stride == 3 ? 2 : 0;
    graphics_rectangle(x,y+19,16,4,COLOR(36,57,43));
    graphics_rectangle(x+3,y+16-left,4,6,COLOR(34,42,57));
    graphics_rectangle(x+9,y+16-right,4,6,COLOR(34,42,57));
    graphics_rectangle(x,y+8,16,9,npc ? COLOR(184,133,181) : COLOR(76,174,161));
    graphics_rectangle(x+3,y+1,10,8,COLOR(238,203,154));
    graphics_rectangle(x+1,y,14,3,COLOR(218,119,65));
    if (p->facing == FACE_UP) {
        graphics_rectangle(x+3,y+3,10,4,COLOR(98,66,47));
        graphics_rectangle(x+4,y+10,8,7,COLOR(163,107,62));
    } else if (p->facing == FACE_DOWN) {
        graphics_rectangle(x+5,y+5,2,2,COLOR(35,43,48));
        graphics_rectangle(x+10,y+5,2,2,COLOR(35,43,48));
    } else {
        int eye = p->facing == FACE_LEFT ? 3 : 11;
        graphics_rectangle(x+eye,y+5,2,2,COLOR(35,43,48));
        graphics_rectangle(x+(p->facing == FACE_LEFT ? 12 : 0),y+9,4,8,
                           COLOR(163,107,62));
    }
}

void world_draw(const Map *map, const Player *player, const Camera *camera,float animation)
{
    /* At most 16 columns x 10 rows, independent of total map size. */
    int first_x = camera->x / TILE_SIZE;
    int first_y = camera->y / TILE_SIZE;
    int last_x = (camera->x + SCREEN_WIDTH - 1) / TILE_SIZE;
    int last_y = (camera->y + SCREEN_HEIGHT - 1) / TILE_SIZE;
    for (int y = first_y; y <= last_y && y < map->height; ++y) {
        for (int x = first_x; x <= last_x && x < map->width; ++x) {
            tile_draw(map_tile(map,x,y), x*TILE_SIZE-camera->x,
                      y*TILE_SIZE-camera->y, (x+y)%2,(int)(animation*4)%4);
        }
    }
    world_actor_draw(player, camera, 0);
}
````

## tests/battle_party_test.c

````text
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "battle.h"

static void confirm(Battle *b) { battle_update(b,&(Input){.confirm=1}); }
static void cancel(Battle *b) { battle_update(b,&(Input){.cancel=1}); }

static Party make_party(int count)
{
    Party p={0};
    p.count=count;
    for(int i=0;i<count;++i) {
        creature_create(&p.members[i],SPECIES_CINDLET,5);
        snprintf(p.members[i].nickname,sizeof(p.members[i].nickname),"PARTNER %d",i+1);
    }
    return p;
}

static void same_creature(const Creature *a,const Creature *b)
{
    assert(a->species==b->species && !strcmp(a->nickname,b->nickname));
    assert(a->element==b->element && a->level==b->level);
    assert(a->experience==b->experience && a->hp==b->hp && a->max_hp==b->max_hp);
    assert(a->attack==b->attack && a->defense==b->defense && a->speed==b->speed);
    for(int i=0;i<CREATURE_MOVES;++i) {
        assert(a->moves[i]==b->moves[i]);
        assert(a->uses[i]==b->uses[i]);
    }
}

static int total_uses(const Creature *c)
{
    int total=0;
    for(int i=0;i<CREATURE_MOVES;++i) total+=c->uses[i];
    return total;
}

static void start(Battle *b,const Party *p,unsigned int seed)
{
    battle_begin_party(b,p,SPECIES_MOSSLET,3,seed);
    assert(b->phase==BATTLE_MESSAGE && b->capture_charges==3);
    assert(b->active==p->lead);
    same_creature(&b->ally,&p->members[p->lead]);
    /* Use a 100%-accurate move for checks that count enemy responses. */
    for(int i=0;i<CREATURE_MOVES;++i) {
        b->enemy.moves[i]=MOVE_NUDGE;
        b->enemy.uses[i]=24;
    }
    confirm(b);
    assert(b->phase==BATTLE_MENU);
}

static void messages(Battle *b)
{
    int count=0;
    while(b->phase==BATTLE_MESSAGE && count++<30) confirm(b);
    assert(count<30);
}

static void open_capture(Battle *b)
{
    assert(b->phase==BATTLE_MENU);
    b->cursor=1;confirm(b);
    assert(b->phase==BATTLE_CAPTURE);
}

static void open_switch(Battle *b)
{
    assert(b->phase==BATTLE_MENU);
    b->cursor=2;confirm(b);
    assert(b->phase==BATTLE_SWITCH);
}

static void attack(Battle *b)
{
    assert(b->phase==BATTLE_MENU);
    b->cursor=0;confirm(b);
    assert(b->phase==BATTLE_ATTACKS);
    b->move_cursor=0;confirm(b);
}

static void item_use(void)
{
    Party p=make_party(1);
    Battle b;
    start(&b,&p,19);
    b.ally.hp-=10;
    b.cursor=3;confirm(&b);
    assert(b.phase==BATTLE_ITEMS && b.inventory.quantities[ITEM_PULSE_TONIC]==3);
    confirm(&b);
    /* The tonic restores the missing 10 HP and cannot exceed maximum HP. */
    assert(b.phase==BATTLE_MESSAGE && b.ally.hp==b.ally.max_hp &&
           b.inventory.quantities[ITEM_PULSE_TONIC]==2 && total_uses(&b.enemy)==96);
    messages(&b);
    assert(b.phase==BATTLE_MENU && total_uses(&b.enemy)==95);
}

static void capture_cases(unsigned int *success_seed,unsigned int *failure_seed)
{
    Party p=make_party(2);
    Battle b;
    int healthy=0,weakened=0;
    *success_seed=0;*failure_seed=0;
    /* The identical seed sequence is used at full and one HP. This checks the
       battle actually passes current HP into capture, rather than only testing
       a capture probability helper in isolation. */
    for(unsigned int seed=1;seed<=400;++seed) {
        int caught[2]={0};
        for(int weak=0;weak<=1;++weak) {
            start(&b,&p,seed);
            if(weak) b.enemy.hp=1;
            open_capture(&b);confirm(&b);
            assert(b.capture_charges==2);
            caught[weak]=b.result==BATTLE_CAUGHT;
            if(caught[weak]) {
                if(!weak && !*success_seed) *success_seed=seed;
                assert(b.party.count==3 && b.party.stored==0);
                assert(b.party.members[2].species==SPECIES_MOSSLET);
                assert(b.party.members[2].level==3);
                assert(!b.reward_given && b.experience_reward==0);
                messages(&b);
                assert(b.phase==BATTLE_DONE);
                assert(b.ally.experience==p.members[0].experience);
                for(int i=0;i<p.count;++i)
                    same_creature(&b.party.members[i],&p.members[i]);
            } else {
                if(!weak && !*failure_seed) *failure_seed=seed;
                assert(b.result==BATTLE_ONGOING);
                assert(total_uses(&b.enemy)==96); /* Reply awaits dismissal. */
                messages(&b);
                assert(b.phase==BATTLE_MENU && total_uses(&b.enemy)==95);
                assert(b.party.count==p.count && b.party.stored==0);
                assert(!b.reward_given && b.ally.experience==p.members[0].experience);
            }
        }
        assert(!caught[0] || caught[1]); /* Weakening cannot reduce the chance. */
        healthy+=caught[0];weakened+=caught[1];
    }
    assert(healthy>0 && healthy<400 && weakened>healthy);
    assert(*success_seed && *failure_seed);

    p=make_party(4);
    start(&b,&p,*success_seed);
    b.ally.hp-=7;b.ally.uses[0]-=2;
    Creature final_active=b.ally;
    open_capture(&b);confirm(&b);
    assert(b.result==BATTLE_CAUGHT && b.party.count==4 && b.party.stored==1);
    assert(b.party.collection[0].species==SPECIES_MOSSLET);
    assert(!b.reward_given && b.experience_reward==0);
    messages(&b);assert(b.phase==BATTLE_DONE);
    same_creature(&b.party.members[0],&final_active);
    for(int i=1;i<p.count;++i) same_creature(&b.party.members[i],&p.members[i]);

    /* A full collection blocks the action before spending a charge or RNG. */
    p.stored=(int)(sizeof(p.collection)/sizeof(p.collection[0]));
    for(int i=0;i<p.stored;++i) creature_create(&p.collection[i],SPECIES_FLINTLING,3);
    start(&b,&p,*success_seed);
    unsigned int random_before=b.random;
    open_capture(&b);confirm(&b);messages(&b);
    assert(b.phase==BATTLE_MENU && b.result==BATTLE_ONGOING);
    assert(b.capture_charges==3 && total_uses(&b.enemy)==96);
    assert(b.random==random_before && b.party.count==p.count && b.party.stored==p.stored);
    for(int i=0;i<p.stored;++i) same_creature(&b.party.collection[i],&p.collection[i]);
}

static void capture_charges_and_cancel(unsigned int failure_seed)
{
    Party p=make_party(1);
    Battle b;
    start(&b,&p,failure_seed);
    unsigned int random_before=b.random;
    open_capture(&b);cancel(&b);
    assert(b.phase==BATTLE_MENU && b.capture_charges==3);
    assert(b.random==random_before && total_uses(&b.enemy)==96);
    assert(b.ally.hp==p.members[0].hp);
    for(int i=0;i<3;++i) {
        b.random=failure_seed;
        open_capture(&b);confirm(&b);
        assert(b.capture_charges==2-i && b.result==BATTLE_ONGOING);
        assert(total_uses(&b.enemy)==96-i);
        messages(&b);
        assert(b.phase==BATTLE_MENU && total_uses(&b.enemy)==95-i);
        assert(b.party.count==1 && b.party.stored==0);
    }
    random_before=b.random;
    int hp=b.ally.hp;
    open_capture(&b);confirm(&b);messages(&b);
    assert(b.phase==BATTLE_MENU && b.capture_charges==0);
    assert(b.random==random_before && b.ally.hp==hp);
    assert(total_uses(&b.enemy)==93 && b.result==BATTLE_ONGOING);
}

static void voluntary_switch(void)
{
    Party p=make_party(3);
    p.members[2].hp=0;
    Battle b;
    start(&b,&p,12);
    b.ally.hp-=11;b.ally.uses[0]=3;
    Creature outgoing=b.ally;
    open_switch(&b);cancel(&b);
    assert(b.phase==BATTLE_MENU && b.active==0 && total_uses(&b.enemy)==96);
    open_switch(&b);
    b.switch_cursor=0;confirm(&b);messages(&b);
    assert(b.phase==BATTLE_SWITCH && b.active==0 && total_uses(&b.enemy)==96);
    b.switch_cursor=2;confirm(&b);messages(&b);
    assert(b.phase==BATTLE_SWITCH && b.active==0 && total_uses(&b.enemy)==96);
    b.switch_cursor=1;confirm(&b);
    assert(b.phase==BATTLE_MESSAGE && b.active==1 && total_uses(&b.enemy)==96);
    same_creature(&b.party.members[0],&outgoing);
    same_creature(&b.ally,&p.members[1]);
    confirm(&b);
    assert(total_uses(&b.enemy)==95 && b.ally.hp<p.members[1].hp);
    messages(&b);
    assert(b.phase==BATTLE_MENU && total_uses(&b.enemy)==95);
    for(int i=0;i<4;++i) battle_update(&b,&(Input){0});
    assert(total_uses(&b.enemy)==95);
    same_creature(&b.party.members[0],&outgoing);
    same_creature(&b.party.members[2],&p.members[2]);

    /* End-of-battle synchronization must keep the actual outgoing and active
       creatures instead of overwriting the original lead with its replacement. */
    Creature final_active=b.ally;
    b.cursor=4;b.escape_attempts=2;confirm(&b);messages(&b);
    assert(b.phase==BATTLE_DONE && b.result==BATTLE_ESCAPED && b.party.lead==1);
    same_creature(&b.party.members[0],&outgoing);
    same_creature(&b.party.members[1],&final_active);
    same_creature(&b.party.members[2],&p.members[2]);
}

static void forced_replacement_and_team_loss(void)
{
    Party p=make_party(3);
    p.members[0].hp=1;p.members[2].hp=0;
    Battle b;
    start(&b,&p,12);b.enemy.speed=999;
    int ally_uses=total_uses(&b.ally);
    attack(&b);
    assert(b.ally.hp==0 && b.result==BATTLE_ONGOING && b.forced_switch);
    assert(total_uses(&b.ally)==ally_uses && total_uses(&b.enemy)==95);
    confirm(&b);
    assert(b.phase==BATTLE_SWITCH && b.party.members[0].hp==0);
    cancel(&b);
    assert(b.phase==BATTLE_SWITCH && b.forced_switch && total_uses(&b.enemy)==95);
    b.switch_cursor=0;confirm(&b);messages(&b);
    assert(b.phase==BATTLE_SWITCH && b.forced_switch);
    b.switch_cursor=2;confirm(&b);messages(&b);
    assert(b.phase==BATTLE_SWITCH && b.forced_switch && total_uses(&b.enemy)==95);
    b.switch_cursor=1;confirm(&b);messages(&b);
    assert(b.phase==BATTLE_MENU && b.active==1 && !b.forced_switch);
    assert(total_uses(&b.enemy)==95); /* Replacement is free after a knockout. */
    same_creature(&b.ally,&p.members[1]);
    assert(b.enemy.hp==b.enemy.max_hp); /* Knocked-out ally's turn was discarded. */
    b.ally.hp=1;
    attack(&b);
    assert(b.ally.hp==0 && b.result==BATTLE_LOSS && !b.forced_switch);
    messages(&b);
    assert(b.phase==BATTLE_DONE && total_uses(&b.enemy)==94);
    for(int i=0;i<b.party.count;++i) assert(b.party.members[i].hp==0);
    assert(b.party.lead==1 && !b.reward_given);
}

static void victory_updates_only_active(void)
{
    Party p=make_party(3);
    p.members[1].experience=creature_xp_for_level(6)-1;
    p.members[2].hp-=9;p.members[2].uses[0]-=4;
    Battle b;
    start(&b,&p,42);
    b.ally.hp-=7;b.ally.uses[0]-=3;
    Creature outgoing=b.ally;
    open_switch(&b);b.switch_cursor=1;confirm(&b);messages(&b);
    assert(b.phase==BATTLE_MENU && b.active==1);
    b.enemy.hp=1;b.ally.speed=999;
    attack(&b);
    assert(b.result==BATTLE_WIN);
    confirm(&b);
    assert(b.reward_given && b.ally.level==6);
    int earned=b.ally.experience;
    assert(earned==p.members[1].experience+species_get(SPECIES_MOSSLET)->experience_yield*3);
    messages(&b);
    assert(b.phase==BATTLE_LEARN);
    b.learn_cursor=1;confirm(&b);messages(&b);
    assert(b.phase==BATTLE_DONE && b.party.lead==1);
    assert(b.ally.moves[1]==MOVE_HEAT && b.party.members[1].moves[1]==MOVE_HEAT);
    same_creature(&b.party.members[1],&b.ally);
    same_creature(&b.party.members[0],&outgoing);
    same_creature(&b.party.members[2],&p.members[2]);
    confirm(&b);confirm(&b);
    assert(b.ally.experience==earned && b.party.members[1].experience==earned);
    /* The caller owns the original team; a battle works on its own snapshot. */
    assert(p.members[0].hp==p.members[0].max_hp);
    assert(p.members[1].level==5 && p.members[1].moves[1]==MOVE_CINDER);

    /* Beginning with a changed lead must load that member, not slot zero. */
    p.lead=2;
    start(&b,&p,42);
    assert(b.active==2);
}

int main(void)
{
    unsigned int success_seed,failure_seed;
    capture_cases(&success_seed,&failure_seed);
    capture_charges_and_cancel(failure_seed);
    voluntary_switch();
    forced_replacement_and_team_loss();
    victory_updates_only_active();
    item_use();
    puts("PASS: capture odds/charges/storage, one enemy reply, switching, forced replacement, team loss, active-only growth, item turns and party sync");
    return 0;
}
````

## tests/battle_test.c

````text
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "battle.h"

static void press(Battle *b)
{
    battle_update(b,&(Input){0,0,1,0,0,0});
}
static void start(Battle *b,unsigned int seed)
{
    Battler ally; battler_starter(&ally);
    battle_begin(b,&ally,SPECIES_MOSSLET,3,seed);
    press(b); assert(b->phase==BATTLE_MENU);
}
static void choose(Battle *b,int move)
{
    b->cursor=0; press(b); assert(b->phase==BATTLE_ATTACKS);
    b->move_cursor=move; press(b);
}
static void finish_messages(Battle *b)
{
    int limit=8;
    while(b->phase==BATTLE_MESSAGE && limit-->0) press(b);
    assert(limit>0);
}
int main(void)
{
    Battle b;
    start(&b,12);
    assert(b.ally.level==5 && b.ally.hp==b.ally.max_hp);
    int hp=b.enemy.hp;
    choose(&b,0);
    assert(b.ally.uses[0]==23 && b.enemy.hp<hp);
    assert(b.ally.hp==b.ally.max_hp); /* Enemy has not acted yet. */
    press(&b);
    assert(b.ally.hp<b.ally.max_hp);
    press(&b); assert(b.phase==BATTLE_MENU);

    start(&b,12);
    b.enemy.hp=1;
    choose(&b,0);
    assert(b.result==BATTLE_WIN && b.enemy.hp==0);
    finish_messages(&b);
    assert(b.phase==BATTLE_DONE && b.ally.hp==b.ally.max_hp);
    battle_update(&b,&(Input){0,0,1,0,0,0}); assert(b.phase==BATTLE_DONE);

    start(&b,12);
    b.ally.hp=1; b.enemy.speed=999;
    for(int i=0;i<4;++i) b.enemy.moves[i]=MOVE_NUDGE;
    choose(&b,0);
    assert(b.result==BATTLE_LOSS && b.ally.hp==0 && b.ally.uses[0]==24);
    finish_messages(&b); assert(b.phase==BATTLE_DONE);

    start(&b,12);
    b.cursor=0; press(&b);
    battle_update(&b,&(Input){0,0,0,1,0,0});
    assert(b.phase==BATTLE_MENU && b.enemy.hp==b.enemy.max_hp);
    /* Menu navigation advances once per new direction, not once per frame. */
    for(int i=0;i<20;++i) battle_update(&b,&(Input){0,1,0,0,0,0});
    assert(b.cursor==1);
    battle_update(&b,&(Input){0});
    battle_update(&b,&(Input){0,1,0,0,0,0}); assert(b.cursor==2);
    b.cursor=1;press(&b);assert(b.phase==BATTLE_CAPTURE);
    battle_update(&b,&(Input){.cancel=1});assert(b.phase==BATTLE_MENU);
    b.cursor=2;press(&b);assert(b.phase==BATTLE_SWITCH);
    battle_update(&b,&(Input){.cancel=1});assert(b.phase==BATTLE_MENU);
    b.cursor=3;press(&b);assert(b.phase==BATTLE_ITEMS);
    battle_update(&b,&(Input){.cancel=1});
    assert(b.phase==BATTLE_MENU && b.ally.hp==b.ally.max_hp);
    b.ally.uses[0]=0; choose(&b,0);
    assert(strstr(b.message,"NO USES") && b.enemy.hp==b.enemy.max_hp);
    finish_messages(&b);
    for(int i=0;i<4;++i) b.ally.uses[i]=0;
    choose(&b,1); assert(strstr(b.message,"PRESS ON"));
    finish_messages(&b);
    for(int i=0;i<4;++i) assert(b.ally.uses[i]==0);

    int saw_fail=0,saw_success=0,saw_miss=0;
    for(unsigned int seed=1;seed<100;++seed) {
        start(&b,seed); b.cursor=4; press(&b);
        if(b.result==BATTLE_ESCAPED) {
            saw_success=1;finish_messages(&b);assert(b.phase==BATTLE_DONE);
        } else {
            saw_fail=1;
            int uses=0;for(int i=0;i<4;++i) uses+=b.enemy.uses[i];
            finish_messages(&b);assert(b.phase==BATTLE_MENU);
            int after=0;for(int i=0;i<4;++i) after+=b.enemy.uses[i];
            assert(after==uses-1); /* Exactly one response to failed escape. */
            b.escape_attempts=2;b.cursor=4;press(&b);
            assert(b.result==BATTLE_ESCAPED);
        }
        start(&b,seed);choose(&b,2);
        if(strstr(b.message,"MISSED")) {
            saw_miss=1;assert(b.enemy.hp==b.enemy.max_hp && b.ally.uses[2]==7);
        }
    }
    assert(saw_fail && saw_success && saw_miss);
    start(&b,7);
    int weak=battle_damage(&b.ally,&b.enemy,attack_get(MOVE_NUDGE),90);
    int strong=battle_damage(&b.ally,&b.enemy,attack_get(MOVE_NUDGE),100);
    assert(weak>=1 && strong>=weak);
    assert(attack_effectiveness(ELEMENT_EMBER,ELEMENT_GROVE)==4);
    assert(attack_effectiveness(ELEMENT_GROVE,ELEMENT_EMBER)==1);
    assert(attack_effectiveness(ELEMENT_PLAIN,ELEMENT_STONE)==2);
    assert(attack_effectiveness(ELEMENT_EMBER,ELEMENT_EMBER)==1);
    b.enemy.defense=0;assert(battle_damage(&b.ally,&b.enemy,attack_get(MOVE_NUDGE),100)>0);
    puts("PASS: turns, speed, HP, victory/defeat, accuracy, uses, fallback, menu, escape, damage");
    return 0;
}
````

## tests/creature_test.c

````text
#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include "battle.h"

static void confirm(Battle *b) { battle_update(b,&(Input){0,0,1,0,0,0}); }
static void victory(Battle *b,Creature *c)
{
    battle_begin(b,c,SPECIES_MOSSLET,3,42);
    b->enemy.hp=1;b->ally.speed=999;
    confirm(b);confirm(b);confirm(b); /* Intro, fight, basic attack. */
    assert(b->result==BATTLE_WIN);
    confirm(b); /* Award XP exactly once. */
}
static void close_growth(Battle *b)
{
    for(int i=0;i<30 && b->phase!=BATTLE_DONE;++i) {
        if(b->phase==BATTLE_LEARN) battle_update(b,&(Input){0,0,0,1,0,0});
        else confirm(b);
    }
    assert(b->phase==BATTLE_DONE);
}
int main(void)
{
    assert(SPECIES_COUNT==12);
    for(int id=0;id<SPECIES_COUNT;++id) {
        const Species *s=species_get(id);
        assert((int)s->id==id && s->name[0] && s->description[0]);
        assert(s->base_hp>0 && s->base_attack>0 && s->base_defense>0 && s->base_speed>0);
        assert(s->experience_yield>0 && s->learn_count>0 && s->learn_count<=8);
        assert(s->overworld_sprite==id && s->battle_sprite==id);
        if(s->evolution_level) assert(s->evolved_species>=0 && s->evolved_species<SPECIES_COUNT && s->evolved_species!=id);
        for(int i=0;i<s->learn_count;++i) {
            assert(s->learnset[i].move>=0 && s->learnset[i].move<MOVE_COUNT);
            assert(s->learnset[i].level>=1 && s->learnset[i].level<=100);
        }
        for(int level=1;level<=100;++level) {
            Creature c;creature_create(&c,id,level);
            assert(c.level==level && c.hp==c.max_hp && c.experience==creature_xp_for_level(level));
            for(int i=0;i<4;++i) {
                assert(c.moves[i]>=-1 && c.moves[i]<MOVE_COUNT);
                assert(c.uses[i]==(c.moves[i]<0?0:attack_get(c.moves[i])->uses));
                for(int j=0;j<i;++j) assert(c.moves[i]<0 || c.moves[i]!=c.moves[j]);
            }
        }
    }
    Creature c;CreatureGrowth g;
    creature_create(&c,SPECIES_CINDLET,5);
    int hp=c.hp;c.hp-=7;
    creature_gain_xp(&c,179,&g);assert(c.level==5 && g.move_count==0);
    creature_gain_xp(&c,1,&g);assert(c.level==6 && c.hp==hp-7+5);
    assert(g.move_count==1 && g.moves[0]==MOVE_HEAT);
    assert(c.moves[1]==MOVE_CINDER); /* Learning requires a choice. */
    assert(creature_learn(&c,MOVE_HEAT,1));
    assert(c.moves[1]==MOVE_HEAT && c.uses[1]==10);
    assert(!creature_learn(&c,MOVE_HEAT,2));
    assert(!creature_learn(&c,MOVE_COUNT,0));
    assert(!creature_learn(&c,MOVE_LEAF,4));

    strcpy(c.nickname,"SPARK");
    creature_gain_xp(&c,creature_xp_for_level(8)-c.experience,&g);
    assert(c.species==SPECIES_EMBERLYN && c.level==8 && c.attack==42);
    assert(!strcmp(creature_name(&c),"SPARK"));
    assert(g.old_species==SPECIES_CINDLET && c.moves[1]==MOVE_HEAT);
    creature_gain_xp(&c,0,&g);assert(g.move_count==0 && g.old_species==SPECIES_EMBERLYN);
    creature_gain_xp(&c,-5,&g);assert(c.experience==creature_xp_for_level(8));
    creature_gain_xp(&c,INT_MAX,&g);
    assert(c.level==100 && c.experience==creature_xp_for_level(100) && !creature_xp_remaining(&c));
    creature_gain_xp(&c,INT_MAX,&g);assert(g.move_count==0);
    creature_create(&c,SPECIES_MOSSLET,7);c.hp=0;
    creature_gain_xp(&c,creature_xp_for_level(8)-c.experience,&g);
    assert(c.species==SPECIES_MOSSHORN && c.hp==0); /* Stats cannot revive a fainted creature. */
    creature_create(&c,SPECIES_FLINTLING,1);
    creature_gain_xp(&c,creature_xp_for_level(12),&g);
    assert(c.species==SPECIES_FLINTAUR && c.level==12 && g.move_count<=MOVE_COUNT);

    Battle b;
    creature_create(&c,SPECIES_CINDLET,5);c.experience=creature_xp_for_level(6)-1;
    victory(&b,&c);int earned=b.ally.experience;
    assert(b.ally.level==6 && b.reward_given);
    for(int i=0;i<8 && b.phase!=BATTLE_LEARN;++i) confirm(&b);
    assert(b.phase==BATTLE_LEARN && b.learn_cursor==4);
    b.learn_cursor=1;confirm(&b);
    assert(b.ally.moves[1]==MOVE_HEAT);
    close_growth(&b);confirm(&b);assert(b.ally.experience==earned);
    victory(&b,&c);
    for(int i=0;i<8 && b.phase!=BATTLE_LEARN;++i) confirm(&b);
    battle_update(&b,&(Input){0,0,0,1,0,0});close_growth(&b);
    assert(b.ally.moves[1]==MOVE_CINDER);

    creature_create(&c,SPECIES_CINDLET,7);c.experience=creature_xp_for_level(8)-1;
    victory(&b,&c);assert(b.ally.species==SPECIES_EMBERLYN);
    close_growth(&b);assert(b.ally.species==SPECIES_EMBERLYN);
    creature_create(&c,SPECIES_CINDLET,5);
    battle_begin(&b,&c,SPECIES_MOSSLET,3,2);confirm(&b);
    b.cursor=4;b.escape_attempts=2;confirm(&b);confirm(&b);
    assert(b.phase==BATTLE_DONE && b.ally.experience==c.experience);
    puts("PASS: 12 species, 100 levels, XP boundaries/cap, evolution, stats, nicknames, move choices, single rewards");
    return 0;
}
````

## tests/host/audio_stub.c

````text
#include "audio.h"
int audio_init(void) { return 1; }
void audio_shutdown(void) { }
void audio_scene(MusicScene scene) { (void)scene; }
void audio_play(Sound sound) { (void)sound; }
void audio_settings(int music,int effects) { (void)music;(void)effects; }
````

## tests/host/pspgu.h

````text
#ifndef TEST_PSPGU_H
#define TEST_PSPGU_H
/* Host-only color packing; never included by the PSP Makefile. */
#define GU_RGBA(r,g,b,a) ((unsigned int)(r) | ((unsigned int)(g)<<8) | ((unsigned int)(b)<<16) | ((unsigned int)(a)<<24))
#endif
````

## tests/host/psputility_savedata.h

````text
#ifndef TEST_PSPUTILITY_SAVEDATA_H
#define TEST_PSPUTILITY_SAVEDATA_H
#include "psputility.h"
typedef enum {
    PSP_UTILITY_SAVEDATA_LISTLOAD=4, PSP_UTILITY_SAVEDATA_LISTSAVE=5
} PspUtilitySavedataMode;
enum { PSP_UTILITY_SAVEDATA_FOCUS_LATEST=3, PSP_UTILITY_SAVEDATA_FOCUS_FIRSTEMPTY=7 };
typedef struct {
    void *buf;
    size_t bufSize, size;
} PspUtilitySavedataFileData;
typedef struct {
    PspUtilitySavedataFileData icon0;
    char *title;
} PspUtilitySavedataListSaveNewData;
typedef struct {
    pspUtilityDialogCommon base;
    PspUtilitySavedataMode mode;
    int overwrite, focus;
    char gameName[13], saveName[20], fileName[13];
    char (*saveNameList)[20];
    void *dataBuf;
    size_t dataBufSize, dataSize;
    PspUtilitySavedataFileData icon0FileData;
    PspUtilitySavedataListSaveNewData *newData;
    struct {
        char title[128], savedataTitle[128], detail[1024];
        unsigned char parentalLevel;
    } sfoParam;
} SceUtilitySavedataParam;
int sceUtilitySavedataInitStart(SceUtilitySavedataParam *params);
int sceUtilitySavedataGetStatus(void);
int sceUtilitySavedataShutdownStart(void);
void sceUtilitySavedataUpdate(int step);
#endif
````

## tests/host/psputility_sysparam.h

````text
#ifndef TEST_PSPUTILITY_SYSPARAM_H
#define TEST_PSPUTILITY_SYSPARAM_H
#define PSP_SYSTEMPARAM_LANGUAGE_ENGLISH 1
#endif
````

## tests/host/psputility.h

````text
#ifndef TEST_PSPUTILITY_H
#define TEST_PSPUTILITY_H
/* Host declarations for exercising savedata control flow, not PSP ABI layout. */
#include <stddef.h>
enum {
    PSP_UTILITY_DIALOG_NONE, PSP_UTILITY_DIALOG_INIT, PSP_UTILITY_DIALOG_VISIBLE,
    PSP_UTILITY_DIALOG_QUIT, PSP_UTILITY_DIALOG_FINISHED
};
#define PSP_UTILITY_ACCEPT_CROSS 1
typedef struct {
    unsigned int size;
    int language, buttonSwap, graphicsThread, accessThread, fontThread, soundThread;
    int result;
} pspUtilityDialogCommon;
#endif
````

## tests/host/save_data_stub.c

````text
#include "save_data.h"

int save_data_begin_save(const SavePayload *payload)
{
    (void)payload;
    return -1;
}

int save_data_begin_load(void) { return -1; }
void save_data_update(void) { }
SaveStatus save_data_status(void) { return SAVE_STATUS_IDLE; }
int save_data_take_loaded(SavePayload *payload)
{
    (void)payload;
    return 0;
}
````

## tests/inventory_test.c

````text
#include <assert.h>
#include <stdio.h>
#include "inventory.h"

int main(void)
{
    Inventory bag;
    Creature c;
    inventory_init(&bag);
    creature_create(&c,SPECIES_CINDLET,5);
    c.hp=10;
    assert(bag.quantities[ITEM_PULSE_TONIC]==3 && bag.quantities[ITEM_HEARTH_RESTORE]==1 && bag.embermarks==250);
    assert(inventory_buy(&bag,ITEM_PULSE_TONIC) && bag.quantities[ITEM_PULSE_TONIC]==4 && bag.embermarks==200);
    assert(inventory_use_healing(&bag,ITEM_PULSE_TONIC,&c)==25 && c.hp==35);
    assert(bag.quantities[ITEM_PULSE_TONIC]==3);
    c.hp=1;
    assert(inventory_use_healing(&bag,ITEM_HEARTH_RESTORE,&c)==c.max_hp-1 && c.hp==c.max_hp);
    assert(bag.quantities[ITEM_HEARTH_RESTORE]==0);
    assert(!inventory_use_healing(&bag,ITEM_HEARTH_RESTORE,&c));
    c.hp=0;
    assert(!inventory_use_healing(&bag,ITEM_PULSE_TONIC,&c));
    puts("PASS: inventory defaults, capped tonic healing, full restore, and invalid item use");
    return 0;
}
````

## tests/overworld_test.c

````text
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
    tick(&p,map,(Input){1,0,0,0,0,0},1,0.025f);
    assert(p.moving && p.x > 160 && p.x < 192);
    /* Release completes exactly one step. */
    tick(&p,map,(Input){0,0,0,0,0,0},20,0.025f);
    assert(!p.moving && p.tile_x == 6 && p.x == 192);

    player_init(&p,map);
    tick(&p,map,(Input){1,0,0,0,0,0},1,0.025f);
    tick(&p,map,(Input){0,-1,0,0,0,0},40,0.025f);
    assert(p.tile_x == 6 && p.tile_y < 11 && p.facing == FACE_UP);

    /* Held motion covers the same distance at 30, 60 and 120 Hz. */
    const int rates[] = {30,60,120};
    for (int i = 0; i < 3; ++i) {
        player_init(&p,map);
        tick(&p,map,(Input){1,0,0,0,0,0},rates[i],1.0f/rates[i]);
        assert(fabsf(p.x - 288.0f) < 0.01f);
    }
    player_init(&p,map);
    tick(&p,map,(Input){1,-1,0,0,0,0},20,0.025f);
    assert(p.y == 352); /* Horizontal priority; no diagonal corner cutting. */
    player_init(&p,map);
    tick(&p,map,(Input){-1,0,0,0,0,0},100,0.05f);
    assert(p.tile_x == 1 && p.x == 32 && !p.moving);
    tick(&p,map,(Input){0,-1,0,0,0,0},200,0.05f);
    assert(p.tile_y == 1 && p.y == 32 && !p.moving);

    /* Collision from all four sides of a blocked tile. */
    const char *const rows[] = {".....",".....","..O..",".....","....."};
    const int starts[][2] = {{1,2},{3,2},{2,1},{2,3}};
    const Input directions[] = {{1,0,0,0,0,0},{-1,0,0,0,0,0},{0,1,0,0,0,0},{0,-1,0,0,0,0}};
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
    tick(&p,map,(Input){1,0,0,0,0,0},1,10.0f);
    assert(p.x <= 166.401f); /* Long pauses are clamped. */
    puts("PASS: map, collision, release, turning, frame rates, camera, pause cap");
    return 0;
}
````

## tests/party_capture_test.c

````text
#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include "party.h"
#include "capture.h"

static void same_creature(const Creature *a,const Creature *b)
{
    assert(a->species==b->species && !strcmp(a->nickname,b->nickname));
    assert(a->name==b->name && a->element==b->element);
    assert(a->level==b->level && a->experience==b->experience);
    assert(a->max_hp==b->max_hp && a->hp==b->hp && a->attack==b->attack);
    assert(a->defense==b->defense && a->speed==b->speed);
    for(int i=0;i<CREATURE_MOVES;++i) {
        assert(a->moves[i]==b->moves[i] && a->uses[i]==b->uses[i]);
    }
}

static Creature distinctive(int id)
{
    Creature c;
    creature_create(&c,id%SPECIES_COUNT,5+id%6);
    snprintf(c.nickname,sizeof(c.nickname),"FRIEND %d",id);
    c.experience+=id+1;
    c.hp-=1+id%5;
    c.uses[0]=id%3;
    return c;
}

static void party_checks(void)
{
    Party p;party_init(&p);
    assert(p.count==1 && p.stored==0 && p.lead==0 && party_has_space(&p));
    assert(p.members[0].species==SPECIES_CINDLET && p.members[0].level==5);
    Creature original=p.members[0],c;
    assert(!party_deposit(&p,0) && !party_set_lead(&p,1));
    assert(!party_exchange(&p,0,0) && !party_add(&p,NULL));
    for(int i=1;i<PARTY_MAX+COLLECTION_MAX;++i) {
        c=distinctive(i);
        assert(party_add(&p,&c)==(i<PARTY_MAX?1:2));
        same_creature(&c,i<PARTY_MAX?&p.members[i]:&p.collection[i-PARTY_MAX]);
    }
    assert(p.count==PARTY_MAX && p.stored==COLLECTION_MAX && !party_has_space(&p));
    same_creature(&p.members[0],&original);
    Party before=p;
    assert(!party_add(&p,&c) && !party_deposit(&p,0));
    assert(!party_exchange(&p,COLLECTION_MAX,0) && !party_exchange(&p,0,PARTY_MAX));
    assert(!party_exchange(&p,-1,0) && !party_exchange(&p,0,-1));
    assert(!party_set_lead(&p,-1) && !party_set_lead(&p,PARTY_MAX));
    for(int i=0;i<PARTY_MAX;++i) same_creature(&p.members[i],&before.members[i]);
    for(int i=0;i<COLLECTION_MAX;++i) same_creature(&p.collection[i],&before.collection[i]);
    assert(party_set_lead(&p,2));
    Creature outgoing=p.members[2],incoming=p.collection[COLLECTION_MAX-1];
    assert(party_exchange(&p,COLLECTION_MAX-1,2) && p.lead==2);
    same_creature(&p.members[2],&incoming);
    same_creature(&p.collection[COLLECTION_MAX-1],&outgoing);
    assert(p.count==PARTY_MAX && p.stored==COLLECTION_MAX);

    party_init(&p);
    for(int i=1;i<4;++i) { c=distinctive(i);assert(party_add(&p,&c)==1); }
    assert(party_set_lead(&p,3));
    Creature leader=p.members[3],deposited=p.members[1];
    assert(party_deposit(&p,1) && p.count==3 && p.stored==1 && p.lead==2);
    same_creature(&p.members[p.lead],&leader);
    same_creature(&p.collection[0],&deposited);
    assert(party_exchange(&p,0,p.count) && p.count==4 && p.stored==0 && p.lead==2);
    same_creature(&p.members[3],&deposited);
    assert(party_deposit(&p,p.lead) && p.lead==0);
    same_creature(&p.collection[0],&leader);
    assert(party_deposit(&p,1));
    Creature second=p.collection[1];
    assert(party_exchange(&p,0,p.count) && p.count==3 && p.stored==1);
    same_creature(&p.members[2],&leader);
    same_creature(&p.collection[0],&second); /* Withdrawal compacts collection. */

    p.members[1].hp=0;
    assert(!party_set_lead(&p,1) && p.lead==0);
    p.collection[0].hp=0;
    assert(party_exchange(&p,0,0));
    assert(p.lead==2 && p.members[0].hp==0); /* Select a healthy remaining member. */
    p.members[2].hp=0;
    assert(party_deposit(&p,2) && p.count==2 && p.lead>=0 && p.lead<p.count);
    before=p;
    party_restore(&p);
    for(int i=0;i<p.count;++i) {
        creature_restore(&before.members[i]);
        same_creature(&p.members[i],&before.members[i]);
    }
    for(int i=0;i<p.stored;++i) {
        creature_restore(&before.collection[i]);
        same_creature(&p.collection[i],&before.collection[i]);
    }
    assert(party_deposit(&p,0) && p.count==1 && !party_deposit(&p,0));
    assert(!party_has_space(NULL) && !party_add(NULL,&c));
    assert(!party_set_lead(NULL,0) && !party_exchange(NULL,0,0) && !party_deposit(NULL,0));
    party_init(NULL);party_restore(NULL);
}

static void capture_checks(void)
{
    assert(capture_rarity(SPECIES_MOSSLET)==0 && capture_rarity(SPECIES_FLINTLING)==0);
    assert(capture_rarity(SPECIES_CINDLET)==1 && capture_rarity(SPECIES_TWIGLINT)==1);
    assert(capture_rarity(SPECIES_DUSKWISP)==1);
    const int rare[]={SPECIES_EMBERLYN,SPECIES_MOSSHORN,SPECIES_FLINTAUR,SPECIES_GLOWMOTH,SPECIES_ECHOCRAG};
    for(unsigned int i=0;i<sizeof(rare)/sizeof(rare[0]);++i) assert(capture_rarity((SpeciesId)rare[i])==2);
    Creature c;creature_create(&c,SPECIES_MOSSLET,5);
    assert(capture_chance(&c,1)==35);
    c.species=SPECIES_TWIGLINT;assert(capture_chance(&c,1)==23);
    c.species=SPECIES_ECHOCRAG;assert(capture_chance(&c,1)==11);
    for(int id=0;id<SPECIES_COUNT;++id) {
        creature_create(&c,id,10);
        int last=0;
        for(c.hp=c.max_hp;c.hp>0;--c.hp) {
            int chance=capture_chance(&c,1);
            assert(chance>=last && chance>=5 && chance<=95);last=chance;
            for(int strength=1;strength<=5;++strength) {
                int probability=capture_chance(&c,strength),successes=0;
                assert(probability>=chance && probability<=95);chance=probability;
                for(unsigned int roll=0;roll<100;++roll) successes+=capture_attempt(&c,strength,roll);
                assert(successes==probability);
                assert(capture_attempt(&c,strength,(unsigned int)probability-1));
                assert(!capture_attempt(&c,strength,(unsigned int)probability));
            }
        }
        assert(capture_chance(&c,5)==0 && !capture_attempt(&c,5,0));
    }
    creature_create(&c,SPECIES_MOSSLET,5);c.hp=1;
    assert(capture_chance(&c,INT_MAX)==95);
    assert(capture_chance(&c,INT_MIN)==capture_chance(&c,1));
    assert(!capture_attempt(&c,5,100) && !capture_attempt(&c,5,UINT_MAX));
    c.max_hp=INT_MAX;
    assert(capture_chance(&c,1)==84); /* Health arithmetic cannot overflow. */
    c.hp=c.max_hp;assert(capture_chance(&c,1)==35);
    c.max_hp=0;assert(!capture_chance(&c,1));
    c.max_hp=50;c.hp=51;assert(!capture_chance(&c,1));
    c.hp=-1;assert(!capture_chance(&c,1));
    c.hp=25;c.species=SPECIES_COUNT;assert(!capture_chance(&c,1));
    c.species=(SpeciesId)-1;assert(!capture_chance(&c,1));
    assert(!capture_chance(NULL,1) && !capture_attempt(NULL,1,0));
}

int main(void)
{
    party_checks();capture_checks();
    puts("PASS: party/storage limits, lossless transfers, lead selection, restoration, capture rarity/health/strength odds and roll boundaries");
    return 0;
}
````

## tests/polish_test.c

````text
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "audio_synth.h"
#include "player_menu.h"
#include "text.h"

static int bounds, left, top, right, bottom, rectangles;
void graphics_rectangle(int x,int y,int width,int height,unsigned int color)
{
    (void)color;
    assert(width>0 && height>0);
    if (bounds) assert(x>=left && y>=top && x+width<=right && y+height<=bottom);
    ++rectangles;
}
static void le32(FILE *file,unsigned int value)
{
    for(int i=0;i<4;++i) fputc((value>>(i*8))&255,file);
}
static void audio_checks(void)
{
    AudioSynth synth;
    int16_t frames[1024*2];
    const char *const names[]={"hearthlight","fernwind","hollow-echo","loom-duel"};
    for(int scene=0;scene<MUSIC_COUNT;++scene) {
        audio_synth_init(&synth);audio_synth_scene(&synth,(MusicScene)scene);
        char path[120];snprintf(path,sizeof(path),"previews/%s.wav",names[scene]);
        FILE *f=fopen(path,"wb");assert(f);
        unsigned int remaining=AUDIO_RATE*8, samples=0, nonzero=0;
        fputs("RIFF",f);le32(f,36+remaining*4);fputs("WAVEfmt ",f);le32(f,16);
        fputc(1,f);fputc(0,f);fputc(2,f);fputc(0,f);
        le32(f,AUDIO_RATE);le32(f,AUDIO_RATE*4);
        fputc(4,f);fputc(0,f);fputc(16,f);fputc(0,f);
        fputs("data",f);le32(f,remaining*4);
        while(remaining) {
            unsigned int count=remaining>1024?1024:remaining;
            audio_synth_render(&synth,frames,count,1,1);
            for(unsigned int i=0;i<count*2;++i) {
                assert(abs(frames[i])<6000);
                if(frames[i]) ++nonzero;
                unsigned int value=(uint16_t)frames[i];
                fputc(value&255,f);fputc(value>>8,f);
            }
            remaining-=count;samples+=count;
        }
        assert(samples==AUDIO_RATE*8 && nonzero>AUDIO_RATE);
        fclose(f);
        audio_synth_render(&synth,frames,1024,0,0);
        for(int i=512;i<2048;++i) assert(frames[i]==0);
    }
    for(int effect=SOUND_CURSOR;effect<SOUND_COUNT;++effect) {
        audio_synth_init(&synth);audio_synth_effect(&synth,(Sound)effect);
        int nonzero=0;
        for(int block=0;block<40;++block) {
            audio_synth_render(&synth,frames,1024,0,1);
            for(int i=0;i<2048;++i) { assert(abs(frames[i])<6000);nonzero+=frames[i]!=0; }
        }
        assert(nonzero && synth.effect==SOUND_NONE);
    }
    /* Callback buffer size must not alter the generated stream. */
    AudioSynth a,b;int16_t whole[4096],pieces[4096];
    audio_synth_init(&a);audio_synth_init(&b);
    audio_synth_effect(&a,SOUND_HEAL);audio_synth_effect(&b,SOUND_HEAL);
    audio_synth_render(&a,whole,2048,1,1);
    audio_synth_render(&b,pieces,777,1,1);
    audio_synth_render(&b,pieces+1554,1271,1,1);
    assert(!memcmp(whole,pieces,sizeof(whole)));
}
int main(void)
{
    Party party;Inventory bag;PlayerMenu m;GameOptions options={1,1,1};
    party_init(&party);inventory_init(&bag);player_menu_open(&m);
    assert(player_menu_update(&m,&party,&bag,&options,&(Input){.confirm=1})==MENU_PARTY);
    m.cursor=2;
    assert(player_menu_update(&m,&party,&bag,&options,&(Input){.confirm=1})==MENU_COLLECTION);
    m.cursor=1;
    player_menu_update(&m,&party,&bag,&options,&(Input){.confirm=1});
    assert(m.page==MENU_BAG);
    player_menu_update(&m,&party,&bag,&options,&(Input){.confirm=1});
    assert(bag.quantities[0]==3); /* Full HP: no item consumed. */
    party.members[0].hp-=5;
    player_menu_update(&m,&party,&bag,&options,&(Input){.confirm=1});
    assert(bag.quantities[0]==2 && party.members[0].hp==party.members[0].max_hp);
    party.members[0].hp=0;
    player_menu_update(&m,&party,&bag,&options,&(Input){.confirm=1});
    assert(bag.quantities[0]==2 && party.members[0].hp==0);
    player_menu_update(&m,&party,&bag,&options,&(Input){.cancel=1});
    m.cursor=5;player_menu_update(&m,&party,&bag,&options,&(Input){.confirm=1});
    for(int i=0;i<3;++i) { m.cursor=i;player_menu_update(&m,&party,&bag,&options,&(Input){.confirm=1}); }
    assert(!options.music && !options.effects && !options.motion);
    assert(player_menu_update(&m,&party,&bag,&options,&(Input){.menu=INPUT_MENU_SAVE})==MENU_SAVE);
    assert(player_menu_update(&m,&party,&bag,&options,&(Input){.menu=INPUT_MENU_LOAD})==MENU_LOAD);
    assert(player_menu_update(&m,&party,&bag,&options,&(Input){.menu=INPUT_MENU_OPEN})==MENU_CLOSE);
    left=0;top=0;right=480;bottom=272;bounds=1;
    for(int page=MENU_HOME;page<=MENU_OPTIONS;++page) {
        player_menu_open(&m);m.page=(PlayerMenuPage)page;
        player_menu_draw(&m,&party,&bag,&options,4);
    }
    left=18;top=191;right=462;bottom=244;
    text_box(18,191,444,53,"YOUR PROGRESS IS SAFE ON THE MEMORY STICK.",0xffffffff);
    text_box(18,191,444,53,"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA",0xffffffff);
    text_box(18,191,444,53,"ONE\nTWO\nTHREE\nFOUR\nFIVE",0xffffffff);
    assert(text_wrap(18,191,444,53,"",0xffffffff,2)==0);
    int before=rectangles;
    assert(text_wrap(18,191,444,0,"A SHORT MESSAGE",0xffffffff,2)==1);
    assert(rectangles==before);
    audio_checks();
    puts("PASS: player menu, item validation, independent options, bounded text, original PCM tracks/effects and mute");
    return 0;
}
````

## tests/preview.py

````text
"""Convert the host renderer's test frames to PNG using only the standard library.

Run after world_systems_test. These are software previews, not PSP captures.
"""
import pathlib
import struct
import zlib


def chunk(kind, payload):
    return (struct.pack('>I', len(payload)) + kind + payload
            + struct.pack('>I', zlib.crc32(kind + payload)))


output = pathlib.Path(__file__).resolve().parent.parent / 'previews'
output.mkdir(exist_ok=True)
for name in ('dialogue', 'encounter', 'battle-menu', 'battle-moves', 'learn-move', 'evolution', 'partner',
             'capture', 'captured', 'party', 'collection', 'collection-swap', 'battle-switch',
             'collection-empty', 'collection-full', 'items', 'shop',
             'player-menu', 'field-items', 'options', 'marsh', 'lantern-rest',
             'sunfinch', 'reedskip', 'battle-impact', 'saved-dialogue'):
    data = (output / (name + '.ppm')).read_bytes()
    magic, dimensions, maximum, pixels = data.split(b'\n', 3)
    assert magic == b'P6' and maximum == b'255'
    width, height = map(int, dimensions.split())
    assert len(pixels) == width * height * 3
    scanlines = b''.join(b'\0' + pixels[y*width*3:(y+1)*width*3]
                         for y in range(height))
    png = (b'\x89PNG\r\n\x1a\n'
           + chunk(b'IHDR', struct.pack('>IIBBBBB', width, height, 8, 2, 0, 0, 0))
           + chunk(b'IDAT', zlib.compress(scanlines)) + chunk(b'IEND', b''))
    (output / (name + '.png')).write_bytes(png)
````

## tests/run.sh

````text
#!/bin/sh
set -eu
# Run from the psp directory. Host checks use PSP API stubs, not an emulator.
mkdir -p previews
cc -std=c99 -Wall -Wextra -Werror -fsanitize=address,undefined -Itests/host -Iinclude \
    tests/save_data_test.c src/save_data.c -o previews/save-data-test
previews/save-data-test
cc -std=c99 -Wall -Wextra -Werror -fsanitize=address,undefined -Iinclude \
    tests/overworld_test.c src/map.c src/player.c src/camera.c -lm -o previews/overworld-test
previews/overworld-test
cc -std=c99 -Wall -Wextra -Werror -fsanitize=address,undefined -Itests/host -Iinclude \
    tests/world_systems_test.c src/game.c src/map.c src/player.c src/camera.c \
    src/npc.c src/dialogue.c src/encounter.c src/world_draw.c src/text.c \
    src/attacks.c src/battle.c src/battle_draw.c src/creature.c \
    src/party.c src/capture.c src/party_menu.c src/inventory.c src/player_menu.c \
    tests/host/save_data_stub.c tests/host/audio_stub.c -o previews/world-test
previews/world-test
cc -std=c99 -Wall -Wextra -Werror -fsanitize=address,undefined -Iinclude \
    tests/battle_test.c src/battle.c src/attacks.c src/creature.c src/party.c src/capture.c src/inventory.c -o previews/battle-test
previews/battle-test
cc -std=c99 -Wall -Wextra -Werror -fsanitize=address,undefined -Iinclude \
    tests/creature_test.c src/creature.c src/battle.c src/attacks.c src/party.c src/capture.c src/inventory.c -o previews/creature-test
previews/creature-test
cc -std=c99 -Wall -Wextra -Werror -fsanitize=address,undefined -Iinclude \
    tests/party_capture_test.c src/party.c src/capture.c src/creature.c src/attacks.c -o previews/party-capture-test
previews/party-capture-test
cc -std=c99 -Wall -Wextra -Werror -fsanitize=address,undefined -Iinclude \
    tests/battle_party_test.c src/battle.c src/attacks.c src/creature.c src/party.c src/capture.c src/inventory.c -o previews/battle-party-test
previews/battle-party-test
cc -std=c99 -Wall -Wextra -Werror -fsanitize=address,undefined -Iinclude \
    tests/inventory_test.c src/inventory.c src/creature.c src/attacks.c -o previews/inventory-test
previews/inventory-test
cc -std=c99 -Wall -Wextra -Werror -fsanitize=address,undefined -Iinclude \
    tests/polish_test.c src/player_menu.c src/audio_synth.c src/text.c src/map.c \
    src/inventory.c src/party.c src/creature.c src/attacks.c -o previews/polish-test
previews/polish-test
python3 tests/preview.py
````

## tests/save_data_test.c

````text
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <psputility_savedata.h>
#include "save_data.h"

static SceUtilitySavedataParam *active;
static int state, shutdowns, updates, init_result, shutdown_result;

int sceUtilitySavedataInitStart(SceUtilitySavedataParam *params)
{
    active=params;
    state=PSP_UTILITY_DIALOG_INIT;
    shutdowns=updates=0;
    return init_result;
}
int sceUtilitySavedataGetStatus(void) { return state; }
int sceUtilitySavedataShutdownStart(void)
{
    ++shutdowns;
    return shutdown_result;
}
void sceUtilitySavedataUpdate(int step) { assert(step==1); ++updates; }

static void tick(int next)
{
    state=next;
    save_data_update();
}
static void complete(int result, int see_finished)
{
    active->base.result=result;
    tick(PSP_UTILITY_DIALOG_QUIT);
    assert(save_data_status()==SAVE_STATUS_BUSY && shutdowns==1);
    tick(PSP_UTILITY_DIALOG_QUIT);
    assert(shutdowns==1);
    if (see_finished) {
        tick(PSP_UTILITY_DIALOG_FINISHED);
        assert(save_data_status()==SAVE_STATUS_BUSY && shutdowns==1);
    }
    tick(PSP_UTILITY_DIALOG_NONE);
}

int main(void)
{
    /* Phase 8 format/IDs remain unchanged; Phase 9 only appends content IDs. */
    assert(sizeof(SavePayload)==1960 && SAVE_DATA_VERSION==1);
    assert(SPECIES_CINDLET==0 && SPECIES_ECHOCRAG==9);
    SavePayload saved={.magic=SAVE_DATA_MAGIC, .version=SAVE_DATA_VERSION,
                       .map_id=1, .tile_x=7, .embermarks=123};
    SavePayload loaded;
    assert(save_data_status()==SAVE_STATUS_IDLE);
    assert(save_data_begin_save(NULL)<0);
    for (int finished=0;finished<=1;++finished) {
        assert(save_data_begin_save(&saved)==0);
        assert(active->mode==PSP_UTILITY_SAVEDATA_LISTSAVE);
        assert(!strcmp(active->gameName,"EMBRWAKE") && !strcmp(active->saveName,"0000"));
        tick(PSP_UTILITY_DIALOG_NONE); /* Initial idle poll is not completion. */
        assert(save_data_status()==SAVE_STATUS_BUSY);
        tick(PSP_UTILITY_DIALOG_VISIBLE);
        assert(updates==1);
        /* Rejected overlapping operations must not alter the in-flight data. */
        assert(save_data_begin_load()<0);
        assert(save_data_begin_save(&(SavePayload){0})<0);
        assert(memcmp(active->dataBuf,&saved,sizeof(saved))==0);
        complete(0,finished);
        assert(save_data_status()==SAVE_STATUS_SUCCEEDED);
        assert(!save_data_take_loaded(&loaded));
    }
    /* Canceled and failed operations must never report success. */
    const int failures[]={1, -1, (int)0x80110383u};
    for (unsigned int i=0;i<sizeof(failures)/sizeof(failures[0]);++i) {
        assert(save_data_begin_save(&saved)==0);
        complete(failures[i],i%2);
        assert(save_data_status()==SAVE_STATUS_FAILED);
    }
    /* The final result can be written during shutdown, before NONE is seen. */
    assert(save_data_begin_save(&saved)==0);
    tick(PSP_UTILITY_DIALOG_QUIT);
    active->base.result=0;
    tick(PSP_UTILITY_DIALOG_NONE);
    assert(save_data_status()==SAVE_STATUS_SUCCEEDED);

    assert(save_data_begin_load()==0);
    assert(active->mode==PSP_UTILITY_SAVEDATA_LISTLOAD && active->dataSize==0);
    memcpy(active->dataBuf,&saved,sizeof(saved));
    active->dataSize=sizeof(saved);
    complete(0,0);
    assert(save_data_status()==SAVE_STATUS_SUCCEEDED);
    assert(save_data_take_loaded(&loaded) && memcmp(&saved,&loaded,sizeof(saved))==0);
    assert(!save_data_take_loaded(&loaded));
    /* Reject a truncated file even when its magic and version are intact. */
    assert(save_data_begin_load()==0);
    memcpy(active->dataBuf,&saved,sizeof(saved));
    active->dataSize=sizeof(saved)-1;
    complete(0,1);
    assert(save_data_status()==SAVE_STATUS_FAILED && !save_data_take_loaded(&loaded));
    assert(save_data_begin_load()==0);
    memcpy(active->dataBuf,&saved,sizeof(saved));
    ((SavePayload *)active->dataBuf)->magic=0;
    active->dataSize=sizeof(saved);
    complete(0,0);
    assert(save_data_status()==SAVE_STATUS_FAILED && !save_data_take_loaded(&loaded));
    /* Retry shutdown if the utility is temporarily not ready. */
    assert(save_data_begin_save(&saved)==0);
    shutdown_result=-1;
    tick(PSP_UTILITY_DIALOG_QUIT);
    assert(save_data_status()==SAVE_STATUS_BUSY);
    shutdown_result=0;
    active->base.result=0;
    tick(PSP_UTILITY_DIALOG_QUIT);
    assert(shutdowns==2);
    tick(PSP_UTILITY_DIALOG_NONE);
    assert(save_data_status()==SAVE_STATUS_SUCCEEDED);
    init_result=-1;
    assert(save_data_begin_load()<0 && save_data_status()==SAVE_STATUS_FAILED);
    puts("PASS: savedata completion with/without FINISHED, errors, cancellation, load validation, busy guard");
    return 0;
}
````

## tests/world_systems_test.c

````text
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
    assert(portals==10);
    Game g;
    place(&g,MAP_CLEARING,5,10);
    update(&g,(Input){0,-1,0,0,0,0},10);
    assert(g.map_id==MAP_LODGE);
    update(&g,(Input){0,1,0,0,0,0},10);
    assert(g.map_id==MAP_CLEARING && g.player.tile_y==10);
    place(&g,MAP_CLEARING,37,11);
    update(&g,(Input){1,0,0,0,0,0},10);
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
        update(&g,(Input){g.player.tile_x>=15?-1:1,0,0,0,0,0},1);
    assert(g.in_battle);
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
    battle_begin(&g.battle,&g.party.members[g.party.lead],SPECIES_ECHOCRAG,7,99);g.in_battle=1;
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
    battle_begin(&g.battle,&g.party.members[g.party.lead],SPECIES_MOSSLET,3,42);g.in_battle=1;
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
    battle_begin(&g.battle,&g.party.members[g.party.lead],SPECIES_MOSSLET,3,42);g.in_battle=1;
    g.battle.enemy.hp=1;g.battle.ally.speed=999;g.battle.ally.moves[0]=MOVE_NUDGE;
    update(&g,(Input){0,0,1,0,0,0},6);
    assert(g.battle.ally.species==SPECIES_EMBERLYN && strstr(g.battle.message,"EVOLUTION"));
    render(&g,"previews/evolution.ppm");
    for(int i=0;i<25 && g.in_battle;++i) {
        if(g.battle.phase==BATTLE_LEARN) update(&g,(Input){0,0,0,1,0,0},1);
        else update(&g,(Input){0,0,1,0,0,0},1);
    }
    assert(!g.in_battle && g.party.members[g.party.lead].species==SPECIES_EMBERLYN);
    saved_xp=g.party.members[g.party.lead].experience;
    g.player.tile_x=g.player.target_x=5;g.player.tile_y=g.player.target_y=10;
    g.player.x=160;g.player.y=320;g.player.moving=0;
    update(&g,(Input){0,-1,0,0,0,0},10);
    assert(g.map_id==MAP_LODGE && g.party.members[g.party.lead].species==SPECIES_EMBERLYN && g.party.members[g.party.lead].experience==saved_xp);
    update(&g,(Input){0,0,0,0,1,0},1);
    assert(g.dialogue.active);fits(g.dialogue.pages[0]);fits(g.dialogue.pages[1]);
    render(&g,"previews/partner.ppm");
    battle_begin(&g.battle,&g.party.members[g.party.lead],SPECIES_MOSSLET,3,42);
    assert(g.battle.ally.species==SPECIES_EMBERLYN && strstr(g.battle.message,"EMBERLYN IS READY"));
    /* Capture into a full party flows through battle into collection management. */
    place(&g,MAP_CLEARING,5,11);
    Creature extra;
    for(int i=0;i<3;++i) {
        creature_create(&extra,SPECIES_MOSSLET+i,4+i);
        assert(party_add(&g.party,&extra)==1);
    }
    battle_begin_party(&g.battle,&g.party,SPECIES_MOSSLET,3,1);g.in_battle=1;
    update(&g,(Input){.confirm=1},1);
    g.battle.cursor=1;update(&g,(Input){.confirm=1},1);
    assert(g.battle.phase==BATTLE_CAPTURE);
    render(&g,"previews/capture.ppm");
    g.battle.enemy.hp=1;
    update(&g,(Input){.confirm=1},1);
    assert(g.battle.result==BATTLE_CAUGHT);
    render(&g,"previews/captured.ppm");
    update(&g,(Input){.confirm=1},1);
    assert(!g.in_battle && g.party.count==4 && g.party.stored==1);
    assert(g.party.collection[0].species==SPECIES_MOSSLET && g.party.collection[0].level==3);
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
    assert(g.party.members[0].species==SPECIES_MOSSLET && g.party.collection[0].species==SPECIES_CINDLET);
    update(&g,(Input){.horizontal=-1},1);
    update(&g,(Input){.vertical=1},1);
    update(&g,(Input){.confirm=1},2);
    assert(g.party.lead==1);
    update(&g,(Input){.menu=1},1);
    assert(!g.roster_open);
    battle_begin_party(&g.battle,&g.party,SPECIES_FLINTLING,5,2);g.in_battle=1;
    assert(g.battle.active==1 && g.battle.ally.species==g.party.members[1].species);
    update(&g,(Input){.confirm=1},1);
    g.battle.cursor=2;update(&g,(Input){.confirm=1},1);
    render(&g,"previews/battle-switch.ppm");
    g.battle.switch_cursor=0;update(&g,(Input){.confirm=1},3);
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
    creature_create(&extra,SPECIES_FLINTLING,5);party_add(&g.party,&extra);
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
    int species_seen[12]={0};
    encounter_init(&e,456);
    for(int i=0;i<20000;++i) if(encounter_step(&e,3,&result)) {
        assert(result.level>=5 && result.level<=8);
        assert(result.species==SPECIES_REEDSKIP || result.species==SPECIES_GLOWMOTH || result.species==SPECIES_SUNFINCH);
        ++species_seen[result.species];
    }
    assert(species_seen[SPECIES_REEDSKIP]>0 && species_seen[SPECIES_SUNFINCH]>0);
    battle_begin_party(&g.battle,&g.party,SPECIES_SUNFINCH,7,42);g.in_battle=1;
    render(&g,"previews/sunfinch.ppm");
    battle_begin_party(&g.battle,&g.party,SPECIES_REEDSKIP,6,42);
    render(&g,"previews/reedskip.ppm");
    g.battle.enemy.hp-=10;g.battle.hit_time=.2f;g.battle.hit_side=1;
    battle_animate(&g.battle,.05f,1);
    assert(g.battle.enemy_hp_shown>g.battle.enemy.hp && g.battle.enemy_hp_shown<g.battle.enemy.max_hp);
    render(&g,"previews/battle-impact.ppm");
    battle_animate(&g.battle,.05f,0);
    assert(g.battle.hit_time==0 && g.battle.enemy_hp_shown==g.battle.enemy.hp);
    g.in_battle=0;
    dialogue_open(&g.dialogue,"SESSION SAVED","YOUR PROGRESS IS SAFE ON THE MEMORY STICK.","KEEP MOVING.");
    render(&g,"previews/saved-dialogue.ppm");
    puts("PASS: world systems, progression, capture retention, party menu, inventory/shop/healing, storage swaps/scrolling, battle lead, drawing budget");
    return 0;
}
````
