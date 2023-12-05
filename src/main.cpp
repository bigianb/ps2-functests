#include <tamtypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <libcdvd.h>
#include <loadfile.h>
#include <sifrpc.h>

#include "memcard_test.h"

int main()
{
    printf("Hello\n");
    SifInitRpc(0);
    sceCdInit(SCECdINIT);

    int sifLoadRet = SifLoadModule("rom0:SIO2MAN", 0, NULL);
	if (sifLoadRet < 0) {
		printf("Failed to load module: SIO2MAN");
        return -1;
	}

	sifLoadRet = SifLoadModule("rom0:MCMAN", 0, NULL);
	if (sifLoadRet < 0) {
		printf("Failed to load module: MCMAN");
        return -1;
	}

	sifLoadRet = SifLoadModule("rom0:MCSERV", 0, NULL);
	if (sifLoadRet < 0) {
		printf("Failed to load module: MCSERV");
        return -1;
	}

    auto memcardTest = MemcardTest();
    memcardTest.run();

    printf("That's all folks\n");

    return 0;
}
