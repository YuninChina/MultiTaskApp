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
	printf("do %s ...\n",__func__);
    return 0;
}

static void kernel_exit(void)
{
	printf("do %s ...\n",__func__);
}


module_init(kernel_init);
module_exit(kernel_exit);


#ifdef  __cplusplus
}
#endif


