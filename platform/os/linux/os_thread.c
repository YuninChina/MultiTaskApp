
#include <pthread.h>
#include "os_thread.h"


int os_pthread_create(os_pthread_t *tid, const os_pthread_attr_t *attr, void *(*func) (void *), void *arg)
{
	return 0;
}

int os_pthread_join (os_pthread_t tid, void ** status)
{
	return 0;
}

os_pthread_t os_pthread_self (void)
{
	return NULL;
}

int os_pthread_detach (os_pthread_t tid)
{
	return 0;
}

void os_pthread_exit (void *status)
{

}


int os_pthread_mutex_init(os_pthread_mutex_t * mutex,const os_pthread_mutexattr_t *attr)
{
	return 0;
}
int os_pthread_mutex_destroy(os_pthread_mutex_t *mutex){
	return 0;
}
int os_pthread_mutex_lock(os_pthread_mutex_t *mutex){
	return 0;
}
int os_pthread_mutex_trylock(os_pthread_mutex_t *mutex){
	return 0;
}
int os_pthread_mutex_unlock(os_pthread_mutex_t *mutex){
	return 0;
}

///////////////////////////////////////////////////////////////////
int os_pthread_cond_destroy(os_pthread_cond_t *cond){
	return 0;
}
int os_pthread_cond_init(os_pthread_cond_t * cond,
       const os_pthread_condattr_t * attr){
	return 0;
}
int os_pthread_cond_timedwait(os_pthread_cond_t * cond,
       os_pthread_mutex_t * mutex,
       const os_timespec_t * abstime){
	return 0;
}
int os_pthread_cond_wait(os_pthread_cond_t * cond,
       os_pthread_mutex_t * mutex){
	return 0;
}
int os_pthread_cond_broadcast(os_pthread_cond_t *cond){
	return 0;
}
int os_pthread_cond_signal(os_pthread_cond_t *cond){
	return 0;
}


///////////////////////////////////////////////////////////////////
int os_sem_init(os_sem_t *sem, int pshared, unsigned int value){
	return 0;
}

int os_sem_wait(os_sem_t *sem){
	return 0;
}

int os_sem_trywait(os_sem_t *sem){
	return 0;
}

int os_sem_post(os_sem_t * sem){
	return 0;
}

int os_sem_destroy(os_sem_t * sem){
	return 0;
}



