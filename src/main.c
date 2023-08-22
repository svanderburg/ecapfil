#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include "tilecatalog.h"
#include "viewer.h"

#define PACKAGE_NAME "ecapfil"
#define PACKAGE_VERSION "0.1"

static void printUsage(const char *command)
{
    printf("Usage: %s [OPTION]\n\n", command);

    puts(
    "This command provides a ART files viewer and extended ECAPFIL.TXT processor that\n"
    "makes it possible to assemble ART files from images specified in a textual\n"
    "specification.\n"
    );

    puts(
    "Options:\n"
    "      --operation      Specifies the operation to execute. 'viewer' (the\n"
    "                       default) displays a viewer. 'process-capfil' processes\n"
    "                       all entries in the ECAPFIL.TXT file, 'pretty-print'\n"
    "                       prints a textual representation of all data files\n"
    "                       involved."
    );

    puts(
    "      --index NUM      Displays the provided tile first (defaults to: 0)\n"
    "      --palette-filename FILENAME\n"
    "                       Specifies the name of the palette file (defaults to:\n"
    "                       PALETTE.DAT)\n"
    "      --capfil-filename FILENAME\n"
    "                       Specifies the name of the capfil file (defaults to:\n"
    "                       ECAPFIL.TXT)"
    );

    puts(
    "  -h, --help           Shows the usage of this command to the user\n"
    "  -v, --version        Shows the version of this command to the user"
    );
}

static void printVersion(const char *command)
{
    printf(
    "%s (" PACKAGE_NAME ") " PACKAGE_VERSION "\n\n"
    "Copyright (C) 2023 Sander van der Burg\n"
    , command);
}

int main(int argc, char *argv[])
{
    TileCatalog tileCatalog;

    /* Parse options */
    capfil_long index = 0;
    char *operation = "viewer";
    char *paletteFileName = "PALETTE.DAT";
    char *capfilFileName = "ECAPFIL.TXT";

    int c;
    int option_index = 0;
    struct option long_options[] =
    {
        {"operation", required_argument, 0, 'o'},
        {"index", required_argument, 0, 'i'},
        {"palette-filename", required_argument, 0, 'p'},
        {"capfil-filename", required_argument, 0, 'c'},
        {"help", no_argument, 0, 'h'},
        {"version", no_argument, 0, 'v'},
        {0, 0, 0, 0}
    };

    while((c = getopt_long(argc, argv, "hv", long_options, &option_index)) != -1)
    {
        switch(c)
        {
            case 'i':
                index = atoi(optarg);
                break;
            case 'p':
                paletteFileName = optarg;
                break;
            case 'c':
                capfilFileName = optarg;
                break;
            case 'o':
                operation = optarg;
                break;
            case 'h':
                printUsage(argv[0]);
                return 0;
            case 'v':
                printVersion(argv[0]);
                return 0;
            case '?':
                printUsage(argv[0]);
                return 1;
        }
    }

    /* Check values */
    if(index > TOTAL_NUM_OF_TILES)
    {
        fprintf(stderr, "The index should be lower than: %u\n", TOTAL_NUM_OF_TILES);
        return 1;
    }

    /* Execute operation */

    if(!generateTileCatalog(&tileCatalog, paletteFileName))
    {
        fprintf(stderr, "Cannot generate tile catalog!\n");
        clearTileCatalog(&tileCatalog);
        return 1;
    }

    if(strcmp(operation, "viewer") == 0)
    {
        if(!showTileViewer(&tileCatalog, capfilFileName, index))
        {
            fprintf(stderr, "Cannot open viewer!\n");
            clearTileCatalog(&tileCatalog);
            return 1;
        }
    }
    else if(strcmp(operation, "process-capfil") == 0)
    {
        if(!regrabTilesInCapfil(&tileCatalog, capfilFileName))
        {
            clearTileCatalog(&tileCatalog);
            return 1;
        }
    }
    else if(strcmp(operation, "pretty-print") == 0)
        printTileCatalog(&tileCatalog);
    else
    {
        fprintf(stderr, "Unknown operation: %s\n", operation);
        clearTileCatalog(&tileCatalog);
        return 1;
    }

    clearTileCatalog(&tileCatalog);

    return 0;
}
