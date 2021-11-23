#include <stdarg.h>
#include <stdio.h>
#include <sys/prctl.h>
#include <time.h>
#include <sys/time.h>
#include <assert.h>
#include <strings.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>

#include "os_log.h"
#include "multitask.h"

extern pid_t gettid(void);

void time2str(char *buf,int size)
{
    assert(buf && size > 0);
    struct timeval tv;
    struct tm tm;
    int pos = 0;
    long millisecond = 0;
    bzero(buf,size);
    gettimeofday(&tv,NULL);
    localtime_r(&tv.tv_sec,&tm);
    //格式化化为字符串输出.
    strftime(buf,size,"%F %T",&tm);
    pos = strlen(buf);
    millisecond = tv.tv_usec / 1000;
    snprintf(buf+pos,size-pos,".%lu",millisecond);
}

void thread_name_get(char *task_name)
{
	prctl(PR_GET_NAME,task_name);
}

void thread_name_set(const char *task_name)
{
	prctl(PR_SET_NAME,task_name);
}


int child_pid_serialize2string(char *strs,unsigned int size)
{
#define CHILD_PID_CMD "ls /proc/%d/task/ | xargs"
	char cmd[1024] = {0,};
	FILE *fp = NULL;
    char *pResult = NULL;
	RETURN_VAL_IF_FAIL(strs && size, -1);
	pid_t pid = getpid();
	memset(cmd,0,sizeof(cmd));
    snprintf(cmd,sizeof(cmd),CHILD_PID_CMD,pid);
	//printf("cmd: %s\n",cmd);
    fp = popen (cmd, "r");
    assert(fp);
	memset(strs,0,size);
	pResult = fgets (strs, size, fp);
    assert(pResult);
    fclose(fp);
    fp = NULL;
	//printf("strs: %s\n",strs);
    return 0;
}


int child_pid_get(unsigned long **pp_arr,unsigned int *p_size)
{
	RETURN_VAL_IF_FAIL(pp_arr && p_size, -1);
	char buf[1024] = {0};
	int ret = -1;
	int i;
    char *sub = NULL,*str = NULL;
    unsigned int cnt = 0;
    unsigned long *p_arr = NULL;
	ret = child_pid_serialize2string(buf,sizeof(buf));
	RETURN_VAL_IF_FAIL(0 == ret, -1);

	for(i = 0;i < sizeof(buf) && 0 != buf[i];i++)
	{
		if(' ' == buf[i])
			cnt++;
	}
	cnt += 1;
	p_arr = MALLOC(cnt*sizeof(unsigned long));
	RETURN_VAL_IF_FAIL(p_arr, -1);

	i = 0;
	str = buf;
	do {
		sub = strtok(str," ");
		if(NULL == sub)
			break;
		sscanf(str,"%lu",&p_arr[i]);
		str += (strlen(sub)+1);
		i++;
	} while(1);
	
	*p_size = cnt;
	*pp_arr = p_arr;
	return 0;
}


void child_pid_free(unsigned long *p_arr)
{
	if(p_arr) FREE(p_arr);
}


