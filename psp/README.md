# Emberwake — Phase 3

Original PSP homebrew RPG prototype in C / PSPSDK. The creatures are called
**Veylings**. Phase 3 extends the existing Phase 2 project with connected maps,
NPC interaction, dialogue, and encounter zones. Battles begin in Phase 4.

## Play this build

The verified build is **EBOOT-PHASE3.PBP**. The older EBOOT.PBP was locked by another
process during packaging and remains the Phase 2 binary. Use the new file.

Copy EBOOT-PHASE3.PBP to the Memory Stick, naming the destination:

    ms0:/PSP/GAME/EMBERWAKE/EBOOT.PBP

Launch **Emberwake - Phase 3** from Game > Memory Stick.

- D-pad: smooth four-direction tile movement; horizontal wins when two directions
  are held. Release finishes the current tile.
- X: speak to a stationary NPC in the tile you face; advance/close dialogue.
- Circle: close dialogue immediately.
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
6. Walk through dark tall grass until a Veyling notice appears. X or Circle
   dismisses it. Standing still never triggers an encounter.
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

The current result is a short name/level notice followed by the creature departing.
These are encounter-table placeholders, not the full species database. No
combat, capture, leveling, healing, shops, saving, or RPG menus are implemented.

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
        camera.h
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
        camera.c
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
        overworld_test.c
        world_systems_test.c
        preview.py
        run.sh

SOURCE_CONTENTS.md contains the full text of all authored PSP project files
except itself. Generated binaries and previews are ignored by Git.

main.c initializes PSP services, seeds encounter randomness, and runs the existing
vblank-paced loop. game.c coordinates map entry, step events, NPC interaction,
and encounter notices. Player movement accepts an optional occupancy callback;
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

To add a map, add its rows, ID, dimensions, name, portal connections, and optional
NPC definitions / encounter terrain. Keep coordinates within the declared map
and extend tests for new dimensions if they exceed the current test grid.

## Build commands

This machine uses the existing Ubuntu/WSL PSPDEV environment.

From PowerShell, build the Phase 3 artifact without overwriting the locked file:

    wsl -d Ubuntu -- bash -lc 'cd /mnt/c/Users/polo1/OneDrive/Documents/app/psp && make PSP_EBOOT=EBOOT-PHASE3.PBP EXTRA_TARGETS=EBOOT-PHASE3.PBP'

In a configured Linux/WSL shell, from the psp directory:

    make PSP_EBOOT=EBOOT-PHASE3.PBP EXTRA_TARGETS=EBOOT-PHASE3.PBP

For the conventional EBOOT.PBP output when that file is not open elsewhere:

    make

The user-mode PRX targets 6.60/6.61 custom firmware. The Makefile generates header
dependencies automatically and repackages the title when its settings change.

## Validation and rendering budget

From the psp directory in Linux/WSL:

    sh tests/run.sh

Both test executables use AddressSanitizer and UndefinedBehaviorSanitizer.
Checks include Phase 2 regression coverage plus portal reachability and all six
transitions, valid/non-trigger arrival tiles, NPC collisions in both directions,
patrol limits, dialogue pause/advance/cancel, no stationary encounters, weighted
table level ranges, encounter grace steps, text bounds, and drawing budget.
The host test renders the real draw functions into software pixel buffers;
tests/preview.py converts those buffers to PNG without third-party dependencies.

Software previews are saved in previews/dialogue.png and previews/encounter.png.
They are not emulator or hardware screenshots.

Only visible terrain is drawn (at most 160 tiles). The GU list reserves 1 MiB for
tile, actor, and bitmap text commands. The tested scenes stay below a conservative
6000-rectangle budget. Two color buffers still occupy 1,114,112 bytes of VRAM.
The main loop retains its 50 ms elapsed-time cap and HOME callback service.

Host regression/integration tests passed. The PSP compiler and linker passed
with -Wall -Wextra -Werror, and the Phase 3 PBP was packaged successfully.
Real PSP visuals, performance, and input/exit behavior still require your test.

Stop here before Phase 4.

## Official references

- [PSPDEV installation](https://pspdev.github.io/installation.html)
- [PSPSDK GU reference](https://pspdev.github.io/pspsdk/group__GU.html)
- [PSPSDK build rules](https://github.com/pspdev/pspsdk/blob/master/src/base/build.mak)
