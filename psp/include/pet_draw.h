#ifndef EMBERWAKE_PET_DRAW_H
#define EMBERWAKE_PET_DRAW_H
#include <stdint.h>
#include "creature.h"
#define PET_TEXTURE_SIZE 128
#define PET_CANVAS_SIZE 96
extern const uint16_t pet_pixels[SPECIES_COUNT][PET_TEXTURE_SIZE * PET_TEXTURE_SIZE];
void pet_draw(int species,int x,int y,int size,int flip);
#endif
