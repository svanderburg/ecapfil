#include "picanm.h"
#include <stdio.h>

capfil_byte deriveAnimSpeed(const capfil_long picanm)
{
    return (picanm & 0xff000000) >> 24;
}

capfil_sbyte deriveYCenterOffset(const capfil_long picanm)
{
    return (picanm & 0xff0000) >> 16;
}

capfil_sbyte deriveXCenterOffset(const capfil_long picanm)
{
    return (picanm & 0xff00) >> 8;
}

capfil_byte deriveAnimateType(const capfil_long picanm)
{
    return (picanm & 0xc0) >> 6;
}

capfil_byte deriveAnimateNumber(const capfil_long picanm)
{
    return (picanm & 0x3f);
}

void printPicanm(const capfil_long picanm)
{
    printf("{ animSpeed = %u, YCenterOffset = %d, XCenterOffset = %d, animateType = %u, animateNumber = %u }",
        deriveAnimSpeed(picanm),
        deriveYCenterOffset(picanm),
        deriveXCenterOffset(picanm),
        deriveAnimateType(picanm),
        deriveAnimateNumber(picanm));
}
