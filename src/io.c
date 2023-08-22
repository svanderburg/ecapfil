#include "io.h"

capfil_bool readByte(FILE *file, capfil_byte *value)
{
    int byte = fgetc(file);

    if(byte == EOF)
        return FALSE;
    else
    {
        *value = byte;
        return TRUE;
    }
}

capfil_bool readShort(FILE *file, capfil_short *value)
{
    capfil_short readValue;

    if(fread(&readValue, sizeof(capfil_short), 1, file) == 1)
    {
#if BUILD_BIG_ENDIAN == 1
	*value = (readValue & 0xff) << 8 | (readValue & 0xff00) >> 8;
#else
	/* Byte swap it */
	*value = readValue;
#endif
	return TRUE;
    }
    else
        return FALSE;
}

capfil_bool readLong(FILE* file, capfil_long *value)
{
    capfil_long readValue;

    if(fread(&readValue, sizeof(capfil_long), 1, file) == 1)
    {
#if BUILD_BIG_ENDIAN == 1
        /* Byte swap it */
        *value = (readValue & 0xff) << 24 | (readValue & 0xff00) << 8 | (readValue & 0xff0000) >> 8 | (readValue & 0xff000000) >> 24;
#else
        *value = readValue;
#endif
        return TRUE;
    }
    else
        return FALSE;
}

capfil_bool writeByte(FILE *file, const capfil_byte value)
{
    return (fputc(value, file) != EOF);
}

capfil_bool writeShort(FILE *file, const capfil_short value)
{
#if BUILD_BIG_ENDIAN == 1
    /* Byte swap it */
    capfil_short writeValue = (value & 0xff) << 8 | (value & 0xff00) >> 8;
#else
    capfil_short writeValue = value;
#endif

    return (fwrite(&writeValue, sizeof(capfil_short), 1, file) == 1);
}

capfil_bool writeLong(FILE *file, const capfil_long value)
{
#if IFF_BIG_ENDIAN == 1
    /* Byte swap it */
    capfil_long writeValue = (value & 0xff) << 24 | (value & 0xff00) << 8 | (value & 0xff0000) >> 8 | (value & 0xff000000) >> 24;
#else
    capfil_long writeValue = value;
#endif

    return (fwrite(&writeValue, sizeof(capfil_long), 1, file) == 1);
}
