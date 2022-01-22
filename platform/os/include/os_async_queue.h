#ifndef __MT_ASYNCQUEUE_H
#define __MT_ASYNCQUEUE_H

#ifdef  __cplusplus
extern "C" {
#endif


typedef struct os_async_queue_s os_async_queue_t;

os_async_queue_t *os_async_queue_new(void);
void os_async_queue_free(os_async_queue_t *q);
unsigned int os_async_queue_length(os_async_queue_t *q);
void os_async_queue_push(os_async_queue_t *q,void *data);
void *os_async_queue_pop(os_async_queue_t *q);


#ifdef  __cplusplus
}
#endif


#endif

