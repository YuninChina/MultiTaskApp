#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#include "multitask.h"
#include "glib.h"


os_async_queue_t *os_async_queue_new(void)
{
	os_async_queue_t *q = (os_async_queue_t *)g_async_queue_new();
	return q;
}

void os_async_queue_free(os_async_queue_t *q)
{
	if(q)
	{
		g_async_queue_unref((GAsyncQueue *)q);
	}
}

unsigned int os_async_queue_length(os_async_queue_t *q)
{
	return g_async_queue_length((GAsyncQueue *)q);
}

void os_async_queue_push_tail(os_async_queue_t *q,void *data)
{
	g_async_queue_push((GAsyncQueue *)q,data);
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
	data = g_async_queue_pop((GAsyncQueue *)q);
	return data;
}


