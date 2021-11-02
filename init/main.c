
#include "multitask.h"

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

	while(1)
	{
		if(os_system("clear"));
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
	}
	
	return 0;
}

