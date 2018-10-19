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
#include "log.h"

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
	INFO("do %s ...",__func__);
    return 0;
}

static void bsp_exit(void)
{
	INFO("do %s ...",__func__);
}


modules_init(bsp_init);
modules_exit(bsp_exit);


#ifdef  __cplusplus
}
#endif


