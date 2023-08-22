#include "tilecatalog.h"
#include "tilesurface.h"

static capfil_bool openArtFiles(TileCatalog *tileCatalog)
{
    capfil_long i;

    memset(tileCatalog->artFile, '\0', NUM_OF_ART_FILES * sizeof(ArtFile));

    for(i = 0; i < NUM_OF_ART_FILES; i++)
    {
        if(!readArtFileIndexOrCreateEmpty(i, &tileCatalog->artFile[i]))
            return FALSE;
    }

    return TRUE;
}

static capfil_bool renderTileSurfaces(TileCatalog *tileCatalog)
{
    capfil_long i;

    memset(tileCatalog->tileSurface, '\0', TOTAL_NUM_OF_TILES * sizeof(SDL_Surface*));

    for(i = 0; i < NUM_OF_ART_FILES; i++)
    {
        if(!renderTileSurfacesFromArtFile(&tileCatalog->artFile[i], tileCatalog->vgaPalette, tileCatalog->tileSurface))
            return FALSE;
    }

    return TRUE;
}

capfil_bool generateTileCatalog(TileCatalog *tileCatalog, const char *paletteFileName)
{
    /* Open the palette file */
    if(!readVGAPalette(paletteFileName, tileCatalog->vgaPalette))
    {
        fprintf(stderr, "Cannot open: %s\n", paletteFileName);
        return FALSE;
    }

    /* Open all art files */
    if(!openArtFiles(tileCatalog))
        return FALSE;

    /* Convert all tiles to SDL surfaces */
    if(!renderTileSurfaces(tileCatalog))
        return FALSE;

    return TRUE;
}

static void freeArtFiles(ArtFile *artFile)
{
    unsigned int i;

    for(i = 0; i < NUM_OF_ART_FILES; i++)
        clearArtFile(&artFile[i]);
}

static void freeTileSurfaces(SDL_Surface **tileSurface)
{
    unsigned int i;

    for(i = 0; i < TOTAL_NUM_OF_TILES; i++)
    {
         if(tileSurface[i] != NULL)
             SDL_FreeSurface(tileSurface[i]);
    }
}

void clearTileCatalog(TileCatalog *tileCatalog)
{
    freeArtFiles(tileCatalog->artFile);
    freeTileSurfaces(tileCatalog->tileSurface);
}

static void printArtFiles(const ArtFile *artFile)
{
    capfil_long i;

    for(i = 0; i < NUM_OF_ART_FILES; i++)
    {
        printf("artfile[%u] = \n", i);
        printArtFile(&artFile[i]);
        printf("\n");
    }
}

void printTileCatalog(const TileCatalog *tileCatalog)
{
    printf("VGA palette:\n");
    printVGAPalette(tileCatalog->vgaPalette);
    printf("\nArt files:\n");
    printArtFiles(tileCatalog->artFile);
}

capfil_bool writeArtFilesInTileCatalog(TileCatalog *tileCatalog, const NumericSet *numericSet)
{
    capfil_long i;

    for(i = 0; i < numericSet->length; i++)
    {
        capfil_long currentIndex = numericSet->values[i];
        fprintf(stderr, "Updating ART file with index: %u\n", currentIndex);

        if(!writeArtFileIndex(currentIndex, &tileCatalog->artFile[currentIndex]))
            return FALSE;
    }

    return TRUE;
}

capfil_bool reloadArtFilesInTileCatalog(TileCatalog *tileCatalog, const NumericSet *numericSet)
{
    capfil_long i;

    for(i = 0; i < numericSet->length; i++)
    {
        capfil_long currentIndex = numericSet->values[i];
        fprintf(stderr, "Reloading ART file with index: %u\n", currentIndex);

        clearArtFile(&tileCatalog->artFile[currentIndex]);

        if(!readArtFileIndexOrCreateEmpty(currentIndex, &tileCatalog->artFile[currentIndex]))
            return FALSE;

        freeTileSurfacesOfArtFile(&tileCatalog->artFile[currentIndex], tileCatalog->tileSurface);

        if(!renderTileSurfacesFromArtFile(&tileCatalog->artFile[currentIndex], tileCatalog->vgaPalette, tileCatalog->tileSurface))
            return FALSE;
    }

    return TRUE;
}

capfil_bool regrabTilesInCapfil(TileCatalog *tileCatalog, const char *capfilFileName)
{
    Capfil capfil;
    capfil_bool status;

    if(!readCapfil(capfilFileName, &capfil))
        return FALSE;

    status = processCapfil(&capfil, tileCatalog);

    clearCapfil(&capfil);
    return status;
}
