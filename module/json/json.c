/**
  ******************************************************************************
  * @file json.c
  * @author  leon.xie
  * @version v1.0.0
  * @date 2018-10-20 16:42:12
  * @brief  This file provides all the json functions. 
  ******************************************************************************
  * @attention
  *
  * File For Yunin Software Team Only
  *
  * Copyright (C), 2017-2027, Yunin Software Team
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "glib.h"
#include "log.h"
#include "init.h"

#include "diagnosis.h"

#include "json.h"

#ifdef  __cplusplus
extern "C" {
#endif

/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

static int json_init(void)
{
	INFO("do %s ...",__func__);
    return 0;
}

static void json_exit(void)
{
	INFO("do %s ...",__func__);
}

modules_init(json_init);
modules_exit(json_exit);


#ifdef  __cplusplus
}
#endif


