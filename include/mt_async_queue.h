#ifndef __MT_ASYNCQUEUE_H
#define __MT_ASYNCQUEUE_H

typedef struct mt_async_queue_s mt_async_queue_t;


mt_async_queue_t *mt_async_queue_new(void);
void mt_async_queue_free(mt_async_queue_t *q);
unsigned int mt_async_queue_length(mt_async_queue_t *q);
void mt_async_queue_push_tail(mt_async_queue_t *q,void *data);
void mt_async_queue_push_head(mt_async_queue_t *q,void *data);
void *mt_async_queue_pop_tail(mt_async_queue_t *q);
void *mt_async_queue_pop_head(mt_async_queue_t *q);

#endif

