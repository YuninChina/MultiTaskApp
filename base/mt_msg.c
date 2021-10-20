#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <assert.h>

#include "mt_msg.h"
#include "mt_log.h"
#include "mt_async_queue.h"
#include "mm.h"
#include "task.h"


int mt_msg_send(mt_msg_t *msg)
{
	RETURN_VAL_IF_FAIL(msg, -1);
	mt_async_queue_t *q = NULL;
	q = (mt_async_queue_t *)task_aq_get(msg->dst);
	RETURN_VAL_IF_FAIL(q, -1);
	mt_async_queue_push_tail(q, msg);
	return 0;
}


mt_msg_t *mt_msg_recv(void)
{
	mt_msg_t *msg = NULL;
	mt_async_queue_t *q;
	q = task_aq_self();
	RETURN_VAL_IF_FAIL(q, NULL);
	msg = mt_async_queue_pop_head(q);
	RETURN_VAL_IF_FAIL(msg, NULL);
	return msg;
}


int mt_msg_send2(const char *dst,int priority,void *data,unsigned int size)
{
	mt_msg_t *msg = NULL;
	RETURN_VAL_IF_FAIL(dst , -1);
	msg = MALLOC(sizeof(*msg)+size);
	RETURN_VAL_IF_FAIL(msg, -1);
	msg->src = task_name_get_from_tid((unsigned long)pthread_self());
	msg->dst = dst;
	msg->priority = priority;
	msg->size = size;
	memcpy(msg->data,data,size);
	/////////////////////////////
	mt_async_queue_t *q = NULL;
	q = (mt_async_queue_t *)task_aq_get(dst);
	GOTO_LABEL_IF_FAIL(q, fail);
	mt_async_queue_push_tail(q, msg);
	return 0;

fail:
	FREE(msg);
	msg = NULL;
	return -1;
}


void *mt_msg_recv2(unsigned int *p_size)
{
	mt_msg_t *msg = NULL;
	void *data = NULL;
	mt_async_queue_t *q;
	q = task_aq_self();
	RETURN_VAL_IF_FAIL(q, NULL);
	msg = mt_async_queue_pop_head(q);
	RETURN_VAL_IF_FAIL(msg, NULL);
	data = MALLOC(msg->size);
	RETURN_VAL_IF_FAIL2(data,{FREE(msg);msg=NULL;},NULL);
	if(p_size) *p_size = msg->size;
	memcpy(data,msg->data,msg->size);
	FREE(msg);
	msg = NULL;
	return data;
}

