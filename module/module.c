/**
  ******************************************************************************
  * @file module.c
  * @author  leon.xie
  * @version v1.0.0
  * @date 2018-10-18 21:48:36
  * @brief  This file provides all the module functions. 
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

#include "module.h"

#ifdef  __cplusplus
extern "C" {
#endif

/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static int modules_init(void)
{
	printf("do %s ...\n",__func__);
    return 0;
}

static void modules_exit(void)
{
	printf("do %s ...\n",__func__);
}


module_init(modules_init);
module_exit(modules_exit);

#ifdef  __cplusplus
}
#endif


