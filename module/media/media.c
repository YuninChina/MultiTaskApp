/**
  ******************************************************************************
  * @file media.c
  * @author  leon.xie
  * @version v1.0.0
  * @date 2018-10-20 16:42:33
  * @brief  This file provides all the media functions. 
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

#include "glib.h"
#include "log.h"
#include "init.h"

#include "diagnosis.h"

#include "media.h"

#ifdef  __cplusplus
extern "C" {
#endif

/* Private typedef -----------------------------------------------------------*/
typedef struct media_info_node_s {
    /*< private >*/
    GMutex mutex;
    GCond cond;
    BOOL updateFlag;
    //info
    media_info_t info;
}media_info_node_t;


static media_info_node_t g_mediaInfoNodes[MEDIA_TYPE_BUTT];
static GMutex g_mediaInfoNodeMutexLocks[MEDIA_TYPE_BUTT];
static BOOL g_mediaModuleState = FALSE;  // 0: 模块未初始化 1:模块已初始化

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/***************************************************
 * Function : media_read
 * Author : leon.xie
 * Creat Date : 2018/11/10  20:23:55
 * Description : 音视频读取
 * In-Parameter : as below
 * Return : as below
 * Modify : none
 **************************************************/
int media_read(media_type_e type,media_info_t *mediaOutput,L64 time)
{
	RETURN_VAL_IF_FAIL_ARGS(TRUE == g_mediaModuleState,-1,DIAGNO_STR_EUNINIT);
	RETURN_VAL_IF_FAIL_ARGS(type < MEDIA_TYPE_BUTT && mediaOutput, -1,DIAGNO_STR_EINVAL);
	int ret = -1;
	if(time < 0)
	{
		g_cond_wait (&g_mediaInfoNodes[type].cond, &g_mediaInfoNodes[type].mutex);
	}
	else
	{
		gint64 end_time = g_get_monotonic_time () + time;
		g_cond_wait_until (&g_mediaInfoNodes[type].cond, &g_mediaInfoNodes[type].mutex, end_time);
	}
	
	if(false == g_mediaInfoNodes[type].updateFlag)
	{
		//立即返回时.
		(0 == time)?(ret = 0):(ret = -1);
	}
	else
	{
		ret = 0;
	}
	//
	memcpy(mediaOutput,&g_mediaInfoNodes[type].info,sizeof(media_info_t));
	g_mediaInfoNodes[type].updateFlag = FALSE;
	return ret;
}


/***************************************************
 * Function : media_update
 * Author : leon.xie
 * Creat Date : 2018/11/10  21:1:34
 * Description : none
 * In-Parameter : as below
 * Return : as below
 * Modify : none
 **************************************************/
int media_update(media_type_e type,media_info_t *media)
{
	RETURN_VAL_IF_FAIL_ARGS(TRUE == g_mediaModuleState,-1,DIAGNO_STR_EUNINIT);
	RETURN_VAL_IF_FAIL_ARGS(type < MEDIA_TYPE_BUTT && media, -1,DIAGNO_STR_EINVAL);
	//
	g_mutex_lock(&g_mediaInfoNodeMutexLocks[type]);
	memcpy(&g_mediaInfoNodes[type].info,media,sizeof(media_info_t));
	g_mediaInfoNodes[type].updateFlag = true;
	g_mutex_unlock(&g_mediaInfoNodeMutexLocks[type]);
	//
    g_cond_signal (&g_mediaInfoNodes[type].cond);    
    return 0;
}

//=============================================

static int media_init(void)
{
	media_type_e type;
	INFO("do %s ...",__func__);
	RETURN_VAL_IF_FAIL_ARGS(FALSE == g_mediaModuleState,-1,DIAGNO_STR_EILLEGAL);
	g_mediaModuleState = TRUE;
    memset(&g_mediaInfoNodes,0,sizeof(g_mediaInfoNodes));
	for(type = MEDIA_TYPE_VIDEO;type < MEDIA_TYPE_BUTT;type++)
	{
		g_mutex_init(&g_mediaInfoNodeMutexLocks[type]);
		g_mutex_init(&g_mediaInfoNodes[type].mutex);
		g_cond_init(&g_mediaInfoNodes[type].cond);
	}
    return 0;
}

static void media_exit(void)
{
	media_type_e type;
	INFO("do %s ...",__func__);
	for(type = MEDIA_TYPE_VIDEO;type < MEDIA_TYPE_BUTT;type++)
	{
		g_cond_clear(&g_mediaInfoNodes[type].cond);
		g_mutex_clear(&g_mediaInfoNodes[type].mutex);
		g_mutex_clear(&g_mediaInfoNodeMutexLocks[type]);
	}
	g_mediaModuleState = FALSE;
}

modules_init(media_init);
modules_exit(media_exit);

#ifdef  __cplusplus
}
#endif


