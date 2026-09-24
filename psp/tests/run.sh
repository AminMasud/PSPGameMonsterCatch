#!/bin/sh
set -eu
# Run from the psp directory. Host checks use PSP API stubs, not an emulator.
mkdir -p previews
python3 tools/check_pet_assets.py
cc -std=c99 -Wall -Wextra -Werror -fsanitize=address,undefined -Itests/host -Iinclude \
    tests/save_data_test.c src/save_data.c src/save_codec.c src/creature.c src/attacks.c -o previews/save-data-test
previews/save-data-test
cc -std=c99 -Wall -Wextra -Werror -fsanitize=address,undefined -Iinclude \
    tests/overworld_test.c src/map.c src/player.c src/camera.c -lm -o previews/overworld-test
previews/overworld-test
cc -std=c99 -Wall -Wextra -Werror -fsanitize=address,undefined -Itests/host -Iinclude \
    tests/world_systems_test.c src/game.c src/map.c src/player.c src/camera.c \
    src/npc.c src/npc_battle.c src/progression.c src/dialogue.c src/encounter.c src/world_draw.c src/text.c \
    src/attacks.c src/battle.c src/battle_draw.c src/creature.c src/npc_ai.c \
    src/party.c src/capture.c src/party_menu.c src/inventory.c src/player_menu.c src/pet_draw.c src/pet_assets.S \
    src/ready_prompt.c \
    tests/host/save_data_stub.c tests/host/audio_stub.c -o previews/world-test
previews/world-test
cc -std=c99 -Wall -Wextra -Werror -fsanitize=address,undefined -Iinclude \
    tests/battle_test.c src/battle.c src/npc_ai.c src/attacks.c src/creature.c src/party.c src/capture.c src/inventory.c -o previews/battle-test
previews/battle-test
cc -std=c99 -Wall -Wextra -Werror -fsanitize=address,undefined -Iinclude \
    tests/battle_turn_test.c src/battle.c src/npc_ai.c src/attacks.c src/creature.c src/party.c src/capture.c src/inventory.c -o previews/battle-turn-test
previews/battle-turn-test
cc -std=c99 -Wall -Wextra -Werror -fsanitize=address,undefined -Iinclude \
    tests/creature_test.c src/creature.c src/battle.c src/npc_ai.c src/attacks.c src/party.c src/capture.c src/inventory.c -o previews/creature-test
previews/creature-test
cc -std=c99 -Wall -Wextra -Werror -fsanitize=address,undefined -Iinclude \
    tests/party_capture_test.c src/party.c src/capture.c src/creature.c src/attacks.c -o previews/party-capture-test
previews/party-capture-test
cc -std=c99 -Wall -Wextra -Werror -fsanitize=address,undefined -Iinclude \
    tests/battle_party_test.c src/battle.c src/npc_ai.c src/attacks.c src/creature.c src/party.c src/capture.c src/inventory.c -o previews/battle-party-test
previews/battle-party-test
cc -std=c99 -Wall -Wextra -Werror -fsanitize=address,undefined -Iinclude \
    tests/npc_battle_test.c src/npc_battle.c src/progression.c src/battle.c src/npc_ai.c src/attacks.c src/creature.c \
    src/party.c src/capture.c src/inventory.c -o previews/npc-battle-test
previews/npc-battle-test
cc -std=c99 -Wall -Wextra -Werror -fsanitize=address,undefined -Iinclude \
    tests/progression_test.c src/progression.c -o previews/progression-test
previews/progression-test
cc -std=c99 -Wall -Wextra -Werror -fsanitize=address,undefined -Iinclude \
    tests/npc_ai_test.c src/npc_ai.c -o previews/npc-ai-test
previews/npc-ai-test
cc -std=c99 -Wall -Wextra -Werror -fsanitize=address,undefined -Iinclude \
    tests/inventory_test.c src/inventory.c src/creature.c src/attacks.c -o previews/inventory-test
previews/inventory-test
cc -std=c99 -Wall -Wextra -Werror -fsanitize=address,undefined -Iinclude \
    tests/polish_test.c src/player_menu.c src/audio_synth.c src/text.c src/map.c \
    src/inventory.c src/party.c src/creature.c src/attacks.c -o previews/polish-test
previews/polish-test
python3 tests/preview.py
