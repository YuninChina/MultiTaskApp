/**
  ******************************************************************************
  * @file test_server_client.c
  * @author  leon.xie
  * @version v1.0.0
  * @date 2018-11-12 22:15:06
  * @brief  This file provides all the test_server_client functions. 
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
/* Private macro -------------------------------------------------------------*/
#define TEST_SERVER_CLIENT_IP "127.0.0.1"
#define TEST_SERVER_CLIENT_PORT 5555

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static gboolean
__test_server_client_handler (GThreadedSocketService *service,
         GSocketConnection      *connection,
         GSocketListener        *listener,
         gpointer                user_data)
{
    GOutputStream *out;
    GInputStream *in;
    gssize size;
	int cnt = 0;
    char buffer[1024] = {0};
    
    out = g_io_stream_get_output_stream (G_IO_STREAM (connection));
    in = g_io_stream_get_input_stream (G_IO_STREAM (connection));

    while (0 < (size = g_input_stream_read (in, buffer,
                                            sizeof(buffer), NULL, NULL)))
	{
		MESSAGE("recv msg from client(%p) : %s",connection,buffer);
        g_output_stream_write (out, buffer, size, NULL, NULL);
        cnt++;
        if(cnt > 10)
        {
        	//关闭连接
        	WARN("server: close connection(%p)",connection);
        	g_io_stream_close(G_IO_STREAM (connection),NULL,NULL);
        }
	}                                            

    return FALSE;
}


static gpointer __test_client1_thread_routine(gpointer data)
{
	MESSAGE("task client_thread1 start...");
	client_t *client = NULL;
	char send_buffer[1024] = {0};
	char recv_buffer[1024] = {0};
	int cnt = 0;

	client = client_create(CLIENT_CONNECT_ADDR_TYPE_IP,TEST_SERVER_CLIENT_IP,
	TEST_SERVER_CLIENT_PORT,2);
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
	ERROR("thread client_thread1 exit...");
	client_destroy(client);
	return NULL;
}

static gpointer __test_client2_thread_routine(gpointer data)
{
	MESSAGE("task client_thread2 start...");
	client_t *client = NULL;
	char send_buffer[1024] = {0};
	char recv_buffer[1024] = {0};
	int cnt = 0;

	client = client_create(CLIENT_CONNECT_ADDR_TYPE_IP,TEST_SERVER_CLIENT_IP,
	TEST_SERVER_CLIENT_PORT,2);
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
	ERROR("thread client_thread2 exit...");
	client_destroy(client);
	return NULL;
}

static gpointer __test_client3_thread_routine(gpointer data)
{
	MESSAGE("task client_thread3 start...");
	client_t *client = NULL;
	char send_buffer[1024] = {0};
	char recv_buffer[1024] = {0};
	int cnt = 0;

	client = client_create(CLIENT_CONNECT_ADDR_TYPE_IP,TEST_SERVER_CLIENT_IP,
	TEST_SERVER_CLIENT_PORT,2);
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
	ERROR("thread client_thread3 exit...");
	client_destroy(client);
	return NULL;
}



int main(int argc, char *argv[])
{
	server_create(TEST_SERVER_CLIENT_PORT,10,__test_server_client_handler,NULL);
	
	g_thread_new("client_thread1",__test_client1_thread_routine,NULL);
	g_thread_new("client_thread2",__test_client2_thread_routine,NULL);
	g_thread_new("client_thread3",__test_client3_thread_routine,NULL);
	
	main_loop_start();
	g_assert_not_reached ();
	return 0;
}

#ifdef  __cplusplus
}
#endif


