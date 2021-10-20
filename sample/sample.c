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


static void *task_routine_no1(void *arg)
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

static void *task_routine_no2(void *arg)
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


static void *task_routine_normal(void *arg)
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


static void *task_routine_dummy(void *arg)
{
	int cnt = 0;
	void *p = NULL;
	while (1)
	{
		p = MALLOC((cnt+1)*64);
		cnt++;
		sleep(1);
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
	assert(p1);
	p2 = MALLOC(20);
	assert(p1);
	p3 = MALLOC(30);
	assert(p1);

	
	task_t *t1 = NULL,*t2 = NULL,*t3 = NULL,*t4 = NULL;
	
	t1 = task_create("no1",0,0, task_routine_no1, (void *)NULL);
	t2 = task_create("no2",0,0, task_routine_no2, (void *)NULL);
	t3 = task_create("normal",0,0, task_routine_normal, (void *)NULL);
	t4 = task_create("dummy",0,0, task_routine_dummy, (void *)NULL);
	
	while(1)
	{
		system("clear");
		task_mm_show();
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
		if(cnt > 10)
			break;
	}

	mm_show();
	
	return 0;
}

