#include <stdio.h>   
#include <stdlib.h>   
#include <string.h>   
#include <time.h>
#include <unistd.h>
#include <assert.h>   
#include <ctype.h>

#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#include "os_broadcast.h"


struct os_broadcast_s {
    /*< private >*/
    int socket_fd;
    struct sockaddr_in addr;
};


#ifndef DEBUG
#define DEBUG(args...) printf(args)
#endif

#ifndef ERROR
#define ERROR(args...) printf(args)
#endif

os_broadcast_t *os_broadcast_create(os_broadcast_type_e type,unsigned short port)
{
	int ret = -1;
	const int opt=-1;
	os_broadcast_t *b = NULL;
	b = malloc(sizeof(os_broadcast_t));
	assert(b);
	b->socket_fd = socket(AF_INET,SOCK_DGRAM,0);
	if(b->socket_fd < 0)
	{
		ERROR("socket fail...\n");
		goto fail;
	}
	ret = setsockopt(b->socket_fd,SOL_SOCKET,SO_BROADCAST,(char*)&opt,sizeof(opt));//设置套接字类型
	if(ret < 0)
	{
		ERROR("setsockopt fail...\n");
		goto set_fail;
	}
	bzero(&b->addr,sizeof(struct sockaddr_in));
	b->addr.sin_family = AF_INET;
	b->addr.sin_addr.s_addr = (BROADCAST_TYPE_CLIENT == type)? htonl(INADDR_ANY) : htonl(INADDR_BROADCAST);
	b->addr.sin_port = htons(port);

	if(BROADCAST_TYPE_CLIENT == type)
	{
		if(-1 == bind(b->socket_fd,(struct sockaddr*)&(b->addr),sizeof(b->addr)))
		{
				ERROR("bind error...\n");
				goto bind_fail;
		}
	}
	return b;
	
bind_fail:
set_fail:
	close(b->socket_fd);
fail:
	free(b);
	return NULL;
}

void os_broadcast_destroy(os_broadcast_t *b)
{
	if(b)
	{
		if(b->socket_fd > 0) close(b->socket_fd);
		free(b);
	}
}

int os_broadcast_send(os_broadcast_t *b,unsigned char *data,unsigned int size)
{
	assert(b);
	socklen_t len = sizeof(b->addr);
	return sendto(b->socket_fd,data,size,0,(struct sockaddr*)&b->addr,len);//向广播地址发布消息
}


int os_broadcast_recv(os_broadcast_t *b,unsigned char *data,unsigned int size)
{
	assert(b);
	socklen_t len = sizeof(b->addr);
	return recvfrom(b->socket_fd,data,size,0,(struct sockaddr*)&b->addr,&len);
}

