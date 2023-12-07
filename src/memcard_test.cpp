#include "memcard_test.h"
#include <cstdio>
#include <string.h>

#include <tamtypes.h>
#include <kernel.h>
#include <sifrpc.h>
#include <libmc.h>

bool MemcardTest::init()
{
    printf("Initialising memcard using MC_TYPE_XMC\n");
    int rval = mcInit(MC_TYPE_XMC);
    printf("mcInit returned %d\n", rval);
    if (rval < 0)
    {
        printf("Failed to initialise memcard server!\n");
        return false;
    }
    return true;
}

bool containsDir(sceMcTblGetDir mcDir[], int numEntries, const char* targetDir)
{
    bool found = false;

    for (int i = 0; i < numEntries && !found; i++)
    {
        if ((mcDir[i].AttrFile & MC_ATTR_SUBDIR) == MC_ATTR_SUBDIR)
        {
            if (0 == strcmp((char *)mcDir[i].EntryName, targetDir))
            {
                found = true;
            }
        }
    }
    return found;
}

void syncDeleteDir(int port, int slot, const char* dirName)
{
    int rval = mcDelete(port, slot, dirName);
    printf("    mcDelete returned %d\n", rval);
    int ret;
    mcSync(0, NULL, &ret);
    printf("    mcDelete sync returned %d\n", ret);
}

void syncMkDir(int port, int slot, const char* dirName)
{
    int rval = mcMkDir(port, slot, dirName);
    printf("mcMkDir '%s' returned %d\n", dirName, rval);
    int ret;
    mcSync(0, NULL, &ret);
    printf("    mcMkDir sync returned %d\n", ret);
}

bool syncMcChdir(int port, int slot, const char* dirName, int expectedStatus, const char* expectedReturnDir)
{
    bool success = true;
    char cdir[256];
    // mcChdir does not zero teminate the return.
    memset(cdir, 0, 256);

    int rval = mcChdir(port, slot, dirName, cdir);
    printf("mcChdir '%s' returned %d\n", dirName, rval);
    int ret;
    mcSync(0, NULL, &ret);
    printf("    mcChdir sync returned %d\n", ret);
    printf("    mcChdir dir returned '%s'\n", cdir);
    if (ret != expectedStatus){
        printf(" **** ERROR: Expected '%d' to be returned from mcChdir ****\n", expectedStatus);
        success = false;
    }
    if (strcmp(cdir, expectedReturnDir) != 0){
        printf(" **** ERROR: Expected dir '%s' to be returned from mcChdir ****\n", expectedReturnDir);
        success = false;
    }
    return success;
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

    // Change to root directory
    syncMcChdir(port, slot, "/", 0, "/");
    
    const int ARRAY_ENTRIES = 64;
    sceMcTblGetDir mcDir[ARRAY_ENTRIES] __attribute__((aligned(64)));
    ret = mcGetDir(0, 0, "/*", 0, ARRAY_ENTRIES, mcDir);
    int numEntries;
    mcSync(0, NULL, &numEntries);
    printf("mcGetDir /* returned %d and %d entries\n", ret, numEntries);

    const char *ourDirName = "FUNCTEST";
    const char *ourAbsoluteDirName = "/FUNCTEST";
    const char *ourDotDotDirName = "../FUNCTEST";

    const char *functest2Name = "FUNCTEST2";

    if (containsDir(mcDir, numEntries, ourDirName))
    {
        printf("dir %s exists, so deleting it\n", ourDirName);
        syncDeleteDir(port, slot, ourDirName);
    }

    if (containsDir(mcDir, numEntries, functest2Name))
    {
        printf("dir %s exists, so deleting it\n", functest2Name);
        syncDeleteDir(port, slot, functest2Name);
    }

    syncMkDir(port, slot, ourDirName);
    syncMkDir(port, slot, functest2Name);
    
    success &= syncMcChdir(port, slot, ourDirName, 0, ourAbsoluteDirName);
    success &= syncMcChdir(port, slot, "/notthere", -4, ourAbsoluteDirName);
    success &= syncMcChdir(port, slot, "/nothere", -4, ourAbsoluteDirName);


    // cd .. beyond the root
    success &= syncMcChdir(port, slot, "/", 0, "/");
    success &= syncMcChdir(port, slot, "..", 0, "/");
    success &= syncMcChdir(port, slot, "../notthere", -4, "/");
    success &= syncMcChdir(port, slot, ourDotDotDirName, 0, ourAbsoluteDirName);
    success &= syncMcChdir(port, slot, "../FUNCTEST2", 0, "/FUNCTEST2");
    success &= syncMcChdir(port, slot, "../../../../FUNCTEST", 0, "/FUNCTEST");

    return success;
}

