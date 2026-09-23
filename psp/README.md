# Emberwake — Phase 2

An original PSP RPG prototype, built incrementally on the hardware-tested Phase 1.
Future creatures are called Veylings. This phase adds only the overworld foundation.

## What changed

- 40 x 24 tiles, each 32 pixels: a 1280 x 768 clearing.
- Reusable grass, path, flower, tree, rock, and pond tile graphics.
- Four-direction, smooth tile-to-tile movement at 128 pixels/second.
- Four facing poses and alternating foot animation.
- Trees, rocks, water, and map boundaries block movement.
- A pixel-aligned camera follows the player and clamps at world edges.
- Rendering visits only visible tiles (at most 160), with no per-frame heap allocation.
- The GU command buffer is now 256 KiB to accommodate tile drawing.
- Existing HOME callback handling, controller polling, double buffering, and
  elapsed-time cap remain in place.

Release the D-pad to finish the current step. Direction changes happen at tile
boundaries. If two directions are held, horizontal takes priority. This is now
four-direction grid movement, rather than Phase 1's free diagonal movement.

## Folder structure

psp/
  .gitignore
  Makefile
  README.md
  SOURCE_CONTENTS.md
  assets/maps/clearing.inc
  include/
    camera.h
    game.h
    graphics.h
    input.h
    map.h
    player.h
    world_draw.h
  src/
    main.c
    camera.c
    game.c
    graphics.c
    input.c
    map.c
    player.c
    world_draw.c
  tests/overworld_test.c

SOURCE_CONTENTS.md contains the complete contents of all authored project files,
excluding the listing itself. Generated output includes EBOOT.PBP, PARAM.SFO,
emberwake.elf, emberwake.prx, and src/*.o / src/*.d.

## Architecture

main.c manages startup, HOME exit, and the frame loop. game.c coordinates the
world. map.c exposes tile lookup and collision. player.c handles step timing and
facing independently of rendering. camera.c computes the viewport. world_draw.c
draws reusable tiles and the traveler using graphics.c's existing rectangle API.

Only the original PSPSDK GU/GE, display, controller, and kernel facilities are
required. No new libraries or external art assets are needed.

## Map format

assets/maps/clearing.inc contains 24 quoted C strings, each exactly 40 characters,
followed by commas. The map is compiled into the EBOOT; no files need to be copied
alongside it. Coordinates start at zero in the top-left corner.

| Character | Tile | Walkable |
| --- | --- | --- |
| . | Grass | Yes |
| = | Path | Yes |
| , | Flowers | Yes |
| # | Tree | No |
| O | Rock | No |
| ~ | Water | No |

Unknown tile codes and coordinates outside the map are blocked. Map dimensions
and spawn coordinates are declared in src/map.c. Spawn is (5, 11). When editing,
keep all row lengths consistent, the dimensions accurate, and the spawn walkable.
Run the logic test after map edits. Changing the map include triggers a rebuild.

There are no entrances, transitions, NPCs, dialogue, or encounters yet.
Those belong to Phase 3.

## Exact build commands

From PowerShell on this computer (existing Ubuntu PSPDEV environment):

    wsl -d Ubuntu -- bash -lc 'cd /mnt/c/Users/polo1/OneDrive/Documents/app/psp && make'

For a clean build:

    wsl -d Ubuntu -- bash -lc 'cd /mnt/c/Users/polo1/OneDrive/Documents/app/psp && make clean && make'

From Linux/WSL in this project's psp folder, with PSPDEV configured:

    make

The build targets a user-mode PRX for 6.60/6.61 custom firmware and packages
EBOOT.PBP. No toolchain installation or system configuration was changed.

## Logic checks

In Linux/WSL, from the psp folder:

    cc -std=c99 -Wall -Wextra -Werror -fsanitize=address,undefined -Iinclude tests/overworld_test.c src/map.c src/player.c src/camera.c -lm -o /tmp/emberwake-overworld-test
    /tmp/emberwake-overworld-test

Checks cover map row dimensions and tile codes, spawn validity, world bounds,
solid tiles, partial movement, release-to-complete behavior, turning, diagonal
priority, speed at 30/60/120 updates per second, collision from four directions,
camera centering and edge clamps, small maps, and long-frame clamping.

## Install and hardware test

Replace the previous file with this build:

    ms0:/PSP/GAME/EMBERWAKE/EBOOT.PBP

Open Game > Memory Stick > Emberwake - Phase 2.

1. Expect the traveler on a sandy horizontal path in a green clearing.
2. Walk in all four directions; check facing and stepping animation.
3. Release mid-step: the traveler should settle onto the next tile.
4. Follow the path right: the camera should scroll. Walk toward all world edges:
   the camera should stop without showing outside the map.
5. Try walking into trees, rocks, and the pond. The player should remain outside.
   The pond is northeast of the starting area, trees north, and rocks south.
6. Try turns and two held directions near corners; do not pass through obstacles.
7. Confirm HOME > Cancel resumes play and HOME > Quit returns to the system menu.
8. Suspend/resume and check that there is no large movement jump.

## Validation status

The host logic checks passed with AddressSanitizer and UndefinedBehaviorSanitizer.
The PSP build uses -Wall -Wextra -Werror. Hardware visuals, actual frame rate,
camera feel, suspend/resume, and HOME behavior need your Phase 2 PSP test.

Phase 1 movement/rendering/boundaries/HOME were confirmed by you on PSP hardware.
Development stops at Phase 2 pending your next test.

## Official references

- [PSPDEV installation](https://pspdev.github.io/installation.html)
- [PSPSDK GU reference](https://pspdev.github.io/pspsdk/group__GU.html)
- [PSPSDK build rules](https://github.com/pspdev/pspsdk/blob/master/src/base/build.mak)
