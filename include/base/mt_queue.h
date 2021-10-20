#ifndef __MT_QUEUE_H
#define __MT_QUEUE_H
#include "klist.h"

typedef struct mt_queue_s mt_queue_t;


mt_queue_t *mt_queue_new(void);
void mt_queue_free(mt_queue_t *q);
unsigned int mt_queue_length(mt_queue_t *q);
unsigned int mt_queue_empty(mt_queue_t *q);
void mt_queue_push_tail(mt_queue_t *q,void *data);
void mt_queue_push_head(mt_queue_t *q,void *data);
void *mt_queue_pop_tail(mt_queue_t *q);
void *mt_queue_pop_head(mt_queue_t *q);

#endif
