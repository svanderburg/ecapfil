#include "numericset.h"
#include <stdlib.h>

void initNumericSet(NumericSet *numericSet, capfil_long length)
{
    numericSet->values = (capfil_long*)malloc(length * sizeof(capfil_long));
    numericSet->length = 0;
}

void clearNumericSet(NumericSet *numericSet)
{
    free(numericSet->values);
}

capfil_bool numericSetHasValue(NumericSet *numericSet, capfil_long value)
{
    unsigned int i;
    capfil_bool found = FALSE;

    for(i = 0; i < numericSet->length; i++)
    {
        if(numericSet->values[i] == value)
        {
            found = TRUE;
            break;
        }
    }

    return found;
}

void addNumericSetValue(NumericSet *numericSet, capfil_long value)
{
    if(!numericSetHasValue(numericSet, value))
    {
        numericSet->values[numericSet->length] = value;
        numericSet->length++;
    }
}
