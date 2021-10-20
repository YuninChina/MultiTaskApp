#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <assert.h>
#include <sys/prctl.h>

#include "mm.h"
#include "task.h"
#include "mt_msg.h"
#include "mt_log.h"


#define TASK_PRODUCER	"producer"
#define TASK_CONSUMER1	"consumer1"
#define TASK_CONSUMER2	"consumer2"

static void *task_routine_no1(task_self_t *self,void *arg)
{
	int cnt = 0;
	void *p = NULL;
	while (1)
	{
		p = MALLOC((cnt+1)*16);
		sleep(1);
		cnt++;
		if(cnt > 5)
			break;
	}
	return NULL;
}

static void *task_routine_no2(task_self_t *self,void *arg)
{
	int cnt = 0;
	void *p = NULL;
	while (1)
	{
		p = MALLOC((cnt+1)*512);
		sleep(1);
		cnt++;
		if(cnt > 10)
			break;
	}
	return NULL;
}


static void *task_routine_normal(task_self_t *self,void *arg)
{
	int cnt = 0;
	void *p = NULL;
	while (1)
	{
		p = MALLOC((cnt+1)*2048);
		cnt++;
		FREE(p);
		sleep(1);
		if(cnt > 15)
			break;
		
	}
	return NULL;
}


static void *task_routine_producer(task_self_t *self,void *arg)
{
	mt_msg_t *msg;
	while (1)
	{
		msg = MALLOC(sizeof(*msg)+32);
		assert(msg);
		memset(msg,0,(sizeof(*msg)+32));
		strcpy(msg->data,"hello");
		msg->src = TASK_PRODUCER;
		msg->dst = TASK_CONSUMER1;
		msg->priority = 0;
		msg->size = 32;
		mt_msg_send(msg);
		sleep(1);
		
		msg = MALLOC(sizeof(*msg)+32);
		assert(msg);
		memset(msg,0,(sizeof(*msg)+32));
		strcpy(msg->data,"world");
		msg->src = TASK_PRODUCER;
		msg->dst = TASK_CONSUMER2;
		msg->priority = 0;
		msg->size = 32;
		mt_msg_send(msg);
		sleep(2);
	}
	
	return NULL;
}

static void *task_routine_consumer1(task_self_t *self,void *arg)
{
	const char *str = NULL;
	mt_msg_t *msg;
	while (1)
	{
		msg = mt_msg_recv();
		str = msg->data;
		MLOGM("[From: %s To: %s]str: %s\n",msg->src,msg->dst,str);
		FREE(msg);
	}
	
	return NULL;
}

static void *task_routine_consumer2(task_self_t *self,void *arg)
{
	const char *str = NULL;
	mt_msg_t *msg;
	while (1)
	{
		msg = mt_msg_recv();
		str = msg->data;
		MLOGM("[From: %s To: %s]str: %s\n",msg->src,msg->dst,str);
		FREE(msg);
	}
	
	return NULL;
}




int main(void)
{
	void *p1,*p2,*p3;
	int cnt = 0;
	MLOGD("Task start ....");
	p1 = MALLOC(10);
	assert(p1);
	p2 = MALLOC(20);
	assert(p1);
	p3 = MALLOC(30);
	assert(p1);
	
	task_create("no1",0,0, task_routine_no1, (void *)NULL);
	task_create("no2",0,0, task_routine_no2, (void *)NULL);
	task_create("normal",0,0, task_routine_normal, (void *)NULL);
	task_create(TASK_PRODUCER,0,0, task_routine_producer, (void *)NULL);
	task_create(TASK_CONSUMER1,0,0, task_routine_consumer1, (void *)NULL);
	task_create(TASK_CONSUMER2,0,0, task_routine_consumer2, (void *)NULL);
	
	while(1)
	{
		//system("clear");
		task_mm_show();
		mm_show();
		sleep(1);

		if(cnt == 3*1)
		{
			if(p1) {FREE(p1); p1=NULL;}
		}
		if(cnt == 3*2)
		{
			if(p2) {FREE(p2); p2=NULL;}
		}
		if(cnt == 3*3)
		{
			if(p3) {FREE(p3); p3=NULL;}
		}
		cnt++;
		
	}

	
	return 0;
}

