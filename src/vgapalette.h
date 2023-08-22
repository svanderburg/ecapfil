#ifndef __VGAPALETTE_H
#define __VGAPALETTE_H
#include <stdio.h>
#include <SDL.h>
#include "capfil_types.h"

#define NUM_OF_COLOR_COMPONENTS 3
#define NUM_OF_COLOR_REGISTERS 256
#define COLOR_COMPONENT_DEPTH 6
#define VGA_PALETTE_SIZE (NUM_OF_COLOR_COMPONENTS * NUM_OF_COLOR_REGISTERS)

typedef capfil_byte VGAPalette[VGA_PALETTE_SIZE];

capfil_bool readVGAPaletteFd(FILE *file, VGAPalette palette);

capfil_bool readVGAPalette(const char *filename, VGAPalette palette);

void printVGAPalette(const VGAPalette palette);

void generateSDLPaletteColors(const VGAPalette vgaPalette, SDL_Color *colors);

#endif
