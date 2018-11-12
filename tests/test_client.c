/**
  ******************************************************************************
  * @file test_client.c
  * @author  leon.xie
  * @version v1.0.0
  * @date 2018-11-6 22:24:03
  * @brief  This file provides all the test_client functions. 
  ******************************************************************************
  * @attention
  *
  * File For Yunin Software Team Only
  *
  * Copyright (C), 2017-2027, Yunin Software Team
  ******************************************************************************
  */ 

//参考gio/send-data.c 例子
/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "glib.h"
#include "gio/gio.h"

#include "diagnosis.h"
#include "init.h"
#include "log.h"
#include "kernel.h"
#include "net.h"

#ifdef  __cplusplus
extern "C" {
#endif

/* Private typedef -----------------------------------------------------------*/
typedef struct test_client_info_s {
	union {
		char ip[16];
		char url[16];
		char domain[16];
	};
	int port;
}test_client_info_t;

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static test_client_info_t __testClientInfo;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static gpointer __test_client_thread_routine(gpointer data)
{
	INFO("task client_thread start...");
	test_client_info_t *testClient = (test_client_info_t *)data;
	RETURN_VAL_IF_FAIL_ARGS(testClient, NULL,DIAGNO_STR_EINVAL);
	client_t *client = NULL;
    char send_buffer[1024] = {0};
    char recv_buffer[1024] = {0};
    int cnt = 0;
    
	client = client_create(CLIENT_CONNECT_ADDR_TYPE_IP,testClient->ip,testClient->port,2);
	RETURN_VAL_IF_FAIL_ARGS(client, NULL,"client_create fail...");
	while(1)
	{
		snprintf(send_buffer,sizeof(send_buffer),"say hello: %d",cnt++);
		GOTO_LABEL_IF_FAIL_ARGS(client_write(client,send_buffer,sizeof(send_buffer)) >= 0,
		over,"client_write fail...");
		printf("client_write %s\n",send_buffer);
		GOTO_LABEL_IF_FAIL_ARGS(client_read(client,recv_buffer,sizeof(recv_buffer)) >= 0,
		over,"client_read fail...");
		printf("recv msg from server : %s\n",recv_buffer);
		sleep(1);
	}
	
over:
	ERROR("thread client_thread exit...");
	client_destroy(client);
	return NULL;
}

int main(int argc, char *argv[])
{
	char *serverAddr = NULL;
	int port = 6666;
	
	if (argc != 3) {
		ERROR("Usage: %s <addr> <port>\n",argv[0]);
		exit(EXIT_FAILURE);
	}
	bzero(&__testClientInfo,sizeof(__testClientInfo));
	serverAddr = argv[1];
	port = atoi(argv[2]);
	strncpy(__testClientInfo.ip,serverAddr,sizeof(__testClientInfo.ip));
	__testClientInfo.port = port;
	
	g_thread_new("client_thread",__test_client_thread_routine,&__testClientInfo);
	
	main_loop_start();
	g_assert_not_reached ();
	return 0;
}

#ifdef  __cplusplus
}
#endif


