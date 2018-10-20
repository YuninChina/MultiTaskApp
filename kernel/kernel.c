/**
  ******************************************************************************
  * @file kernel.c
  * @author  leon.xie
  * @version v1.0.0
  * @date 2018-10-18 21:48:15
  * @brief  This file provides all the kernel functions. 
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

#include "kernel.h"
#include "log.h"

#ifdef  __cplusplus
extern "C" {
#endif

/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

static int kernel_init(void)
{
	INFO("do %s ...",__func__);
    return 0;
}

static void kernel_exit(void)
{
	INFO("do %s ...",__func__);
}


cores_init(kernel_init);
cores_exit(kernel_exit);


#ifdef  __cplusplus
}
#endif


