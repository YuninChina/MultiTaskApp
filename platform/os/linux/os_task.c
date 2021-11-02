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

#include "parson.h"

#include "multitask.h"

extern pid_t gettid(void);

typedef struct mt_task_info_s {
	const char *name;
	unsigned long stack_size;
	int priority;
	mt_task_func_t func;
	void *arg;
	/* other */
	unsigned long tid;
	unsigned long pid;
}mt_task_info_t;

struct mt_task_s {
	struct list_head list;
	///////////////////////
	struct list_head head;
	pthread_mutex_t mutex;
	mt_task_info_t info;
	mt_async_queue_t *q;
};


pthread_mutex_t mt_task_mutex = PTHREAD_MUTEX_INITIALIZER;
static LIST_HEAD(mt_task_list);


static void mt_task_exit(mt_task_t *task)
{
	mt_task_t *node = NULL,*tmp = NULL;
	mt_task_mm_node_t *mnode = NULL,*tmnode = NULL;
	
	if(task)
	{
		MLOGW("mt_task_exit: %s(%lu : %lu)\n",task->info.name,task->info.pid,task->info.tid);
		if(task->q) mt_async_queue_free(task->q);
		task->q = NULL;
		pthread_mutex_lock(&mt_task_mutex);
		list_for_each_entry_safe(node, tmp,&mt_task_list, list) {
			if(node == task)
			{
				list_del(&node->list);
				break;
			}
		}
		pthread_mutex_unlock(&mt_task_mutex);
		printf("%-15s %-15s %-15s %-32s %-15s %-15s %-15s\n",
		"[task]","[tid]","[pid]","[function]","[line]","[addr]","[size]");
		list_for_each_entry_safe(mnode, tmnode,&task->head, list) {
			printf("%-15s %-15lu %-15lu %-32s %-15lu %-15p %-15lu\n",mnode->mt_task_name,
			mnode->tid,mnode->pid,mnode->func,mnode->line,mnode->addr,mnode->size);
			list_del(&mnode->list);
			free(mnode);
		}
		pthread_mutex_destroy(&task->mutex);
		free(task);
		task = NULL;
	}
}

static void *__mt_task_routine(void *arg)
{
	pthread_detach(pthread_self());
	mt_task_t *task = (mt_task_t *)arg;
	RETURN_VAL_IF_FAIL(task,NULL);
	task->info.tid = (unsigned long)pthread_self();
	task->info.pid = (unsigned long)gettid();
	prctl(PR_SET_NAME,task->info.name);
	if(task->info.func) 
		task->info.func(task->info.arg);
	mt_task_exit(task);
	return NULL;
}

mt_task_t *mt_task_create(const char *name,unsigned long stack_size,int priority,mt_task_func_t func,void *arg)
{
	mt_task_t *task = NULL;
	pthread_t thread_id;
	task = malloc(sizeof(*task));
	RETURN_VAL_IF_FAIL(task,NULL);
	memset(task,0,sizeof(*task));
	task->info.name = name;
	task->info.stack_size = stack_size;
	task->info.priority = priority;
	task->info.func = func;
	task->info.arg = arg;
	task->q = mt_async_queue_new();
	
	INIT_LIST_HEAD(&task->head);
	pthread_mutex_init(&task->mutex, NULL);
	pthread_mutex_lock(&mt_task_mutex);
	list_add_tail(&task->list, &mt_task_list);
	pthread_mutex_unlock(&mt_task_mutex);
	
	if(pthread_create(&thread_id, NULL, __mt_task_routine, (void *)task) != 0)
	{
		mt_async_queue_free(task->q);
		task->q = NULL;
		list_del_init(&task->list);
		pthread_mutex_destroy(&task->mutex);
		free(task);
		task=NULL;
		return NULL;
	}
	return task;
}

