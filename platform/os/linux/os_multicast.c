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

#include "os_multicast.h"



struct os_multicast_s {
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

os_multicast_t *os_multicast_create(os_multicast_type_e type,const char *addr,unsigned short port)
{
	os_multicast_t *m = NULL;
	m = malloc(sizeof(os_multicast_t));
	int err;
	assert(m);
	m->socket_fd = socket(AF_INET,SOCK_DGRAM,0);
	if(m->socket_fd < 0)
	{
		printf("socket create fail.\n");
        return NULL;
	}
	memset(&m->addr,0,sizeof(m->addr));
	m->addr.sin_family = AF_INET;
	m->addr.sin_addr.s_addr = (MULTICAST_TYPE_CLIENT == type)?htonl(INADDR_ANY) :inet_addr(addr);
	m->addr.sin_port = htons(port);

	if(MULTICAST_TYPE_CLIENT == type)
	{
		/*绑定socket*/
		err = bind(m->socket_fd,(struct sockaddr*)&m->addr,sizeof(m->addr));
		if(err<0)
		{
				printf("bind fail.\n");
				return NULL;
		}
		/*设置回环许可*/
		int loop=1;
		err=setsockopt(m->socket_fd,IPPROTO_IP,IP_MULTICAST_LOOP,&loop,sizeof(loop));
		if(err<0)
		{
				printf("set sock error\n");
				return NULL;
		}
		struct ip_mreq mreq;/*加入广播组*/
		mreq.imr_multiaddr.s_addr=inet_addr(addr);//广播地址
		mreq.imr_interface.s_addr=htonl(INADDR_ANY); //网络接口为默认
		/*将本机加入广播组*/
		err=setsockopt(m->socket_fd,IPPROTO_IP,IP_ADD_MEMBERSHIP,&mreq,sizeof(mreq));
		if(err<0)
		{
				printf("set sock error\n");
				return NULL;
		}
	}
	return m;
	
}

void os_multicast_destroy(os_multicast_t *m)
{
	if(m)
	{
		if(m->socket_fd > 0) close(m->socket_fd);
		free(m);
	}
}

int os_multicast_send(os_multicast_t *m,unsigned char *data,unsigned int size)
{
	assert(m);
	//向局部多播地址发送多播内容
	return sendto(m->socket_fd,data,size,0,(struct sockaddr*)&m->addr,sizeof(m->addr));
}

int os_multicast_recv(os_multicast_t *m,unsigned char *data,unsigned int size)
{
	assert(m);
	socklen_t len = sizeof(m->addr);
	return recvfrom(m->socket_fd,data,size,0,(struct sockaddr*)&m->addr,&len);
}




