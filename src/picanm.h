#ifndef __PICANM_H
#define __PICANM_H
#include "capfil_types.h"

typedef enum
{
    PICANM_TYPE_NOANIM = 0x0, /* No animation */
    PICANM_TYPE_OSCIL  = 0x1, /* Oscillate */
    PICANM_TYPE_ANMFD  = 0x2, /* Forwards */
    PICANM_TYPE_ANMBK  = 0x3  /* Backwards */
}
PicAnmType;

capfil_byte deriveAnimSpeed(const capfil_long picanm);

capfil_sbyte deriveYCenterOffset(const capfil_long picanm);

capfil_sbyte deriveXCenterOffset(const capfil_long picanm);

capfil_byte deriveAnimateType(const capfil_long picanm);

void printPicanm(const capfil_long picanm);

#endif
