#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <fcntl.h>

#include "diagnosis.h"
#include "init.h"
#include "glib.h"
#include "log.h"
#include "kernel.h"

//===============================================================================
#undef VERSIONS
#define VERSIONS	"0.0.7"
static void version_usage(void)
{
	MESSAGE("###########################################");
	MESSAGE("########## version: v%-10s ##########",VERSIONS);
	MESSAGE("###########################################");
}

int
main(int argc, char *argv[])
{
	version_usage();
	kernel_init();
	main_loop_start();
	return 0;
}

