/**
  ******************************************************************************
  * @file client.c
  * @author  leon.xie
  * @version v1.0.0
  * @date 2018-11-11 10:38:00
  * @brief  This file provides all the client functions. 
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
struct _client_s{
	GSocketConnection *connection;
	GInputStream *in;
	GOutputStream *out;
};

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/***************************************************
 * Function : client_create
 * Author : leon.xie
 * Creat Date : 2018/11/11  11:33:32
 * Description : 创建客户端
 * In-Parameter : as below
 * Return : as below
 * Modify : none
 **************************************************/
client_t *client_create(client_connect_addr_type_e addrType,char *addr,int port,U32 timeout)
{
	RETURN_VAL_IF_FAIL_ARGS(addrType < CLIENT_CONNECT_ADDR_TYPE_BUTT && addr, NULL,DIAGNO_STR_EINVAL);
	GSocketClient *client = NULL;
	GError *error = NULL;
	GSocketConnection *connection = NULL;
	GSocketAddress *address = NULL;
	char addrBuf[1024] = {0};
	GInputStream *in = NULL;
	GOutputStream *out = NULL;
	client = g_socket_client_new ();
	RETURN_VAL_IF_FAIL(client, NULL);
	g_socket_client_set_timeout (client,timeout);
	switch(addrType)
	{
		case CLIENT_CONNECT_ADDR_TYPE_IP:
		{
			connection = g_socket_client_connect_to_host(client,addr,port,NULL,&error);
		}break;
		case CLIENT_CONNECT_ADDR_TYPE_URL:
		{
			connection = g_socket_client_connect_to_uri(client,addr,port,NULL,&error);
		}break;
		default:
			ASSERT(0);
			break;
	}
	GOTO_LABEL_IF_FAIL_ARGS(connection, fail,"g_socket_client_connect_to_host fail");
	g_object_unref (client);
	address = g_socket_connection_get_remote_address (connection, &error);
	GOTO_LABEL_IF_FAIL_ARGS(address, fail,"Error getting remote address: %s",error->message);
	GOTO_LABEL_IF_FAIL_ARGS(0 == socket_address2string(address,addrBuf,sizeof(addrBuf)),fail2,"socket_address2string fail");
	MESSAGE("Connected to address: %s",addrBuf);
    g_object_unref (address);
    in = g_io_stream_get_input_stream (G_IO_STREAM (connection));
	out = g_io_stream_get_output_stream (G_IO_STREAM (connection));
	//
	client_t *clientRusult = NULL;
	clientRusult = malloc(sizeof(client_t));
	GOTO_LABEL_IF_FAIL_ARGS(clientRusult, fail2, DIAGNO_STR_ENOMEM);
	clientRusult->connection = connection;
	clientRusult->in = in;
	clientRusult->out = out;
	
    return clientRusult;
    
fail2:
    g_object_unref (address);
fail:
	g_object_unref (client);
	return NULL;
}

/***************************************************
 * Function : client_destroy
 * Author : leon.xie
 * Creat Date : 2018/11/11  11:33:54
 * Description : 销毁客户端
 * In-Parameter : as below
 * Return : as below
 * Modify : none
 **************************************************/
void client_destroy(client_t *client)
{
	GError *error = NULL;
	RETURN_IF_FAIL_ARGS(client,DIAGNO_STR_EINVAL);
	if(FALSE == g_io_stream_close (G_IO_STREAM (client->connection), NULL, &error))
	{
		WARN("close error: %s",error->message);
	}
	if(TRUE != g_io_stream_is_closed(G_IO_STREAM(client->connection)))
	{
		g_io_stream_close(G_IO_STREAM(client->connection),NULL,NULL);
	}
	g_object_unref (client->connection);
	free(client);
}

/***************************************************
 * Function : client_write
 * Author : leon.xie
 * Creat Date : 2018/11/11  18:31:7
 * Description : 向服务端写数据
 * In-Parameter : as below
 * Return : as below
 * Modify : none
 **************************************************/
int client_write(client_t *client,void *data,U32 size)
{
	RETURN_VAL_IF_FAIL_ARGS(client && data && size > 0,-1,DIAGNO_STR_EINVAL);
	GError *error = NULL;
	int ret = -1;
	if ((ret = g_output_stream_write (client->out, data, size, NULL, &error)) < 0) 
	{
		if(error)
		{
            WARN ("send error: %s",  error->message);
            g_error_free (error);
            error = NULL;
		}
    }
    return ret;
}

/***************************************************
 * Function : client_write_string
 * Author : leon.xie
 * Creat Date : 2018/11/12  22:31:6
 * Description : 向服务端写入格式化字符串
 * In-Parameter : as below
 * Return : as below
 * Modify : none
 **************************************************/
int client_write_string(client_t *client,const char *format,...)
{
	RETURN_VAL_IF_FAIL_ARGS(client ,-1,DIAGNO_STR_EINVAL);
	GError *error = NULL;
	int ret = -1;
	gsize bytes_written = 0;
    gboolean success;
    va_list  args;
    va_start (args, format);
    success = g_output_stream_vprintf(client->out,&bytes_written, NULL, &error,format,args);
    va_end (args);
	if (FALSE == success) 
	{
		if(error)
		{
			WARN ("send error: %s",  error->message);
			g_error_free (error);
			error = NULL;
		}
	}
	else
	{
		ret = (int)bytes_written;
	}
	
	return ret;
}



/***************************************************
 * Function : client_read
 * Author : leon.xie
 * Creat Date : 2018/11/11  18:35:43
 * Description : none
 * In-Parameter : 读服务端数据
 * Return : as below
 * Modify : none
 **************************************************/
int client_read(client_t *client,void *data,U32 size)
{
	RETURN_VAL_IF_FAIL_ARGS(client && data && size > 0,-1,DIAGNO_STR_EINVAL);
	GError *error = NULL;
	int ret = -1;
	if ((ret = g_input_stream_read(client->in, data, size, NULL, &error)) < 0) 
	{
		if(error)
		{
            WARN ("send error: %s",  error->message);
            g_error_free (error);
            error = NULL;
		}
    }
    return ret;
}


#ifdef  __cplusplus
}
#endif


