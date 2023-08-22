#ifndef __TILESURFACE_H
#define __TILESURFACE_H
#include <SDL.h>
#include "capfil_types.h"
#include "vgapalette.h"
#include "tile.h"

SDL_Surface *generateSDLSurfaceForTile(const Tile *tile, const VGAPalette vgaPalette);

SDL_Surface *extractAreaFromTile(SDL_Surface *tileSurface, const VGAPalette vgaPalette, capfil_short topX, capfil_short topY, capfil_short bottomX, capfil_short bottomY);

#endif
