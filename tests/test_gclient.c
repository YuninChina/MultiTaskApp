/**
  ******************************************************************************
  * @file test_gclient.c
  * @author  leon.xie
  * @version v1.0.0
  * @date 2018-11-12 20:52:49
  * @brief  This file provides all the test_gclient functions. 
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


#ifdef  __cplusplus
extern "C" {
#endif

/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static char *socket_address_to_string (GSocketAddress *address)
{
    GInetAddress *inet_address;
    char *str, *res;
    int port;

    inet_address = g_inet_socket_address_get_address (G_INET_SOCKET_ADDRESS (address));
    str = g_inet_address_to_string (inet_address);
    port = g_inet_socket_address_get_port (G_INET_SOCKET_ADDRESS (address));
    res = g_strdup_printf ("%s:%d", str, port);
    g_free (str);
    return res;
}

int main(int argc, char *argv[])
{
	GSocketClient *client = NULL;
	char *serverAddr = NULL;
	GSocketConnection *connection = NULL;
	GError *error = NULL;
	GSocketAddress *address = NULL;
	GOutputStream *out = NULL;
	int port = 6666;
	char buffer[1000] = {0};
	int cnt = 0;
	
	if (argc != 3) {
		ERROR("Usage: %s <addr> <port>\n",argv[0]);
		exit(EXIT_FAILURE);
	}
	//
	serverAddr = argv[1];
	port = atoi(argv[2]);
	//
	client = g_socket_client_new ();
	RETURN_VAL_IF_FAIL(client, -1);
	g_socket_client_set_timeout (client, 0);
	connection = g_socket_client_connect_to_host (client,
                     serverAddr,
                     port,
                     NULL,&error);
	RETURN_VAL_IF_FAIL(connection, -1);
	g_object_unref (client);
	address = g_socket_connection_get_remote_address (connection, &error);
    if (!address) {
        g_printerr ("Error getting remote address: %s\n",
                    error->message);
        return 1;
    }
    g_print ("Connected to address: %s\n",
             socket_address_to_string (address));
    g_object_unref (address);

    out = g_io_stream_get_output_stream (G_IO_STREAM (connection));
	RETURN_VAL_IF_FAIL(out, -1);
	
    while (1) {
        /* FIXME if (async) */
        sprintf(buffer,"hello world %d",cnt);
        if (!g_output_stream_write_all (out, buffer, strlen (buffer),
                                        NULL, NULL, &error)) {
            g_warning ("send error: %s\n",  error->message);
            g_error_free (error);
            error = NULL;
            break;
        }
        sleep(1);
        cnt++;
    }

    g_print ("closing stream\n");
    if (!g_io_stream_close (G_IO_STREAM (connection), NULL, &error)) {
            g_warning ("close error: %s\n",  error->message);
            g_error_free (error);
            error = NULL;
            return 1;
    }
    
	return 0;
}


#ifdef  __cplusplus
}
#endif


