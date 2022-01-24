#include <time.h>
#include <stdio.h>
#include <strings.h>


#include "os_timer.h"

#ifdef  __cplusplus
extern "C" {
#endif


long long get_monotonic_time (void)
{
	struct timespec ts;
	clock_gettime (CLOCK_MONOTONIC, &ts);
	return (((long long) ts.tv_sec) * 1000000) + (ts.tv_nsec / 1000);
}


void os_timer_start(os_timer_t *timer)
{
	if(timer)
	{
		bzero(timer,sizeof(*timer));
		timer->start = get_monotonic_time();
	}
}

unsigned int os_timer_stop(os_timer_t *timer)
{
	unsigned int time = 0;
	long long elapsed = 0;
	if(timer)
	{
		timer->end = get_monotonic_time();
		elapsed = timer->end - timer->start;
		time = elapsed % 1000000;
	}
	return time;
}




#ifdef  __cplusplus
}
#endif


