#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <assert.h>
#include <unistd.h>

#include <errno.h>
#include <fcntl.h>		/* O_CLOEXEC */
#include <string.h>		/* memset() */
#include <sys/epoll.h>
#include <sys/ioctl.h>
#include <sys/select.h>		/* for select() workaround */
#include <sys/signalfd.h>	/* struct signalfd_siginfo */
#include <sys/timerfd.h>
#include <signal.h>

#include "multitask.h"

#define MAX_EVENTS  10 


struct os_evdriver_s {
	int dummy;
};

struct os_evdriver_node_s {
	int dummy;
};

os_evdriver_t *os_evdriver_create2(const char *name)
{
	os_evdriver_t *evdriver = NULL;

	return evdriver;
}

os_evdriver_t *os_evdriver_create(void)
{
	return os_evdriver_create2("evdriver");
}


void os_evdriver_destroy(os_evdriver_t *evdriver)
{
	
}

int os_evdriver_run2(os_evdriver_t *evdriver,int flags)
{
	
	return 0;
}

int os_evdriver_run(os_evdriver_t *evdriver)
{
	return os_evdriver_run2(evdriver,0);
}

os_evdriver_node_t *os_evdriver_add(os_evdriver_t *evdriver,os_event_t *ev,os_evdriver_callback_t cb,void *user_data)
{
	os_evdriver_node_t *node = NULL;
	
	return node;
}

void os_evdriver_del(os_evdriver_node_t *evnode)
{
	
}



