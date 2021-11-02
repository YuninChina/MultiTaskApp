#include "multitask.h"


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
		ASSERT(p);
		cnt++;
		FREE(p);
		os_sleep(1);
		if(cnt > 15)
			break;
		
	}
	return NULL;
}


static void *task_routine_dummy(void *arg)
{
	int cnt = 0;
	void *p = NULL;
	while (1)
	{
		p = MALLOC((cnt+1)*64);
		ASSERT(p);
		cnt++;
		os_sleep(1);
		if(cnt > 20)
			break;
	}
	
	return NULL;
}


int main(void)
{
	void *p1,*p2,*p3;
	int cnt = 0;
	p1 = MALLOC(10);
	ASSERT(p1);
	p2 = MALLOC(20);
	ASSERT(p1);
	p3 = MALLOC(30);
	ASSERT(p1);

	
	os_task_t *t1 = NULL,*t2 = NULL,*t3 = NULL,*t4 = NULL;
	
	t1 = os_task_create("no1",0,0, task_routine_no1, (void *)NULL);
	ASSERT(t1);
	t2 = os_task_create("no2",0,0, task_routine_no2, (void *)NULL);
	ASSERT(t2);
	t3 = os_task_create("normal",0,0, task_routine_normal, (void *)NULL);
	ASSERT(t3);
	t4 = os_task_create("dummy",0,0, task_routine_dummy, (void *)NULL);
	ASSERT(t4);
	
	while(1)
	{
		os_system("clear");
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
		if(cnt > 10)
			break;
	}

	os_mm_show();
	
	return 0;
}

