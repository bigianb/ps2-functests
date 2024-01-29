#include <tamtypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <libcdvd.h>
#include <loadfile.h>
#include <sifrpc.h>

#include "memcard_test.h"
#include "vu1/vu1_test.h"

int main()
{
    printf("Hello\n");
    SifInitRpc(0);
    sceCdInit(SCECdINIT);

    int sifLoadRet = SifLoadModule("rom0:XSIO2MAN", 0, NULL);
	if (sifLoadRet < 0) {
		printf("Failed to load module: XSIO2MAN");
        return -1;
	}

	sifLoadRet = SifLoadModule("rom0:XMCMAN", 0, NULL);
	if (sifLoadRet < 0) {
		printf("Failed to load module: XMCMAN");
        return -1;
	}

	sifLoadRet = SifLoadModule("rom0:XMCSERV", 0, NULL);
	if (sifLoadRet < 0) {
		printf("Failed to load module: XMCSERV");
        return -1;
	}

    bool allSuccess = true;

    auto memcardTest = MemcardTest();
    auto success = memcardTest.run();

    if (success){
        printf("Memcard tests passed\n");
    } else {
        printf(" ***** Memcard tests failed *****\n");
    }
    allSuccess &= success;

    auto vu1Test = VU1Test();
    success = vu1Test.run();

    if (success){
        printf("VU1 tests passed\n");
    } else {
        printf(" ***** VU1 tests failed *****\n");
    }
    allSuccess &= success;

    printf("That's all folks\n");

    return 0;
}
