#ifndef __OS_TASK__H
#define __OS_TASK__H
#include "klist.h"
#include "mt_async_queue.h"

#ifdef  __cplusplus
extern "C" {
#endif

#define CONFIG_OS_TASK_MM_TRACE 1

typedef struct mt_task_mm_node_s{
	struct list_head list;
	/////////////////////
	char mt_task_name[32];
	unsigned long pid;  //process ID
	unsigned long tid;  //thread ID
	const char *func;
	unsigned long line;
	unsigned long size;
	void *addr;
}mt_task_mm_node_t;

typedef void *(*mt_task_func_t)(void *user_data);
typedef struct mt_task_s mt_task_t;


mt_task_t *mt_task_create(const char *name,unsigned long stack_size,int priority,mt_task_func_t func,void *arg);
mt_task_t *mt_task_create2(const char *name,unsigned long stack_size,int priority,int async,mt_task_func_t func,void *arg);

void mt_task_mm_show(void);
void mt_task_mm_add(unsigned long tid,mt_task_mm_node_t *mnode);
void mt_task_mm_del(unsigned long tid,void *addr);
mt_async_queue_t *mt_task_aq_get(const char *name);
mt_async_queue_t *mt_task_aq_self(void);
const char *mt_task_name_get_from_pid(unsigned long pid);
const char *mt_task_name_get_from_tid(unsigned long tid);


#ifdef  __cplusplus
}
#endif


#endif
