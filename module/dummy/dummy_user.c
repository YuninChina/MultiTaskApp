/**
  ******************************************************************************
  * @file dummy_user.c
  * @author  leon.xie
  * @version v1.0.0
  * @date 2018-11-5 22:24:49
  * @brief  This file provides all the dummy_user functions. 
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

#include "dummy.h"

#ifdef  __cplusplus
extern "C" {
#endif

/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static GAsyncQueue *g_aqDummyUser = NULL;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

static gpointer dummy_user_task_routine(gpointer data)
{
	RETURN_VAL_IF_FAIL(g_aqDummyUser, NULL);
	dummy_user_info_t dummy;
	while(1)
	{
		bzero(&dummy,sizeof(dummy));
		//阻塞式读取.
		GOTO_LABEL_IF_FAIL(0 == dummy_user_recv(&dummy),fail);
		DEBUG("res = %d",dummy.res);
	}
	
fail:
	ERROR("dummy_user_task exit...");
	return NULL;
}

/***************************************************
 * Function : dummy_user_send
 * Author : leon.xie
 * Creat Date : 2018/11/05  22:33:44
 * Description : none
 * In-Parameter : as below
 * Return : as below
 * Modify : none
 **************************************************/
int dummy_user_send(dummy_user_info_t *usrInput)
{
	RETURN_VAL_IF_FAIL(g_aqDummyUser && usrInput, -1);
	dummy_user_info_t *usrInfoNode = NULL;
	usrInfoNode = malloc(sizeof(dummy_user_info_t));
	RETURN_VAL_IF_FAIL(usrInfoNode, -1);
	memcpy(usrInfoNode,usrInput,sizeof(dummy_user_info_t));
	g_async_queue_push(g_aqDummyUser,(gpointer)usrInfoNode);
    return 0;
}

/***************************************************
 * Function : dummy_user_recv
 * Author : leon.xie
 * Creat Date : 2018/11/05  22:40:40
 * Description : none
 * In-Parameter : as below
 * Return : as below
 * Modify : none
 **************************************************/
int dummy_user_recv(dummy_user_info_t *usrOutput)
{
	RETURN_VAL_IF_FAIL(g_aqDummyUser && usrOutput, -1);
	gpointer data = NULL;
	//阻塞式读取.
	data = g_async_queue_pop(g_aqDummyUser);
	RETURN_VAL_IF_FAIL(data, -1);
	memcpy(usrOutput,data,sizeof(dummy_user_info_t));
	free(data);
    return 0;
}



/***************************************************
 * Function : dummy_user_init
 * Author : leon.xie
 * Creat Date : 2018/11/05  22:25:45
 * Description : none
 * In-Parameter : as below
 * Return : as below
 * Modify : none
 **************************************************/
int dummy_user_init(void)
{
	g_aqDummyUser = g_async_queue_new();
	RETURN_VAL_IF_FAIL(g_aqDummyUser, -1);
	g_thread_new("dummy_user_task", dummy_user_task_routine,NULL);
    return 0;
}

/***************************************************
 * Function : dummy_user_
 * Author : leon.xie
 * Creat Date : 2018/11/05  22:25:57
 * Description : none
 * In-Parameter : as below
 * Return : as below
 * Modify : none
 **************************************************/
void dummy_user_exit(void)
{
    
}

#ifdef  __cplusplus
}
#endif


