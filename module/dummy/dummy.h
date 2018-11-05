/**
  ******************************************************************************
  * @file dummy.h 
  * @author leon.xie
  * @version v1.0.0
  * @date 2018-11-5 22:01:37
  * @brief This file contains all the functions prototypes for the BOARD 
  *  dummy 
  ******************************************************************************
  * @attention
  *
  * File For Yunin Software Team Only
  *
  * Copyright (C), 2017-2027, Yunin Software Team
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __DUMMY_H
#define __DUMMY_H

/* Includes ------------------------------------------------------------------*/

#include "configs.h" 
#include "type.h" 
#include "glib.h"
#include "diagnosis.h"
#include "log.h"

#ifdef  __cplusplus
extern "C" {
#endif

/* Exported typedef ----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
typedef struct dummy_user_info_s{
	int res;
}dummy_user_info_t;

typedef struct dummy_user1_info_s{
	int res;
}dummy_user1_info_t;

typedef struct dummy_user2_info_s{
	int res;
}dummy_user2_info_t;


/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
int dummy_maker_init(void);
void dummy_maker_exit(void);
//
int dummy_user_send(dummy_user_info_t *usrInput);
int dummy_user_recv(dummy_user_info_t *usrOutput);
int dummy_user_init(void);
void dummy_user_exit(void);
//
int dummy_user1_send(dummy_user1_info_t *usrInput);
int dummy_user1_recv(dummy_user1_info_t *usrOutput);
int dummy_user1_init(void);
void dummy_user1_exit(void);
//
int dummy_user2_send(dummy_user2_info_t *usrInput);
int dummy_user2_recv(dummy_user2_info_t *usrOutput);
int dummy_user2_init(void);
void dummy_user2_exit(void);


#ifdef  __cplusplus
}
#endif

#endif  /* __DUMMY_H */


