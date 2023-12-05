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
    printf("    mcGetInfo sync returned %d\n", ret);
    printf("    Type: %d Free: %d Format: %d\n", mc_Type, mc_Free, mc_Format);
    if (ret != -1)
    {
        success = false;
    }

    rval = mcGetInfo(port, slot, &mc_Type, &mc_Free, &mc_Format);
    printf("mcGetInfo returned %d\n", rval);
    mcSync(0, NULL, &ret);
    printf("    mcGetInfo sync returned %d\n", ret);
    printf("    Type: %d Free: %d Format: %d\n", mc_Type, mc_Free, mc_Format);
    if (ret != 0)
    {
        success = false;
    }

    char cdir[256];
    // Change to root directory
    rval = mcChdir(port, slot, "/", cdir);
    printf("mcChdir '%s' returned %d\n", "/", rval);
    mcSync(0, NULL, &ret);
    printf("    mcChdir sync returned %d\n", ret);
    printf("    mcChdir old dir returned '%s'\n", cdir);
    if (ret != 0){
        printf(" **** ERROR: Expected 0 to be returned from mcChdir ****\n");
        success = false;
    }
    if (strcmp(cdir, "/") != 0){
        printf(" **** ERROR: Expected old dir '/' to be returned from mcChdir ****\n");
        success = false;
    }

    const int ARRAY_ENTRIES = 64;
    sceMcTblGetDir mcDir[ARRAY_ENTRIES] __attribute__((aligned(64)));
    ret = mcGetDir(0, 0, "/*", 0, ARRAY_ENTRIES, mcDir);
    int numEntries;
    mcSync(0, NULL, &numEntries);
    printf("mcGetDir /* returned %d and %d entries", ret, numEntries);

    const char *ourDirName = "FUNCTEST";
    const char *ourAbsoluteDirName = "/FUNCTEST";
    bool foundOurDir = false;

    for (int i = 0; i < numEntries; i++)
    {
        if ((mcDir[i].AttrFile & MC_ATTR_SUBDIR) == MC_ATTR_SUBDIR)
        {
            if (0 == strcmp((char *)mcDir[i].EntryName, ourDirName))
            {
                foundOurDir = true;
            }
        }
    }

    if (foundOurDir)
    {
        printf("dir %s exists, so deleting it\n", ourDirName);
        rval = mcDelete(port, slot, ourDirName);
        printf("    mcDelete returned %d\n", rval);
        mcSync(0, NULL, &ret);
        printf("    mcDelete sync returned %d\n", ret);
    }

    rval = mcMkDir(port, slot, ourDirName);
    printf("mcMkDir '%s' returned %d\n", ourDirName, rval);
    mcSync(0, NULL, &ret);
    printf("    mcMkDir sync returned %d\n", ret);
    
    rval = mcChdir(port, slot, ourDirName, cdir);
    printf("mcChdir '%s' returned %d\n", ourDirName, rval);
    mcSync(0, NULL, &ret);
    printf("    mcChdir sync returned %d\n", ret);
    printf("    mcChdir dir returned '%s'\n", cdir);
    if (ret != 0){
        printf(" **** ERROR: Expected 0 to be returned from mcChdir ****\n");
        success = false;
    }
    if (strcmp(cdir, ourAbsoluteDirName) != 0){
        printf(" **** ERROR: Expected dir '%s' to be returned from mcChdir ****\n", ourAbsoluteDirName);
        success = false;
    }

    rval = mcChdir(port, slot, "/notthere", cdir);
    printf("mcChdir '/notthere' returned %d\n", rval);
    mcSync(0, NULL, &ret);
    printf("    mcChdir sync returned %d\n", ret);
    printf("    mcChdir dir returned '%s'\n", cdir);
    // Expect -4 to be returned from sync and the old dir to be ourDirName
    if (ret != -4){
        printf(" **** ERROR: Expected -4 to be returned from mcChdir ****\n");
        success = false;
    }
    if (strcmp(cdir, ourAbsoluteDirName) != 0){
        printf(" **** ERROR: Expected dir '%s' to be returned from mcChdir ****\n", ourAbsoluteDirName);
        success = false;
    }


    rval = mcChdir(port, slot, "/nothere", cdir);
    printf("mcChdir '/nothere' returned %d\n", rval);
    mcSync(0, NULL, &ret);
    printf("    mcChdir sync returned %d\n", ret);
    printf("    mcChdir dir returned '%s'\n", cdir);
    // Expect -4 to be returned from sync and the old dir to be ourDirName
    // Note that the failure to chdir does not change the working dir.
    if (ret != -4){
        printf(" **** ERROR: Expected -4 to be returned from mcChdir ****\n");
        success = false;
    }
    if (strcmp(cdir, ourAbsoluteDirName) != 0){
        printf(" **** ERROR: Expected dir '%s' to be returned from mcChdir ****\n", ourAbsoluteDirName);
        success = false;
    }

    return success;
}
