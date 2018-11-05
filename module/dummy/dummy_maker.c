/**
  ******************************************************************************
  * @file dummy_maker.c
  * @author  leon.xie
  * @version v1.0.0
  * @date 2018-11-5 22:22:57
  * @brief  This file provides all the dummy_maker functions. 
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

#include "dummy.h"

#ifdef  __cplusplus
extern "C" {
#endif

/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

static gpointer dummy_maker_task_routine(gpointer data)
{
	dummy_user_info_t usrInput;
	dummy_user1_info_t usr1Input;
	dummy_user2_info_t usr2Input;
	bzero(&usrInput,sizeof(usrInput));
	bzero(&usr1Input,sizeof(usr1Input));
	bzero(&usr2Input,sizeof(usr2Input));
	
	while(1)
	{
		//
		dummy_user_send(&usrInput);
		sleep(5);
		
		//
		dummy_user1_send(&usr1Input);
		sleep(10);
		//
		dummy_user2_send(&usr2Input);
		sleep(2);
		
		usrInput.res += 1;
		usr1Input.res += 2;
		usr2Input.res += 3;
	}
	return NULL;
}

/***************************************************
 * Function : dummy_maker_init
 * Author : leon.xie
 * Creat Date : 2018/11/05  22:23:10
 * Description : none
 * In-Parameter : as below
 * Return : as below
 * Modify : none
 **************************************************/
int dummy_maker_init(void)
{
	g_thread_new("dummy_maker_task", dummy_maker_task_routine,NULL);
    return 0;
}


/***************************************************
 * Function : dummy_maker_exit
 * Author : leon.xie
 * Creat Date : 2018/11/05  22:24:11
 * Description : none
 * In-Parameter : as below
 * Return : as below
 * Modify : none
 **************************************************/
void dummy_maker_exit(void)
{
    
}


#ifdef  __cplusplus
}
#endif


