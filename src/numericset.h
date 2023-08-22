#include "capfil_types.h"
#ifndef __NUMERICSET_H
#define __NUMERICSET_H

typedef struct
{
    capfil_long *values;
    capfil_long length;
}
NumericSet;

void initNumericSet(NumericSet *numericSet, capfil_long length);

void clearNumericSet(NumericSet *numericSet);

capfil_bool numericSetHasValue(NumericSet *numericSet, capfil_long value);

void addNumericSetValue(NumericSet *numericSet, capfil_long value);

#endif
