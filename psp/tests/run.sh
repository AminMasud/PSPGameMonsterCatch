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