mt_task_t *mt_task_create2(const char *name,unsigned long stack_size,int priority,int async,mt_task_func_t func,void *arg)
{
	mt_task_t *task = NULL;
	pthread_t thread_id;
	task = malloc(sizeof(*task));
	assert(task);
	memset(task,0,sizeof(*task));
	task->info.name = name;
	task->info.stack_size = stack_size;
	task->info.priority = priority;
	task->info.func = func;
	task->info.arg = arg;
	if(async > 0) task->q = mt_async_queue_new();
	
	INIT_LIST_HEAD(&task->head);
	pthread_mutex_init(&task->mutex, NULL);
	pthread_mutex_lock(&mt_task_mutex);
	list_add_tail(&task->list, &mt_task_list);
	pthread_mutex_unlock(&mt_task_mutex);
	
	if(pthread_create(&thread_id, NULL, __mt_task_routine, (void *)task) != 0)
	{
		mt_async_queue_free(task->q);
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


mt_async_queue_t *mt_task_aq_get(const char *name)
{
	mt_async_queue_t *qq = NULL;
	mt_task_t *node = NULL,*tmp = NULL;
	pthread_mutex_lock(&mt_task_mutex);
	list_for_each_entry_safe(node, tmp,&mt_task_list, list) {
		if(0 == strcmp(name,node->info.name))
		{
			qq = node->q;
			break;
		}
	}
	pthread_mutex_unlock(&mt_task_mutex);
	return (mt_async_queue_t *)qq;
}

const char *mt_task_name_get_from_pid(unsigned long pid)
{
	mt_task_t *node = NULL,*tmp = NULL;
	const char *name = NULL;
	pthread_mutex_lock(&mt_task_mutex);
	list_for_each_entry_safe(node, tmp,&mt_task_list, list) {
		if(pid == node->info.pid)
		{
			name = node->info.name;
			break;
		}
	}
	pthread_mutex_unlock(&mt_task_mutex);
	return name;
}

const char *mt_task_name_get_from_tid(unsigned long tid)
{
	mt_task_t *node = NULL,*tmp = NULL;
	const char *name = NULL;
	pthread_mutex_lock(&mt_task_mutex);
	list_for_each_entry_safe(node, tmp,&mt_task_list, list) {
		if(tid == node->info.tid)
		{
			name = node->info.name;
			break;
		}
	}
	pthread_mutex_unlock(&mt_task_mutex);
	return name;
}


mt_async_queue_t *mt_task_aq_self(void)
{
	mt_async_queue_t *qq = NULL;
	mt_task_t *node = NULL,*tmp = NULL;
	unsigned long tid = (unsigned long)pthread_self();
	pthread_mutex_lock(&mt_task_mutex);
	list_for_each_entry_safe(node, tmp,&mt_task_list, list) {
		if(tid == node->info.tid)
		{
			qq = node->q;
			break;
		}
	}
	pthread_mutex_unlock(&mt_task_mutex);
	return qq;
}



void mt_task_mm_add(unsigned long tid,mt_task_mm_node_t *mnode)
{
	mt_task_t *node = NULL,*tmp = NULL;
	pthread_mutex_lock(&mt_task_mutex);
	list_for_each_entry_safe(node, tmp,&mt_task_list, list) {
		if(tid == node->info.tid)
		{
			list_add_tail(&mnode->list, &node->head);
			break;
		}
	}
	pthread_mutex_unlock(&mt_task_mutex);
}


void mt_task_mm_del(unsigned long tid,void *addr)
{
	mt_task_t *node = NULL,*tmp = NULL;
	pthread_mutex_lock(&mt_task_mutex);
	list_for_each_entry_safe(node, tmp,&mt_task_list, list) {
		mt_task_mm_node_t *mnode = NULL,*tmnode = NULL;
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
	pthread_mutex_unlock(&mt_task_mutex);
}


void mt_task_mm_show(void)
{
	mt_task_t *node = NULL,*tmp = NULL;
    char time[64] = {0,};
    unsigned long mt_task_pid = 0,mt_task_tid = 0;
	unsigned long mem_size = 0;
	const char *mt_task_name = NULL;
	unsigned long *p_arr = NULL;
	unsigned int arr_size = 0;
	int ret = 0;
	int i;
	//////////////////////////////
    time2str(time,sizeof(time));
	printf("\n\n=========================================== mt_task_mm_show [%s] ===========================================\n",time);
	printf("%-20s %-20s %-20s %-20s\n","[name]","[pid]","[tid]","[size]");
	
	ret = child_pid_get(&p_arr,&arr_size);
	RETURN_IF_FAIL(0 == ret);
	for(i = 0;i < arr_size;i++)
	{
		mt_task_pid = p_arr[i];
		///////////////////////////////
		mem_size = 0;
		mt_task_name = NULL;
		list_for_each_entry_safe(node, tmp,&mt_task_list, list) {
			if(mt_task_pid == node->info.pid)
			{
				mt_task_name = node->info.name;
				mt_task_tid = node->info.tid;
				///////////////////////////////
				mt_task_mm_node_t *mnode = NULL,*tmnode = NULL;
				list_for_each_entry_safe(mnode, tmnode,&node->head, list) {
					mem_size += mnode->size;
				}
			}
		}
		printf("%-20s %-20lu %-20lu %-20lu\n",mt_task_name,mt_task_pid,mt_task_tid,mem_size);
	}
	child_pid_free(p_arr);
}

int mt_task_mm_json_get(char **ppjson)
{
	char *pjson = NULL;
	RETURN_VAL_IF_FAIL(ppjson,-1);
	
	mt_task_t *node = NULL,*tmp = NULL;

	char cmd[1024] = {0,};
	FILE *fp = NULL;
    char buf[1024] = {0,};
    char *pResult = NULL;
    unsigned long mt_task_pid = 0,mt_task_tid = 0;
	unsigned long mem_size = 0;
	const char *mt_task_name = NULL;
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
		sscanf(str,"%lu",&mt_task_pid);
		//printf("pid=%d, mt_task_pid: %d\n",pid,mt_task_pid);
		str += (strlen(sub)+1);

		///////////////////////////////
		mem_size = 0;
		mt_task_name = NULL;
		list_for_each_entry_safe(node, tmp,&mt_task_list, list) {
			if(mt_task_pid == node->info.pid)
			{
				mt_task_name = node->info.name;
				mt_task_tid = node->info.tid;
				///////////////////////////////
				mt_task_mm_node_t *mnode = NULL,*tmnode = NULL;
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
		json_object_dotset_string(jObj, "name", mt_task_name);
		json_object_dotset_number(jObj, "pid", mt_task_pid);
		json_object_dotset_number(jObj, "tid", mt_task_tid);
		json_object_dotset_number(jObj, "size", mem_size);
		json_array_append_value(jArrRoot,jVal);
	} while(1);
	pjson = json_serialize_to_string(jValRoot);
	json_value_free(jValRoot);
	///////////////////////////////////////////
	*ppjson = pjson;
	return 0;
}



static mt_task_t _task;
static int __mt_task_init(void)
{
	//printf("enter __mt_task_init()\n");
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
	pthread_mutex_lock(&mt_task_mutex);
	list_add_tail(&_task.list, &mt_task_list);
	pthread_mutex_unlock(&mt_task_mutex);
	return 0;
}

static void __mt_task_exit(void)
{
	pthread_mutex_destroy(&_task.mutex);
}

pure_init(__mt_task_init);
module_exit(__mt_task_exit);

