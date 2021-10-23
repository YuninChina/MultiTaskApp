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

#include "os_evdriver.h"
#include "multitask.h"



struct os_evdriver_s {
	struct list_head list;
	pthread_mutex_t mutex;
	/* INFO */
	const char *owner;
	int loop;
};

struct os_evdriver_node_s {
	struct list_head list;
	os_evdriver_t *evdriver;
	int event_fd;
	os_evdriver_callback_t cb;
	void *user_data;
};


static int __evdriver_epoll_add(os_evdriver_node_t *node,unsigned int events)
{
	struct epoll_event ev;
	RETURN_VAL_IF_FAIL(node, -1);
	ev.events   = events | EPOLLRDHUP;
	ev.data.ptr = node;
	if (epoll_ctl(node->evdriver->loop, EPOLL_CTL_ADD, node->event_fd, &ev) < 0) {
		if (errno != EPERM)
			return -1;
	}
	return 0;
}


static int __evdriver_io_add(os_evdriver_node_t *node, int fd)
{
	node->event_fd = fd;
	return __evdriver_epoll_add(node,EPOLLIN);
}

static int __evdriver_signal_add(os_evdriver_node_t *node, int signo)
{
	int fd = -1;
	sigset_t mask;
	
	node->event_fd = fd;
	return 0;
}

static int __evdriver_timer_add(os_evdriver_node_t *node, unsigned long when,unsigned long interval)
{
	int fd = -1;
	node->event_fd = fd;
	return 0;
}



os_evdriver_t *os_evdriver_create2(const char *name)
{
	os_evdriver_t *evdriver = NULL;
	evdriver = MALLOC(sizeof(*evdriver));
	RETURN_VAL_IF_FAIL(evdriver, NULL);
	evdriver->loop = -1;
	evdriver->owner = name;
	pthread_mutex_init(&evdriver->mutex, NULL);
	INIT_LIST_HEAD(&evdriver->list);
	return evdriver;
}

os_evdriver_t *os_evdriver_create(void)
{
	return os_evdriver_create2("evdriver");
}


void os_evdriver_destroy(os_evdriver_t *evdriver)
{
	os_evdriver_node_t *node = NULL,*tmp = NULL;
	if(evdriver)
	{
		pthread_mutex_lock(&evdriver->mutex);
		list_for_each_entry_safe(node, tmp,&evdriver->list, list) {
			list_del(&node->list);
			close(node->event_fd);
			node->event_fd = -1;
			FREE(node);
			node = NULL;
		}
		pthread_mutex_unlock(&evdriver->mutex);
		pthread_mutex_destroy(&evdriver->mutex);
		FREE(evdriver);
		evdriver = NULL;
	}
}

int os_evdriver_run(os_evdriver_t *evdriver)
{
	return 0;
}



os_evdriver_node_t *os_evdriver_add(os_evdriver_t *evdriver,os_event_t *ev,os_evdriver_callback_t cb,void *user_data)
{
	os_evdriver_node_t *node = NULL;
	int ret = -1;
	RETURN_VAL_IF_FAIL(evdriver && ev && cb, NULL);
	RETURN_VAL_IF_FAIL(ev->event < OS_EVDRIVER_BUTT, NULL);
	node = MALLOC(sizeof(*evdriver));
	RETURN_VAL_IF_FAIL(node, NULL);
	node->cb = cb;
	node->user_data = user_data;
	node->event_fd = -1;
	node->evdriver = evdriver;
	switch (ev->event)
	{
		case OS_EVDRIVER_IO: 
		{
			ret = __evdriver_io_add(node,ev->io.fd);
		}break;
		case OS_EVDRIVER_TIMER: 
		{
			ret = __evdriver_timer_add(node,ev->timer.when,ev->timer.interval);
		}break;
		case OS_EVDRIVER_SIGNAL: 
		{
			ret = __evdriver_signal_add(node,ev->signal.signo);
		}break;
		default:
		{
			ASSERT(0);
		}break;
	}
	pthread_mutex_lock(&evdriver->mutex);
	list_add_tail(&node->list, &evdriver->list);
	pthread_mutex_unlock(&evdriver->mutex);
	return node;
}

void os_evdriver_del(os_evdriver_t *evdriver,os_evdriver_node_t *evnode)
{
	os_evdriver_node_t *node = NULL,*tmp = NULL;
	if(NULL == evnode) return;
	
	if(evdriver)
	{
		pthread_mutex_lock(&evdriver->mutex);
		list_for_each_entry_safe(node, tmp,&evdriver->list, list) {
			if(evnode == node)
			{
				list_del(&node->list);
				close(node->event_fd);
				node->event_fd = -1;
				FREE(node);
				node = NULL;
				break;
			}
		}
		pthread_mutex_unlock(&evdriver->mutex);
	}
}





