#include "vgapalette.h"
#include "io.h"

capfil_bool readVGAPaletteFd(FILE *file, VGAPalette palette)
{
    unsigned int i;

    for(i = 0; i < VGA_PALETTE_SIZE; i++)
    {
        if(!readByte(file, &palette[i]))
            return FALSE;
    }

    return TRUE;
}

capfil_bool readVGAPalette(const char *filename, VGAPalette palette)
{
    FILE *file = fopen(filename, "rb");

    if(file == NULL)
        return FALSE;
    else
    {
        capfil_bool status = readVGAPaletteFd(file, palette);
        fclose(file);
        return status;
    }
}

void printVGAPalette(const VGAPalette palette)
{
    capfil_long i;

    for(i = 0; i < VGA_PALETTE_SIZE; i += NUM_OF_COLOR_COMPONENTS)
        printf("{ r: %u, g: %u, b: %u }\n", palette[i], palette[i + 1], palette[i + 2]);
}

void generateSDLPaletteColors(const VGAPalette vgaPalette, SDL_Color *colors)
{
    unsigned int i, vgaPaletteIndex = 0;
    /* The size of each color component in VGA mode (6-bits) is different than modern displays (8-bits). We need to shift it to match it */
    unsigned int bitsToLShift = 8 - COLOR_COMPONENT_DEPTH;

    for(i = 0; i < NUM_OF_COLOR_REGISTERS; i++)
    {
        SDL_Color *color = &colors[i];

        color->r = vgaPalette[vgaPaletteIndex] << bitsToLShift;
        vgaPaletteIndex++;
        color->g = vgaPalette[vgaPaletteIndex] << bitsToLShift;
        vgaPaletteIndex++;
        color->b = vgaPalette[vgaPaletteIndex] << bitsToLShift;
        vgaPaletteIndex++;
        color->a = 0;
    }
}
