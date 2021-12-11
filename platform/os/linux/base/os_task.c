#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <assert.h>
#include <sys/prctl.h>
#include <semaphore.h>

#include <sys/types.h>
#include <unistd.h>

#include "multitask.h"

#include <sys/syscall.h>

__attribute__((weak)) pid_t gettid(void) 
{
	return syscall(SYS_gettid);
}


typedef struct os_task_info_s {
	const char *name;
	unsigned long stack_size;
	int priority;
	os_task_func_t func;
	void *arg;
	/* other */
	unsigned long tid;
	unsigned long pid;
}os_task_info_t;

struct os_task_s {
	struct list_head list;
	///////////////////////
	struct list_head head;
	pthread_mutex_t mutex;
	os_task_info_t info;
	os_async_queue_t *q;
};


pthread_mutex_t os_task_mutex = PTHREAD_MUTEX_INITIALIZER;
static LIST_HEAD(os_task_list);


static void os_task_exit(os_task_t *task)
{
	os_task_t *node = NULL,*tmp = NULL;
	os_task_mm_node_t *mnode = NULL,*tmnode = NULL;
	
	if(task)
	{
		MLOGW("os_task_exit: %s(%lu : %lu)\n",task->info.name,task->info.pid,task->info.tid);
		if(task->q) os_async_queue_free(task->q);
		task->q = NULL;
		pthread_mutex_lock(&os_task_mutex);
		list_for_each_entry_safe(node, tmp,&os_task_list, list) {
			if(node == task)
			{
				list_del(&node->list);
				break;
			}
		}
		pthread_mutex_unlock(&os_task_mutex);
		printf("%-15s %-15s %-15s %-32s %-15s %-15s %-15s\n",
		"[task]","[tid]","[pid]","[function]","[line]","[addr]","[size]");
		list_for_each_entry_safe(mnode, tmnode,&task->head, list) {
			printf("%-15s %-15lu %-15lu %-32s %-15lu %-15p %-15lu\n",mnode->os_task_name,
			mnode->tid,mnode->pid,mnode->func,mnode->line,mnode->addr,mnode->size);
			list_del(&mnode->list);
			free(mnode);
		}
		pthread_mutex_destroy(&task->mutex);
		free(task);
		task = NULL;
	}
}

static void *__os_task_routine(void *arg)
{
	pthread_detach(pthread_self());
	os_task_t *task = (os_task_t *)arg;
	RETURN_VAL_IF_FAIL(task,NULL);
	task->info.tid = (unsigned long)pthread_self();
	task->info.pid = (unsigned long)gettid();
	prctl(PR_SET_NAME,task->info.name);
	if(task->info.func) 
		task->info.func(task->info.arg);
	os_task_exit(task);
	return NULL;
}

os_task_t *os_task_create(const char *name,unsigned long stack_size,int priority,os_task_func_t func,void *arg)
{
	os_task_t *task = NULL;
	pthread_t thread_id;
	task = malloc(sizeof(*task));
	RETURN_VAL_IF_FAIL(task,NULL);
	memset(task,0,sizeof(*task));
	task->info.name = name;
	task->info.stack_size = stack_size;
	task->info.priority = priority;
	task->info.func = func;
	task->info.arg = arg;
	task->q = os_async_queue_new();
	
	INIT_LIST_HEAD(&task->head);
	pthread_mutex_init(&task->mutex, NULL);
	pthread_mutex_lock(&os_task_mutex);
	list_add_tail(&task->list, &os_task_list);
	pthread_mutex_unlock(&os_task_mutex);
	
	if(pthread_create(&thread_id, NULL, __os_task_routine, (void *)task) != 0)
	{
		os_async_queue_free(task->q);
		task->q = NULL;
		list_del_init(&task->list);
		pthread_mutex_destroy(&task->mutex);
		free(task);
		task=NULL;
		return NULL;
	}
	return task;
}

os_task_t *os_task_create2(const char *name,unsigned long stack_size,int priority,int async,os_task_func_t func,void *arg)
{
	os_task_t *task = NULL;
	pthread_t thread_id;
	task = malloc(sizeof(*task));
	assert(task);
	memset(task,0,sizeof(*task));
	task->info.name = name;
	task->info.stack_size = stack_size;
	task->info.priority = priority;
	task->info.func = func;
	task->info.arg = arg;
	if(async > 0) task->q = os_async_queue_new();
	
	INIT_LIST_HEAD(&task->head);
	pthread_mutex_init(&task->mutex, NULL);
	pthread_mutex_lock(&os_task_mutex);
	list_add_tail(&task->list, &os_task_list);
	pthread_mutex_unlock(&os_task_mutex);
	
	if(pthread_create(&thread_id, NULL, __os_task_routine, (void *)task) != 0)
	{
		os_async_queue_free(task->q);
		task->q = NULL;
		list_del_init(&task->list);
		pthread_mutex_destroy(&task->mutex);
		free(task);
		task=NULL;
		return NULL;
		return NULL;
	}
	return task;
}


