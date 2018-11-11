/**
  ******************************************************************************
  * @file server.c
  * @author  leon.xie
  * @version v1.0.0
  * @date 2018-11-11 10:38:31
  * @brief  This file provides all the server functions. 
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
struct _server_s{
	GSocketService *service;
	gulong run_signal_handler_id;
};

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/***************************************************
 * Function : server_create
 * Author : leon.xie
 * Creat Date : 2018/11/11  10:43:2
 * Description : 创建服务端
 * In-Parameter : port:端口 , max_threads:最大线程数量, -1表示不限制
 * Return : as below
 * Modify : none
 **************************************************/
server_t *server_create(int port,int max_threads,server_hander_t callback,gpointer user_data)
{
	GSocketService *service = NULL;
	GError *error = NULL;
	server_t *server = NULL;
	gulong run_signal_handler_id;
	RETURN_VAL_IF_FAIL_ARGS(callback && 0 != max_threads, NULL,DIAGNO_STR_EINVAL);
	service = g_threaded_socket_service_new (10);
	if (!g_socket_listener_add_inet_port (G_SOCKET_LISTENER (service),
                                          port,
                                          NULL,
                                          &error)) {
        ERROR ("%s\n",error->message);
        RETURN_VAL_IF_FAIL_ARGS(server,NULL,"do g_socket_listener_add_inet_port fail");
    }

    MESSAGE("Echo service listening on port %d\n", port) ;

    run_signal_handler_id = g_signal_connect (service, "run", G_CALLBACK (callback), user_data);
    server = malloc(sizeof(server_t));
    RETURN_VAL_IF_FAIL_ARGS(server,NULL,DIAGNO_STR_ENOMEM);
    server->service = service;
    server->run_signal_handler_id = run_signal_handler_id;
    return server;
}


/***************************************************
 * Function : server_destroy
 * Author : leon.xie
 * Creat Date : 2018/11/11  10:55:58
 * Description : 销毁服务器
 * In-Parameter : as below
 * Return : as below
 * Modify : none
 **************************************************/
void server_destroy(server_t *server)
{
	RETURN_IF_FAIL_ARGS(server,DIAGNO_STR_EINVAL);
	g_signal_handler_disconnect(G_SOCKET_LISTENER (server->service),server->run_signal_handler_id);
	server->run_signal_handler_id = 0;
	g_socket_listener_close(G_SOCKET_LISTENER (server->service));
	g_socket_service_stop(server->service);
	free(server);
}


#ifdef  __cplusplus
}
#endif


