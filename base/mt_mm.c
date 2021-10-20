#define _GNU_SOURCE             /* See feature_test_macros(7) */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <assert.h>
#include <sys/types.h>

#include "mm.h"
#include "task.h"

#if defined(CONFIG_MM_TRACE)	
pthread_mutex_t mm_mutex = PTHREAD_MUTEX_INITIALIZER;
static LIST_HEAD(mm_list);
#endif


void *mm_malloc(const char *func,unsigned long line,unsigned long size)
{
	void *addr = NULL;
	addr = malloc(size);
	assert(addr);
	unsigned long pid = (unsigned long)gettid();  //process ID
	unsigned long tid = (unsigned long)pthread_self();  //thread ID

#if defined(CONFIG_TASK_MM_TRACE)	
	task_mm_node_t *node = NULL;
	node = malloc(sizeof(*node));
	assert(node);
	node->tid = tid;
	node->pid = pid;
	node->func = func;
	node->line = line;
	node->size = size;
	node->addr = addr;
	pthread_getname_np(tid,node->task_name,sizeof(node->task_name));
	task_mm_add(node->tid,node);
#endif

#if defined(CONFIG_MM_TRACE)	
	task_mm_node_t *mm_node = NULL;
	mm_node = malloc(sizeof(*mm_node));
	assert(mm_node);
	mm_node->tid = tid;
	mm_node->pid = pid;
	mm_node->func = func;
	mm_node->line = line;
	mm_node->size = size;
	mm_node->addr = addr;
	pthread_getname_np(tid,mm_node->task_name,sizeof(mm_node->task_name));
	pthread_mutex_lock(&mm_mutex);
	list_add_tail(&mm_node->list, &mm_list);
	pthread_mutex_unlock(&mm_mutex);
#endif

	return addr;
}


void mm_free(void *addr)
{
	if(addr)
	{
#if defined(CONFIG_TASK_MM_TRACE)	
		unsigned long tid;	//thread ID
		tid = (unsigned long)pthread_self();
		task_mm_del(tid,addr);
#endif

#if defined(CONFIG_MM_TRACE)
		task_mm_node_t *mm_node = NULL,*mm_tmp = NULL;
		pthread_mutex_lock(&mm_mutex);
		list_for_each_entry_safe(mm_node, mm_tmp,&mm_list, list) {
			if(mm_node->addr == addr)
			{
				list_del(&mm_node->list);
				free(mm_node);
				mm_node = NULL;
				break;
			}
		}
		pthread_mutex_unlock(&mm_mutex);
#endif

		free(addr);
		addr = NULL;
	}
}


void mm_show(void)
{
	task_mm_node_t *node = NULL,*tmp = NULL;
	printf("\n\n=========================================== mm_show ===========================================\n");
	printf("%-15s %-15s %-15s %-32s %-15s %-15s %-15s\n",
	"[task]","[tid]","[pid]","[function]","[line]","[addr]","[size]");
	list_for_each_entry_safe(node, tmp,&mm_list, list) {
		printf("%-15s %-15lu %-15lu %-32s %-15lu %-15p %-15lu\n",
		node->task_name,node->tid,node->pid,node->func,node->line,node->addr,node->size);
	}
}


void mm_show2(void (*show)(const char *))
{
	task_mm_node_t *node = NULL,*tmp = NULL;
	char buf[1024] = {0,};
	printf("\n\n=========================================== mm_show ===========================================\n");
	printf("%-15s %-15s %-15s %-32s %-15s %-15s %-15s\n",
	"[task]","[tid]","[pid]","[function]","[line]","[addr]","[size]");
	list_for_each_entry_safe(node, tmp,&mm_list, list) {
		memset(buf,0,sizeof(buf));
		snprintf(buf,sizeof(buf),"%-15s %-15lu %-15lu %-32s %-15lu %-15p %-15lu\n",
		node->task_name,node->tid,node->pid,node->func,node->line,node->addr,node->size);	
		printf("%s",buf);
		if(show) show((const char *)buf);
	}
}