os_async_queue_t *os_task_aq_get(const char *name)
{
	os_async_queue_t *qq = NULL;
	os_task_t *node = NULL,*tmp = NULL;
	pthread_mutex_lock(&os_task_mutex);
	list_for_each_entry_safe(node, tmp,&os_task_list, list) {
		if(0 == strcmp(name,node->info.name))
		{
			qq = node->q;
			break;
		}
	}
	pthread_mutex_unlock(&os_task_mutex);
	return (os_async_queue_t *)qq;
}

const char *os_task_name_get_from_pid(unsigned long pid)
{
	os_task_t *node = NULL,*tmp = NULL;
	const char *name = NULL;
	pthread_mutex_lock(&os_task_mutex);
	list_for_each_entry_safe(node, tmp,&os_task_list, list) {
		if(pid == node->info.pid)
		{
			name = node->info.name;
			break;
		}
	}
	pthread_mutex_unlock(&os_task_mutex);
	return name;
}

const char *os_task_name_get_from_tid(unsigned long tid)
{
	os_task_t *node = NULL,*tmp = NULL;
	const char *name = NULL;
	pthread_mutex_lock(&os_task_mutex);
	list_for_each_entry_safe(node, tmp,&os_task_list, list) {
		if(tid == node->info.tid)
		{
			name = node->info.name;
			break;
		}
	}
	pthread_mutex_unlock(&os_task_mutex);
	return name;
}


os_async_queue_t *os_task_aq_self(void)
{
	os_async_queue_t *qq = NULL;
	os_task_t *node = NULL,*tmp = NULL;
	unsigned long tid = (unsigned long)pthread_self();
	pthread_mutex_lock(&os_task_mutex);
	list_for_each_entry_safe(node, tmp,&os_task_list, list) {
		if(tid == node->info.tid)
		{
			qq = node->q;
			break;
		}
	}
	pthread_mutex_unlock(&os_task_mutex);
	return qq;
}



void os_task_mm_add(unsigned long tid,os_task_mm_node_t *mnode)
{
	os_task_t *node = NULL,*tmp = NULL;
	pthread_mutex_lock(&os_task_mutex);
	list_for_each_entry_safe(node, tmp,&os_task_list, list) {
		if(tid == node->info.tid)
		{
			list_add_tail(&mnode->list, &node->head);
			break;
		}
	}
	pthread_mutex_unlock(&os_task_mutex);
}


void os_task_mm_del(unsigned long tid,void *addr)
{
	os_task_t *node = NULL,*tmp = NULL;
	pthread_mutex_lock(&os_task_mutex);
	list_for_each_entry_safe(node, tmp,&os_task_list, list) {
		os_task_mm_node_t *mnode = NULL,*tmnode = NULL;
		list_for_each_entry_safe(mnode, tmnode,&node->head, list) {
			if(addr == mnode->addr)
			{
				list_del(&mnode->list);
				free(mnode);
				goto succ;
			}
		}
	}
succ:	
	pthread_mutex_unlock(&os_task_mutex);
}


void os_task_mm_show(void)
{
	os_task_t *node = NULL,*tmp = NULL;
    char time[64] = {0,};
    unsigned long os_task_pid = 0,os_task_tid = 0;
	unsigned long mem_size = 0;
	const char *os_task_name = NULL;
	unsigned long *p_arr = NULL;
	unsigned int arr_size = 0;
	int ret = 0;
	int i;
	//////////////////////////////
    time2str(time,sizeof(time));
	printf("\n\n=========================================== os_task_mm_show [%s] ===========================================\n",time);
	printf("%-20s %-20s %-20s %-20s\n","[name]","[pid]","[tid]","[size]");
	
	ret = child_pid_get(&p_arr,&arr_size);
	RETURN_IF_FAIL(0 == ret);
	for(i = 0;i < arr_size;i++)
	{
		os_task_pid = p_arr[i];
		///////////////////////////////
		mem_size = 0;
		os_task_name = NULL;
		list_for_each_entry_safe(node, tmp,&os_task_list, list) {
			if(os_task_pid == node->info.pid)
			{
				os_task_name = node->info.name;
				os_task_tid = node->info.tid;
				///////////////////////////////
				os_task_mm_node_t *mnode = NULL,*tmnode = NULL;
				list_for_each_entry_safe(mnode, tmnode,&node->head, list) {
					mem_size += mnode->size;
				}
			}
		}
		printf("%-20s %-20lu %-20lu %-20lu\n",os_task_name,os_task_pid,os_task_tid,mem_size);
	}
	child_pid_free(p_arr);
}

