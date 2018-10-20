/**
  ******************************************************************************
  * @file log.h 
  * @author leon.xie
  * @version v1.0.0
  * @date 2018-10-19 21:23:25
  * @brief This file contains all the functions prototypes for the BOARD 
  *  log 
  ******************************************************************************
  * @attention
  *
  * File For Yunin Software Team Only
  *
  * Copyright (C), 2017-2027, Yunin Software Team
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __LOG_H
#define __LOG_H

/* Includes ------------------------------------------------------------------*/

#include "type.h" 
#include "glib.h" 

#ifdef  __cplusplus
extern "C" {
#endif

/* Exported typedef ----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define LOG_FILENAME	"app.log"
/* Exported functions ------------------------------------------------------- */
//log
#define DEBUG_INT(filename, linenr, funcname, args...)             \
    log_message(G_LOG_LEVEL_DEBUG, filename, linenr, funcname,args)
#define INFO_INT(filename, linenr, funcname, args...)             \
    log_message(G_LOG_LEVEL_INFO, filename, linenr, funcname,args)
#define MESSAGE_INT(filename, linenr, funcname, args...)             \
	log_message(G_LOG_LEVEL_MESSAGE, filename, linenr, funcname,args)
#define WARN_INT(filename, linenr, funcname, args...)             \
	log_message(G_LOG_LEVEL_WARNING, filename, linenr, funcname,args)
#define ERROR_INT(filename, linenr, funcname, args...)             \
	log_message(G_LOG_LEVEL_CRITICAL, filename, linenr, funcname,args)

#undef DEBUG
#define DEBUG(args...) 		DEBUG_INT(__FILE__, __LINE__, __func__,args)
#undef INFO
#define INFO(args...) 		INFO_INT(__FILE__, __LINE__, __func__,args)
#undef MESSAGE
#define MESSAGE(args...)	MESSAGE_INT(__FILE__, __LINE__, __func__,args)
#undef WARN
#define WARN(args...) 		WARN_INT(__FILE__, __LINE__, __func__,args)
#undef ERROR
#define ERROR(args...) 		ERROR_INT(__FILE__, __LINE__, __func__,args)


int log_message(guint32 priority,
					const char *filename,
					int linenr,
					const char *funcname,
					const char *fmt, ...);

#ifdef  __cplusplus
}
#endif

#endif  /* __LOG_H */


