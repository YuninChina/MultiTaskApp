/**
  ******************************************************************************
  * @file log.c
  * @author  leon.xie
  * @version v1.0.0
  * @date 2018-10-19 21:23:11
  * @brief  This file provides all the log functions. 
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
#include <string.h>
#include <unistd.h>
#include <sys/prctl.h>

#include "glib.h"
#include "log.h"
#include "timer.h"
#include "init.h"

#include "diagnosis.h"

#ifdef  __cplusplus
extern "C" {
#endif

/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define LOG_MESSAGE_MAX	2048

#define LOG_BUFFER_LEN	4096
#define LOG_TASK_NAME_LEN 128
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static const gchar *_log_level_string(int level)
{
	const char *ret = "UNKNOW";
	switch(level)
	{
		case G_LOG_LEVEL_CRITICAL: ret = "CRITICAL";break;
		case G_LOG_LEVEL_WARNING: ret = "WARNING";break;
		case G_LOG_LEVEL_MESSAGE: ret = "MESSAGE";break;
		case G_LOG_LEVEL_INFO: ret = "INFO";break;
		case G_LOG_LEVEL_DEBUG: ret = "DEBUG";break;
		case G_LOG_FLAG_RECURSION: 
		case G_LOG_FLAG_FATAL: 
		case G_LOG_LEVEL_ERROR: 
		case G_LOG_LEVEL_MASK: 
		default: ret = "ERROR";break;
	}
	return ret;
}

static void _log_hander(const gchar   *log_domain,
        GLogLevelFlags log_level,
        const gchar   *message,
        gpointer       user_data)
{
	ASSERT(NULL != message);
	
	gchar buffer[LOG_BUFFER_LEN] = {0};
	const gchar *colour_str = NULL;
	const gchar *non_colour_str = NULL;
	gchar task_name[LOG_TASK_NAME_LEN] = {0};
	const gchar *level_str = NULL;
	gchar time_str[128] = {0};
	guint64 timer;
	const gchar *domains = NULL;

	//env
	if((G_LOG_LEVEL_DEBUG == log_level)
	|| (G_LOG_LEVEL_INFO == log_level))
	{
		domains = g_getenv ("LOG");
		if(NULL == domains 
		|| strcmp (domains, "all") != 0)
		{
			return;
		}
	}
	
	gint ret;
	//[colour]
#if !defined(CONFIG_LOG_ENABLE)	
	switch(log_level)
	{
		case G_LOG_FLAG_FATAL:
		case G_LOG_LEVEL_ERROR: colour_str = COLOUR_RED;break;
		case G_LOG_LEVEL_CRITICAL: colour_str = COLOUR_RED;break;
		case G_LOG_LEVEL_WARNING: colour_str = COLOUR_YELLOW;break;
		case G_LOG_LEVEL_MESSAGE: colour_str = COLOUR_L_GREEN;break;
		case G_LOG_LEVEL_INFO: colour_str = COLOUR_L_CYAN;break;
		case G_LOG_LEVEL_DEBUG: 
		default: colour_str = NULL;break;
	}
#endif

	colour_str = colour_str?colour_str:"";
	ASSERT(NULL != colour_str);
	
	//[non-colour]
	non_colour_str = colour_str? COLOUR_NONE:"";
	ASSERT(NULL != non_colour_str);

	//task
	prctl (PR_GET_NAME, task_name);
	task_name[sizeof(task_name)-1] = 0;
	//time
	ret = timer_millisecond_get(&timer);
	ASSERT(SUCC == ret);
	ret = timer_millisecond2string(timer,time_str);
	ASSERT(SUCC == ret);
	time_str[sizeof(time_str)-1] = 0;
	ASSERT(NULL != time_str);
	//level
	level_str = _log_level_string(log_level);
	ASSERT(NULL != level_str);
	//message
	
	if(G_LOG_LEVEL_INFO != log_level
	&& G_LOG_LEVEL_MESSAGE != log_level)
	{
		/* [colour]-time-task-level-message-[non-colour] */
		snprintf(buffer,sizeof(buffer),"%s%s(%s)%s: %s%s",
				colour_str,time_str,task_name,level_str,message,non_colour_str);
	}
	else
	{
		/* [colour]task-level-message-[non-colour] */
		snprintf(buffer,sizeof(buffer),"%s(%s)%s: %s%s",
				colour_str,task_name,level_str,message,non_colour_str);
	}
	buffer[sizeof(buffer)-1] = 0;
	//output
	#if defined(CONFIG_LOG_ENABLE)
	ASSERT(0);
	#else
	printf("%s%s%s\n",colour_str,buffer,non_colour_str);
	#endif
}

		
/***************************************************
 * Function : log_init
 * Author : leon.xie
 * Creat Date : 2018/10/19  21:29:8
 * Description : none
 * In-Parameter : as below
 * Return : as below
 * Modify : none
 **************************************************/
