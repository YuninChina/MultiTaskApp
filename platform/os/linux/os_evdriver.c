#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <assert.h>
#include <unistd.h>

#include "os_evdriver.h"
#include "multitask.h"



struct os_evdriver_s {
	struct list_head list;
	pthread_mutex_t mutex;
	/* INFO */
	const char *owner;
	int loop;
};

typedef struct os_evdriver_node_s {
	struct list_head list;
	int fd;
	os_evdriver_type_e event;
	os_evdriver_callback_t cb;
	void *user_data;
}os_evdriver_node_t;



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
			close(node->fd);
			node->fd = -1;
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

int os_evdriver_add(os_evdriver_t *evdriver,int event,int fd,os_evdriver_callback_t cb,void *user_data)
{
	os_evdriver_node_t *node = NULL;
	RETURN_VAL_IF_FAIL(evdriver && event < OS_EVDRIVER_BUTT&& cb, -1);
	node = MALLOC(sizeof(*evdriver));
	RETURN_VAL_IF_FAIL(node, -1);
	node->cb = cb;
	node->event = event;
	node->fd = fd;
	node->user_data = user_data;
	pthread_mutex_lock(&evdriver->mutex);
	list_add_tail(&node->list, &evdriver->list);
	pthread_mutex_unlock(&evdriver->mutex);
	return 0;
}

void os_evdriver_del(os_evdriver_t *evdriver,int fd)
{
	os_evdriver_node_t *node = NULL,*tmp = NULL;
	if(fd < 0) return;
	
	if(evdriver)
	{
		pthread_mutex_lock(&evdriver->mutex);
		list_for_each_entry_safe(node, tmp,&evdriver->list, list) {
			if(fd == node->fd)
			{
				list_del(&node->list);
				close(node->fd);
				node->fd = -1;
				FREE(node);
				node = NULL;
				break;
			}
		}
		pthread_mutex_unlock(&evdriver->mutex);
	}
}





