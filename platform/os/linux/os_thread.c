
#include <pthread.h>
#include "os_thread.h"


int os_pthread_create(os_pthread_t *tid, const os_pthread_attr_t *attr, void *(*func) (void *), void *arg)
{
	return 0;
}

int os_pthread_join (os_pthread_t tid, void ** status)
{
	return 0;
}

os_pthread_t os_pthread_self (void)
{
	return NULL;
}

int os_pthread_detach (os_pthread_t tid)
{
	return 0;
}

void os_pthread_exit (void *status)
{

}




