TARGET = emberwake
OBJS = src/main.o src/game.o src/input.o src/graphics.o src/map.o src/player.o src/camera.o src/world_draw.o src/npc.o src/npc_battle.o src/npc_ai.o src/progression.o src/gate.o src/dialogue.o src/encounter.o src/text.o src/attacks.o src/battle.o src/battle_draw.o src/creature.o src/party.o src/capture.o src/party_menu.o src/inventory.o src/save_data.o src/player_menu.o src/ready_prompt.o src/audio.o src/audio_synth.o src/pet_draw.o src/pet_assets.o src/save_codec.o

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
PSP_EBOOT_TITLE = Emberwake - Phase 20

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

src/pet_assets.o: assets/generated/pets.rgba4444
.PHONY: check-pets rebuild-pets
check-pets:
	python3 tools/check_pet_assets.py
rebuild-pets:
	python3 tools/build_pet_assets.py
$(TARGET).elf: | check-pets
