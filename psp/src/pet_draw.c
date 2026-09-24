#include "pet_draw.h"
#include "graphics.h"
void pet_draw(int species,int x,int y,int size,int flip)
{
    if(species<0 || species>=SPECIES_COUNT || size<=0) return;
    graphics_texture(x,y,size,size,pet_pixels[species],flip);
}
