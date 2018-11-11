/**
  ******************************************************************************
  * @file service_protocol.c
  * @author  leon.xie
  * @version v1.0.0
  * @date 2018-11-11 18:55:52
  * @brief  This file provides all the service_protocol functions. 
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

#include "service_protocol.h"
#include "glib.h"
#include "gio/gio.h"

#include "diagnosis.h"
#include "init.h"
#include "log.h"
#include "kernel.h"

#include "media.h"
#include "net.h"

#ifdef  __cplusplus
extern "C" {
#endif

/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define SERVICE_PROTOCOL_SLEF_IP	"127.0.0.1"
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static gboolean
__service_protocol_server_handler (GThreadedSocketService *service,
         GSocketConnection      *connection,
         GSocketListener        *listener,
         gpointer                user_data)
{
    GOutputStream *out;
    GInputStream *in;
    gssize size;
	int cnt = 0;
	media_info_t media;
    out = g_io_stream_get_output_stream (G_IO_STREAM (connection));
    in = g_io_stream_get_input_stream (G_IO_STREAM (connection));

    while (0 < (size = g_input_stream_read (in, &media,
                                            sizeof(media), NULL, NULL)))
	{
		MESSAGE("recv msg from client(%p) : %d",connection,media.video.res);
        g_output_stream_write (out, &media, size, NULL, NULL);
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

static gpointer __service_protocol_media_thread_routine(gpointer data)
{
	INFO("task protocol_media start...");
	media_info_t media;
	bzero(&media,sizeof(media));
	while(1)
	{
		GOTO_LABEL_IF_FAIL_ARGS(0 == media_update(MEDIA_TYPE_VIDEO,&media),
		over,"media_read fail...");
		usleep(1000*500);
		media.video.res++;
	}
	
over:
	ERROR("thread protocol_media exit...");
	return NULL;
}


static gpointer __service_protocol_client0_thread_routine(gpointer data)
{
	
	INFO("task protocol_client0 start...");
	media_info_t media;
	client_t *client = NULL;
	
	client = client_create(SERVICE_PROTOCOL_SLEF_IP,DEFAULT_SERVER_PORT,2);
	RETURN_VAL_IF_FAIL_ARGS(client, NULL,"client_create fail...");
	
	while(1)
	{
		bzero(&media,sizeof(media));
		
		GOTO_LABEL_IF_FAIL_ARGS(0 == media_read(MEDIA_TYPE_VIDEO,&media,-1),
		over,"media_read fail...");
		GOTO_LABEL_IF_FAIL_ARGS(client_write(client,&media,sizeof(media)) >= 0,
		over,"client_write fail...");
		DEBUG("client_write %d",media.video.res);
		/*
		GOTO_LABEL_IF_FAIL_ARGS(client_read(client,&media,sizeof(media)) >= 0,
		over,"client_read fail...");
		INFO("recv msg from server : %d",media.video.res);
		*/
	}
	
over:
	ERROR("thread protocol_client0 exit...");
	client_destroy(client);
	return NULL;
}

static gpointer __service_protocol_client1_thread_routine(gpointer data)
{
	
	INFO("task protocol_client1 start...");
	media_info_t media;
	client_t *client = NULL;
	
	client = client_create(SERVICE_PROTOCOL_SLEF_IP,DEFAULT_SERVER_PORT,2);
	RETURN_VAL_IF_FAIL_ARGS(client, NULL,"client_create fail...");
	
	while(1)
	{
		bzero(&media,sizeof(media));
		
		GOTO_LABEL_IF_FAIL_ARGS(0 == media_read(MEDIA_TYPE_VIDEO,&media,-1),
		over,"media_read fail...");
		GOTO_LABEL_IF_FAIL_ARGS(client_write(client,&media,sizeof(media)) >= 0,
		over,"client_write fail...");
		DEBUG("client_write %d",media.video.res);
		/*
		GOTO_LABEL_IF_FAIL_ARGS(client_read(client,&media,sizeof(media)) >= 0,
		over,"client_read fail...");
		INFO("recv msg from server : %d",media.video.res);
		*/
	}
	
over:
	ERROR("thread protocol_client1 exit...");
	client_destroy(client);
	return NULL;
}


static gpointer __service_protocol_client2_thread_routine(gpointer data)
{
	
	INFO("task protocol_client2 start...");
	media_info_t media;
	client_t *client = NULL;
	
	client = client_create(SERVICE_PROTOCOL_SLEF_IP,DEFAULT_SERVER_PORT,2);
	RETURN_VAL_IF_FAIL_ARGS(client, NULL,"client_create fail...");
	
	while(1)
	{
		bzero(&media,sizeof(media));
		
		GOTO_LABEL_IF_FAIL_ARGS(0 == media_read(MEDIA_TYPE_VIDEO,&media,-1),
		over,"media_read fail...");
		GOTO_LABEL_IF_FAIL_ARGS(client_write(client,&media,sizeof(media)) >= 0,
		over,"client_write fail...");
		DEBUG("client_write %d",media.video.res);
		/*
		GOTO_LABEL_IF_FAIL_ARGS(client_read(client,&media,sizeof(media)) >= 0,
		over,"client_read fail...");
		INFO("recv msg from server : %d",media.video.res);
		*/
	}
	
over:
	ERROR("thread protocol_client2 exit...");
	client_destroy(client);
	return NULL;
}



static void __service_task_protocol_init(int order)
{
	switch(order)
	{
		case TASK_ORDER_BEFORE_INIT:
		case TASK_ORDER_INIT: break;
		case TASK_ORDER_RUN:
		{
			server_create(DEFAULT_SERVER_PORT,10,__service_protocol_server_handler,NULL);
			g_thread_new("ProtoMedia",__service_protocol_media_thread_routine,NULL);
			g_thread_new("ProtoClient0",__service_protocol_client0_thread_routine,NULL);
			g_thread_new("ProtoClient1",__service_protocol_client1_thread_routine,NULL);
			g_thread_new("ProtoClient2",__service_protocol_client2_thread_routine,NULL);
		}break;
		default:
			INFO("task protocol init succ...");
			break;
	}
}

void __service_task_protocol_run(msg_t *msg_header,void *msg_data)
{
	switch(msg_header->src_task_id)
	{
		case TASK_IDLE:
		{
			MESSAGE("from idle...");
		}break;
		case TASK_MASTER:
		{
			MESSAGE("from master...");
		}break;
		case TASK_PROTOCOL:
		{
			MESSAGE("from protocol...");
		}break;
		default:
			ASSERT(0);
			break;
	}
}

/***************************************************
 * Function : service_protocol_init
 * Author : leon.xie
 * Creat Date : 2018/11/11  18:56:20
 * Description : none
 * In-Parameter : as below
 * Return : as below
 * Modify : none
 **************************************************/
int service_protocol_init(void)
{
	RETURN_VAL_IF_FAIL(0 == task_register(TASK_PROTOCOL,E2S(TASK_PROTOCOL),
	__service_task_protocol_init,__service_task_protocol_run),-1);
    return 0;
}

/***************************************************
 * Function : service_protocol_exit
 * Author : leon.xie
 * Creat Date : 2018/11/11  18:56:43
 * Description : none
 * In-Parameter : as below
 * Return : as below
 * Modify : none
 **************************************************/
void service_protocol_exit(void)
{

}

#ifdef  __cplusplus
}
#endif


