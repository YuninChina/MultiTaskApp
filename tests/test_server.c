/**
  ******************************************************************************
  * @file test_server.c
  * @author  leon.xie
  * @version v1.0.0
  * @date 2018-11-6 22:24:11
  * @brief  This file provides all the test_server functions. 
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

#ifdef  __cplusplus
extern "C" {
#endif

/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

static gboolean
server_handler (GThreadedSocketService *service,
         GSocketConnection      *connection,
         GSocketListener        *listener,
         gpointer                user_data)
{
    GOutputStream *out;
    GInputStream *in;
    char buffer[1024];
    gssize size;

    out = g_io_stream_get_output_stream (G_IO_STREAM (connection));
    in = g_io_stream_get_input_stream (G_IO_STREAM (connection));

    while (0 < (size = g_input_stream_read (in, buffer,
                                            sizeof buffer, NULL, NULL)))
	{
		printf("recv msg from client : %s\n",buffer);
        g_output_stream_write (out, buffer, size, NULL, NULL);
	}                                            

    return TRUE;
}

int main(int argc, char *argv[])
{
	GSocketService *service = NULL;
	GError *error = NULL;
	int port = 7777;
	if (argc != 2) {
		ERROR("Usage: %s <port>\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	RETURN_VAL_IF_FAIL(1 == sscanf(argv[1],"%d",&port), -1);
	
	service = g_threaded_socket_service_new (10);
	if (!g_socket_listener_add_inet_port (G_SOCKET_LISTENER (service),
                                          port,
                                          NULL,
                                          &error)) {
        ERROR ("%s: %s\n", argv[0], error->message);
        return 1;
    }

    MESSAGE("Echo service listening on port %d\n", port) ;

    g_signal_connect (service, "run", G_CALLBACK (server_handler), NULL);
    
	//
	main_loop_start();
	g_assert_not_reached ();
	return 0;
}


#ifdef  __cplusplus
}
#endif


