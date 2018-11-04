/**
  ******************************************************************************
  * @file chip.c
  * @author  leon.xie
  * @version v1.0.0
  * @date 2018-11-4 21:48:03
  * @brief  This file provides all the chip functions. 
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

#include "chip.h"

#ifdef  __cplusplus
extern "C" {
#endif

/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/***************************************************
 * Function : chip_init
 * Author : leon.xie
 * Creat Date : 2018/11/04  21:49:14
 * Description : none
 * In-Parameter : as below
 * Return : as below
 * Modify : none
 **************************************************/
int chip_init(void)
{
	INFO("do %s ...",__func__);
    return 0;
}


/***************************************************
 * Function : chip_exit
 * Author : leon.xie
 * Creat Date : 2018/11/04  21:49:34
 * Description : none
 * In-Parameter : as below
 * Return : as below
 * Modify : none
 **************************************************/
void chip_exit(void)
{
    INFO("do %s ...",__func__);
}


modules_init(chip_init);
modules_exit(chip_exit);


#ifdef  __cplusplus
}
#endif


