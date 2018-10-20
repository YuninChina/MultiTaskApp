/**
  ******************************************************************************
  * @file task.c
  * @author  leon.xie
  * @version v1.0.0
  * @date 2018-10-20 9:54:29
  * @brief  This file provides all the task functions. 
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
#include <string.h>

#include "diagnosis.h"
#include "glib.h"
#include "task.h"
#include "init.h"
#include "log.h"

#ifdef  __cplusplus
extern "C" {
#endif

/* Private typedef -----------------------------------------------------------*/
typedef struct task_s
{
	U32 id;  // task id
	BOOL status;
	char taskName[16];
	//
	GThread *thread;
	GAsyncQueue *syncQueue;

	//
	task_init_t init;
	task_run_t run;
}task_t;

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static GMutex g_tasksMutex;
static task_t *g_tasks;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static void *__task_route(void *arg)
{
	U32 task_id;
	task_id = (U32)arg;
	if(task_id >= TASK_BUTT)
	{
		ASSERT(0);
		return NULL;
	}
	task_t *osTask = (task_t *)&g_tasks[task_id];
	ASSERT(osTask);
	INFO("[task]: %s running ... ",osTask->taskName);
	GAsyncQueue *syncQueue = NULL;
	syncQueue = osTask->syncQueue;
	ASSERT(syncQueue);
	
	for(;;)
	{
		msg_t *os_msg = NULL;
		os_msg = (msg_t *)g_async_queue_pop(syncQueue);
		GOTO_LABEL_IF_FAIL(os_msg, over);
		// do it.
		if(osTask->run)
		{
			//run task.
			osTask->run(os_msg,os_msg->data);
		}
		//release
		if(os_msg)
			free(os_msg);
	}

over:
	ERROR("task %s exit...",osTask->taskName);
	return NULL;
}

/***************************************************
 * Function : task_async_queue_get
 * Author : leon.xie
 * Creat Date : 2018/10/20  11:10:8
 * Description : none
 * In-Parameter : as below
 * Return : as below
 * Modify : none
 **************************************************/
GAsyncQueue *task_async_queue_get(U32 task_id)
{
	GAsyncQueue *p = NULL;
	RETURN_VAL_IF_FAIL(g_tasks, NULL);
	if(task_id >= TASK_BUTT)
	{
		ASSERT(0);
		return NULL;
	}
	g_mutex_trylock(&g_tasksMutex);
	p = g_tasks[task_id].syncQueue;
	g_mutex_unlock(&g_tasksMutex);
	return p;
}

/***************************************************
 * Function : task_
 * Author : leon.xie
 * Creat Date : 2018/10/20  12:20:56
 * Description : none
 * In-Parameter : as below
 * Return : as below
 * Modify : none
 **************************************************/
int task_register(U32 task_id,const char *task_name,task_init_t init,task_run_t run)
{
	GAsyncQueue *syncQueue = NULL;
	RETURN_VAL_IF_FAIL(g_tasks, -1);
	if(task_id >= TASK_BUTT)
	{
		ASSERT(0);
		return FAIL;
	}
	if(TRUE == g_tasks[task_id].status)
		return SUCC;
		
	g_mutex_lock(&g_tasksMutex);
	g_tasks[task_id].id = task_id;
	g_tasks[task_id].status = TRUE;
	g_tasks[task_id].init = init;
	g_tasks[task_id].run = run;
	strncpy(g_tasks[task_id].taskName,task_name,sizeof(g_tasks[task_id].taskName)-1);
	syncQueue = g_async_queue_new();
	RETURN_VAL_IF_FAIL(syncQueue,-1);
	g_tasks[task_id].syncQueue = syncQueue;
	g_mutex_unlock(&g_tasksMutex);
    return SUCC;
}

/***************************************************
 * Function : task_queue_length_get
 * Author : leon.xie
 * Creat Date : 2018/10/20  12:22:9
 * Description : none
 * In-Parameter : as below
 * Return : as below
 * Modify : none
 **************************************************/
int task_queue_length_get(U32 task_id,U32 *dst_length)
{
	if(task_id >= TASK_BUTT
	|| NULL == dst_length)
	{
		ASSERT(0);
		return FAIL;
	}
	GAsyncQueue *p = task_async_queue_get(task_id);
	if(!p)
	{
		ASSERT(0);
		return FAIL;
	}
    *dst_length = g_async_queue_length(p);
    return SUCC;
}

/***************************************************
 * Function : task_create
 * Author : leon.xie
 * Creat Date : 2018/10/20  12:28:45
 * Description : none
 * In-Parameter : as below
 * Return : as below
 * Modify : none
 **************************************************/
int task_create(U32 task_id,void *(*func)(void *),void *data)
{
	GThread *thread = NULL;
	RETURN_VAL_IF_FAIL(g_tasks, -1);
	if(task_id >= TASK_BUTT)
		return FAIL;
	RETURN_VAL_IF_FAIL((thread = g_thread_new(g_tasks[task_id].taskName,func,data)),-1);
	g_tasks[task_id].thread = thread;
	return SUCC;
}

/***************************************************
 * Function : task_start
 * Author : leon.xie
 * Creat Date : 2018/10/20  12:26:49
 * Description : none
 * In-Parameter : as below
 * Return : as below
 * Modify : none
 **************************************************/
int task_start(void)
{
	U32 id;
	int order;
	//
	for(order = TASK_ORDER_BEFORE_INIT;order < TASK_ORDER_BUTT;order++)
	{
		// task init
		for(id = TASK_IDLE;id < TASK_BUTT;id++)
		{
			if(FALSE == g_tasks[id].status)
				continue;
			if(g_tasks[id].init)
				g_tasks[id].init(order);
		}
	}
	//task start
	for(id = TASK_IDLE;id < TASK_BUTT;id++)
	{
		if(FALSE == g_tasks[id].status)
			continue;
		if(task_create(id,__task_route,(void *)id))
		{
			ASSERT(0);
			continue;
		}
	}	
	//
	return SUCC;
}

/***************************************************
 * Function : task_init
 * Author : leon.xie
 * Creat Date : 2018/10/20  11:26:12
 * Description : none
 * In-Parameter : as below
 * Return : as below
 * Modify : none
 **************************************************/
static int task_init(void)
{
	U32 id;
	INFO("do %s ...",__func__);
	g_tasks = malloc(sizeof(task_t)*TASK_BUTT);
	RETURN_VAL_IF_FAIL(g_tasks, -1);
	memset(g_tasks,0,(sizeof(task_t)*TASK_BUTT));
	for(id = TASK_IDLE;id < TASK_BUTT;id++)
	{
		g_tasks[id].id = id;
		g_tasks[id].status = FALSE;
		g_tasks[id].thread = NULL;
		g_tasks[id].syncQueue = NULL;
		g_tasks[id].init = NULL;
		g_tasks[id].run = NULL;
	}		
    return 0;
}

/***************************************************
 * Function : task_exit
 * Author : leon.xie
 * Creat Date : 2018/10/20  11:28:44
 * Description : none
 * In-Parameter : as below
 * Return : as below
 * Modify : none
 **************************************************/
static void task_exit(void)
{
	//mutex init
	g_mutex_clear(&g_tasksMutex);
    if(g_tasks)
    {
    	free(g_tasks);
    	g_tasks = NULL;
    }
}

cores_init(task_init);
cores_exit(task_exit);

#ifdef  __cplusplus
}
#endif


