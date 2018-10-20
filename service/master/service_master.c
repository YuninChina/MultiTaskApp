/**
  ******************************************************************************
  * @file service_master.c
  * @author  leon.xie
  * @version v1.0.0
  * @date 2018-10-20 13:44:38
  * @brief  This file provides all the service_master functions. 
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

#include "service_master.h"

#ifdef  __cplusplus
extern "C" {
#endif

/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static gpointer __service_master_thread_routine(gpointer data)
{
	msg_t *msgMaster = NULL;
	while(1)
	{
		msgMaster = malloc(sizeof(msg_t));
		GOTO_LABEL_IF_FAIL(msgMaster, over);
		msgMaster->src_task_id = TASK_MASTER;
		msgMaster->dest_task_id = TASK_IDLE;
		msgMaster->priority = MSG_PRIORITY_LOW;		
		msg_send(msgMaster);
		INFO("sleep 13s ...");
		sleep(13);
	}
	
over:
	ERROR("thread exit...");
	return NULL;
}


static void __service_task_master_init(int order)
{
	switch(order)
	{
		case TASK_ORDER_BEFORE_INIT:
		case TASK_ORDER_INIT: break;
		case TASK_ORDER_RUN:
		default:
			INFO("task master init succ...");
			g_thread_new("MasterThread",__service_master_thread_routine,NULL);
			break;
	}
}

void __service_task_master_run(msg_t *msg_header,void *msg_data)
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
 * Function : service_master_init
 * Author : leon.xie
 * Creat Date : 2018/10/20  13:50:11
 * Description : none
 * In-Parameter : as below
 * Return : as below
 * Modify : none
 **************************************************/
int service_master_init(void)
{
	RETURN_VAL_IF_FAIL(0 == task_register(TASK_MASTER,E2S(TASK_MASTER),__service_task_master_init,__service_task_master_run),-1);
    return 0;
}

/***************************************************
 * Function : service_master_exit
 * Author : leon.xie
 * Creat Date : 2018/10/20  13:50:42
 * Description : none
 * In-Parameter : as below
 * Return : as below
 * Modify : none
 **************************************************/
void service_master_exit(void)
{

}

#ifdef  __cplusplus
}
#endif


