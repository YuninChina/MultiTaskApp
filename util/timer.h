/**
  ******************************************************************************
  * @file timer.h 
  * @author leon.xie
  * @version v1.0.0
  * @date 2018-10-19 22:29:56
  * @brief This file contains all the functions prototypes for the BOARD 
  *  timer 
  ******************************************************************************
  * @attention
  *
  * File For Yunin Software Team Only
  *
  * Copyright (C), 2017-2027, Yunin Software Team
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __TIMER_H
#define __TIMER_H

/* Includes ------------------------------------------------------------------*/

#include "configs.h" 
#include "type.h" 

#ifdef  __cplusplus
extern "C" {
#endif

/* Exported typedef ----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define TIME_STRING_BUFLEN \
    (4 + 1 + 2 + 1 + 2 + 1 + 2 + 1 + 2 + 1 + 2 + 1 + 3 + 5 + 1)
	/*	 Yr 	 Mon	 Day	 Hour	 Min	 Sec	 Ms  TZ  NULL */

/* Exported functions ------------------------------------------------------- */
int timer_millisecond_get(guint64 *timer);
void timer_fields_get(guint64 timer, struct tm *fields);
int timer_millisecond2string(guint64 timer,gchar *buf);

#ifdef  __cplusplus
}
#endif

#endif  /* __TIMER_H */


