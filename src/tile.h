#ifndef __TILE_H
#define __TILE_H

typedef struct Tile Tile;

#include <SDL.h>
#include "capfil_types.h"

struct Tile
{
    capfil_short tilesizx;
    capfil_short tilesizy;
    capfil_long picanm;
    capfil_byte *artdata;
};

void generateTileFromSDLSurface(Tile *tile, SDL_Surface *surface);

#endif
