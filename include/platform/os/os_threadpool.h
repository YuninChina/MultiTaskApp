#ifndef __MT_THREADPOOL_H
#define __MT_THREADPOOL_H


#define MAX_THREADS 64
#define MAX_QUEUE 65536


typedef struct mt_threadpool_s mt_threadpool_t;

mt_threadpool_t *mt_threadpool_create(int thread_count, int queue_size, int flags);
void mt_threadpool_destroy(mt_threadpool_t *pool, int flags);
int mt_threadpool_add(mt_threadpool_t *pool, void (*routine)(void *),void *arg, int flags);


#endif


