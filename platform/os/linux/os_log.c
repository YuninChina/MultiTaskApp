/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdarg.h>

#include "os_log.h"
#include "os_misc.h"

#ifdef  __cplusplus
extern "C" {
#endif

/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static os_log_callback_t g_os_log_callback = NULL;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

const char *os_log_enum2str(MMP_LOG_ENUM level)
{
	const char *ret = "[U]";
	switch(level)
	{
		case MLOG_VERBOSE: 	ret = "[V]";break;
		case MLOG_DEBUG: 	ret = "[D]";break;
		case MLOG_INFO: 	ret = "[I]";break;
		case MLOG_MESSAGE: 	ret = "[M]";break;
		case MLOG_WARN: 	ret = "[W]";break;
		case MLOG_ERROR: 	ret = "[E]";break;
		case MLOG_FATAL: 	ret = "[F]";break;
		default: break;
	}
	return ret;
}


const char *os_log_colour_get(MMP_LOG_ENUM level)
{
    const char *ret = COLOUR_NONE;
    switch(level)
    {
        case MLOG_VERBOSE: 	ret = COLOUR_NONE;break;
        case MLOG_DEBUG: 	ret = COLOUR_WHITE;break;
        case MLOG_INFO: 	ret = COLOUR_WHITE;break;
        case MLOG_MESSAGE: 	ret = COLOUR_L_GREEN;break;
        case MLOG_WARN: 	ret = COLOUR_YELLOW;break;
        case MLOG_ERROR: 	ret = COLOUR_L_RED;break;
        case MLOG_FATAL: 	ret = COLOUR_RED;break;
        default: break;
    }
    return ret;
}

void os_log_message(int level
	,const char *file_name
	,int line
	,const char *func_name
	,const char *fmt, ...)
{
	int ret = 0;
	/* 头-实际内容 */
	/* 日志头 */
	char task_name[32] = {0};
	char time_buf[64] = {0};
	char log_buffer[MLOG_BUFF_SIZE] = {0};
	thread_name_get(task_name);
	time2str(time_buf,sizeof(time_buf));

	ret = snprintf(log_buffer+ret,sizeof(log_buffer)-ret,"%s%s ",os_log_colour_get(level),time_buf);
	ret += snprintf(log_buffer+ret,sizeof(log_buffer)-ret,"(%s,%d)",file_name,line);
	ret += snprintf(log_buffer+ret,sizeof(log_buffer)-ret,"[%s]",task_name);
	ret += snprintf(log_buffer+ret,sizeof(log_buffer)-ret,"%s",os_log_enum2str(level));
	/* 组包 */
    va_list args;
    va_start(args, fmt);
    vsnprintf(log_buffer+ret,sizeof(log_buffer)-ret,fmt,args);
    va_end(args);
	/* 输出 */
	if(g_os_log_callback) 
		g_os_log_callback(level,log_buffer);
	else
		printf("%s\n""\e[0m",log_buffer);
}


void os_log_assert(const char *source
	,const char *expr
	,const char *file_name
	,int line
	,const char *fmt, ...)
{
	int ret = 0;
	/* 头-实际内容 */
	/* 日志头 */
	char task_name[32] = {0};
	char time_buf[64] = {0};
	char log_buffer[MLOG_BUFF_SIZE] = {0};
	const char *colour = COLOUR_L_RED;
	thread_name_get(task_name);
	time2str(time_buf,sizeof(time_buf));

	ret = snprintf(log_buffer+ret,sizeof(log_buffer)-ret,"%s%s ",colour?colour:"",time_buf);
	ret += snprintf(log_buffer+ret,sizeof(log_buffer)-ret,"(%s,%d)",file_name,line);
	ret += snprintf(log_buffer+ret,sizeof(log_buffer)-ret,"[%s]",task_name);
	ret += snprintf(log_buffer+ret,sizeof(log_buffer)-ret,"assertion failed: # %s #",expr?expr:"");
	/* 组包 */
	va_list args;
	va_start(args, fmt);
	vsnprintf(log_buffer+ret,sizeof(log_buffer)-ret,fmt,args);
	va_end(args);
	/* 输出 */
	if(g_os_log_callback) 
		g_os_log_callback(MLOG_FATAL,log_buffer);
	else
		printf("%s\n""\e[0m",log_buffer);
}


#ifdef  __cplusplus
}
#endif



