/**
  ******************************************************************************
  * @file net.c
  * @author  leon.xie
  * @version v1.0.0
  * @date 2018-11-4 21:25:06
  * @brief  This file provides all the net functions. 
  ******************************************************************************
  * @attention
  *
  * File For Yunin Software Team Only
  *
  * Copyright (C), 2017-2027, Yunin Software Team
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "log.h"
#include "init.h"

#include "net.h"

#ifdef  __cplusplus
extern "C" {
#endif

/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/***************************************************
 * Function : net_init
 * Author : leon.xie
 * Creat Date : 2018/11/04  21:36:35
 * Description : none
 * In-Parameter : as below
 * Return : as below
 * Modify : none
 **************************************************/
int net_init(void)
{
	INFO("do %s ...",__func__);
    return 0;
}

/***************************************************
 * Function : net_exit
 * Author : leon.xie
 * Creat Date : 2018/11/04  21:36:50
 * Description : none
 * In-Parameter : as below
 * Return : as below
 * Modify : none
 **************************************************/
void net_exit(void)
{
	INFO("do %s ...",__func__);
}

modules_init(net_init);
modules_exit(net_exit);


#ifdef  __cplusplus
}
#endif


