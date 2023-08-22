#include "artfile.h"
#include <stdlib.h>
#include "io.h"
#include "picanm.h"
#include "tilesurface.h"

#define ART_FILENAME_LENGTH 13 /* Length of an ART filename: TILES???.ART plus NUL termination */
#define DEFAULT_ART_VERSION 1

void createEmptyArtFile(ArtFile *artFile, const capfil_long index)
{
    capfil_long i;

    artFile->artversion = DEFAULT_ART_VERSION;
    artFile->numtiles = (index + 1) * NUM_OF_TILES_PER_ART_FILE;
    artFile->localtilestart = index * NUM_OF_TILES_PER_ART_FILE;
    artFile->localtileend = artFile->localtilestart + NUM_OF_TILES_PER_ART_FILE - 1;
    artFile->tilesizx = (capfil_short*)calloc(NUM_OF_TILES_PER_ART_FILE, sizeof(capfil_short));
    artFile->tilesizy = (capfil_short*)calloc(NUM_OF_TILES_PER_ART_FILE, sizeof(capfil_short));
    artFile->picanm = (capfil_long*)calloc(NUM_OF_TILES_PER_ART_FILE, sizeof(capfil_long));
    artFile->artdata = (capfil_byte**)malloc(NUM_OF_TILES_PER_ART_FILE * sizeof(capfil_byte*));

    for(i = 0; i < NUM_OF_TILES_PER_ART_FILE; i++)
        artFile->artdata[i] = (capfil_byte*)malloc(0);
}

static capfil_bool readTilesizx(FILE *file, ArtFile *artFile, const capfil_long numOfTiles)
{
    capfil_long i;
    artFile->tilesizx = (capfil_short*)malloc(sizeof(capfil_short) * numOfTiles);

    for(i = 0; i < numOfTiles; i++)
    {
        if(!readShort(file, &artFile->tilesizx[i]))
            return FALSE;
    }

    return TRUE;
}

static capfil_bool readTilesizy(FILE *file, ArtFile *artFile, const capfil_long numOfTiles)
{
    capfil_long i;
    artFile->tilesizy = (capfil_short*)malloc(sizeof(capfil_short) * numOfTiles);

    for(i = 0; i < numOfTiles; i++)
    {
        if(!readShort(file, &artFile->tilesizy[i]))
            return FALSE;
    }

    return TRUE;
}

static capfil_bool readPicanm(FILE *file, ArtFile *artFile, const capfil_long numOfTiles)
{
    capfil_long i;
    artFile->picanm = (capfil_long*)malloc(sizeof(capfil_long) * numOfTiles);

    for(i = 0; i < numOfTiles; i++)
    {
        if(!readLong(file, &artFile->picanm[i]))
            return FALSE;
    }

    return TRUE;
}

capfil_long computeTileSize(const ArtFile *artFile, const capfil_long index)
{
    return artFile->tilesizx[index] * artFile->tilesizy[index];
}

static capfil_bool readArtdata(FILE *file, ArtFile *artFile, const capfil_long numOfTiles)
{
    capfil_long i;
    artFile->artdata = (capfil_byte**)malloc(sizeof(capfil_byte*) * numOfTiles);

    for(i = 0; i < numOfTiles; i++)
    {
        capfil_long tileSize = computeTileSize(artFile, i);
        artFile->artdata[i] = (capfil_byte*)malloc(sizeof(capfil_byte) * tileSize);

        if(fread(artFile->artdata[i], sizeof(capfil_byte), tileSize, file) < tileSize)
            return FALSE;
    }

    return TRUE;
}

static capfil_bool composeArtFileName(char *filename, const capfil_long index)
{
    if(index < 10)
    {
        sprintf(filename, "TILES00%u.ART", index);
        return TRUE;
    }
    else if(index < 100)
    {
        sprintf(filename, "TILES0%u.ART", index);
        return TRUE;
    }
    else if(index < 1000)
    {
        sprintf(filename, "TILES%u.ART", index);
        return TRUE;
    }
    else
        return FALSE;
}

