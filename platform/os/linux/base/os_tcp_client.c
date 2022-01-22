

#include "os_tcp_client.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>


struct os_tcp_client_s {
	const char *name;
	int fd;
};

os_tcp_client_t *os_tcp_client_create(const char *name,const char *ip,unsigned short port)
{
	return NULL;
}

void os_tcp_client_destroy(os_tcp_client_t *s)
{

}

int os_tcp_client_write(os_tcp_client_t *s,void *data,unsigned int size)
{
	return 0;
}

int os_tcp_client_read(os_tcp_client_t *s,void *data,unsigned int size)
{
	return 0;
}









