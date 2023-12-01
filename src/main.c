#include <tamtypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <libcdvd.h>
#include <sifrpc.h>

int main()
{
	printf("Hello\n");
	SifInitRpc(0);
	sceCdInit(SCECdINIT);
	
	printf("That's all folks\n");

	return 0;
}
