#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#include "multitask.h"
#include "glib.h"


#define os_async_queue_s _GAsyncQueue

os_async_queue_t *os_async_queue_new(void)
{
	os_async_queue_t *q = NULL;
	return q;
}

void os_async_queue_free(os_async_queue_t *q)
{
	if(q)
	{
	}
}

unsigned int os_async_queue_length(os_async_queue_t *q)
{
	return 0;
}

void os_async_queue_push_tail(os_async_queue_t *q,void *data)
{
}

void os_async_queue_push_head(os_async_queue_t *q,void *data)
{
}

void *os_async_queue_pop_tail(os_async_queue_t *q)
{
	void *data = NULL;

	return data;
}

void *os_async_queue_pop_head(os_async_queue_t *q)
{
	void *data = NULL;

	return data;
}


