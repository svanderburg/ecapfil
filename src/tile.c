#include "tile.h"
#include <string.h>

static capfil_byte *rotatePixelsToTile(SDL_Surface *surface)
{
    capfil_byte *artdata = (capfil_byte*)malloc(surface->h * surface->w * sizeof(capfil_byte));
    Uint8 *pixels = (Uint8*)surface->pixels;
    int i;
    unsigned int surfacePixelPos = 0, tilePixelPos;

    for(i = 0; i < surface->h; i++)
    {
        int j;
        tilePixelPos = i;

        for(j = 0; j < surface->w; j++)
        {
            artdata[tilePixelPos] = pixels[surfacePixelPos];
            surfacePixelPos++;
            tilePixelPos += surface->h;
        }

        for(j = 0; j < surface->pitch - surface->w; j++)
            surfacePixelPos++;
    }

    return artdata;
}

void generateTileFromSDLSurface(Tile *tile, SDL_Surface *surface)
{
    tile->tilesizx = surface->w;
    tile->tilesizy = surface->h;
    tile->picanm = 0;
    tile->artdata = rotatePixelsToTile(surface);
}
