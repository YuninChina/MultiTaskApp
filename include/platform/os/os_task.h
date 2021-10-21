#ifndef __OS_TASK__H
#define __OS_TASK__H
#include "klist.h"
#include "mt_async_queue.h"

#ifdef  __cplusplus
extern "C" {
#endif

#define CONFIG_os_task_MM_TRACE 1

typedef struct os_task_mm_node_s{
	struct list_head list;
	/////////////////////
	char os_task_name[32];
	unsigned long pid;  //process ID
	unsigned long tid;  //thread ID
	const char *func;
	unsigned long line;
	unsigned long size;
	void *addr;
}os_task_mm_node_t;

typedef void *(*os_task_func_t)(void *user_data);
typedef struct os_task_s os_task_t;

typedef struct {
	const char *name;
	/* task ops */
	os_task_t *(*create)(const char *name,unsigned long stack_size,int priority,os_task_func_t func,void *arg);
	os_task_t *(*create2)(const char *name,unsigned long stack_size,int priority,int async,os_task_func_t func,void *arg);

	/* mem ops */
	void (*mm_show)(void);
	void (*mm_add)(unsigned long tid,os_task_mm_node_t *mnode);
	void (*mm_del)(unsigned long tid,void *addr);
	
	/* misc */
	mt_async_queue_t *(*aq_get)(const char *name);
	mt_async_queue_t *(*aq_self)(void);
	const char *(*name_get_from_pid)(unsigned long pid);
	const char *(*name_get_from_tid)(unsigned long tid);
	
}os_task_ops;

os_task_t *os_task_create(const char *name,unsigned long stack_size,int priority,os_task_func_t func,void *arg);
os_task_t *os_task_create2(const char *name,unsigned long stack_size,int priority,int async,os_task_func_t func,void *arg);

void os_task_mm_show(void);
void os_task_mm_add(unsigned long tid,os_task_mm_node_t *mnode);
void os_task_mm_del(unsigned long tid,void *addr);
mt_async_queue_t *os_task_aq_get(const char *name);
mt_async_queue_t *os_task_aq_self(void);
const char *os_task_name_get_from_pid(unsigned long pid);
const char *os_task_name_get_from_tid(unsigned long tid);


#ifdef  __cplusplus
}
#endif


#endif
