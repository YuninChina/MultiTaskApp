/**
  ******************************************************************************
  * @file task.h 
  * @author leon.xie
  * @version v1.0.0
  * @date 2018-10-20 9:54:33
  * @brief This file contains all the functions prototypes for the BOARD 
  *  task 
  ******************************************************************************
  * @attention
  *
  * File For Yunin Software Team Only
  *
  * Copyright (C), 2017-2027, Yunin Software Team
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __TASK_H
#define __TASK_H

/* Includes ------------------------------------------------------------------*/

#include "configs.h" 
#include "type.h" 
#include "msg.h" 

#ifdef  __cplusplus
extern "C" {
#endif

/* Exported typedef ----------------------------------------------------------*/
typedef void (*task_init_t)(int order);
typedef void (*task_run_t)(msg_t *msg_header,void *msg_data);

/* Exported variables --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
typedef enum {
	TASK_IDLE,
	TASK_MASTER,
	TASK_PROTOCOL,
	TASK_BUTT
}task_id_e;

typedef enum {
	TASK_ORDER_BEFORE_INIT,
	TASK_ORDER_INIT,
	TASK_ORDER_RUN,
	TASK_ORDER_BUTT
}task_order_e;

/* Exported functions ------------------------------------------------------- */
GAsyncQueue *task_async_queue_get(U32 task_id);
int task_register(U32 task_id,const char *task_name,task_init_t init,task_run_t run);
int task_start(void);

#ifdef  __cplusplus
}
#endif

#endif  /* __TASK_H */