static capfil_bool checkIfFileExists(const char *filename)
{
    FILE *file = fopen(filename, "r");

    if(file == NULL)
        return FALSE;
    else
    {
        fclose(file);
        return TRUE;
    }
}

capfil_bool readArtFileIndexOrCreateEmpty(const capfil_long index, ArtFile *artFile)
{
    char filename[ART_FILENAME_LENGTH];

    if(!composeArtFileName(filename, index))
        return FALSE;

    if(checkIfFileExists(filename))
    {
        if(!readArtFile(filename, artFile))
            return FALSE;
    }
    else
        createEmptyArtFile(artFile, index);

    return TRUE;
}

static capfil_long computeNumberOfTiles(const ArtFile *artFile)
{
    return artFile->localtileend - artFile->localtilestart + 1;
}

capfil_bool readArtFileFd(FILE *file, ArtFile *artFile)
{
    capfil_long numOfTiles;

    if (!readLong(file, &artFile->artversion))
        return FALSE;

    if(artFile->artversion != 1)
    {
        fprintf(stderr, "We can only open an ART file having version 1! This file has version: %u\n", artFile->artversion);
        return FALSE;
    }

    if(!readLong(file, &artFile->numtiles)
        || !readLong(file, &artFile->localtilestart)
        || !readLong(file, &artFile->localtileend))
        return FALSE;

    numOfTiles = computeNumberOfTiles(artFile);

    return (readTilesizx(file, artFile, numOfTiles)
        && readTilesizy(file, artFile, numOfTiles)
        && readPicanm(file, artFile, numOfTiles)
        && readArtdata(file, artFile, numOfTiles));
}

capfil_bool readArtFile(const char *filename, ArtFile *artFile)
{
    FILE *file = fopen(filename, "rb");

    if(file == NULL)
        return FALSE;
    else
    {
        capfil_bool status = readArtFileFd(file, artFile);
        fclose(file);
        return status;
    }
}

static capfil_bool writeTilesizx(FILE *file, const ArtFile *artFile, const capfil_long numOfTiles)
{
    capfil_long i;

    for(i = 0; i < numOfTiles; i++)
    {
        if(!writeShort(file, artFile->tilesizx[i]))
            return FALSE;
    }

    return TRUE;
}

static capfil_bool writeTilesizy(FILE *file, const ArtFile *artFile, const capfil_long numOfTiles)
{
    capfil_long i;

    for(i = 0; i < numOfTiles; i++)
    {
        if(!writeShort(file, artFile->tilesizy[i]))
            return FALSE;
    }

    return TRUE;
}

static capfil_bool writePicanm(FILE *file, const ArtFile *artFile, const capfil_long numOfTiles)
{
    capfil_long i;

    for(i = 0; i < numOfTiles; i++)
    {
        if(!writeLong(file, artFile->picanm[i]))
            return FALSE;
    }

    return TRUE;
}

static capfil_bool writeArtdata(FILE *file, const ArtFile *artFile, const capfil_long numOfTiles)
{
    capfil_long i;

    for(i = 0; i < numOfTiles; i++)
    {
        capfil_long tile_size = computeTileSize(artFile, i);
        if(fwrite(artFile->artdata[i], sizeof(capfil_byte), tile_size, file) < tile_size)
            return FALSE;
    }

    return TRUE;
}

capfil_bool writeArtFileFd(FILE *file, const ArtFile *artFile)
{
    capfil_long numOfTiles = computeNumberOfTiles(artFile);

    return (writeLong(file, artFile->artversion)
        && writeLong(file, artFile->numtiles)
        && writeLong(file, artFile->localtilestart)
        && writeLong(file, artFile->localtileend)
        && writeTilesizx(file, artFile, numOfTiles)
        && writeTilesizy(file, artFile, numOfTiles)
        && writePicanm(file, artFile, numOfTiles)
        && writeArtdata(file, artFile, numOfTiles));
}

