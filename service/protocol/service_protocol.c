/**
  ******************************************************************************
  * @file service_protocol.c
  * @author  leon.xie
  * @version v1.0.0
  * @date 2018-11-11 18:55:52
  * @brief  This file provides all the service_protocol functions. 
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
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "service_protocol.h"
#include "glib.h"
#include "gio/gio.h"

#include "diagnosis.h"
#include "init.h"
#include "log.h"
#include "kernel.h"

#include "media.h"
#include "net.h"

#ifdef  __cplusplus
extern "C" {
#endif

/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

static void __service_task_protocol_init(int order)
{
	switch(order)
	{
		case TASK_ORDER_BEFORE_INIT:
		case TASK_ORDER_INIT: break;
		case TASK_ORDER_RUN:
		{
		}break;
		default:
			INFO("task protocol init succ...");
			break;
	}
}

void __service_task_protocol_run(msg_t *msg_header,void *msg_data)
{
	switch(msg_header->src_task_id)
	{
		case TASK_IDLE:
		{
			MESSAGE("from idle...");
		}break;
		case TASK_MASTER:
		{
			MESSAGE("from master...");
		}break;
		case TASK_PROTOCOL:
		{
			MESSAGE("from protocol...");
		}break;
		default:
			ASSERT(0);
			break;
	}
}

/***************************************************
 * Function : service_protocol_init
 * Author : leon.xie
 * Creat Date : 2018/11/11  18:56:20
 * Description : none
 * In-Parameter : as below
 * Return : as below
 * Modify : none
 **************************************************/
int service_protocol_init(void)
{
	RETURN_VAL_IF_FAIL(0 == task_register(TASK_PROTOCOL,E2S(TASK_PROTOCOL),
	__service_task_protocol_init,__service_task_protocol_run),-1);
    return 0;
}

/***************************************************
 * Function : service_protocol_exit
 * Author : leon.xie
 * Creat Date : 2018/11/11  18:56:43
 * Description : none
 * In-Parameter : as below
 * Return : as below
 * Modify : none
 **************************************************/
void service_protocol_exit(void)
{

}

#ifdef  __cplusplus
}
#endif