static int log_init(void)
{
	printf("do %s ...\n",__func__);
	g_log_set_default_handler(_log_hander,NULL);
    return 0;
}

/***************************************************
 * Function : log_exit
 * Author : leon.xie
 * Creat Date : 2018/10/19  21:29:24
 * Description : none
 * In-Parameter : as below
 * Return : as below
 * Modify : none
 **************************************************/
static void log_exit(void)
{

}

/***************************************************
 * Function : log_message
 * Author : leon.xie
 * Creat Date : 2018/10/19  21:34:6
 * Description : none
 * In-Parameter : as below
 * Return : as below
 * Modify : none
 **************************************************/
int log_message(guint32 priority,
					const char *filename,
					int linenr,
					const char *funcname,
					const char *fmt, ...)
{
	g_return_val_if_fail(NULL != filename,FAIL);
	g_return_val_if_fail(NULL != funcname,FAIL);
	/*message_body-file-line-funcname*/
	va_list args;
	gchar message[LOG_MESSAGE_MAX] = {0};
	int ret = 0;
	//message body
	va_start(args, fmt);
	ret = vsnprintf(message, sizeof(message) - ret, fmt, args);
	va_end(args);

	g_return_val_if_fail(ret > 0,FAIL);
	//filename-linenr-funcname
	if(G_LOG_LEVEL_INFO != priority
	&& G_LOG_LEVEL_MESSAGE != priority)
	{
		snprintf(message+ret, sizeof(message) - ret, " (%s,%d,%s)",
		filename,linenr,funcname);
	}

	message[sizeof(message) - 1] = 0;
	
	//output
	switch(priority)
	{
		case G_LOG_FLAG_FATAL: 
		case G_LOG_LEVEL_ERROR: g_error(message);break;
		case G_LOG_LEVEL_CRITICAL: g_critical(message);break;
		case G_LOG_LEVEL_WARNING: g_warning(message);break;
		case G_LOG_LEVEL_MESSAGE: g_message(message);break;
		case G_LOG_LEVEL_INFO: g_info(message);break;
		case G_LOG_LEVEL_DEBUG: g_debug(message);break;
		case G_LOG_FLAG_RECURSION:
		default: g_return_val_if_fail(0,FAIL);
	}
	return SUCC;
}

/***************************************************
 * Function : log_
 * Author : leon.xie
 * Creat Date : 2018/10/19  21:43:39
 * Description : none
 * In-Parameter : as below
 * Return : as below
 * Modify : none
 **************************************************/
void log_assert(const char *source
	,const char *expr
	,const char *filename
	,int linenr
	,const char *fmt, ...)
{
	char _log_buffer[1024*8] = {0};
	char threadName[256] = {0};
	int len = 0;
	const char *colour = NULL;
	
	prctl (PR_GET_NAME, threadName);
	threadName[sizeof(threadName)-1] = 0;
	
#if !defined(CONFIG_LOG_ENABLE)
	colour = COLOUR_RED;
#endif

	len += snprintf(_log_buffer+len,sizeof(_log_buffer)-len,"%s(%s): assertion failed %s: %d: %s "
	,colour?colour:""
	,source ? source : threadName,
	filename, linenr, expr?expr:"");
	
	//
	va_list args;
	va_start(args, fmt);
	len += vsnprintf(_log_buffer+len,sizeof(_log_buffer)-len,fmt, args);
	va_end(args);
	
	//
	len += snprintf(_log_buffer+len,sizeof(_log_buffer)-len,"%s\n",colour?COLOUR_NONE:"");
	//
	_log_buffer[sizeof(_log_buffer)-1] = 0;
	//output
#if defined(CONFIG_LOG_ENABLE)	
	ASSERT(0);
#else
	printf(_log_buffer);
#endif		
}


cores_init(log_init);
cores_exit(log_exit);

#ifdef  __cplusplus
}
#endif


