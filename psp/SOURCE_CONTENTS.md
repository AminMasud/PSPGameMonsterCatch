# Complete Phase 5 source contents

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

## Makefile

````text
TARGET = emberwake
OBJS = src/main.o src/game.o src/input.o src/graphics.o src/map.o src/player.o src/camera.o src/world_draw.o src/npc.o src/dialogue.o src/encounter.o src/text.o src/attacks.o src/battle.o src/battle_draw.o src/creature.o

INCDIR = include
CFLAGS = -O2 -G0 -std=c99 -Wall -Wextra -Werror -MMD -MP
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)
LIBDIR =
LDFLAGS =
LIBS = -lpspgu -lpspge -lpspdisplay -lpspctrl

BUILD_PRX = 1
PSP_FW_VERSION = 660
EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = Emberwake - Phase 5

PSPSDK = $(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak

src/main.o: include/game.h include/graphics.h include/input.h
src/game.o: include/game.h include/graphics.h include/input.h
src/input.o: include/input.h
src/graphics.o: include/graphics.h

src/map.o: assets/maps/clearing.inc
PARAM.SFO: Makefile
-include $(OBJS:.o=.d)
````

## README.md

````text
# Emberwake — Phase 5

Original PSP homebrew RPG prototype in C / PSPSDK. The creatures are called
**Veylings**. Phase 5 adds species data, experience, levels, learned attacks, and evolution.
The existing world, dialogue, and turn-based battles remain available.

## Play this build

The verified build is **EBOOT-PHASE5.PBP**. Use this explicitly named artifact;
older EBOOT files are retained and are not the Phase 5 build.

Copy EBOOT-PHASE5.PBP to the Memory Stick, naming the destination:

    ms0:/PSP/GAME/EMBERWAKE/EBOOT.PBP

Launch **Emberwake - Phase 5** from Game > Memory Stick.

- D-pad: smooth four-direction tile movement; horizontal wins when two directions
  are held. Release finishes the current tile.
- X: speak to a stationary NPC in the tile you face; advance/close dialogue.
- Circle: close dialogue immediately.
- Select: view partner stats, experience, and species description while stationary.
- HOME: system exit menu, as in earlier phases.

X and Circle use new-press detection, so holding a button does not skip pages.
Dialogue pauses player movement and NPC patrols. Moving NPCs must finish their
current step before you can speak to them.

## World and test route

| Area | Contents | Connections |
| --- | --- | --- |
| Hearth Clearing | Original map, lodge exterior, Mira, patrolling Orin | Lodge door northwest of spawn; woods at east end of horizontal path |
| Fernveil Woods | Sen, dark tall grass, northern cave entrance | West path returns to clearing; follow right-side path north into cave |
| Wayfarer Lodge | Indoor floor, furniture, Tavi | South door returns to clearing |
| Hollowstone Cave | Rock formations, rough encounter floor, Nel | Southwest doorway returns to woods |

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
There is no saved world state yet.

## Encounters

Only completed steps roll for encounters. Paths, flowers, ordinary grass, and
lodge floors are safe. An encounter has an 18% chance on eligible terrain after
a four-step grace period following map entry or the previous encounter.
The grace counter counts completed steps on any terrain.

| Terrain | Common 60% | Uncommon 30% | Rare 10% |
| --- | --- | --- | --- |
| Woods tall grass | Mosslet, level 2-4 | Twiglint, level 3-5 | Glowmoth, level 4-6 |
| Cave rough floor | Flintling, level 3-5 | Duskwisp, level 4-6 | Echocrag, level 5-7 |

Encounters reference stable species IDs in the creature database. Wild creatures
use species-specific stats and up to four attacks unlocked at their level.
Capture, team switching, inventory, and saves remain future work.

## Battle prototype

Your partner starts as **Cindlet**, a level 5 Ember Veyling with four attacks. Wild
opponents come from the existing woods/cave tables. Battles display both
creatures, names, levels, elements, numeric HP, and HP bars.

Controls:
- Tap D-pad up/down to select. Left/right also moves one menu entry.
- X confirms an option or advances the current battle message.
- Circle returns from attack selection to the main battle menu.
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
escape allows exactly one enemy action. CAPTURE, CREATURES, and ITEMS show clear
not-yet-available messages and do not spend a turn.

**Temporary Phase 5 rule:** your partner's HP and attack uses refill after every
battle, including escape. Victory or escape returns to the same exploration
position. Defeat returns you to Hearth Clearing at the original starting tile.
All outcomes grant four safe steps before another encounter can roll. This rule
makes battle testing repeatable before later inventory/healing systems.
Experience, levels, evolution, and chosen attacks are retained through subsequent
battles and map transitions in this running session. Quitting or restarting resets
progress: Memory Stick saves are planned for Phase 8. Capture is still Phase 6.

Additional PSP checks:
1. Enter woods tall grass and try all four attacks across multiple battles.
2. Check that uses decrease, HP bars change, and each action requires X.
3. Cancel attack selection with Circle without spending a turn.
4. Try RUN; a failure should produce only one enemy response.
5. Win a battle and confirm you return to the same place with a fresh partner.
6. Lose against a stronger cave opponent and confirm return to the clearing.
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

The encounter tables keep their six wild species. Cindlet is the starter, and
three evolved forms complete the ten-species test database. Other species'
progression paths are implemented and tested but cannot yet be collected.

A victory awards enemy species experience-yield multiplied by enemy level.
Escaping or losing grants no XP. XP is awarded once per battle. The cumulative
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
partner's stats/total XP, followed by its species description. Progress survives
battles and changing maps within the session, not quitting the game.

Phase 5 PSP test:
1. Press Select while standing still; check Cindlet's level, XP, and description.
2. Win forest battles and verify total XP increases; escape once and confirm no XP.
3. Reach level 6, choose a move to replace with HEAT SPIRAL, and use it next battle.
4. Reach level 8 and check the Emberlyn name, new silhouette, and increased stats.
5. Try declining an offered attack without losing existing moves.
6. Enter/leave the lodge and check that level, species, XP, and attacks are retained.
7. Confirm HOME exit still works; relaunch intentionally starts a fresh session.

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
      include/
        attacks.h
        battle.h
        camera.h
        creature.h
        dialogue.h
        encounter.h
        game.h
        graphics.h
        input.h
        map.h
        npc.h
        player.h
        text.h
        world_draw.h
      src/
        main.c
        attacks.c
        battle.c
        battle_draw.c
        camera.c
        creature.c
        dialogue.c
        encounter.c
        game.c
        graphics.c
        input.c
        map.c
        npc.c
        player.c
        text.c
        world_draw.c
      tests/
        host/pspgu.h
        battle_test.c
        creature_test.c
        overworld_test.c
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
No additional PSPSDK libraries, engines, or external art files are required.

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
| _ | Safe floor | Yes |
| g | Woods encounter grass | Yes |
| c | Cave encounter floor | Yes |

Portal behavior is defined by map ID and coordinates in map.c's portals table,
not by the visual tile alone. Each portal declares an explicit walkable arrival
tile away from the return trigger. Unknown codes/out-of-bounds locations block
movement. All maps and graphics are embedded in the EBOOT.

attacks.c defines attack power, accuracy, type, and uses. battle.c owns battle
turn sequencing, results, escape logic, and damage. battle_draw.c owns the
battle screen and original placeholder creature graphics. creature.c owns the
immutable species database and reusable individual-creature data, stat calculation,
experience thresholds, evolution, and attack learning.

To add a map, add its rows, ID, dimensions, name, portal connections, and optional
NPC definitions / encounter terrain. Keep coordinates within the declared map
and extend tests for new dimensions if they exceed the current test grid.

## Build commands

This machine uses the existing Ubuntu/WSL PSPDEV environment.

From PowerShell, build the Phase 5 artifact:

    wsl -d Ubuntu -- bash -lc 'cd /mnt/c/Users/polo1/OneDrive/Documents/app/psp && make PSP_EBOOT=EBOOT-PHASE5.PBP EXTRA_TARGETS=EBOOT-PHASE5.PBP'

In a configured Linux/WSL shell, from the psp directory:

    make PSP_EBOOT=EBOOT-PHASE5.PBP EXTRA_TARGETS=EBOOT-PHASE5.PBP

For the conventional EBOOT.PBP output when that file is not open elsewhere:

    make

The user-mode PRX targets 6.60/6.61 custom firmware. The Makefile generates header
dependencies automatically and repackages the title when its settings change.

## Validation and rendering budget

From the psp directory in Linux/WSL:

    sh tests/run.sh

All four test executables use AddressSanitizer and UndefinedBehaviorSanitizer.
Checks include Phase 2 regression coverage plus portal reachability and all six
transitions, valid/non-trigger arrival tiles, NPC collisions in both directions,
patrol limits, dialogue pause/advance/cancel, no stationary encounters, weighted
table level ranges, encounter grace steps, text bounds, and drawing budget.
Battle checks additionally cover turn order, accuracy, elemental damage, spent
moves, fallback attacks, win/loss, no retaliation after knockout, menu controls,
escape success/failure, battle freezing the world, and defeat returning home.
Creature checks cover all ten species at levels 1-100, distinct known attacks,
XP boundaries, large awards, the level cap, evolution, nickname preservation,
replacement/decline controls, and rewards being applied only once. Integration
checks carry an evolved partner and chosen attacks through world transitions.
The host test renders the real draw functions into software pixel buffers;
tests/preview.py converts those buffers to PNG without third-party dependencies.

Software previews are saved in previews/dialogue.png, previews/encounter.png,
previews/battle-menu.png, previews/battle-moves.png, previews/learn-move.png,
previews/evolution.png, and previews/partner.png.
They are not emulator or hardware screenshots.

Only visible terrain is drawn (at most 160 tiles). The GU list reserves 1 MiB for
tile, actor, and bitmap text commands. The tested scenes stay below a conservative
6000-rectangle budget. Two color buffers still occupy 1,114,112 bytes of VRAM.
The main loop retains its 50 ms elapsed-time cap and HOME callback service.

Host regression/integration tests passed. The PSP compiler and linker passed
with -Wall -Wextra -Werror, and the Phase 5 PBP was packaged successfully.
Real PSP visuals, performance, and input/exit behavior still require your test.

Stop here before Phase 6.

## Official references

- [PSPDEV installation](https://pspdev.github.io/installation.html)
- [PSPSDK GU reference](https://pspdev.github.io/pspsdk/group__GU.html)
- [PSPSDK build rules](https://github.com/pspdev/pspsdk/blob/master/src/base/build.mak)
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
"#>=============================#",
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
"W_____________W",
"W_OOO_____OOO_W",
"W_____________W",
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

## include/battle.h

````text
#ifndef EMBERWAKE_BATTLE_H
#define EMBERWAKE_BATTLE_H
#include <stdint.h>
#include "input.h"
#include "attacks.h"
#include "creature.h"
#define BATTLE_MOVES 4
typedef Creature Battler;
typedef enum { BATTLE_MESSAGE, BATTLE_MENU, BATTLE_ATTACKS, BATTLE_LEARN, BATTLE_DONE } BattlePhase;
typedef enum { BATTLE_ONGOING, BATTLE_WIN, BATTLE_LOSS, BATTLE_ESCAPED } BattleResult;
typedef enum { AFTER_MENU, AFTER_TURN, AFTER_GROWTH, AFTER_DONE } BattleAfter;
typedef struct {
    Battler ally, enemy;
    BattlePhase phase;
    BattleResult result;
    BattleAfter after;
    uint32_t random;
    int cursor, move_cursor, previous_direction;
    int choices[2], order[2], turn_index;
    int escape_attempts;
    CreatureGrowth growth;
    int growth_stage, growth_move, learn_cursor, reward_given, experience_reward;
    char message[160];
} Battle;
void battler_starter(Battler *b);
void battler_restore(Battler *b);
void battle_begin(Battle *b,const Battler *ally,SpeciesId species,int level,uint32_t seed);
void battle_update(Battle *b,const Input *input);
int battle_damage(const Battler *attacker,const Battler *defender,const Attack *attack,int variation);
void battle_draw(const Battle *b);
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
    SPECIES_DUSKWISP, SPECIES_ECHOCRAG, SPECIES_COUNT
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
typedef struct {
    const Map *map;
    Player player;
    Camera camera;
    int map_id;
    Npcs npcs;
    Dialogue dialogue;
    Encounter encounter;
    Battler partner;
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
} Input;

int input_init(void);
void input_poll(Input *input);

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
enum { MAP_CLEARING, MAP_FOREST, MAP_LODGE, MAP_CAVE, MAP_COUNT };
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

## include/text.h

````text
#ifndef EMBERWAKE_TEXT_H
#define EMBERWAKE_TEXT_H
void text_draw(int x, int y, const char *text, unsigned int color, int scale);
#endif
````

## include/world_draw.h

````text
#ifndef EMBERWAKE_WORLD_DRAW_H
#define EMBERWAKE_WORLD_DRAW_H
#include "camera.h"
void world_draw(const Map *map, const Player *player, const Camera *camera);
void world_actor_draw(const Player *p, const Camera *camera, int npc);
#endif
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

## src/battle_draw.c

````text
#include <stdio.h>
#include <pspgu.h>
#include "battle.h"
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
    }
    if(!back) {
        graphics_rectangle(x+17,y+23,6,7,C(23,31,41));
        graphics_rectangle(x+38,y+23,6,7,C(23,31,41));
        graphics_rectangle(x+27,y+35,8,3,C(58,54,57));
    } else {
        graphics_rectangle(x+21,y+20,23,5,C(255,179,94));
    }
}
static void status(const Battler *unit,int x,int y,int width)
{
    char line[64];
    graphics_rectangle(x,y,width,58,C(25,35,45));
    snprintf(line,sizeof(line),"%s  LV %d",creature_name(unit),unit->level);
    text_draw(x+8,y+7,line,C(241,232,207),1);
    text_draw(x+8,y+20,element_name(unit->element),C(174,192,188),1);
    int bar_width=width-16;
    graphics_rectangle(x+8,y+32,bar_width,6,C(66,72,78));
    int filled=bar_width*unit->hp/unit->max_hp;
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
    creature_draw(325,38,&b->enemy,0);
    creature_draw(65,104,&b->ally,1);
    status(&b->enemy,18,24,202);
    status(&b->ally,253,109,210);
    graphics_rectangle(6,176,468,90,C(177,144,94));
    graphics_rectangle(8,178,464,86,C(21,30,38));
    if(b->phase==BATTLE_LEARN) {
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
        text_draw(18,188,b->message,C(236,236,218),2);
        text_draw(18,251,"X CONTINUE",C(167,194,180),1);
    } else if(b->phase==BATTLE_MENU) {
        text_draw(18,191,"CHOOSE YOUR NEXT MOVE.",C(236,236,218),1);
        text_draw(18,210,"D-PAD SELECT   X CONFIRM",C(167,194,180),1);
        char growth[64];
        snprintf(growth,sizeof(growth),"%s - NEXT LEVEL IN %d XP",creature_name(&b->ally),creature_xp_remaining(&b->ally));
        text_draw(18,230,growth,C(233,173,115),1);
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
    *b=(Battle){0};
    b->ally=*ally; b->random=seed?seed:0x3291u;
    creature_create(&b->enemy,species,level);
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
        int effect=attack_effectiveness(move->element,d->element);
        snprintf(b->message,sizeof(b->message),"%s USED %s.\n%d DAMAGE. %s",creature_name(a),move->name,damage,
                 effect==4?"STRONG MATCH.":effect==1?"RESISTED.":"");
    }
    b->phase=BATTLE_MESSAGE; b->after=AFTER_TURN;
    if(d->hp==0) {
        b->result=side==0?BATTLE_WIN:BATTLE_LOSS;
        /* Damage is shown first; the next confirmation shows the outcome. */
    }
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
    message(b,"PARTNER RESTORED AFTER BATTLE.\nX RETURN TO EXPLORING",AFTER_DONE);
}
void battle_update(Battle *b,const Input *input)
{
    int nav=navigation(b,input);
    if(b->phase==BATTLE_DONE) return;
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
        if(b->after==AFTER_DONE) { b->phase=BATTLE_DONE; return; }
        if(b->after==AFTER_MENU) { b->phase=BATTLE_MENU; return; }
        if(b->after==AFTER_GROWTH) { growth_next(b);return; }
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
            message(b,"YOUR PARTNER NEEDS A REST.\nRETURNING TO HEARTH CLEARING.\nPARTNER RESTORED AFTER BATTLE.",AFTER_DONE);
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
        message(b,"CAPTURE IS NOT AVAILABLE YET.\nIT ARRIVES IN PHASE 6.",AFTER_MENU); break;
    case 2:
        message(b,"YOU HAVE ONE PARTNER.\nTEAM SWITCHING ARRIVES IN PHASE 6.",AFTER_MENU); break;
    case 3:
        message(b,"YOUR ITEM BAG IS EMPTY.\nITEMS ARRIVE IN PHASE 7.",AFTER_MENU); break;
    default:
        ++b->escape_attempts;
        if(b->escape_attempts>=3 || random_next(b)%100<70) {
            b->result=BATTLE_ESCAPED;
            message(b,"YOU GOT AWAY SAFELY.\nPARTNER RESTORED AFTER BATTLE.",AFTER_DONE);
        } else {
            b->choices[1]=choose_enemy(b);
            /* Only the enemy acts after a failed escape. */
            b->order[1]=1; b->turn_index=1;
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
  {L(1,MOVE_NUDGE),L(2,MOVE_PEBBLE),L(4,MOVE_LUNGE),L(6,MOVE_FAULT)},4}
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
    if (area < 1 || area > 2 || next(e)%100 >= 18) return 0;
    const Entry *table = area == 1 ? forest : cave;
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
            g->partner=g->battle.ally; /* Preserve species, XP, levels, and chosen moves. */
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
    if(input->details && !g->player.moving) {
        char summary[160];
        snprintf(summary,sizeof(summary),"LEVEL %d %s - HP %d\nATK %d DEF %d SPEED %d\nXP %d - NEXT IN %d",g->partner.level,
                 element_name(g->partner.element),g->partner.max_hp,g->partner.attack,
                 g->partner.defense,g->partner.speed,g->partner.experience,creature_xp_remaining(&g->partner));
        dialogue_open(&g->dialogue,creature_name(&g->partner),summary,species_get(g->partner.species)->description);
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
            battle_begin(&g->battle,&g->partner,result.species,result.level,g->encounter.random);
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
    text_draw(290,4,"X TALK  SELECT PARTNER",GU_RGBA(210,221,211,255),1);
    if (g->dialogue.active) {
        graphics_rectangle(6,167,468,99,GU_RGBA(184,150,96,255));
        graphics_rectangle(8,169,464,95,GU_RGBA(21,30,36,255));
        text_draw(18,176,g->dialogue.title,GU_RGBA(242,198,117,255),1);
        text_draw(18,191,g->dialogue.pages[g->dialogue.page],GU_RGBA(235,237,220,255),2);
        text_draw(18,251,g->dialogue.page+1 < g->dialogue.count ? "X NEXT   O CLOSE" : "X CLOSE   O CLOSE",GU_RGBA(163,189,178,255),1);
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
    input->confirm = input->cancel = input->details = 0;
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
}
````

## src/main.c

````text
#include <pspkernel.h>
#include "game.h"
#include "graphics.h"
#include "input.h"

/* User-mode PRX: no kernel access or extra PSP-3000 RAM required. */
PSP_MODULE_INFO("Emberwake", 0, 0, 1);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER);
PSP_HEAP_SIZE_KB(1024);

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
static const Portal portals[] = {
    {MAP_CLEARING,38,11,MAP_FOREST,2,11},
    {MAP_FOREST,1,11,MAP_CLEARING,37,11},
    {MAP_CLEARING,5,9,MAP_LODGE,7,6},
    {MAP_LODGE,7,7,MAP_CLEARING,5,10},
    {MAP_FOREST,28,5,MAP_CAVE,2,10},
    {MAP_CAVE,2,11,MAP_FOREST,28,6}
};
const Map *map_get(int id)
{
    const Map *const maps[] = {&clearing,&forest,&lodge,&cave};
    return id >= 0 && id < MAP_COUNT ? maps[id] : &clearing;
}
const char *map_name(int id)
{
    const char *const names[] = {"HEARTH CLEARING","FERNVEIL WOODS","WAYFARER LODGE","HOLLOWSTONE CAVE"};
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
    return tile == 'g' && id == MAP_FOREST ? 1 : tile == 'c' && id == MAP_CAVE ? 2 : 0;
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
           tile == '>' || tile == 'g' || tile == 'c' || tile == '_';
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
    } else {
        add(n,5,10,"NEL","FOOTSTEPS ECHO IN THIS CAVE.","VEYLINGS STIR ON THE ROUGH FLOOR.\nTHE SOUTHWEST EXIT LEADS OUT.",5);
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

## src/text.c

````text
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
````

## src/world_draw.c

````text
#include <pspgu.h>
#include "graphics.h"
#include "world_draw.h"

#define COLOR(r,g,b) GU_RGBA(r,g,b,255)

static void tile_draw(char tile, int x, int y, int variant)
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
        graphics_rectangle(x,y,32,32,COLOR(169,148,104));
        graphics_rectangle(x+5,y+13,22,6,COLOR(246,219,147));
        graphics_rectangle(x+19,y+8,4,16,COLOR(246,219,147));
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
        graphics_rectangle(x+4,y+9,12,2,COLOR(86,139,151));
        graphics_rectangle(x+17,y+23,10,2,COLOR(63,115,138));
        break;
    case 'O':
        graphics_rectangle(x+3,y+9,26,20,COLOR(44,55,55));
        graphics_rectangle(x+5,y+5,21,20,COLOR(111,118,106));
        graphics_rectangle(x+8,y+5,15,5,COLOR(147,149,126));
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

void world_draw(const Map *map, const Player *player, const Camera *camera)
{
    /* At most 16 columns x 10 rows, independent of total map size. */
    int first_x = camera->x / TILE_SIZE;
    int first_y = camera->y / TILE_SIZE;
    int last_x = (camera->x + SCREEN_WIDTH - 1) / TILE_SIZE;
    int last_y = (camera->y + SCREEN_HEIGHT - 1) / TILE_SIZE;
    for (int y = first_y; y <= last_y && y < map->height; ++y) {
        for (int x = first_x; x <= last_x && x < map->width; ++x) {
            tile_draw(map_tile(map,x,y), x*TILE_SIZE-camera->x,
                      y*TILE_SIZE-camera->y, (x+y)%2);
        }
    }
    world_actor_draw(player, camera, 0);
}
````

## tests/host/pspgu.h

````text
#ifndef TEST_PSPGU_H
#define TEST_PSPGU_H
/* Host-only color packing; never included by the PSP Makefile. */
#define GU_RGBA(r,g,b,a) ((unsigned int)(r) | ((unsigned int)(g)<<8) | ((unsigned int)(b)<<16) | ((unsigned int)(a)<<24))
#endif
````

## tests/battle_test.c

````text
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "battle.h"

static void press(Battle *b)
{
    battle_update(b,&(Input){0,0,1,0,0});
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
    battle_update(&b,&(Input){0,0,1,0,0}); assert(b.phase==BATTLE_DONE);

    start(&b,12);
    b.ally.hp=1; b.enemy.speed=999;
    for(int i=0;i<4;++i) b.enemy.moves[i]=MOVE_NUDGE;
    choose(&b,0);
    assert(b.result==BATTLE_LOSS && b.ally.hp==0 && b.ally.uses[0]==24);
    finish_messages(&b); assert(b.phase==BATTLE_DONE);

    start(&b,12);
    b.cursor=0; press(&b);
    battle_update(&b,&(Input){0,0,0,1,0});
    assert(b.phase==BATTLE_MENU && b.enemy.hp==b.enemy.max_hp);
    /* Menu navigation advances once per new direction, not once per frame. */
    for(int i=0;i<20;++i) battle_update(&b,&(Input){0,1,0,0,0});
    assert(b.cursor==1);
    battle_update(&b,&(Input){0});
    battle_update(&b,&(Input){0,1,0,0,0}); assert(b.cursor==2);
    for(int i=1;i<=3;++i) {
        b.cursor=i; press(&b); assert(b.phase==BATTLE_MESSAGE);
        finish_messages(&b);
        assert(b.phase==BATTLE_MENU && b.ally.hp==b.ally.max_hp);
    }
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

static void confirm(Battle *b) { battle_update(b,&(Input){0,0,1,0,0}); }
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
        if(b->phase==BATTLE_LEARN) battle_update(b,&(Input){0,0,0,1,0});
        else confirm(b);
    }
    assert(b->phase==BATTLE_DONE);
}
int main(void)
{
    assert(SPECIES_COUNT==10);
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
    battle_update(&b,&(Input){0,0,0,1,0});close_growth(&b);
    assert(b.ally.moves[1]==MOVE_CINDER);

    creature_create(&c,SPECIES_CINDLET,7);c.experience=creature_xp_for_level(8)-1;
    victory(&b,&c);assert(b.ally.species==SPECIES_EMBERLYN);
    close_growth(&b);assert(b.ally.species==SPECIES_EMBERLYN);
    creature_create(&c,SPECIES_CINDLET,5);
    battle_begin(&b,&c,SPECIES_MOSSLET,3,2);confirm(&b);
    b.cursor=4;b.escape_attempts=2;confirm(&b);confirm(&b);
    assert(b.phase==BATTLE_DONE && b.ally.experience==c.experience);
    puts("PASS: 10 species, 100 levels, XP boundaries/cap, evolution, stats, nicknames, move choices, single rewards");
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
for name in ('dialogue', 'encounter', 'battle-menu', 'battle-moves', 'learn-move', 'evolution', 'partner'):
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
# Run from the psp directory. Host checks use only a color macro stub, not an emulator.
mkdir -p previews
cc -std=c99 -Wall -Wextra -Werror -fsanitize=address,undefined -Iinclude \
    tests/overworld_test.c src/map.c src/player.c src/camera.c -lm -o previews/overworld-test
previews/overworld-test
cc -std=c99 -Wall -Wextra -Werror -fsanitize=address,undefined -Itests/host -Iinclude \
    tests/world_systems_test.c src/game.c src/map.c src/player.c src/camera.c \
    src/npc.c src/dialogue.c src/encounter.c src/world_draw.c src/text.c \
    src/attacks.c src/battle.c src/battle_draw.c src/creature.c -o previews/world-test
previews/world-test
cc -std=c99 -Wall -Wextra -Werror -fsanitize=address,undefined -Iinclude \
    tests/battle_test.c src/battle.c src/attacks.c src/creature.c -o previews/battle-test
previews/battle-test
cc -std=c99 -Wall -Wextra -Werror -fsanitize=address,undefined -Iinclude \
    tests/creature_test.c src/creature.c src/battle.c src/attacks.c -o previews/creature-test
previews/creature-test
python3 tests/preview.py
````
