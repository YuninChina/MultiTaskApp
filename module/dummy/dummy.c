/**
  ******************************************************************************
  * @file dummy.c
  * @author  leon.xie
  * @version v1.0.0
  * @date 2018-11-5 22:01:31
  * @brief  This file provides all the dummy functions. 
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
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "init.h"
#include "log.h"

#include "dummy.h"

#ifdef  __cplusplus
extern "C" {
#endif

/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static int dummy_init(void)
{
	INFO("do %s ...",__func__);
	dummy_maker_init();
	dummy_user_init();
	dummy_user1_init();
	dummy_user2_init();
    return 0;
}

static void dummy_exit(void)
{
	INFO("do %s ...",__func__);
	dummy_user2_init();
	dummy_user1_init();
	dummy_user_init();
	dummy_maker_init();
}



modules_init(dummy_init);
modules_exit(dummy_exit);


#ifdef  __cplusplus
}
#endif


