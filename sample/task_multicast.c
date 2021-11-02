#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

#include "parson.h"

#include "multitask.h"

#define TASK_PRODUCER	"producer"
#define TASK_CONSUMER1	"consumer1"
#define TASK_CONSUMER2	"consumer2"

#define MUTICAST_ADDR "239.255.255.250"


static void *task_routine_producer(void *arg)
{
	os_msg_t *msg;
	int cnt = 0;
	while (1)
	{
		msg = MALLOC(sizeof(*msg)+32);
		ASSERT(msg);
		MLOGM("msg1=%p\n",msg);
		os_memset(msg,0,(sizeof(*msg)+32));
		os_strcpy((char *)msg->data,"hello");
		msg->src = TASK_PRODUCER;
		msg->dst = TASK_CONSUMER1;
		msg->priority = 0;
		msg->size = 32;
		if(os_msg_send(msg))
		{
			FREE(msg);
			msg = NULL;
		}
		os_sleep(1);
		
		msg = MALLOC(sizeof(*msg)+32);
		ASSERT(msg);
		MLOGM("msg2=%p\n",msg);
		os_memset(msg,0,(sizeof(*msg)+32));
		os_strcpy((char *)msg->data,"world");
		msg->src = TASK_PRODUCER;
		msg->dst = TASK_CONSUMER2;
		msg->priority = 0;
		msg->size = 32;
		if(os_msg_send(msg))
		{
			FREE(msg);
			msg = NULL;
		}
		os_sleep(1);
		if(cnt++ > 5)
			break;
	}
	
	return NULL;
}

static void *task_routine_consumer1(void *arg)
{
	const char *str = NULL;
	os_msg_t *msg;
	int cnt = 0;
	while (1)
	{
		msg = os_msg_recv();
		str = (char *)msg->data;
		MLOGM("[From: %s To: %s]str: %s\n",msg->src,msg->dst,str);
		FREE(msg);
		if(cnt++ > 3)
			break;
	}
	
	return NULL;
}

static void *task_routine_consumer2(void *arg)
{
	const char *str = NULL;
	os_msg_t *msg;
	int cnt = 0;
	while (1)
	{
		msg = os_msg_recv();
		str = (char *)msg->data;
		MLOGM("[From: %s To: %s]str: %s\n",msg->src,msg->dst,str);
		FREE(msg);
		if(cnt++ > 2)
			break;
	}
	
	return NULL;
}



int main(int argc ,char *argv[])  
{
	os_multicast_t *m = NULL;
	unsigned short port = 6666;
	char *pjson = NULL;
	if (argc != 2) {
		MLOGE("Usage: %s <port>\n", argv[0]);
		return -1;
	}
	
	ASSERT(1 == sscanf(argv[1],"%hu",&port));

	os_task_create(TASK_PRODUCER,0,0, task_routine_producer, (void *)NULL);
	os_task_create(TASK_CONSUMER1,0,0, task_routine_consumer1, (void *)NULL);
	os_task_create(TASK_CONSUMER2,0,0, task_routine_consumer2, (void *)NULL);

	
	m = os_multicast_create(MULTICAST_TYPE_SERVER,MUTICAST_ADDR,port);
	ASSERT(m);

	while(1)
	{
		if(0 == os_task_mm_json_get(&pjson))
		{
			MLOGD("\n%s\n",pjson);
			os_multicast_send(m, (unsigned char *)pjson, strlen(pjson));
			/////////////////////////////
			FREE(pjson);
		}
		/////////////////////////////
		os_sleep(1);
	}
	
	os_multicast_destroy(m);
	
	return 0;
}

