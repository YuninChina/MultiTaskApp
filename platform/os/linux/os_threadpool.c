#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

#include "mt_async_queue.h"
#include "os_threadpool.h"
#include "mt_log.h"
#include "mt_misc.h"


typedef struct {
    void (*function)(void *);
    void *argument;
} mt_threadpool_task_t;

struct mt_threadpool_s {
	int thread_count;
	int queue_size;
	int shutdown;
	int started;
	pthread_t *threads;
	mt_async_queue_t *queue;
};

static void *mt_threadpool_thread(void *threadpool)
{
	mt_threadpool_t *pool = (mt_threadpool_t *)threadpool;
	RETURN_VAL_IF_FAIL(pool, NULL);
	mt_threadpool_task_t *task = NULL;
	thread_name_set("threadpool");
	while(1)
	{
		task = (mt_threadpool_task_t *)mt_async_queue_pop_head(pool->queue);
		if(task && task->function)
		{
			task->function(task->argument);
			free(task);
			task = NULL;
		}
	}
	
	return NULL;
}

mt_threadpool_t *mt_threadpool_create(int thread_count, int queue_size, int flags)
{
	mt_threadpool_t *pool = NULL;
	int i;
	RETURN_VAL_IF_FAIL(thread_count > 0 && thread_count < MAX_THREADS 
	&& queue_size > 0 && queue_size < MAX_QUEUE, NULL);
	pool = (mt_threadpool_t *)malloc(sizeof(mt_threadpool_t));
	RETURN_VAL_IF_FAIL(pool, NULL);
	/* Initialize */
	pool->thread_count = 0;
	pool->queue_size = queue_size;
	pool->shutdown = pool->started = 0;
	/* Allocate thread and task queue */
	pool->queue = mt_async_queue_new();
	GOTO_LABEL_IF_FAIL(pool->queue, label1);
	pool->threads = (pthread_t *)malloc(sizeof(pthread_t) * thread_count);
	GOTO_LABEL_IF_FAIL(pool->threads, label2);
	/* Start worker threads */
	for(i = 0; i < thread_count; i++) {
	   if(pthread_create(&(pool->threads[i]), NULL,
						 mt_threadpool_thread, (void*)pool) != 0) {
		   mt_threadpool_destroy(pool, 0);
		   return NULL;
	   }
	   pool->thread_count++;
	   pool->started++;
	}
   
	return pool;
	
label2:
	if(pool && pool->queue) mt_async_queue_free(pool->queue);
	pool->queue = NULL;

label1:
	if(pool) free(pool);
	pool = NULL;
	
	return NULL;
}

void mt_threadpool_destroy(mt_threadpool_t *pool, int flags)
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
		if(pool && pool->queue) mt_async_queue_free(pool->queue);
		pool->queue = NULL;
	}
}

int mt_threadpool_add(mt_threadpool_t *pool, void (*routine)(void *),void *arg, int flags)
{
	mt_threadpool_task_t *task = NULL;
	RETURN_VAL_IF_FAIL(pool, -1);
	RETURN_VAL_IF_FAIL(pool->queue_size < MAX_QUEUE, -1);
	task = malloc(sizeof(*task));
	RETURN_VAL_IF_FAIL(task, -1);
	task->function = routine;
	task->argument = arg;
	mt_async_queue_push_tail(pool->queue, task);
	return 0;
}