capfil_bool writeArtFile(const char *filename, const ArtFile *artFile)
{
    FILE *file = fopen(filename, "w");

    if(file == NULL)
        return FALSE;
    else
    {
        capfil_bool status = writeArtFileFd(file, artFile);
        fclose(file);
        return status;
    }
}

capfil_bool writeArtFileIndex(const capfil_long index, const ArtFile *artFile)
{
    char filename[ART_FILENAME_LENGTH];

    return (composeArtFileName(filename, index)
        && writeArtFile(filename, artFile));
}

static void freeArtData(ArtFile *artFile)
{
    capfil_long i;
    capfil_long numOfTiles = computeNumberOfTiles(artFile);

    for(i = 0; i < numOfTiles; i++)
        free(artFile->artdata[i]);

    free(artFile->artdata);
}

void clearArtFile(ArtFile *artFile)
{
    free(artFile->tilesizx);
    free(artFile->tilesizy);
    free(artFile->picanm);
    freeArtData(artFile);
}

static void printTilesizxArray(const ArtFile *artFile, const capfil_long numOfTiles)
{
    capfil_long i;

    for(i = 0; i < numOfTiles; i++)
        printf("tilesizx[%u] = %u\n", i, artFile->tilesizx[i]);
}

static void printTilesizyArray(const ArtFile *artFile, const capfil_long numOfTiles)
{
    capfil_long i;

    for(i = 0; i < numOfTiles; i++)
        printf("tilesizy[%u] = %u\n", i, artFile->tilesizy[i]);
}

static void printPicanmArray(const ArtFile *artFile, const capfil_long numOfTiles)
{
    capfil_long i;

    for(i = 0; i < numOfTiles; i++)
    {
        printf("picanm[%u] = ", i);
        printPicanm(artFile->picanm[i]);
        printf("\n");
    }
}

void printArtFile(const ArtFile *artFile)
{
    capfil_long numOfTiles = computeNumberOfTiles(artFile);

    printf("artversion = %u\n", artFile->artversion);
    printf("numtiles = %u\n", artFile->numtiles);
    printf("localtilestart = %u\n", artFile->localtilestart);
    printf("localtileend = %u\n", artFile->localtileend);
    printTilesizxArray(artFile, numOfTiles);
    printTilesizyArray(artFile, numOfTiles);
    printPicanmArray(artFile, numOfTiles);
}

void replaceTileInArtFile(ArtFile *artFile, Tile *tile, const capfil_long index)
{
    artFile->tilesizx[index] = tile->tilesizx;
    artFile->tilesizy[index] = tile->tilesizy;
    artFile->picanm[index] = tile->picanm;
    free(artFile->artdata[index]);
    artFile->artdata[index] = tile->artdata;
}

static void generateTileFromArtFileIndex(const ArtFile *artFile, Tile *tile, const capfil_long index)
{
    tile->tilesizx = artFile->tilesizx[index];
    tile->tilesizy = artFile->tilesizy[index];
    tile->picanm = artFile->picanm[index];
    tile->artdata = artFile->artdata[index];
}

capfil_bool renderTileSurfacesFromArtFile(const ArtFile *artFile, const VGAPalette vgaPalette, SDL_Surface **tileSurface)
{
    capfil_long i, tileSurfaceCount;
    capfil_long numberOfTiles = computeNumberOfTiles(artFile);

    for(i = 0, tileSurfaceCount = artFile->localtilestart; i < numberOfTiles; i++, tileSurfaceCount++)
    {
        Tile tile;
        generateTileFromArtFileIndex(artFile, &tile, i);
        tileSurface[tileSurfaceCount] = generateSDLSurfaceForTile(&tile, vgaPalette);

        if(tileSurface == NULL)
            return FALSE;
    }

    return TRUE;
}

void freeTileSurfacesOfArtFile(const ArtFile *artFile, SDL_Surface **tileSurface)
{
    capfil_long i;

    for(i = artFile->localtilestart; i <= artFile->localtileend; i++)
        SDL_FreeSurface(tileSurface[i]);
}
