#include <stdio.h>   

#include "os_evdriver.h"



struct os_evdriver_s {
	const char *owner;
	int loop;
};


os_evdriver_t *os_evdriver_create(void)
{
	os_evdriver_t *evdriver = NULL;
	return evdriver;
}

void os_evdriver_destroy(os_evdriver_t *evdriver)
{

}

int os_evdriver_run(os_evdriver_t *evdriver)
{
	return 0;
}

int os_evdriver_add(os_evdriver_t *evdriver,int type,os_evdriver_callback_t *cb,void *user_data)
{
	return 0;
}

int os_evdriver_del(os_evdriver_t *evdriver,int type,os_evdriver_callback_t *cb,void *user_data)
{
	return 0;
}





