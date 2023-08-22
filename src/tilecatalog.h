#ifndef __TILECATALOG_H
#define __TILECATALOG_H

typedef struct TileCatalog TileCatalog;

#include <stdio.h>
#include "artfile.h"
#include "vgapalette.h"
#include "tile.h"
#include "numericset.h"
#include "capfil.h"

#define NUM_OF_ART_FILES 24
#define TOTAL_NUM_OF_TILES (NUM_OF_ART_FILES * NUM_OF_TILES_PER_ART_FILE)

struct TileCatalog
{
    VGAPalette vgaPalette;
    ArtFile artFile[NUM_OF_ART_FILES];
    SDL_Surface *tileSurface[TOTAL_NUM_OF_TILES];
};

capfil_bool generateTileCatalog(TileCatalog *tileCatalog, const char *paletteFileName);

void clearTileCatalog(TileCatalog *tileCatalog);

void printTileCatalog(const TileCatalog *tileCatalog);

capfil_bool writeArtFilesInTileCatalog(TileCatalog *tileCatalog, const NumericSet *numericSet);

capfil_bool reloadArtFilesInTileCatalog(TileCatalog *tileCatalog, const NumericSet *numericSet);

capfil_bool regrabTilesInCapfil(TileCatalog *tileCatalog, const char *capfilFileName);

#endif
