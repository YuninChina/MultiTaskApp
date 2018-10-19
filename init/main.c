#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include "init.h"
#include "glib.h"
#include "log.h"

extern initcall_t __initcall_start[], __initcall_end[];

static void initcalls(void)
{
	initcall_t *call;
	int result;
	INFO("%s do init call (%d)...", __func__,__initcall_end-__initcall_start);
	for (call = __initcall_start; call < __initcall_end; call++) 
	{
		result = (*call)();
		if(result < 0)
		{
			ERROR("do_initcalls(%p): error code %d\n", call,result);
		}
	}
}

static GMainLoop *g_mainLoop = NULL;
static void main_loop_start(void)
{
	MESSAGE("Main loop ...\n");
	g_mainLoop = g_main_new(FALSE);
	g_main_loop_run(g_mainLoop);
	g_main_loop_unref (g_mainLoop);
	MESSAGE("Main exit...\n");
}

int
main(int argc, char *argv[])
{
	initcalls();
	main_loop_start();
	return 0;
}

