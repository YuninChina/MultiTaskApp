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
	printf("do %s ...\n",__func__);
    return 0;
}

static void service_exit(void)
{
	printf("do %s ...\n",__func__);
}


module_init(service_init);
module_exit(service_exit);

#ifdef  __cplusplus
}
#endif


