#ifndef __IO_H
#define __IO_H
#include <stdio.h>
#include "capfil_types.h"

capfil_bool readByte(FILE *file, capfil_byte *value);

capfil_bool readShort(FILE *file, capfil_short *value);

capfil_bool readLong(FILE* file, capfil_long *value);

capfil_bool writeByte(FILE *file, const capfil_byte value);

capfil_bool writeShort(FILE *file, const capfil_short value);

capfil_bool writeLong(FILE *file, const capfil_long value);

#endif
