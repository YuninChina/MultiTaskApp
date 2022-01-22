#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>


#include "os_tcp_client.h"

#include "multitask.h"

struct os_tcp_client_s {
	const char *name;
	int fd;
};

os_tcp_client_t *os_tcp_client_create(const char *name,const char *ip,unsigned short port)
{
	os_tcp_client_t *c = NULL;
	int sockfd;
	struct sockaddr_in servaddr;
	int ret = -1;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	RETURN_VAL_IF_FAIL(sockfd > 0, NULL);
	memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    if( inet_pton(AF_INET, ip, &servaddr.sin_addr) <= 0){
	    MLOGE("inet_pton Fail...");
	    close(sockfd);
	    return NULL;
    }
    ret = connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
	RETURN_VAL_IF_FAIL2(0 == ret, close(sockfd); , NULL);
	c = MALLOC(sizeof(*c));
	RETURN_VAL_IF_FAIL2(c, close(sockfd); , NULL);
	c->fd = sockfd;
	c->name = name;
	return c;
}

void os_tcp_client_destroy(os_tcp_client_t *c)
{
	if(c)
	{
		close(c->fd);
		FREE(c);
		c = NULL;
	}
}

int os_tcp_client_write(os_tcp_client_t *c,void *data,unsigned int size)
{
	return os_write(c->fd, data, size);
}

int os_tcp_client_read(os_tcp_client_t *c,void *data,unsigned int size)
{
	return os_read(c->fd, data, size);
}









