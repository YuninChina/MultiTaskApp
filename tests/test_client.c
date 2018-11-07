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

    DEBUG ("Echo service listening on port %d\n", port);

    //g_signal_connect (service, "run", G_CALLBACK (handler), NULL);
    
	//
	main_loop_start();
	return 0;
}

#ifdef  __cplusplus
}
#endif


