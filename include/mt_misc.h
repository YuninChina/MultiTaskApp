#ifndef __MT_MISC_H
#define __MT_MISC_H

#ifdef  __cplusplus
extern "C" {
#endif

void time2str(char *buf,int size);
void thread_name_get(char *task_name);
void thread_name_set(const char *task_name);
int child_pid_serialize2string(char *strs,unsigned int size);
int child_pid_get(unsigned long **pp_arr,unsigned int *p_size);
void child_pid_free(unsigned long *p_arr);


#ifdef  __cplusplus
}
#endif

#endif  /* __MT_MISC_H */

