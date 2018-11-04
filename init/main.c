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

//===============================================================================
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

//===============================================================================
static GMainLoop *g_mainLoop = NULL;
static void main_loop_start(void)
{
	MESSAGE("Main loop ...\n");
	g_mainLoop = g_main_new(FALSE);
	g_main_loop_run(g_mainLoop);
	g_main_loop_unref (g_mainLoop);
	MESSAGE("Main exit...\n");
}

//===============================================================================
static void sockets_close(void)
{
	int i;
	for(i = 3;i < 32768;i++)
	{
		fcntl( i, F_SETFD, FD_CLOEXEC );
	}
}

static void catching_sigs(int signo)
{
	switch(signo)
	{
#ifdef __gnu_pth__
    	case SIGALRM:
    		signal_alarm_proc();
    		break;
#endif
        case SIGINT:
            printf("catching_sigs()->SIGINT\n");
            goto signal_exit;
            break;
        case SIGSEGV:
            printf("catching_sigs()->SIGSEGV\n");
            goto signal_exit;
            break;
        case SIGQUIT:
            printf("catching_sigs()->SIGQUIT\n");
            goto signal_exit;
            break;
        case SIGPIPE:
            printf("catching_sigs()->SIGPIPE\n");
            break;
        case SIGKILL:
            printf("catching_sigs()->SIGINT\n");
        case SIGTERM:
        	goto signal_exit;
            break;
        case SIGHUP:
            printf("catching_sigs()->SIGHUP\n");
            goto signal_exit;
            break;
        case SIGXFSZ:
            printf("catching_sigs()->SIGXFSZ\n");
            goto signal_exit;
            break;
        case SIGPWR:
            printf("catching_sigs()->SIGPWR\n");
            goto signal_exit;
            break;
        case SIGSYS:
            goto signal_exit;
            break;
        default :
            printf("catching_sigs()->SIG(%d)\n",signo);
            break;
    }

    return ;
    
signal_exit:    
	sockets_close();
	exit(EXIT_FAILURE);
}

static void signal_init(void)
{
	MESSAGE("signal init\n");
	struct sigaction act;
	
	memset(&act, 0, sizeof(struct sigaction));
	act.sa_handler = catching_sigs;
	act.sa_flags   = 0;
	sigemptyset(&act.sa_mask);		 //初始化一个空的信号集合sa_mask

	if(sigaction(SIGINT, &act, NULL) < 0)
    {
        printf("catch SIGINT failed!\n");
        exit(EXIT_FAILURE);
    }
    if(sigaction(SIGQUIT, &act, NULL) < 0)
    {
        printf("catch SIGQUIT failed!\n");
        exit(EXIT_FAILURE);
    }

    if(sigaction(SIGHUP, &act, NULL) < 0)
    {
        printf("catch SIGQUIT failed!\n");
        exit(EXIT_FAILURE);
    }
    if(sigaction(SIGXFSZ, &act, NULL) < 0)
    {
        printf("catch SIGQUIT failed!\n");
        exit(EXIT_FAILURE);
    }
    if(sigaction(SIGPWR, &act, NULL) < 0)
    {
        printf("catch SIGQUIT failed!\n");
        exit(EXIT_FAILURE);
    }
    if(sigaction(SIGSYS, &act, NULL) < 0)
    {
        printf("catch SIGQUIT failed!\n");
        exit(EXIT_FAILURE);
    }

    //忽略信号
    memset(&act, 0, sizeof(struct sigaction));
    act.sa_handler = SIG_IGN;
    act.sa_flags   = 0;
    if(sigaction(SIGPIPE, &act, NULL) < 0)
    {
        printf("catch SIGQUIT failed!\n");
        exit(EXIT_FAILURE);
    }
    memset(&act, 0, sizeof(struct sigaction));
    act.sa_handler = SIG_IGN;
    act.sa_flags   = 0;
    if(sigaction(SIGUSR1, &act, NULL) < 0)
    {
        printf("catch SIGQUIT failed!\n");
        exit(EXIT_FAILURE);
    }
    
}

//===============================================================================
#undef VERSIONS
#define VERSIONS	"0.0.6"
static void version_usage(void)
{
	MESSAGE("###########################################");
	MESSAGE("########## version: v%-10s ##########",VERSIONS);
	MESSAGE("###########################################");
}

int
main(int argc, char *argv[])
{
	signal_init();
	version_usage();
	initcalls();
	main_loop_start();
	return 0;
}

