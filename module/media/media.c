/**
  ******************************************************************************
  * @file media.c
  * @author  leon.xie
  * @version v1.0.0
  * @date 2018-10-20 16:42:33
  * @brief  This file provides all the media functions. 
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

#include "media.h"

#ifdef  __cplusplus
extern "C" {
#endif

/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


static int media_init(void)
{
	INFO("do %s ...",__func__);
    return 0;
}

static void media_exit(void)
{
	INFO("do %s ...",__func__);
}

modules_init(media_init);
modules_exit(media_exit);

#ifdef  __cplusplus
}
#endif


