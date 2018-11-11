/**
  ******************************************************************************
  * @file service.c
  * @author  leon.xie
  * @version v1.0.0
  * @date 2018-10-18 21:48:57
  * @brief  This file provides all the service functions. 
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
#include "init.h"

#include "service.h"
#include "log.h"
#include "diagnosis.h"

#ifdef  __cplusplus
extern "C" {
#endif

/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static int service_init(void)
{
	INFO("do %s ...",__func__);
	ASSERT(0 == service_idle_init());
	ASSERT(0 == service_master_init());
	ASSERT(0 == service_protocol_init());
	
	//
	ASSERT(0 == task_start());
    return 0;
}

static void service_exit(void)
{
	INFO("do %s ...",__func__);
}


services_init(service_init);
services_exit(service_exit);

#ifdef  __cplusplus
}
#endif


