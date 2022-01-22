
#ifndef __OS_THREAD__H
#define __OS_THREAD__H

#ifdef  __cplusplus
extern "C" {
#endif

///////////////////////////////////////////////////////////////////
typedef void* os_pthread_t;
typedef struct os_pthread_attr_s {
	int res;
}os_pthread_attr_t;
///////////
int os_pthread_create(os_pthread_t *tid, const os_pthread_attr_t *attr, void *(*func) (void *), void *arg);
int os_pthread_join (os_pthread_t tid, void ** status);
os_pthread_t os_pthread_self (void);
int os_pthread_detach (os_pthread_t tid);
void os_pthread_exit (void *status);

///////////////////////////////////////////////////////////////////
typedef void* os_pthread_mutex_t;
typedef struct os_pthread_mutexattr_s {
	int res;
}os_pthread_mutexattr_t;
///////////
int os_pthread_mutex_init(os_pthread_mutex_t * mutex,const os_pthread_mutexattr_t *attr);
int os_pthread_mutex_destroy(os_pthread_mutex_t *mutex);
int os_pthread_mutex_lock(os_pthread_mutex_t *mutex);
int os_pthread_mutex_trylock(os_pthread_mutex_t *mutex);
int os_pthread_mutex_unlock(os_pthread_mutex_t *mutex);

///////////////////////////////////////////////////////////////////
typedef void* os_pthread_cond_t;
typedef struct os_pthread_condattr_s {
	int res;
}os_pthread_condattr_t;

typedef struct os_timespec_s {
	int res;
}os_timespec_t;

///////////
int os_pthread_cond_destroy(os_pthread_cond_t *cond);
int os_pthread_cond_init(os_pthread_cond_t * cond,
       const os_pthread_condattr_t * attr);
int os_pthread_cond_timedwait(os_pthread_cond_t * cond,
       os_pthread_mutex_t * mutex,
       const os_timespec_t * abstime);
int os_pthread_cond_wait(os_pthread_cond_t * cond,
       os_pthread_mutex_t * mutex);
int os_pthread_cond_broadcast(os_pthread_cond_t *cond);
int os_pthread_cond_signal(os_pthread_cond_t *cond);


///////////////////////////////////////////////////////////////////
typedef void* os_sem_t;
///////////
int os_sem_init(os_sem_t *sem, int pshared, unsigned int value);
int os_sem_wait(os_sem_t *sem);
int os_sem_trywait(os_sem_t *sem);
int os_sem_post(os_sem_t * sem);
int os_sem_destroy(os_sem_t * sem);


#ifdef  __cplusplus
}
#endif

#endif 



