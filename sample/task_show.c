#include "multitask.h"


#define TASK_PRODUCER	"producer"
#define TASK_CONSUMER1	"consumer1"
#define TASK_CONSUMER2	"consumer2"

static void *task_routine_no1(void *arg)
{
	int cnt = 0;
	void *p = NULL;
	while (1)
	{
		p = MALLOC((cnt+1)*16);
		ASSERT(p);
		os_sleep(1);
		cnt++;
		if(cnt > 5)
			break;
	}
	return NULL;
}

static void *task_routine_no2(void *arg)
{
	int cnt = 0;
	void *p = NULL;
	while (1)
	{
		p = MALLOC((cnt+1)*512);
		ASSERT(p);
		os_sleep(1);
		cnt++;
		if(cnt > 10)
			break;
	}
	return NULL;
}


static void *task_routine_normal(void *arg)
{
	int cnt = 0;
	void *p = NULL;
	while (1)
	{
		p = MALLOC((cnt+1)*2048);
		cnt++;
		FREE(p);
		ASSERT(p);
		os_sleep(1);
		if(cnt > 15)
			break;
		
	}
	return NULL;
}


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
		str = (const char *)msg->data;
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
		str = (const char *)msg->data;
		MLOGM("[From: %s To: %s]str: %s\n",msg->src,msg->dst,str);
		FREE(msg);
		if(cnt++ > 2)
			break;
	}
	
	return NULL;
}




int main(void)
{
	void *p1,*p2,*p3;
	int cnt = 0;
	MLOGD("Task start ....");
	p1 = MALLOC(10);
	ASSERT(p1);
	p2 = MALLOC(20);
	ASSERT(p1);
	p3 = MALLOC(30);
	ASSERT(p1);
	
	os_task_create("no1",0,0, task_routine_no1, (void *)NULL);
	os_task_create("no2",0,0, task_routine_no2, (void *)NULL);
	os_task_create("normal",0,0, task_routine_normal, (void *)NULL);
	os_task_create(TASK_PRODUCER,0,0, task_routine_producer, (void *)NULL);
	os_task_create(TASK_CONSUMER1,0,0, task_routine_consumer1, (void *)NULL);
	os_task_create(TASK_CONSUMER2,0,0, task_routine_consumer2, (void *)NULL);
	
	while(1)
	{
		//system("clear");
		os_task_mm_show();
		os_sleep(1);

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
		if(cnt > 20)
			break;
	}

	os_mm_show();
	return 0;
}

