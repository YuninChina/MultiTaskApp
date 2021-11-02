#ifndef __MT_THREADPOOL_H
#define __MT_THREADPOOL_H

#ifdef  __cplusplus
extern "C" {
#endif

#define MAX_THREADS 64
#define MAX_QUEUE 65536


typedef struct mt_threadpool_s mt_threadpool_t;

mt_threadpool_t *mt_threadpool_create(int thread_count, int queue_size, int flags);
void mt_threadpool_destroy(mt_threadpool_t *pool, int flags);
int mt_threadpool_add(mt_threadpool_t *pool, void (*routine)(void *),void *arg, int flags);


#ifdef  __cplusplus
}
#endif


#endif


