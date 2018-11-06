/**
  ******************************************************************************
  * @file signal.c
  * @author  leon.xie
  * @version v1.0.0
  * @date 2018-11-6 23:07:16
  * @brief  This file provides all the signal functions. 
  ******************************************************************************
  * @attention
  *
  * File For Yunin Software Team Only
  *
  * Copyright (C), 2017-2027, Yunin Software Team
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
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

#include "signal.h"

#ifdef  __cplusplus
extern "C" {
#endif

/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
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

static int signal_init(void)
{
	MESSAGE("do %s ...",__func__);
	
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
    return 0;
}


static void signal_exit(void)
{
	MESSAGE("do %s ...",__func__);
}

systems_init(signal_init);
systems_exit(signal_exit);


#ifdef  __cplusplus
}
#endif


