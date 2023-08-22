#ifndef __CAPFIL_H
#define __CAPFIL_H

typedef struct Capfil Capfil;

#include "capfil_types.h"
#include "capfilentry.h"

struct Capfil
{
    CapfilEntry *entries;
    capfil_long length;
};

capfil_bool readCapfilFd(FILE *file, Capfil *capfil);

capfil_bool readCapfil(const char *filename, Capfil *capfil);

void clearCapfil(Capfil *capfil);

void printCapfilEntries(FILE *file, const Capfil *capfil);

capfil_bool processCapfil(const Capfil *capfil, TileCatalog *tileCatalog);

#endif
