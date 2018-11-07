/**
  ******************************************************************************
  * @file kernel.c
  * @author  leon.xie
  * @version v1.0.0
  * @date 2018-10-18 21:48:15
  * @brief  This file provides all the kernel functions. 
  ******************************************************************************
  * @attention
  *
  * File For Yunin Software Team Only
  *
  * Copyright (C), 2017-2027, Yunin Software Team
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "init.h"

#include "kernel.h"
#include "diagnosis.h"
#include "init.h"
#include "log.h"

#ifdef  __cplusplus
extern "C" {
#endif

/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

//===============================================================================
extern initcall_t __initcall_start[], __initcall_end[];
static void initcalls(void)
{
	initcall_t *call;
	int result;
	MESSAGE("%s do init call (%d)...", __func__,__initcall_end-__initcall_start);
	for (call = __initcall_start; call < __initcall_end; call++) 
	{
		result = (*call)();
		if(result < 0)
		{
			ERROR("do_initcalls(%p): error code %d\n", call,result);
		}
	}
}

void kernel_init(void)
{
	MESSAGE("do %s ...",__func__);
	initcalls();
}

void kernel_exit(void)
{
	MESSAGE("do %s ...",__func__);
}

//===============================================================================
static GMainLoop *g_mainLoop = NULL;

void main_loop_start(void)
{
	MESSAGE("Main loop ...\n");
	g_mainLoop = g_main_new(FALSE);
	g_main_loop_run(g_mainLoop);
	g_main_loop_unref (g_mainLoop);
	MESSAGE("Main exit...\n");
}

#ifdef  __cplusplus
}
#endif


