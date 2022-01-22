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
#include <semaphore.h>


#include "multitask.h"
#include "os_tcp_server.h"
#include "os_threadpool.h"

struct os_tcp_server_s {
	int listen_fd;
	int threads;
	unsigned int listen_fds;
	int port;
	os_task_t *task;
	os_threadpool_t *pool;
	sem_t sem;
	os_tcp_server_callback_t cb;
	void *user_data;
};

typedef struct os_tcp_server_node_s {
	int conn_fd;
	os_tcp_server_callback_t cb;
	void *user_data;
}os_tcp_server_node_t;


static void thread_pool_tcp_server_routine(void *arg)
{
	os_tcp_server_node_t *node = (os_tcp_server_node_t *)arg;
	RETURN_IF_FAIL(node);
	if(node->cb) 
		node->cb(node->conn_fd,node->user_data);
	FREE(node);
	node = NULL;
}

static void *task_routine_tcp_server(void *arg)
{
	MLOGM("task start...");
	int conn_fd;
	int ret;
	os_threadpool_t *pool;
	os_tcp_server_node_t *node = NULL;
	os_tcp_server_t *s = (os_tcp_server_t *)arg;
	pool = os_threadpool_create(s->threads,TCP_SERVER_QUQUE_MAX,0);
	GOTO_LABEL_IF_FAIL(pool, exit);
	s->pool = pool;
	while(1)
	{
		conn_fd = accept(s->listen_fd, (struct sockaddr*)NULL, NULL);
		CONTINUE_IF_FAIL(conn_fd > 0);
		node = MALLOC(sizeof(*node));
		CONTINUE_IF_FAIL(node);
		node->conn_fd = conn_fd;
		node->cb = s->cb;
		node->user_data = s->user_data;
		ret = os_threadpool_add(pool,thread_pool_tcp_server_routine,node,0);
		CONTINUE_IF_FAIL(0 == ret);
	}
	
exit:
	os_threadpool_destroy(s->pool,0);
	sem_post(&s->sem);
	return NULL;
}


os_tcp_server_t *os_tcp_server_create(int threads,unsigned int listen_fds,int port,os_tcp_server_callback_t cb,void *user_data)
{
	os_tcp_server_t *s = NULL;
	int listen_fd;
	struct sockaddr_in serv_addr;
	int ret = -1;
	os_task_t *task = NULL;
	
	RETURN_VAL_IF_FAIL(threads > 0 && listen_fds > 0 && port > 0, NULL);
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
	s = MALLOC(sizeof(*s));
	RETURN_VAL_IF_FAIL(s, NULL);
	sem_init(&s->sem, 0, 0);
	task = os_task_create(TASK_TCP_SERVER,0,0, task_routine_tcp_server, (void *)s);
	RETURN_VAL_IF_FAIL2(task, FREE(task);,NULL);
	s->listen_fd = listen_fd;
	s->threads = threads;
	s->listen_fds = listen_fds;
	s->port = port;
	s->task = task;
	s->cb = cb;
	s->user_data = user_data;
	return s;
}

void os_tcp_server_destroy(os_tcp_server_t *s)
{
	if(s)
	{
		sem_wait(&s->sem);
		sem_destroy(&s->sem);
		close(s->listen_fd);
		FREE(s);
		s = NULL;
	}
}





