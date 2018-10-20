/**
  ******************************************************************************
  * @file xml.c
  * @author  leon.xie
  * @version v1.0.0
  * @date 2018-10-20 16:42:41
  * @brief  This file provides all the xml functions. 
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

#include "xml.h"

#ifdef  __cplusplus
extern "C" {
#endif

/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

static int xml_init(void)
{
	INFO("do %s ...",__func__);
    return 0;
}

static void xml_exit(void)
{
	INFO("do %s ...",__func__);
}

modules_init(xml_init);
modules_exit(xml_exit);



#ifdef  __cplusplus
}
#endif


