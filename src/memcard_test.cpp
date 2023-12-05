#include "memcard_test.h"
#include <cstdio>
#include <string.h>

#include <tamtypes.h>
#include <kernel.h>
#include <sifrpc.h>
#include <libmc.h>

bool MemcardTest::init()
{
    printf("Initialising memcard\n");
    int rval = mcInit(MC_TYPE_MC);
    printf("mcInit returned %d\n", rval);
    if (rval < 0)
    {
        printf("Failed to initialise memcard server!\n");
        return false;
    }
    return true;
}

bool MemcardTest::run()
{
    printf("Memcard test\n");

    if (!init())
    {
        printf("Init failed\n");
        return false;
    }

    printf("Init succeded\n");

    bool success = true;

    int mc_Type, mc_Free, mc_Format;
    int ret;

    int port = 0;
    int slot = 0;

    int rval = mcGetInfo(port, slot, &mc_Type, &mc_Free, &mc_Format);
    printf("mcGetInfo returned %d\n", rval);
    mcSync(0, NULL, &ret);
    printf("mcGetInfo sync returned %d\n", ret);
    printf("Type: %d Free: %d Format: %d\n", mc_Type, mc_Free, mc_Format);
    if (ret != -1)
    {
        success = false;
    }

    rval = mcGetInfo(port, slot, &mc_Type, &mc_Free, &mc_Format);
    printf("mcGetInfo returned %d\n", rval);
    mcSync(0, NULL, &ret);
    printf("mcGetInfo sync returned %d\n", ret);
    printf("Type: %d Free: %d Format: %d\n", mc_Type, mc_Free, mc_Format);
    if (ret != 0)
    {
        success = false;
    }

    const int ARRAY_ENTRIES = 64;
    sceMcTblGetDir mcDir[ARRAY_ENTRIES] __attribute__((aligned(64)));
    mcGetDir(0, 0, "/*", 0, ARRAY_ENTRIES, mcDir);
    int numEntries;
    mcSync(0, NULL, &numEntries);
    printf("mcGetDir returned %d\n\nListing of root directory on memory card:\n\n", ret);

    const char *ourDirName = "FUNCTEST";
    bool foundOurDir = false;

    for (int i = 0; i < numEntries; i++)
    {
        if ((mcDir[i].AttrFile & MC_ATTR_SUBDIR) == MC_ATTR_SUBDIR)
        {
            printf("[DIR] %s\n", mcDir[i].EntryName);
            if (0 == strcmp((char *)mcDir[i].EntryName, ourDirName))
            {
                foundOurDir = true;
            }
        }
        else
        {
            printf("%s - %d bytes\n", mcDir[i].EntryName, mcDir[i].FileSizeByte);
        }
    }

    if (foundOurDir)
    {
        rval = mcDelete(port, slot, ourDirName);
        printf("mcDelete returned %d\n", rval);
        mcSync(0, NULL, &ret);
        printf("mcDelete sync returned %d\n", ret);
    }

    rval = mcMkDir(port, slot, ourDirName);
    printf("mcMkDir returned %d\n", rval);
    mcSync(0, NULL, &ret);
    printf("mcMkDir sync returned %d\n", ret);

    char cdir[256];
    rval = mcChdir(port, slot, ourDirName, cdir);
    printf("mcChdir returned %d\n", rval);
    mcSync(0, NULL, &ret);
    printf("mcChdir sync returned %d\n", ret);
    printf("mcChdir old dir returned '%s'\n", cdir);

    rval = mcChdir(port, slot, "/notthere", cdir);
    printf("mcChdir returned %d\n", rval);
    mcSync(0, NULL, &ret);
    printf("mcChdir sync returned %d\n", ret);
    printf("mcChdir old dir returned '%s'\n", cdir);

    fflush(nullptr);

    return success;
}
