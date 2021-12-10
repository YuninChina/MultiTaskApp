#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#include "os_queue.h"
#include "os_async_queue.h"
#include "os_log.h"
#include "multitask.h"


struct os_async_queue_s {
	pthread_cond_t cond;
	pthread_mutex_t mutex;
	os_queue_t *queue;
	void (*destroy)(os_async_queue_t *q,void *data);
};


os_async_queue_t *os_async_queue_new(void)
{
	os_async_queue_t *q = NULL;
	q = MALLOC(sizeof(*q));
	RETURN_VAL_IF_FAIL(q, NULL);
	memset(q,0,sizeof(*q));	
	q->queue = os_queue_new();
	GOTO_LABEL_IF_FAIL(q->queue, fail);
	pthread_mutex_init(&q->mutex, NULL);
	pthread_cond_init(&q->cond, NULL);
	
	return q;
fail:
	FREE(q);
	q = NULL;
	return NULL;
}

void os_async_queue_free(os_async_queue_t *q)
{
	if(q)
	{
		pthread_cond_broadcast(&(q->cond));
		pthread_cond_destroy(&q->cond);
		pthread_mutex_destroy(&q->mutex);
		os_queue_free(q->queue);
		if(q->destroy) q->destroy(q,NULL);
		FREE(q);
		q = NULL;
	}
}

unsigned int os_async_queue_length(os_async_queue_t *q)
{
	return os_queue_length(q->queue);
}

void os_async_queue_push(os_async_queue_t *q,void *data)
{
	RETURN_IF_FAIL(q && data);
	pthread_mutex_lock(&q->mutex);
	os_queue_push_tail(q->queue,data);
	pthread_cond_signal(&q->cond);
	pthread_mutex_unlock(&q->mutex);
}


void *os_async_queue_pop(os_async_queue_t *q)
{
	void *data = NULL;
	RETURN_VAL_IF_FAIL(q,NULL);
	pthread_mutex_lock(&q->mutex);
	pthread_cond_wait(&q->cond, &q->mutex);
	data = os_queue_pop_head(q->queue);
	pthread_mutex_unlock(&q->mutex);
	return data;
}


