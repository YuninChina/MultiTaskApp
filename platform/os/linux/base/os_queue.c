#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "os_queue.h"
#include "os_log.h"
#include "multitask.h"

struct os_queue_s {
	struct list_head list;
	void *data;
};


os_queue_t *os_queue_new(void)
{
	os_queue_t *q = NULL;
	q = MALLOC(sizeof(*q));
	RETURN_VAL_IF_FAIL(q, NULL);
	memset(q,0,sizeof(*q));
	INIT_LIST_HEAD(&q->list);
	return q;
}

void os_queue_free(os_queue_t *q)
{
	os_queue_t *queue = NULL,*queue_next = NULL;
	if(q)
	{
		list_for_each_entry_safe(queue, queue_next, &q->list,list) {
			list_del(&queue->list);
			if(queue->data)
			{
				FREE(queue->data);
				queue->data = NULL;
			}
		}
		FREE(q);
		q = NULL;
	}
}

unsigned int os_queue_length(os_queue_t *q)
{
	return list_length(&q->list);
}

unsigned int os_queue_empty(os_queue_t *q)
{
	RETURN_VAL_IF_FAIL(q, 0);
	return list_empty(&q->list);
}


void os_queue_push_tail(os_queue_t *q,void *data)
{
	os_queue_t *node = NULL;
	RETURN_IF_FAIL(q);
	node = MALLOC(sizeof(*node));
	RETURN_IF_FAIL(node);
	node->data = data;
	list_add_tail(&node->list, &q->list);
}

void os_queue_push_head(os_queue_t *q,void *data)
{
	os_queue_t *node = NULL;
	RETURN_IF_FAIL(q);
	node = MALLOC(sizeof(*node));
	RETURN_IF_FAIL(node);
	node->data = data;
	list_add(&node->list, &q->list);
}

void *os_queue_pop_tail(os_queue_t *q)
{
	void *data = NULL;
	os_queue_t *queue = NULL,*queue_next = NULL;
	RETURN_VAL_IF_FAIL(q, NULL);
	list_for_each_entry_safe_reverse(queue, queue_next, &q->list,list) 
	{
		list_del(&queue->list);
		data = queue->data;
		FREE(queue);
		queue = NULL;
		break;
	}
	return data;
}

void *os_queue_pop_head(os_queue_t *q)
{
	void *data = NULL;
	os_queue_t *queue = NULL,*queue_next = NULL;
	RETURN_VAL_IF_FAIL(q, NULL);
	list_for_each_entry_safe(queue, queue_next, &q->list,list) 
	{
		list_del(&queue->list);
		data = queue->data;
		FREE(queue);
		queue = NULL;
		break;
	}
	return data;
}

