/**
  ******************************************************************************
  * @file msg.h 
  * @author leon.xie
  * @version v1.0.0
  * @date 2018-10-20 9:55:01
  * @brief This file contains all the functions prototypes for the BOARD 
  *  msg 
  ******************************************************************************
  * @attention
  *
  * File For Yunin Software Team Only
  *
  * Copyright (C), 2017-2027, Yunin Software Team
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __MSG_H
#define __MSG_H

/* Includes ------------------------------------------------------------------*/

#include "configs.h" 
#include "type.h" 

#ifdef  __cplusplus
extern "C" {
#endif

/* Exported typedef ----------------------------------------------------------*/
typedef enum {
    MSG_PRIORITY_HIGHEST = 0,
	MSG_PRIORITY_NORMAL = 1,
	MSG_PRIORITY_LOW = 2,
	MSG_PRIORITY_BUTT
}os_msg_priority_e;

typedef struct msg_s{
	//msg head
	U32 src_task_id;
	U32 dest_task_id;
	int priority;
	//msg data
	char data[0];
}msg_t;

/* Exported variables --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

//消息队列最大长度
#define MSG_QUEUE_MAX	50
/* Exported functions ------------------------------------------------------- */
int msg_send(msg_t *msg);

#ifdef  __cplusplus
}
#endif

#endif  /* __MSG_H */


