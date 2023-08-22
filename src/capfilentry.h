#ifndef __CAPFILENTRY_H
#define __CAPFILENTRY_H

typedef struct CapfilEntry CapfilEntry;

#include <stdio.h>
#include "capfil_types.h"
#include "tilecatalog.h"

struct CapfilEntry
{
    capfil_long index;
    char *filename;
    capfil_short topX;
    capfil_short topY;
    capfil_short bottomX;
    capfil_short bottomY;
    capfil_long picanm;
};

capfil_bool parseCapfilEntry(CapfilEntry *entry, char *line);

void clearCapfilEntry(CapfilEntry *entry);

void printCapfilEntry(FILE *file, const CapfilEntry *entry);

capfil_bool processTileInCatalog(const CapfilEntry *entry, TileCatalog *tileCatalog, const capfil_long artFileIndex);

#endif