int os_task_mm_json_get(char **ppjson)
{
#if 0
	char *pjson = NULL;
	RETURN_VAL_IF_FAIL(ppjson,-1);
	
	os_task_t *node = NULL,*tmp = NULL;

	char cmd[1024] = {0,};
	FILE *fp = NULL;
    char buf[1024] = {0,};
    char *pResult = NULL;
    unsigned long os_task_pid = 0,os_task_tid = 0;
	unsigned long mem_size = 0;
	const char *os_task_name = NULL;
	pid_t pid = getpid();
	memset(cmd,0,sizeof(cmd));
    snprintf(cmd,sizeof(cmd),"ls /proc/%d/task/ | xargs",pid);
    fp = popen (cmd, "r");
    assert(fp);
	memset(buf,0,sizeof(buf));
	pResult = fgets (buf, sizeof (buf), fp);
    assert(pResult);
    fclose(fp);
    fp = NULL;
    //////////////////////
    
    char *sub = NULL,*str = NULL;
	str = buf;
	JSON_Value *jValRoot = json_value_init_array();
	assert(jValRoot);
	JSON_Array *jArrRoot = json_array(jValRoot);
	assert(jArrRoot);
	do {
		sub = strtok(str," ");
		if(NULL == sub)
			break;
		sscanf(str,"%lu",&os_task_pid);
		//printf("pid=%d, os_task_pid: %d\n",pid,os_task_pid);
		str += (strlen(sub)+1);

		///////////////////////////////
		mem_size = 0;
		os_task_name = NULL;
		list_for_each_entry_safe(node, tmp,&os_task_list, list) {
			if(os_task_pid == node->info.pid)
			{
				os_task_name = node->info.name;
				os_task_tid = node->info.tid;
				///////////////////////////////
				os_task_mm_node_t *mnode = NULL,*tmnode = NULL;
				list_for_each_entry_safe(mnode, tmnode,&node->head, list) {
					mem_size += mnode->size;
				}
			}
		}
		/////////////////////////////
		JSON_Value *jVal = NULL;
		JSON_Object *jObj = NULL;
		jVal = json_value_init_object();
		assert(jVal);
		jObj = json_value_get_object(jVal);
		assert(jObj);
		json_object_dotset_string(jObj, "name", os_task_name);
		json_object_dotset_number(jObj, "pid", os_task_pid);
		json_object_dotset_number(jObj, "tid", os_task_tid);
		json_object_dotset_number(jObj, "size", mem_size);
		json_array_append_value(jArrRoot,jVal);
	} while(1);
	pjson = json_serialize_to_string(jValRoot);
	json_value_free(jValRoot);
	///////////////////////////////////////////
	*ppjson = pjson;
	return 0;
#else
	return -1;
#endif	
	
}



static os_task_t _task;
static int __os_task_init(void)
{
	//printf("enter __os_task_init()\n");
	static char _name[32] = {0,};
	memset(&_task,0,sizeof(_task));
	prctl(PR_GET_NAME,_name);
	_task.info.name = _name;
	_task.info.stack_size = 0;
	_task.info.priority = 0;
	_task.info.func = NULL;
	_task.info.arg = NULL;
	
	_task.info.tid = (unsigned long)pthread_self();
	_task.info.pid = (unsigned long)gettid();
	
	INIT_LIST_HEAD(&_task.head);
	pthread_mutex_init(&_task.mutex, NULL);
	pthread_mutex_lock(&os_task_mutex);
	list_add_tail(&_task.list, &os_task_list);
	pthread_mutex_unlock(&os_task_mutex);
	return 0;
}

static void __os_task_exit(void)
{
	pthread_mutex_destroy(&_task.mutex);
}

pure_init(__os_task_init);
module_exit(__os_task_exit);

