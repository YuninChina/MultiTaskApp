#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

#include "mt_task.h"
#include "broadcast.h"
#include "parson.h"

#include "multitask.h"
#include "mt_msg.h"
#include "mt_log.h"

#define TASK_PRODUCER	"producer"
#define TASK_CONSUMER1	"consumer1"
#define TASK_CONSUMER2	"consumer2"


static void *task_routine_producer(void *arg)
{
	mt_msg_t *msg;
	int cnt = 0;
	while (1)
	{
		msg = MALLOC(sizeof(*msg)+32);
		assert(msg);
		MLOGM("msg1=%p\n",msg);
		memset(msg,0,(sizeof(*msg)+32));
		strcpy(msg->data,"hello");
		msg->src = TASK_PRODUCER;
		msg->dst = TASK_CONSUMER1;
		msg->priority = 0;
		msg->size = 32;
		if(mt_msg_send(msg))
		{
			FREE(msg);
			msg = NULL;
		}
		sleep(1);
		
		msg = MALLOC(sizeof(*msg)+32);
		assert(msg);
		MLOGM("msg2=%p\n",msg);
		memset(msg,0,(sizeof(*msg)+32));
		strcpy(msg->data,"world");
		msg->src = TASK_PRODUCER;
		msg->dst = TASK_CONSUMER2;
		msg->priority = 0;
		msg->size = 32;
		if(mt_msg_send(msg))
		{
			FREE(msg);
			msg = NULL;
		}
		sleep(3);
	}
	
	return NULL;
}

static void *task_routine_consumer1(void *arg)
{
	const char *str = NULL;
	mt_msg_t *msg;
	int cnt = 0;
	while (1)
	{
		msg = mt_msg_recv();
		str = msg->data;
		MLOGM("[From: %s To: %s]str: %s\n",msg->src,msg->dst,str);
		FREE(msg);
		if(cnt++ > 20)
			break;
	}
	
	return NULL;
}

static void *task_routine_consumer2(void *arg)
{
	const char *str = NULL;
	mt_msg_t *msg;
	int cnt = 0;
	while (1)
	{
		msg = mt_msg_recv();
		str = msg->data;
		MLOGM("[From: %s To: %s]str: %s\n",msg->src,msg->dst,str);
		FREE(msg);
		if(cnt++ > 2)
			break;
	}
	
	return NULL;
}

broadcast_t *g_b = NULL;

static void __mm_broadcast(const char *s)
{
	//printf("%s",s);
	broadcast_send(g_b, (unsigned char *)s, strlen(s));
}

int main(int argc ,char *argv[])  
{
	const char *process_name = NULL;
	int ret = -1;
	int i;
	broadcast_t *b = NULL;
	unsigned short port = 6666;
	int cnt = 0;
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <port>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	assert(1 == sscanf(argv[1],"%hu",&port));
	
	task_create(TASK_PRODUCER,0,0, task_routine_producer, (void *)NULL);
	task_create(TASK_CONSUMER1,0,0, task_routine_consumer1, (void *)NULL);
	task_create(TASK_CONSUMER2,0,0, task_routine_consumer2, (void *)NULL);
	
	b = broadcast_create(BROADCAST_TYPE_SERVER,port);
	assert(b);
	g_b = b;
	
	while(1)
	{
		mm_show2(__mm_broadcast);
		/////////////////////////////
		sleep(5);
	}
	
	broadcast_destroy(b);
	
	return 0;
}

