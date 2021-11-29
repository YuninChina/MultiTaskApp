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
	return 0;
}


os_msg_t *os_msg_recv(void)
{
	os_msg_t *msg = NULL;
	return msg;
}


int os_msg_send2(const char *dst,int priority,void *data,unsigned int size)
{
	return 0;
}


void *os_msg_recv2(unsigned int *p_size)
{
	void *data = NULL;
	return data;
}

