
#ifndef __OS_THREAD__H
#define __OS_THREAD__H

#ifdef  __cplusplus
extern "C" {
#endif


typedef void* os_pthread_t;

typedef struct os_pthread_attr_s {
	int res;
}os_pthread_attr_t;


///////////////////////////////////////////////////////////////////
int os_pthread_create(os_pthread_t *tid, const os_pthread_attr_t *attr, void *(*func) (void *), void *arg);
int os_pthread_join (os_pthread_t tid, void ** status);
os_pthread_t os_pthread_self (void);
int os_pthread_detach (os_pthread_t tid);
void os_pthread_exit (void *status);

#ifdef  __cplusplus
}
#endif

#endif 



