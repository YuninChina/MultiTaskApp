/**
  ******************************************************************************
  * @file bsp.c
  * @author  leon.xie
  * @version v1.0.0
  * @date 2018-10-18 21:45:33
  * @brief  This file provides all the bsp functions. 
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

#include "bsp.h"

#ifdef  __cplusplus
extern "C" {
#endif

/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/



static int bsp_init(void)
{
	printf("do %s ...\n",__func__);
    return 0;
}

static void bsp_exit(void)
{
	printf("do %s ...\n",__func__);
}


module_init(bsp_init);
module_exit(bsp_exit);


#ifdef  __cplusplus
}
#endif


