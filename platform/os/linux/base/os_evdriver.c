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

#include "os_evdriver.h"
#include "multitask.h"

#define MAX_EVENTS  10 


struct os_evdriver_s {
	struct list_head list;
	pthread_mutex_t mutex;
	/* INFO */
	const char *owner;
	int loop;
	int running;
};

struct os_evdriver_node_s {
	struct list_head list;
	os_evdriver_t *evdriver;
	int event_fd;
	os_evdriver_type_e event_type;
	os_evdriver_callback_t cb;
	void *user_data;
};

static void msec2tspec(int msec, struct timespec *ts)
{
	if (msec) {
		ts->tv_sec  =  msec / 1000;
		ts->tv_nsec = (msec % 1000) * 1000000;
	} else {
		ts->tv_sec  = 0;
		ts->tv_nsec = 0;
	}
}

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
	sigemptyset(&mask);
	sigaddset(&mask, signo);
	
	fd = signalfd(-1, &mask, SFD_NONBLOCK | SFD_CLOEXEC);
	if (fd < 0)
		return -1;
	if (sigprocmask(SIG_BLOCK, &mask, NULL) == -1)
		return -1;
 	if (signalfd(fd, &mask, SFD_NONBLOCK) < 0)
		return -1;
	node->event_fd = fd;
	return 0;
}

static int __evdriver_timer_add(os_evdriver_node_t *node, unsigned long when,unsigned long interval)
{
	int fd = -1;
	struct itimerspec time;
	
	msec2tspec(when, &time.it_value);
	msec2tspec(interval, &time.it_interval);
	fd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
	RETURN_VAL_IF_FAIL(fd > 0,-1);
	timerfd_settime(fd, 0, &time, NULL);
	node->event_fd = fd;
	return __evdriver_epoll_add(node,EPOLLIN);
}



os_evdriver_t *os_evdriver_create2(const char *name)
{
	os_evdriver_t *evdriver = NULL;
	evdriver = MALLOC(sizeof(*evdriver));
	RETURN_VAL_IF_FAIL(evdriver, NULL);
	memset(evdriver,0,sizeof(*evdriver));
	evdriver->loop = -1;
	evdriver->owner = name;
	evdriver->running = 0;
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
		evdriver->running = 0;
		pthread_mutex_lock(&evdriver->mutex);
		list_for_each_entry_safe(node, tmp,&evdriver->list, list) {
			list_del(&node->list);
			epoll_ctl(node->evdriver->loop, EPOLL_CTL_DEL, node->event_fd, NULL);
			close(node->event_fd);
			node->event_fd = -1;
			FREE(node);
			node = NULL;
		}
		pthread_mutex_unlock(&evdriver->mutex);
		pthread_mutex_destroy(&evdriver->mutex);
		if(evdriver->loop > -1) close(evdriver->loop);
		evdriver->loop = -1;
		FREE(evdriver);
		evdriver = NULL;
	}
}

int os_evdriver_run2(os_evdriver_t *evdriver,int flags)
{
	struct epoll_event ee[MAX_EVENTS];
	int i, nfds;
	os_evdriver_node_t *ev_node = NULL;
	int timeout = -1;
	
	if (flags & EVDIRVER_NONBLOCK)
		timeout = 0;
	
	RETURN_VAL_IF_FAIL(evdriver && evdriver->loop >= 0,-1);
	
	evdriver->running = 1;
	while(evdriver->running)
	{
		unsigned int events;
		nfds = epoll_wait(evdriver->loop, ee, MAX_EVENTS,timeout);
		//printf("nfds=%d\n",nfds);
		for (i = 0; i < nfds; i++) 
		{
			struct signalfd_siginfo fdsi;
			uint64_t exp;
			
			ev_node = (os_evdriver_node_t *)ee[i].data.ptr;
			CONTINUE_IF_FAIL(ev_node);
			events = ee[i].events;
			switch (ev_node->event_type) {
				case OS_EVDRIVER_IO:
					if (events & (EPOLLHUP | EPOLLERR))
					{
						ASSERT(0);
					}
					break;

				case OS_EVDRIVER_SIGNAL:
					{
						if (read(ev_node->event_fd, &fdsi, sizeof(fdsi)) != sizeof(fdsi)) 
						{
							ASSERT(0);
						} 
					}
					break;

				case OS_EVDRIVER_TIMER:
					{
						if (read(ev_node->event_fd, &exp, sizeof(exp)) != sizeof(exp)) {
							ASSERT(0);
						}
					}
					break;
				default:
					ASSERT(0);
					break;
			}
			if(ev_node->cb) ev_node->cb(ev_node,ev_node->user_data);
			
		}
		if (flags & EVDIRVER_ONCE)
			break;
	}
	return 0;
}

int os_evdriver_run(os_evdriver_t *evdriver)
{
	return os_evdriver_run2(evdriver,0);
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
	node->event_type = ev->event;
	switch (ev->event)
	{
		case OS_EVDRIVER_IO: 
		{
			ret = __evdriver_io_add(node,ev->io.fd);
			GOTO_LABEL_IF_FAIL(0 == ret,fail);
		}break;
		case OS_EVDRIVER_TIMER: 
		{
			ret = __evdriver_timer_add(node,ev->timer.when,ev->timer.interval);
			GOTO_LABEL_IF_FAIL(0 == ret,fail);
		}break;
		case OS_EVDRIVER_SIGNAL: 
		{
			ret = __evdriver_signal_add(node,ev->signal.signo);
			GOTO_LABEL_IF_FAIL(0 == ret,fail);
		}break;
		default:
		{
			GOTO_LABEL_IF_FAIL(0,fail);
		}break;
	}
	pthread_mutex_lock(&evdriver->mutex);
	list_add_tail(&node->list, &evdriver->list);
	pthread_mutex_unlock(&evdriver->mutex);
	return node;
fail:
	FREE(node);
	return NULL;
}

void os_evdriver_del(os_evdriver_node_t *evnode)
{
	os_evdriver_node_t *node = NULL,*tmp = NULL;
	RETURN_IF_FAIL(evnode);
	os_evdriver_t *evdriver = evnode->evdriver;
	if(evdriver)
	{
		pthread_mutex_lock(&evdriver->mutex);
		list_for_each_entry_safe(node, tmp,&evdriver->list, list) {
			if(evnode == node)
			{
				list_del(&node->list);
				/* Remove from kernel */
				epoll_ctl(evdriver->loop, EPOLL_CTL_DEL, evnode->event_fd, NULL);
				close(evnode->event_fd);
				evnode->event_fd = -1;
				break;
			}
		}
		pthread_mutex_unlock(&evdriver->mutex);
	}
}






