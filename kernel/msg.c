/**
  ******************************************************************************
  * @file msg.c
  * @author  leon.xie
  * @version v1.0.0
  * @date 2018-10-20 9:54:58
  * @brief  This file provides all the msg functions. 
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

#include "glib.h"
#include "msg.h"
#include "task.h"
#include "diagnosis.h"

#ifdef  __cplusplus
extern "C" {
#endif

/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

static int _msg_priority_compare_func (gconstpointer d1, gconstpointer d2, gpointer data)
{
	int i1, i2;
	msg_t *msg1,*msg2;
	RETURN_VAL_IF_FAIL(NULL != (msg1 = (msg_t *)d1),-1);
	RETURN_VAL_IF_FAIL(NULL != (msg2 = (msg_t *)d2),-1);
	
	i1 = msg1->priority;
	i2 = msg2->priority;

	return i1 - i2;
}


/***************************************************
 * Function : msg_send
 * Author : leon.xie
 * Creat Date : 2018/10/20  11:0:3
 * Description : none
 * In-Parameter : as below
 * Return : as below
 * Modify : none
 **************************************************/
int msg_send(msg_t *msg)
{
	GAsyncQueue *q = NULL;
	if(NULL == msg)
	{
		ASSERT(0);
		return FAIL;
	}
	q = task_async_queue_get(msg->dest_task_id);
	GOTO_LABEL_IF_FAIL(NULL != q, fail);
	GOTO_LABEL_IF_FAIL(g_async_queue_length(q) < MSG_QUEUE_MAX, fail);
	g_async_queue_push_sorted(q, (gpointer)msg,_msg_priority_compare_func,NULL);
	return SUCC;

fail:
	free(msg);
	return FAIL;
}


#ifdef  __cplusplus
}
#endif


