#include "viewer.h"
#include <SDL.h>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

#define PAGE_STEP 60

static void renderTile(const TileCatalog *tileCatalog, SDL_Texture *texture, SDL_Surface *windowSurface, SDL_Renderer *renderer, const capfil_long index)
{
    void *pixels;
    int pitch;

    if(SDL_BlitSurface(tileCatalog->tileSurface[index], NULL, windowSurface, NULL) < 0)
        fprintf(stderr, "Cannot blit surface: %s\n", SDL_GetError());

    if(SDL_LockTexture(texture, NULL, &pixels, &pitch) < 0)
        fprintf(stderr, "Cannot lock texture: %s\n", SDL_GetError());

    if(SDL_ConvertPixels(windowSurface->w, windowSurface->h,
        windowSurface->format->format,
        windowSurface->pixels, windowSurface->pitch,
        SDL_PIXELFORMAT_RGBA8888,
        pixels, pitch) < 0)
        fprintf(stderr, "Cannot convert pixels: %s\n", SDL_GetError());

    SDL_UnlockTexture(texture);

    if(SDL_RenderCopy(renderer, texture, NULL, NULL) < 0)
        fprintf(stderr, "Cannot render copy: %s\n", SDL_GetError());
}

static void clearWindowSurface(SDL_Surface *surface)
{
    memset(surface->pixels, '\0', surface->pitch * surface->h);
}

static void switchToTile(const TileCatalog *tileCatalog, SDL_Texture *texture, SDL_Surface *windowSurface, SDL_Renderer *renderer, const capfil_long index)
{
    fprintf(stderr, "index is: %u\n", index);
    clearWindowSurface(windowSurface);
    renderTile(tileCatalog, texture, windowSurface, renderer, index);
}

capfil_bool showTileViewer(TileCatalog *tileCatalog, const char *capfilFileName, capfil_long index)
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    SDL_Surface *windowSurface;
    capfil_bool active = TRUE;

    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0)
    {
        fprintf(stderr, "Cannot initialise SDL: %s\n", SDL_GetError());
        return FALSE;
    }

    window = SDL_CreateWindow("BUILD tile viewer",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        0);

    if(window == NULL)
    {
        fprintf(stderr, "Cannot create window: %s\n", SDL_GetError());
        return FALSE;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, 640, 480);

    windowSurface = SDL_CreateRGBSurface(0, WINDOW_WIDTH, WINDOW_HEIGHT, 32, 0, 0, 0, 0);

    if(windowSurface == NULL)
    {
        fprintf(stderr, "Cannot create window surface: %s\n", SDL_GetError());
        return FALSE;
    }

    texture = SDL_CreateTexture(renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STREAMING,
        windowSurface->w,
        windowSurface->h);

    if(texture == NULL)
    {
        fprintf(stderr, "Cannot create texture from surface: %s\n", SDL_GetError());
        return FALSE;
    }

    renderTile(tileCatalog, texture, windowSurface, renderer, index);

    while(active)
    {
        SDL_Event event;

        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym)
                    {
                        case SDLK_UP:
                            if(index > 0)
                            {
                                index--;
                                switchToTile(tileCatalog, texture, windowSurface, renderer, index);
                            }
                            break;
                        case SDLK_DOWN:
                            if(index < TOTAL_NUM_OF_TILES - 1)
                            {
                                index++;
                                switchToTile(tileCatalog, texture, windowSurface, renderer, index);
                            }
                            break;
                        case SDLK_HOME:
                            if(index > 0)
                            {
                                index = 0;
                                switchToTile(tileCatalog, texture, windowSurface, renderer, index);
                            }
                            break;
                        case SDLK_END:
                            if(index < TOTAL_NUM_OF_TILES - 1)
                            {
                                index = TOTAL_NUM_OF_TILES - 1;
                                switchToTile(tileCatalog, texture, windowSurface, renderer, index);
                            }
                            break;
                        case SDLK_PAGEUP:
                            if(index > 0)
                            {
                                if(index < PAGE_STEP)
                                    index = 0;
                                else
                                    index -= PAGE_STEP;

                                switchToTile(tileCatalog, texture, windowSurface, renderer, index);
                            }
                            break;
                        case SDLK_PAGEDOWN:
                            if(index < TOTAL_NUM_OF_TILES - 1)
                            {
                                if(index < TOTAL_NUM_OF_TILES - 1 - PAGE_STEP)
                                    index += PAGE_STEP;
                                else
                                    index = TOTAL_NUM_OF_TILES - 1;

                                switchToTile(tileCatalog, texture, windowSurface, renderer, index);
                            }
                            break;
                        case SDLK_u:
                            regrabTilesInCapfil(tileCatalog, capfilFileName);
                            break;
                        case SDLK_p:
                            printTileCatalog(tileCatalog);
                            break;
                        case SDLK_ESCAPE:
                            active = FALSE;
                            break;
                    }
                    break;

                case SDL_QUIT:
                    active = FALSE;
                    break;
            }
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(10);
    }

    SDL_FreeSurface(windowSurface);
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return TRUE;
}
