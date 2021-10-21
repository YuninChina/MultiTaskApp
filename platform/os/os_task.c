#include "parson.h"

#include "multitask.h"
#include "klist.h"
#include "inits.h"
#include "mt_log.h"
#include "mt_misc.h"
os_task_t *os_task_create(const char *name,unsigned long stack_size,int priority,os_task_func_t func,void *arg)
{

}

os_task_t *os_task_create2(const char *name,unsigned long stack_size,int priority,int async,os_task_func_t func,void *arg)
{

}

void os_task_mm_show(void)
{

}

void os_task_mm_add(unsigned long tid,os_task_mm_node_t *mnode)
{

}

void os_task_mm_del(unsigned long tid,void *addr)
{

}

mt_async_queue_t *os_task_aq_get(const char *name)
{

}

mt_async_queue_t *os_task_aq_self(void)
{

}

const char *os_task_name_get_from_pid(unsigned long pid)
{

}

const char *os_task_name_get_from_tid(unsigned long tid)
{

}


