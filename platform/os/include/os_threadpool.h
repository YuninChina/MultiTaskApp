#ifndef __MT_THREADPOOL_H
#define __MT_THREADPOOL_H

#ifdef  __cplusplus
extern "C" {
#endif

#define MAX_THREADS 64
#define MAX_QUEUE 65536


typedef struct os_threadpool_s os_threadpool_t;

os_threadpool_t *os_threadpool_create(int thread_count, int queue_size, int flags);
void os_threadpool_destroy(os_threadpool_t *pool, int flags);
int os_threadpool_add(os_threadpool_t *pool, void (*routine)(void *),void *arg, int flags);


#ifdef  __cplusplus
}
#endif


#endif


