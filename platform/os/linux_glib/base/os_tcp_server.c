#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>

#include "os_tcp_server.h"


struct os_tcp_server_s {
	int listen_fd;
};


os_tcp_server_t *os_tcp_server_create(int threads,unsigned int listen_fds,int port,os_tcp_server_callback_t cb,void *user_data)
{
	return NULL;
}

void os_tcp_server_destroy(os_tcp_server_t *s)
{

}









