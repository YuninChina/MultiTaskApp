/**
  ******************************************************************************
  * @file cpu.c
  * @author  leon.xie
  * @version v1.0.0
  * @date 2018-11-10 11:10:44
  * @brief  This file provides all the cpu functions. 
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


#ifdef  __cplusplus
extern "C" {
#endif

/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

#define __DEBUG__ 0   
#define CK_TIME 1   

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
  
int main(int argc ,char *argv[])  
{  
    FILE *fp;  
    char buf[128];  
    char cpu[8];  
    unsigned long int user,nice,sys,idle,iowait,irq,softirq,steal;  
    unsigned long int all1,all2,idle1,idle2;
    float cpu_usage;  
	char *pResult = NULL;
    while(1)  
    {  
        if((fp = fopen("/proc/stat","r")) == NULL)  
        {  
            perror("fopen /proc/stat:");  
            exit (0);  
        }  

        pResult = fgets(buf,sizeof(buf),fp);
        assert(pResult);
        sscanf(buf,"%s%lu%lu%lu%lu%lu%lu%lu%lu",cpu,&user,&nice,&sys,&idle,&iowait,&irq,&softirq,&steal);  
  
#if __DEBUG__ 
        printf("buf=%s",buf);
        printf("%s,%lu,%lu,%lu,%lu,%lu,%lu,%lu,%lu\n",cpu,user,nice,sys,idle,iowait,irq,softirq,steal); 
#endif 
        all1 = user+nice+sys+idle+iowait+irq+softirq+steal;
        idle1 = idle; 
        
        rewind(fp);  
        sleep(CK_TIME);  
        memset(buf,0,sizeof(buf));  
        cpu[0] = '\0';  
        user=nice=sys=idle=iowait=irq=softirq=steal=0;
        pResult = fgets(buf,sizeof(buf),fp);  
        assert(pResult);
        sscanf(buf,"%s%lu%lu%lu%lu%lu%lu%lu%lu",cpu,&user,&nice,&sys,&idle,&iowait,&irq,&softirq,&steal);  
#if __DEBUG__ 
        printf("buf=%s",buf);  
        printf("%s,%lu,%lu,%lu,%lu,%lu,%lu,%lu,%lu\n",cpu,user,nice,sys,idle,iowait,irq,softirq,steal); 
#endif 

        all2 = user+nice+sys+idle+iowait+irq+softirq+steal;
        idle2 = idle;
        cpu_usage = (all2-all1-(idle2-idle1))*100.0 / (all2-all1) ;  
        fclose(fp);  

        printf("#### cpu use =%6.2f%% ####\n", cpu_usage);
    }  
    return 0;  
}

#ifdef  __cplusplus
}
#endif