/*
          PCSX2 output

Hello
loadmodule: fname rom0:XSIO2MAN args 0 arg 
[  525.0118] RegisterLibraryEntries:  sio2man version 1.02
loadmodule: id 25, ret 0
loadmodule: fname rom0:XMCMAN args 0 arg 
[  525.0264] RegisterLibraryEntries:    mcman version 2.03
loadmodule: id 26, ret 0
loadmodule: fname rom0:XMCSERV args 0 arg 
[  525.0317] RegisterLibraryEntries:   mcserv version 1.01
loadmodule: id 27, ret 0
Memcard test
Initialising memcard using MC_TYPE_XMC
[  525.0359] [EE] Skipping timeout loop at 0x00100D50 -> 0x00100D70
mcInit returned 0
Init succeded
mcGetInfo returned 0
    mcGetInfo sync returned -1
    Type: 2 Free: 7562 Format: 1
mcGetInfo returned 0
    mcGetInfo sync returned 0
    Type: 2 Free: 7562 Format: 1
mcChdir '/' returned 0
    mcChdir sync returned 0
    mcChdir dir returned '/'
mcGetDir /* returned 0 and 3 entries
dir FUNCTEST exists, so deleting it
    mcDelete returned 0
[  525.2720] OSD [MemoryCardSave0]: Memory Card 'Mcd001.ps2' was saved to storage.
    mcDelete sync returned 0
dir FUNCTEST2 exists, so deleting it
    mcDelete returned 0
    mcDelete sync returned 0
mcMkDir 'FUNCTEST' returned 0
    mcMkDir sync returned 0
mcMkDir 'FUNCTEST2' returned 0
    mcMkDir sync returned 0
mcChdir 'FUNCTEST' returned 0
    mcChdir sync returned 0
    mcChdir dir returned '/FUNCTEST'
mcChdir '/notthere' returned 0
    mcChdir sync returned -4
    mcChdir dir returned '/FUNCTEST'
mcChdir '/nothere' returned 0
    mcChdir sync returned -4
    mcChdir dir returned '/FUNCTEST'
mcChdir '/' returned 0
    mcChdir sync returned 0
    mcChdir dir returned '/'
mcChdir '..' returned 0
    mcChdir sync returned 0
    mcChdir dir returned '/'
mcChdir '../notthere' returned 0
    mcChdir sync returned -4
    mcChdir dir returned '/'
mcChdir '../FUNCTEST' returned 0
    mcChdir sync returned 0
    mcChdir dir returned '/FUNCTEST'
mcChdir '../FUNCTEST2' returned 0
    mcChdir sync returned 0
    mcChdir dir returned '/FUNCTEST2'
mcChdir '../../../../FUNCTEST' returned 0
    mcChdir sync returned 0
    mcChdir dir returned '/FUNCTEST'
Memcard tests passed
That's all folks


-------------------------------------------------
Current Play HLE output

Hello
Memcard test
Initialising memcard using MC_TYPE_XMC
mcInit returned 0
Init succeded
mcGetInfo returned 0
    mcGetInfo sync returned -1
    Type: 2 Free: 8192 Format: 1
mcGetInfo returned 0
    mcGetInfo sync returned 0
    Type: 2 Free: 8192 Format: 1
mcChdir '/' returned 0
    mcChdir sync returned 0
    mcChdir dir returned '/'
mcGetDir /* returned 0 and 7 entries
dir FUNCTEST exists, so deleting it
    mcDelete returned 0
    mcDelete sync returned 0
dir FUNCTEST2 exists, so deleting it
    mcDelete returned 0
    mcDelete sync returned 0
mcMkDir 'FUNCTEST' returned 0
    mcMkDir sync returned 0
mcMkDir 'FUNCTEST2' returned 0
    mcMkDir sync returned 0
mcChdir 'FUNCTEST' returned 0
    mcChdir sync returned 0
    mcChdir dir returned '/'
 **** ERROR: Expected dir '/FUNCTEST' to be returned from mcChdir ****
mcChdir '/notthere' returned 0
    mcChdir sync returned -4
    mcChdir dir returned '/FUNCTEST'
mcChdir '/nothere' returned 0
    mcChdir sync returned -4
    mcChdir dir returned '/FUNCTEST'
mcChdir '/' returned 0
    mcChdir sync returned 0
    mcChdir dir returned '/FUNCTEST'
 **** ERROR: Expected dir '/' to be returned from mcChdir ****
mcChdir '..' returned 0
    mcChdir sync returned 0
    mcChdir dir returned '/'
mcChdir '../notthere' returned 0
    mcChdir sync returned 0
    mcChdir dir returned '/'
 **** ERROR: Expected '-4' to be returned from mcChdir ****
mcChdir '../FUNCTEST' returned 0
    mcChdir sync returned 0
    mcChdir dir returned '/'
 **** ERROR: Expected dir '/FUNCTEST' to be returned from mcChdir ****
mcChdir '../FUNCTEST2' returned 0
    mcChdir sync returned 0
    mcChdir dir returned '/'
 **** ERROR: Expected dir '/FUNCTEST2' to be returned from mcChdir ****
mcChdir '../../../../FUNCTEST' returned 0
    mcChdir sync returned 0
    mcChdir dir returned '/'
 **** ERROR: Expected dir '/FUNCTEST' to be returned from mcChdir ****
 ***** Memcard tests failed *****
That's all folks

*/