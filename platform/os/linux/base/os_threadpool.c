#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

#include "os_async_queue.h"
#include "os_threadpool.h"
#include "os_log.h"
#include "os_misc.h"


typedef struct {
    void (*function)(void *);
    void *argument;
} os_threadpool_task_t;

struct os_threadpool_s {
	int thread_count;
	int queue_size;
	int shutdown;
	int started;
	pthread_t *threads;
	os_async_queue_t *queue;
};

static void *os_threadpool_thread(void *threadpool)
{
	os_threadpool_t *pool = (os_threadpool_t *)threadpool;
	RETURN_VAL_IF_FAIL(pool, NULL);
	os_threadpool_task_t *task = NULL;
	thread_name_set("threadpool");
	while(1)
	{
		task = (os_threadpool_task_t *)os_async_queue_pop(pool->queue);
		if(task && task->function)
		{
			task->function(task->argument);
			free(task);
			task = NULL;
		}
	}
	
	return NULL;
}

os_threadpool_t *os_threadpool_create(int thread_count, int queue_size, int flags)
{
	os_threadpool_t *pool = NULL;
	int i;
	RETURN_VAL_IF_FAIL(thread_count > 0 && thread_count < MAX_THREADS 
	&& queue_size > 0 && queue_size < MAX_QUEUE, NULL);
	pool = (os_threadpool_t *)malloc(sizeof(os_threadpool_t));
	RETURN_VAL_IF_FAIL(pool, NULL);
	/* Initialize */
	pool->thread_count = 0;
	pool->queue_size = queue_size;
	pool->shutdown = pool->started = 0;
	/* Allocate thread and task queue */
	pool->queue = os_async_queue_new();
	GOTO_LABEL_IF_FAIL(pool->queue, label1);
	pool->threads = (pthread_t *)malloc(sizeof(pthread_t) * thread_count);
	GOTO_LABEL_IF_FAIL(pool->threads, label2);
	/* Start worker threads */
	for(i = 0; i < thread_count; i++) {
	   if(pthread_create(&(pool->threads[i]), NULL,
						 os_threadpool_thread, (void*)pool) != 0) {
		   os_threadpool_destroy(pool, 0);
		   return NULL;
	   }
	   pool->thread_count++;
	   pool->started++;
	}
	return pool;
	
label2:
	if(pool && pool->queue) os_async_queue_free(pool->queue);
	pool->queue = NULL;

label1:
	if(pool) free(pool);
	pool = NULL;
	
	return NULL;
}

void os_threadpool_destroy(os_threadpool_t *pool, int flags)
{
	int i;
	if(pool)
	{
		/* Join all worker thread */
        for(i = 0; i < pool->thread_count; i++) {
            if(pthread_join(pool->threads[i], NULL) != 0) {
                ASSERT(0);
            }
        }
		if(pool && pool->threads) free(pool->threads);
		pool->threads = NULL;
		if(pool && pool->queue) os_async_queue_free(pool->queue);
		pool->queue = NULL;
	}
}

int os_threadpool_add(os_threadpool_t *pool, void (*routine)(void *),void *arg, int flags)
{
	os_threadpool_task_t *task = NULL;
	RETURN_VAL_IF_FAIL(pool, -1);
	RETURN_VAL_IF_FAIL(pool->queue_size < MAX_QUEUE, -1);
	task = malloc(sizeof(*task));
	RETURN_VAL_IF_FAIL(task, -1);
	task->function = routine;
	task->argument = arg;
	os_async_queue_push(pool->queue, task);
	return 0;
}




