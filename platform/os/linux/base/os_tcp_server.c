#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>

#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>


#include "multitask.h"
#include "os_tcp_server.h"

struct os_tcp_server_s {
	int listen_fd;
	const char *name;
	int threads;
	unsigned int listen_fds;
	int port;
	os_task_t *task;
};

static void *task_routine_tcp_server(void *arg)
{
	MLOGM("task start...");
	int conn_fd;
	os_tcp_server_t *s = (os_tcp_server_t *)arg;
	while(1)
	{
		conn_fd = accept(s->listen_fd, (struct sockaddr*)NULL, NULL);
		CONTINUE_IF_FAIL(conn_fd > 0);
		
	}
	
	return NULL;
}


os_tcp_server_t *os_tcp_server_create(const char *name,int threads,unsigned int listen_fds,int port)
{
	os_tcp_server_t *s = NULL;
	int listen_fd;
	struct sockaddr_in serv_addr;
	int ret = -1;
	os_task_t *task = NULL;
	
	RETURN_VAL_IF_FAIL(name && threads > 0 && listen_fds > 0 && port > 0, NULL);
	listen_fd = socket(AF_INET, SOCK_STREAM, 0);
	RETURN_VAL_IF_FAIL(listen_fd > 0, NULL);
	memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(port);
    ret = bind(listen_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    RETURN_VAL_IF_FAIL(ret >= 0, NULL);
    ret = listen(listen_fd, threads);
    RETURN_VAL_IF_FAIL(ret >= 0, NULL);

	task = os_task_create(TASK_TCP_SERVER,0,0, task_routine_tcp_server, (void *)s);
	RETURN_VAL_IF_FAIL(task, NULL);
	s = MALLOC(sizeof(*s));
	RETURN_VAL_IF_FAIL(s, NULL);
	s->name = name;
	s->listen_fd = listen_fd;
	s->threads = threads;
	s->listen_fds = listen_fds;
	s->port = port;
	s->task = task;
	return s;
}

void os_tcp_server_destroy(os_tcp_server_t *s)
{

}





