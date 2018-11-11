/**
  ******************************************************************************
  * @file net.h 
  * @author leon.xie
  * @version v1.0.0
  * @date 2018-11-4 21:25:18
  * @brief This file contains all the functions prototypes for the BOARD 
  *  net 
  ******************************************************************************
  * @attention
  *
  * File For Yunin Software Team Only
  *
  * Copyright (C), 2017-2027, Yunin Software Team
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __NET_H
#define __NET_H

/* Includes ------------------------------------------------------------------*/

#include "configs.h" 
#include "type.h" 
#include "diagnosis.h"
#include "init.h"
#include "glib.h"
#include "gio/gio.h"
#include "log.h"

#ifdef  __cplusplus
extern "C" {
#endif

/* Exported typedef ----------------------------------------------------------*/
typedef struct _server_s server_t;

typedef bool (*server_hander_t)(GThreadedSocketService *,GSocketConnection *,GSocketListener *,gpointer user_data);
/* Exported variables --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
//默认服务器端口号
#define DEFAULT_SERVER_PORT  6666
/* Exported functions ------------------------------------------------------- */

/***************************************************
 * Function : server_create
 * Author : leon.xie
 * Creat Date : 2018/11/11  10:43:2
 * Description : 创建服务端
 * In-Parameter : port:端口 , max_threads:最大线程数量, -1表示不限制
 * Return : as below
 * Modify : none
 **************************************************/
server_t *server_create(int port,int max_threads,server_hander_t callback,gpointer user_data);

/***************************************************
 * Function : server_destroy
 * Author : leon.xie
 * Creat Date : 2018/11/11  10:55:58
 * Description : 销毁服务器
 * In-Parameter : as below
 * Return : as below
 * Modify : none
 **************************************************/
void server_destroy(server_t *server);

#ifdef  __cplusplus
}
#endif

#endif  /* __NET_H */


