#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "os_queue.h"
#include "os_log.h"
#include "multitask.h"

struct os_queue_s {
	int dummy;
};


os_queue_t *os_queue_new(void)
{
	os_queue_t *q = NULL;
	return q;
}

void os_queue_free(os_queue_t *q)
{
}

unsigned int os_queue_length(os_queue_t *q)
{
	return 0;
}

unsigned int os_queue_empty(os_queue_t *q)
{
	return 0;
}


void os_queue_push_tail(os_queue_t *q,void *data)
{
}

void os_queue_push_head(os_queue_t *q,void *data)
{
}

void *os_queue_pop_tail(os_queue_t *q)
{
	void *data = NULL;

	return data;
}

void *os_queue_pop_head(os_queue_t *q)
{
	void *data = NULL;
	
	return data;
}

