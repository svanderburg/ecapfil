#ifndef __ARTFILE_H
#define __ARTFILE_H

typedef struct ArtFile ArtFile;

#include <stdio.h>
#include "capfil_types.h"
#include "vgapalette.h"
#include "tile.h"

#define NUM_OF_TILES_PER_ART_FILE 256

struct ArtFile
{
    capfil_long artversion;
    capfil_long numtiles;
    capfil_long localtilestart;
    capfil_long localtileend;
    capfil_short *tilesizx;
    capfil_short *tilesizy;
    capfil_long *picanm;
    capfil_byte **artdata;
};

void createEmptyArtFile(ArtFile *artFile, const capfil_long index);

capfil_long computeTileSize(const ArtFile *artFile, const capfil_long index);

capfil_bool readArtFileFd(FILE *file, ArtFile *artFile);

capfil_bool readArtFile(const char *filename, ArtFile *artFile);

capfil_bool readArtFileIndexOrCreateEmpty(const capfil_long index, ArtFile *artFile);

capfil_bool writeArtFileFd(FILE *file, const ArtFile *artFile);

capfil_bool writeArtFile(const char *filename, const ArtFile *artFile);

capfil_bool writeArtFileIndex(const capfil_long index, const ArtFile *artFile);

void clearArtFile(ArtFile *artFile);

void printArtFile(const ArtFile *artFile);

void replaceTileInArtFile(ArtFile *artFile, Tile *tile, const capfil_long index);

capfil_bool renderTileSurfacesFromArtFile(const ArtFile *artFile, const VGAPalette vgaPalette, SDL_Surface **tileSurface);

void freeTileSurfacesOfArtFile(const ArtFile *artFile, SDL_Surface **tileSurface);

#endif
