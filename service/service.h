/**
  ******************************************************************************
  * @file service.h 
  * @author leon.xie
  * @version v1.0.0
  * @date 2018-10-20 13:48:29
  * @brief This file contains all the functions prototypes for the BOARD 
  *  service 
  ******************************************************************************
  * @attention
  *
  * File For Yunin Software Team Only
  *
  * Copyright (C), 2017-2027, Yunin Software Team
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __SERVICE_H
#define __SERVICE_H

/* Includes ------------------------------------------------------------------*/

#include "configs.h" 
#include "type.h" 

#include "diagnosis.h"
#include "log.h"
#include "task.h"
#include "msg.h"
#include "util.h"

#ifdef  __cplusplus
extern "C" {
#endif

/* Exported typedef ----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
int service_idle_init(void);
void service_idle_exit(void);
int service_master_init(void);
void service_master_exit(void);

#ifdef  __cplusplus
}
#endif

#endif  /* __SERVICE_H */


