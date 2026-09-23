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
