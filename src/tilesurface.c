#include "tilesurface.h"

static void rotateArtDataToSDLSurface(const Tile *tile, SDL_Surface *surface)
{
    capfil_long i;
    Uint8 *targetPixels = (Uint8*)surface->pixels;
    capfil_long srcPixelPos, targetPixelCount = 0;

    for(i = 0; i < tile->tilesizy; i++)
    {
        capfil_long j;
        srcPixelPos = i;

        for(j = 0; j < tile->tilesizx; j++)
        {
            targetPixels[targetPixelCount] = tile->artdata[srcPixelPos];
            srcPixelPos += tile->tilesizy;
            targetPixelCount++;
        }

        targetPixelCount += surface->pitch - tile->tilesizx; /* An SDL surface width may be padded. If so, skip over the padding bytes */
    }
}

SDL_Surface *generateSDLSurfaceForTile(const Tile *tile, const VGAPalette vgaPalette)
{
    SDL_Color colors[NUM_OF_COLOR_REGISTERS];
    SDL_Surface *surface = SDL_CreateRGBSurface(0, tile->tilesizx, tile->tilesizy, 8, 0, 0, 0, 0);

    if(surface == NULL)
    {
        fprintf(stderr, "Cannot create surface: %s\n", SDL_GetError());
        return NULL;
    }

    generateSDLPaletteColors(vgaPalette, colors);

    if(SDL_SetPaletteColors(surface->format->palette, colors, 0, NUM_OF_COLOR_REGISTERS) < 0)
    {
        fprintf(stderr, "Cannot set palette: %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        return NULL;
    }

    if(SDL_LockSurface(surface) < 0)
    {
        fprintf(stderr, "Cannot lock surface: %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        return NULL;
    }

    rotateArtDataToSDLSurface(tile, surface);

    SDL_UnlockSurface(surface);

    return surface;
}

static void transferPixelArea(SDL_Surface *tileSurface, SDL_Surface *targetSurface, capfil_short topX, capfil_short topY, capfil_short bottomX, capfil_short bottomY, capfil_short newWidth)
{
    Uint8 *tilePixels = (Uint8*)tileSurface->pixels;
    Uint8 *targetPixels = (Uint8*)targetSurface->pixels;

    Uint32 i;
    capfil_long tileSurfaceIndex = topY * tileSurface->pitch;
    capfil_long targetSurfaceIndex = 0;

    for(i = topY; i < bottomY; i++)
    {
        memcpy(targetPixels + targetSurfaceIndex, tilePixels + tileSurfaceIndex + topX, newWidth);

        tileSurfaceIndex += tileSurface->pitch;
        targetSurfaceIndex += targetSurface->pitch;
    }
}

static capfil_bool checkDimensions(SDL_Surface *tileSurface, capfil_short topX, capfil_short topY, capfil_short bottomX, capfil_short bottomY)
{
    if(topX > tileSurface->w)
    {
        fprintf(stderr, "topX: %u exceeds tile surface width: %u\n", topX, tileSurface->w);
        return FALSE;
    }

    if(bottomX > tileSurface->w)
    {
        fprintf(stderr, "bottomX: %u exceeds tile surface width: %u\n", bottomX, tileSurface->w);
        return FALSE;
    }

    if(topY > tileSurface->h)
    {
        fprintf(stderr, "topY: %u exceeds tile surface height: %u\n", topY, tileSurface->h);
        return FALSE;
    }

    if(bottomY > tileSurface->h)
    {
        fprintf(stderr, "bottomY: %u exceeds tile surface height: %u\n", bottomY, tileSurface->h);
        return FALSE;
    }

    return TRUE;
}

SDL_Surface *extractAreaFromTile(SDL_Surface *tileSurface, const VGAPalette vgaPalette, capfil_short topX, capfil_short topY, capfil_short bottomX, capfil_short bottomY)
{
    SDL_Color colors[NUM_OF_COLOR_REGISTERS];
    capfil_short newWidth = bottomX - topX;
    capfil_short newHeight = bottomY - topY;
    SDL_Surface *targetSurface;

    if(!checkDimensions(tileSurface, topX, topY, bottomX, bottomY))
        return NULL;

    targetSurface = SDL_CreateRGBSurface(0, newWidth, newHeight, 8, 0, 0, 0, 0);

    if(targetSurface == NULL)
    {
        fprintf(stderr, "Cannot create RGB surface: %s\n", SDL_GetError());
        return NULL;
    }

    generateSDLPaletteColors(vgaPalette, colors);

    if(SDL_SetPaletteColors(targetSurface->format->palette, colors, 0, NUM_OF_COLOR_REGISTERS) < 0)
    {
        fprintf(stderr, "Cannot set palette: %s\n", SDL_GetError());
        SDL_FreeSurface(targetSurface);
        return NULL;
    }

    /* Fill the surface with the transparent background color (0xff) by default */
    memset(targetSurface->pixels, 0xff, targetSurface->pitch * targetSurface->h);

    /*
     * We cannot use SDL_BlitSurface() to extract a tile. Although using SDL_BlitSurface() mostly works,
     * the algorithm disregards color indexes that refer to the same color value.
     * In the Duke3D palette, there are two color indexes that refer to the same purple color value (0xf5 and 0xff).
     * 0xff is the index used for transparency. The blit surface algorithm automatically maps any purple
     * color to 0xf5. As a consequence, sprites with a transparent background lose their transparency.
     * A slower but custom implementation can deal with this.
     */

    if(SDL_LockSurface(targetSurface) < 0)
    {
        fprintf(stderr, "Cannot lock surface: %s\n", SDL_GetError());
        SDL_FreeSurface(targetSurface);
        return NULL;
    }

    transferPixelArea(tileSurface, targetSurface, topX, topY, bottomX, bottomY, newWidth);

    SDL_UnlockSurface(targetSurface);

    return targetSurface;
}
