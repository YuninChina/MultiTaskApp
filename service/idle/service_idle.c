/**
  ******************************************************************************
  * @file service_idle.c
  * @author  leon.xie
  * @version v1.0.0
  * @date 2018-10-20 13:44:42
  * @brief  This file provides all the service_idle functions. 
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

#include "service_idle.h"

#ifdef  __cplusplus
extern "C" {
#endif

/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static gpointer __service_idle_thread_routine(gpointer data)
{
	msg_t *msgIdle = NULL;
	while(1)
	{
		msgIdle = malloc(sizeof(msg_t));
		GOTO_LABEL_IF_FAIL(msgIdle, over);
		msgIdle->src_task_id = TASK_IDLE;
		msgIdle->dest_task_id = TASK_MASTER;
		msgIdle->priority = MSG_PRIORITY_LOW;		
		msg_send(msgIdle);
		INFO("sleep 5s ...");
		sleep(5);
	}
	
over:
	ERROR("thread exit...");
	return NULL;
}


static void __service_task_idle_init(int order)
{
	switch(order)
	{
		case TASK_ORDER_BEFORE_INIT:
		case TASK_ORDER_INIT: break;
		case TASK_ORDER_RUN:
		{
			g_thread_new("IdleThread",__service_idle_thread_routine,NULL);
		}break;
		default:
			INFO("task idle init succ...");
			break;
	}
}

void __service_task_idle_run(msg_t *msg_header,void *msg_data)
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
		default:
			ASSERT(0);
			break;
	}
}

/***************************************************
 * Function : service_idle_init
 * Author : leon.xie
 * Creat Date : 2018/07/09  23:18:12
 * Description : none
 * In-Parameter : as below
 * Return : as below
 * Modify : none
 **************************************************/
int service_idle_init(void)
{
	RETURN_VAL_IF_FAIL(0 == task_register(TASK_IDLE,E2S(TASK_IDLE),__service_task_idle_init,__service_task_idle_run),-1);
    return 0;
}

/***************************************************
 * Function : service_idle_exit
 * Author : leon.xie
 * Creat Date : 2018/07/09  23:18:29
 * Description : none
 * In-Parameter : as below
 * Return : as below
 * Modify : none
 **************************************************/
void service_idle_exit(void)
{
   	
}

#ifdef  __cplusplus
}
#endif


