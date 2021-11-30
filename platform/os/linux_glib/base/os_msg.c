#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <assert.h>

#include "os_msg.h"
#include "os_log.h"
#include "os_async_queue.h"
#include "multitask.h"



int os_msg_send(os_msg_t *msg)
{
	RETURN_VAL_IF_FAIL(msg, -1);
	os_async_queue_t *q = NULL;
	q = (os_async_queue_t *)os_task_aq_get(msg->dst);
	RETURN_VAL_IF_FAIL(q, -1);
	os_async_queue_push(q, msg);
	return 0;
}


os_msg_t *os_msg_recv(void)
{
	os_msg_t *msg = NULL;
	os_async_queue_t *q;
	q = os_task_aq_self();
	RETURN_VAL_IF_FAIL(q, NULL);
	msg = os_async_queue_pop(q);
	RETURN_VAL_IF_FAIL(msg, NULL);
	return msg;
}


int os_msg_send2(const char *dst,int priority,void *data,unsigned int size)
{
	os_msg_t *msg = NULL;
	RETURN_VAL_IF_FAIL(dst , -1);
	msg = MALLOC(sizeof(*msg)+size);
	RETURN_VAL_IF_FAIL(msg, -1);
	msg->src = os_task_name_get_from_tid((unsigned long)pthread_self());
	msg->dst = dst;
	msg->priority = priority;
	msg->size = size;
	memcpy(msg->data,data,size);
	/////////////////////////////
	os_async_queue_t *q = NULL;
	q = (os_async_queue_t *)os_task_aq_get(dst);
	GOTO_LABEL_IF_FAIL(q, fail);
	os_async_queue_push(q, msg);
	return 0;

fail:
	FREE(msg);
	msg = NULL;
	return -1;
}


void *os_msg_recv2(unsigned int *p_size)
{
	os_msg_t *msg = NULL;
	void *data = NULL;
	os_async_queue_t *q;
	q = os_task_aq_self();
	RETURN_VAL_IF_FAIL(q, NULL);
	msg = os_async_queue_pop(q);
	RETURN_VAL_IF_FAIL(msg, NULL);
	data = MALLOC(msg->size);
	RETURN_VAL_IF_FAIL2(data,{FREE(msg);msg=NULL;},NULL);
	if(p_size) *p_size = msg->size;
	memcpy(data,msg->data,msg->size);
	FREE(msg);
	msg = NULL;
	return data;
}

