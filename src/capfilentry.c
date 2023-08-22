#include "capfilentry.h"
#include <string.h>
#include <stdlib.h>
#include <SDL_image.h>
#include "tilesurface.h"

static capfil_bool parseLongToken(char *line, capfil_long *value)
{
    char *token = strtok(line, ",");

    if(token == NULL)
        return FALSE;
    else
    {
        *value = atoi(token);
        return TRUE;
    }
}

static capfil_bool parseShortToken(char *line, capfil_short *value)
{
    char *token = strtok(line, ",");

    if(token == NULL)
        return FALSE;
    else
    {
        *value = atoi(token);
        return TRUE;
    }
}

static capfil_bool parseStringToken(char *line, char **value)
{
    char *strToken = strtok(line, ",");

    if(strToken == NULL)
        return FALSE;
    else
    {
        size_t strTokenSize = strlen(strToken) + 1;
        *value = (char*)malloc(strTokenSize * sizeof(char));
        memcpy(*value, strToken, strTokenSize);

        return TRUE;
    }
}

static capfil_bool checkStringStartsWith(const char *str, const char *prefix)
{
    return strlen(str) >= strlen(prefix) && strncmp(str, prefix, strlen(prefix)) == 0;
}

static void processPicanmToken(const char *token, capfil_long *picanm)
{
    if(checkStringStartsWith(token, "animSpeed="))
    {
        capfil_long value = atoi(token + strlen("animSpeed="));
        *picanm = *picanm | (value << 24);
    }
    else if(checkStringStartsWith(token, "YCenterOffset="))
    {
        capfil_long value = atoi(token + strlen("YCenterOffset="));
        *picanm = *picanm | (value << 16);
    }
    else if(checkStringStartsWith(token, "XCenterOffset="))
    {
        capfil_long value = atoi(token + strlen("XCenterOffset="));
        *picanm = *picanm | (value << 8);
    }
    else if(checkStringStartsWith(token, "animateNumber="))
    {
        capfil_long value = atoi(token + strlen("animateNumber="));
        *picanm = *picanm | (value << 6);
    }
    else if(checkStringStartsWith(token, "animateType="))
    {
        capfil_long value = atoi(token + strlen("animateType="));
        *picanm = *picanm | value;
    }
}

static capfil_bool parsePicanm(char *line, capfil_long *picanm)
{
    char *value;
    *picanm = 0;

    if(parseStringToken(line, &value))
    {
        char *token = strtok(value, ";");
        processPicanmToken(token, picanm);

        token = strtok(NULL, ";");
        while(token != NULL)
        {
            processPicanmToken(token, picanm);
            token = strtok(NULL, ";");
        }
    }

    return TRUE;
}

capfil_bool parseCapfilEntry(CapfilEntry *entry, char *line)
{
    if(line == NULL)
        return FALSE;
    else
    {
        return (parseLongToken(line, &entry->index)
            && parseStringToken(NULL, &entry->filename)
            && parseShortToken(NULL, &entry->topX)
            && parseShortToken(NULL, &entry->topY)
            && parseShortToken(NULL, &entry->bottomX)
            && parseShortToken(NULL, &entry->bottomY)
            && parsePicanm(NULL, &entry->picanm));
    }
}

void clearCapfilEntry(CapfilEntry *entry)
{
    free(entry->filename);
}

void printCapfilEntry(FILE *file, const CapfilEntry *entry)
{
    fprintf(stderr, "index: %u\n", entry->index);
    fprintf(stderr, "filename: %s\n", entry->filename);
    fprintf(stderr, "topX: %u\n", entry->topX);
    fprintf(stderr, "topY: %u\n", entry->topY);
    fprintf(stderr, "bottomX: %u\n", entry->bottomX);
    fprintf(stderr, "bottomY: %u\n", entry->bottomY);
    fprintf(stderr, "picanm: %x\n", entry->picanm);
}

capfil_bool processTileInCatalog(const CapfilEntry *entry, TileCatalog *tileCatalog, const capfil_long artFileIndex)
{
    SDL_Surface *tileSurface;
    Tile tile;
    unsigned int tileIndex = entry->index - artFileIndex * NUM_OF_TILES_PER_ART_FILE;

    SDL_Surface *pictureSurface = IMG_Load(entry->filename);

    if(pictureSurface == NULL)
    {
        fprintf(stderr, "Cannot open picture surface!\n");
        return FALSE;
    }

    if(pictureSurface->format->BitsPerPixel != 8)
    {
        fprintf(stderr, "Picture surface should have 8 bits per pixel. Instead it has: %u\n", pictureSurface->format->BitsPerPixel);
        SDL_FreeSurface(pictureSurface);
        return FALSE;
    }

    tileSurface = extractAreaFromTile(pictureSurface, tileCatalog->vgaPalette, entry->topX, entry->topY, entry->bottomX, entry->bottomY);

    if(tileSurface == NULL)
    {
        SDL_FreeSurface(pictureSurface);
        return FALSE;
    }

    generateTileFromSDLSurface(&tile, tileSurface);
    tile.picanm = entry->picanm;

    replaceTileInArtFile(&tileCatalog->artFile[artFileIndex], &tile, tileIndex);

    SDL_FreeSurface(tileSurface);
    SDL_FreeSurface(pictureSurface);
    return TRUE;
}
