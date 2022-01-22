#ifndef __MT_QUEUE_H
#define __MT_QUEUE_H
#include "klist.h"


#ifdef  __cplusplus
extern "C" {
#endif

typedef struct os_queue_s os_queue_t;

os_queue_t *os_queue_new(void);
void os_queue_free(os_queue_t *q);
unsigned int os_queue_length(os_queue_t *q);
unsigned int os_queue_empty(os_queue_t *q);
void os_queue_push_tail(os_queue_t *q,void *data);
void os_queue_push_head(os_queue_t *q,void *data);
void *os_queue_pop_tail(os_queue_t *q);
void *os_queue_pop_head(os_queue_t *q);


#ifdef  __cplusplus
}
#endif


#endif
