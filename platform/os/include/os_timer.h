


#ifndef __OS_TIMER__H
#define __OS_TIMER__H

#ifdef  __cplusplus
extern "C" {
#endif

typedef struct os_timer_s 
{
	unsigned long long start;
	unsigned long long end;
}os_timer_t;


void os_timer_start(os_timer_t *timer);
unsigned int os_timer_stop(os_timer_t *timer);


#ifdef  __cplusplus
}
#endif

#endif 





