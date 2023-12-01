#include <tamtypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <libcdvd.h>
#include <sifrpc.h>

#include "memcard_test.h"

int main()
{
	printf("Hello\n");
	SifInitRpc(0);
	sceCdInit(SCECdINIT);
	
	auto memcardTest = MemcardTest();
	memcardTest.run();

	printf("That's all folks\n");

	return 0;
}
