/**
  ******************************************************************************
  * @file net_util.c
  * @author  leon.xie
  * @version v1.0.0
  * @date 2018-11-11 17:47:09
  * @brief  This file provides all the net_util functions. 
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

#include "net_pri.h"

#ifdef  __cplusplus
extern "C" {
#endif

/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/***************************************************
 * Function : socket_address2string
 * Author : leon.xie
 * Creat Date : 2018/11/11  17:51:16
 * Description : 将网络地址信息转换为字符串
 * In-Parameter : as below
 * Return : as below
 * Modify : none
 **************************************************/
int socket_address2string (GSocketAddress *address,char *strOutput,U32 strLen)
{
    GInetAddress *inet_address;
    char *str;
    int port;
	RETURN_VAL_IF_FAIL_ARGS(address && strOutput && strLen > 0, -1, DIAGNO_STR_EINVAL);
    inet_address = g_inet_socket_address_get_address (G_INET_SOCKET_ADDRESS (address));
    str = g_inet_address_to_string (inet_address);
    port = g_inet_socket_address_get_port (G_INET_SOCKET_ADDRESS (address));
    snprintf(strOutput,strLen,"%s:%d", str, port);
    g_free (str);
    return 0;
}

#ifdef  __cplusplus
}
#endif


