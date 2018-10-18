#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include "init.h"

extern initcall_t __initcall_start[], __initcall_end[];

void initcalls(void)
{
	initcall_t *call;
	int result;
	printf("%s do init call (%d)... \n", __func__,__initcall_end-__initcall_start);
	for (call = __initcall_start; call < __initcall_end; call++) 
	{
		printf("%s: addr = %p\n", __func__,call);
		result = (*call)();
		if(result < 0)
		{
			printf("do_initcalls(%p): error code %d\n", call,result);
		}
	}
}


int
main(int argc, char *argv[])
{
	initcalls();
	return 0;
}

