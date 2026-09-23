TARGET = emberwake
OBJS = src/main.o src/game.o src/input.o src/graphics.o src/map.o src/player.o src/camera.o src/world_draw.o src/npc.o src/dialogue.o src/encounter.o src/text.o src/attacks.o src/battle.o src/battle_draw.o

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
PSP_EBOOT_TITLE = Emberwake - Phase 4

PSPSDK = $(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak

src/main.o: include/game.h include/graphics.h include/input.h
src/game.o: include/game.h include/graphics.h include/input.h
src/input.o: include/input.h
src/graphics.o: include/graphics.h

src/map.o: assets/maps/clearing.inc
PARAM.SFO: Makefile
-include $(OBJS:.o=.d)
