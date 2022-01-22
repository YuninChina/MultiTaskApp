#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>

#include "os_tcp_server.h"


struct os_tcp_server_s {
	int listen_fd;
};


os_tcp_server_t *os_tcp_server_create(const char *name,int threads)
{
	return NULL;
}

void os_tcp_server_destroy(os_tcp_server_t *s)
{

}









