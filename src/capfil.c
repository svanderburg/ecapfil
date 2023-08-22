#include "capfil.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "numericset.h"

#define MAX_LINE_SIZE 1024

/* We can lines that start with a # */
static capfil_bool lineNeedsToBeParsed(const char *line)
{
    return (strlen(line) > 0 && line[0] != '#');
}

capfil_bool readCapfilFd(FILE *file, Capfil *capfil)
{
    char line[MAX_LINE_SIZE];
    unsigned int lineNumber = 1;

    capfil->entries = NULL;
    capfil->length = 0;

    while(fgets(line, MAX_LINE_SIZE, file) != NULL)
    {
        if(lineNeedsToBeParsed(line))
        {
            capfil->entries = (CapfilEntry*)realloc(capfil->entries, (capfil->length + 1) * sizeof(CapfilEntry));

            if(!parseCapfilEntry(&capfil->entries[capfil->length], line))
            {
                fprintf(stderr, "Error parsing capfil line number: %u\n", lineNumber);
                return FALSE;
            }

            capfil->length++;
        }

        lineNumber++;
    }

    return TRUE;
}

capfil_bool readCapfil(const char *filename, Capfil *capfil)
{
    FILE *file = fopen(filename, "r");

    if(file == NULL)
    {
        fprintf(stderr, "Cannot open: %s\n", filename);
        return FALSE;
    }
    else
    {
        capfil_bool status = readCapfilFd(file, capfil);
        fclose(file);
        return status;
    }
}

void clearCapfil(Capfil *capfil)
{
    unsigned int i;

    for(i = 0; i < capfil->length; i++)
        clearCapfilEntry(&capfil->entries[i]);

    free(capfil->entries);
}

void printCapfilEntries(FILE *file, const Capfil *capfil)
{
    unsigned int i;

    for(i = 0; i < capfil->length; i++)
    {
        printCapfilEntry(file, &capfil->entries[i]);
        fprintf(file, "\n");
    }
}

capfil_bool processCapfil(const Capfil *capfil, TileCatalog *tileCatalog)
{
    capfil_long i;
    capfil_bool status;
    NumericSet numericSet;

    initNumericSet(&numericSet, capfil->length);

    for(i = 0; i < capfil->length; i++)
    {
        CapfilEntry *entry = &capfil->entries[i];
        capfil_long artFileIndex = entry->index / NUM_OF_TILES_PER_ART_FILE;

        addNumericSetValue(&numericSet, artFileIndex);

        if(!processTileInCatalog(entry, tileCatalog, artFileIndex))
        {
            clearNumericSet(&numericSet);
            return FALSE;
        }
    }

    status = writeArtFilesInTileCatalog(tileCatalog, &numericSet)
        && reloadArtFilesInTileCatalog(tileCatalog, &numericSet);

    clearNumericSet(&numericSet);
    return status;
}
