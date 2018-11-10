/**
  ******************************************************************************
  * @file task_stat.c
  * @author  leon.xie
  * @version v1.0.0
  * @date 2018-11-10 11:15:07
  * @brief  This file provides all the task_stat functions. 
  ******************************************************************************
  * @attention
  *
  * File For Yunin Software Team Only
  *
  * Copyright (C), 2017-2027, Yunin Software Team
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>   
#include <stdlib.h>   
#include <string.h>   
#include <time.h>
#include <unistd.h>
#include <assert.h>   
#include <ctype.h>

#ifdef  __cplusplus
extern "C" {
#endif

/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

 
#define CK_TIME 1   

#define LINUX_KERNEL_TASK_STAT	"%d %s %c %d %d %d %d %d %u %lu "\
								"%lu %lu %lu %lu %lu %ld %ld %ld %ld %d "\
								"%llu %lu %ld %lu %lu %lu %lu %lu %lu %lu"\
								"%lu %lu %lu %lu %lu %lu %d %lu %u %u "\
								"%llu %lu %lu"

typedef struct task_stat_info_s {
	int task_id;
	char tcomm[32];
	char state;
	int ppid;
	int pgid;
	int sid;
	int tty_nr;
	int tty_pgrp;
	unsigned long task_flags;
	unsigned long min_flt;
	//
	unsigned long cmin_flt;
	unsigned long maj_flt;
	unsigned long cmaj_flt;
	unsigned long utime;
	unsigned long stime;
	long cutime;
	long cstime;
	long priority;
	long nice;
	int num_threads;
	//
	unsigned long long start_time;
	unsigned long vsize;
	unsigned long mm_rss;
	unsigned long rsslim;
	unsigned long mm_start_code;
	unsigned long mm_end_code;
	unsigned long mm_start_stack;
	unsigned long esp;
	unsigned long eip;
	unsigned long pending_signal_sig;
	
	//
	unsigned long blocked_sig;
	unsigned long sigign_sig;
	unsigned long sigcatch_sig;
	unsigned long wchan;
	unsigned long unknow_1;
	unsigned long unknow_2;
	int exit_signal;
	unsigned int task_cpu;
	unsigned int rt_priority;
	unsigned int policy;
	//
	unsigned long long delayacct_blkio_ticks;
	unsigned long cgtime;
	unsigned long gtime;
}task_stat_info_t;
 

void task_stat_parse(const char *str)
{
	int ret;
	task_stat_info_t taskInfo;
	memset(&taskInfo,0,sizeof(taskInfo));
	assert(str);
	
	ret = sscanf(str,LINUX_KERNEL_TASK_STAT,
	&taskInfo.task_id,
	taskInfo.tcomm,
	&taskInfo.state,
	&taskInfo.ppid,
	&taskInfo.pgid,
	&taskInfo.sid,
	&taskInfo.tty_nr,
	&taskInfo.tty_pgrp,
	&taskInfo.task_flags,
	&taskInfo.min_flt,
	//
	&taskInfo.cmin_flt,
	&taskInfo.maj_flt,
	&taskInfo.cmaj_flt,
	&taskInfo.utime,
	&taskInfo.stime,
	&taskInfo.cutime,
	&taskInfo.cstime,
	&taskInfo.priority,
	&taskInfo.nice,
	&taskInfo.num_threads,
	//
	&taskInfo.start_time,
	&taskInfo.vsize,
	&taskInfo.mm_rss,
	&taskInfo.rsslim,
	&taskInfo.mm_start_code,
	&taskInfo.mm_end_code,
	&taskInfo.mm_start_stack,
	&taskInfo.esp,
	&taskInfo.eip,
	&taskInfo.pending_signal_sig,
	//
	&taskInfo.blocked_sig,
	&taskInfo.sigign_sig,
	&taskInfo.sigcatch_sig,
	&taskInfo.wchan,
	&taskInfo.unknow_1,
	&taskInfo.unknow_2,
	&taskInfo.exit_signal,
	&taskInfo.task_cpu,
	&taskInfo.rt_priority,
	&taskInfo.policy,
	//
	&taskInfo.delayacct_blkio_ticks,
	&taskInfo.gtime,
	&taskInfo.cgtime);
	//printf("ret=%d\n",ret);
	assert(43 == ret);
	printf("%-20s %-8d %-8c %-12d %-12u %-12u %-12u %-12u\n",
	taskInfo.tcomm,taskInfo.task_id,taskInfo.state,taskInfo.num_threads
	,taskInfo.utime,taskInfo.stime,taskInfo.cutime,taskInfo.cstime);
}
  
int main(int argc ,char *argv[])  
{
    FILE *fp = NULL;
    char buf[1024*4] = {0,};
    char buf2[1024*4] = {0,};
    char *pResult = NULL;
    int pid = -1;
    char cmd[1024] = {0,};
    int ret = -1;
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <PID>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
	ret = sscanf(argv[1],"%d",&pid);
	assert(1 == ret);
	//printf("pid: %d\n",pid);
	//
	while(1)
	{
		//
		system("clear");
		printf("%-20s %-8s %-8s %-12s %-12s %-12s %-12s %-12s\n",
		"task_name",
		"task_id",
		"state",
		"threads",
		"cpu_utime",
		"cpu_stime",
		"cpu_cutime",
		"cpu_cstime"
		);
		printf("=======================================================================================================\n");
		//
		memset(cmd,0,sizeof(cmd));
	    snprintf(cmd,sizeof(cmd),"ls /proc/%d/task/ | xargs",pid);
	    fp = popen (cmd, "r");
	    assert(fp);
		memset(buf,0,sizeof(buf));
		pResult = fgets (buf, sizeof (buf), fp);
	    assert(pResult);
	    fclose(fp);
	    fp = NULL;
		//printf("%s,%d: buf: %s\n",__func__,__LINE__,buf);

		char *sub = NULL;
		int i = 0;
		sub = strtok(buf," ");
		while(NULL != sub)
		{
			i = 0;
			while(0 != sub[i])
			{
				if(0 == isdigit(sub[i]))
				{
					sub[i] = 0;
					break;
				}
				i++;
			}
			//
			memset(cmd,0,sizeof(cmd));
		    snprintf(cmd,sizeof(cmd),"cat /proc/%d/task/%s/stat",pid,sub);
	//		printf("%s,%d: cmd: %s\n",__func__,__LINE__,cmd);
		    fp = popen (cmd, "r");
		    assert(fp);
			memset(buf2,0,sizeof(buf2));
			pResult = fgets (buf2, sizeof (buf2), fp);
		    assert(pResult);
		    fclose(fp);
		    fp = NULL;
	//		printf("%s,%d: buf2: %s\n",__func__,__LINE__,buf2);
			//
			task_stat_parse(buf2);
			//
			sub = strtok(NULL, " ");
		}	
		sleep(1);
	}
	
	//
	return 0;
}

#ifdef  __cplusplus
}
#endif


