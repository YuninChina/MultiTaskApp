
#ifndef __MT_LOG_H
#define __MT_LOG_H

#ifdef  __cplusplus
extern "C" {
#endif

//输出颜色
#define COLOUR_NONE		"\e[0m"
#define COLOUR_BLACK	"\e[0;30m"
#define COLOUR_L_BLACK	"\e[1;30m"
#define COLOUR_RED		"\e[0;31m"
#define COLOUR_L_RED	"\e[1;31m"
#define COLOUR_GREEN	"\e[0;32m"
#define COLOUR_L_GREEN	"\e[1;32m"
#define COLOUR_BROWN	"\e[0;33m"
#define COLOUR_YELLOW	"\e[1;33m"
#define COLOUR_BLUE		"\e[0;34m"
#define COLOUR_L_BLUE	"\e[1;34m"
#define COLOUR_PURPLE	"\e[0;35m"
#define COLOUR_L_PURPLE	"\e[1;35m"
#define COLOUR_CYAN		"\e[0;36m"
#define COLOUR_L_CYAN	"\e[1;36m"
#define COLOUR_GRAY		"\e[0;37m"
#define COLOUR_WHITE	"\e[1;37m"


typedef enum {
	MLOG_VERBOSE = 0,
	MLOG_DEBUG,
	MLOG_INFO,
	MLOG_MESSAGE,
	MLOG_WARN,
	MLOG_ERROR,
	MLOG_FATAL,
	MLOG_BUTT
}MMP_LOG_ENUM;

#define VERBOSE_INT(filename, linenr, funcname, args...)             \
    os_log_message(MLOG_VERBOSE, filename, linenr, funcname,args)
#define DEBUG_INT(filename, linenr, funcname, args...)             \
    os_log_message(MLOG_DEBUG, filename, linenr, funcname,args)
#define INFO_INT(filename, linenr, funcname, args...)             \
	os_log_message(MLOG_INFO, filename, linenr, funcname,args)
#define MESSAGE_INT(filename, linenr, funcname, args...)             \
	os_log_message(MLOG_MESSAGE, filename, linenr, funcname,args)
#define WARN_INT(filename, linenr, funcname, args...)             \
	os_log_message(MLOG_WARN, filename, linenr, funcname,args)
#define ERROR_INT(filename, linenr, funcname, args...)             \
		os_log_message(MLOG_ERROR, filename, linenr, funcname,args)
#define FATAL_INT(filename, linenr, funcname, args...)             \
		os_log_message(MLOG_FATAL, filename, linenr, funcname,args)

#define MLOGV(args...)	VERBOSE_INT(__FILE__, __LINE__, __func__,args)
#define MLOGD(args...)	DEBUG_INT(__FILE__, __LINE__, __func__,args)
#define MLOGI(args...)	INFO_INT(__FILE__, __LINE__, __func__,args)
#define MLOGM(args...)	MESSAGE_INT(__FILE__, __LINE__, __func__,args)
#define MLOGW(args...)	WARN_INT(__FILE__, __LINE__, __func__,args)
#define MLOGE(args...)	ERROR_INT(__FILE__, __LINE__, __func__,args)
#define MLOGF(args...)	FATAL_INT(__FILE__, __LINE__, __func__,args)

#define MLOG_BUFF_SIZE	1024*4


typedef void (*os_log_callback_t)(int level,const char *message);

void os_log_message(int level
	,const char *file_name
	,int line
	,const char *func_name
	,const char *fmt, ...);
void os_log_assert(const char *source
	,const char *expr
	,const char *filename
	,int linenr
	,const char *fmt, ...);


#undef ASSERT
#define ASSERT(expr) if(!(expr)) os_log_assert(NULL,#expr,__FILE__, __LINE__,NULL)
	
#undef ASSERT_ARGS
#define ASSERT_ARGS(expr,args...) if(!(expr)) os_log_assert(NULL,#expr,__FILE__, __LINE__,args)
	
	
	
#undef RETURN_IF_FAIL
#define RETURN_IF_FAIL(expr)  ({\
		if (!(expr))\
		{\
			os_log_assert(NULL,#expr,__FILE__, __LINE__,NULL);\
			return;\
		}\
	})

#undef RETURN_IF_FAIL2
#define RETURN_IF_FAIL2(expr,_actions)  ({\
				if (!(expr))\
				{\
					{_actions};\
					os_log_assert(NULL,#expr,__FILE__, __LINE__,NULL);\
					return;\
				}\
			})
	
#undef RETURN_IF_FAIL_ARGS
#define RETURN_IF_FAIL_ARGS(expr,args...)  ({\
		if (!(expr))\
		{\
			os_log_assert(NULL,#expr,__FILE__, __LINE__,args);\
			return;\
		}\
	})
	
	
#undef RETURN_VAL_IF_FAIL
#define RETURN_VAL_IF_FAIL(expr,return_val)  ({\
		if (!(expr))\
		{\
			os_log_assert(NULL,#expr,__FILE__, __LINE__,NULL);\
			return return_val;\
		}\
	})

#undef RETURN_VAL_IF_FAIL2
#define RETURN_VAL_IF_FAIL2(expr,_actions,return_val)  ({\
				if (!(expr))\
				{\
					{_actions};\
					os_log_assert(NULL,#expr,__FILE__, __LINE__,NULL);\
					return return_val;\
				}\
			})
	
	
#undef RETURN_VAL_IF_FAIL_ARGS
#define RETURN_VAL_IF_FAIL_ARGS(expr,return_val,args...)  ({\
		if (!(expr))\
		{\
			os_log_assert(NULL,#expr,__FILE__, __LINE__,args);\
			return return_val;\
		}\
	})
	
	
#undef GOTO_LABEL_IF_FAIL
#define GOTO_LABEL_IF_FAIL(expr,label)  ({\
		if (!(expr))\
		{\
			os_log_assert(NULL,#expr,__FILE__, __LINE__,NULL);\
			goto label;\
		}\
	})
	
	
#undef GOTO_LABEL_IF_FAIL_ARGS
#define GOTO_LABEL_IF_FAIL_ARGS(expr,label,args...)  ({\
		if (!(expr))\
		{\
			os_log_assert(NULL,#expr,__FILE__, __LINE__,args);\
			goto label;\
		}\
	})
	
#undef CONTINUE_IF_FAIL
#define CONTINUE_IF_FAIL(expr)  ({\
		if (!(expr))\
		{\
			os_log_assert(NULL,#expr,__FILE__, __LINE__,NULL);\
			continue;\
		}\
	})
	
#undef CONTINUE_IF_FAIL_ARGS
#define CONTINUE_IF_FAIL_ARGS(expr,args...)  ({\
		if (!(expr))\
		{\
			os_log_assert(NULL,#expr,__FILE__, __LINE__,args);\
			continue;\
		}\
	})


#ifdef  __cplusplus
}
#endif

#endif  /* __MT_LOG_H */


