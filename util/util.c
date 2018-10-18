/**
  ******************************************************************************
  * @file util.c
  * @author  leon.xie
  * @version v1.0.0
  * @date 2018-10-18 21:49:21
  * @brief  This file provides all the util functions. 
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

#include "util.h"

#ifdef  __cplusplus
extern "C" {
#endif

/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

static int util_init(void)
{
	printf("do %s ...\n",__func__);
    return 0;
}

static void util_exit(void)
{
	printf("do %s ...\n",__func__);
}


module_init(util_init);
module_exit(util_exit);


#ifdef  __cplusplus
}
#endif


