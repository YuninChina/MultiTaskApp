#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>
#include <semaphore.h>

#include "os_log.h"

#include "os_thread.h"


int os_pthread_create(os_pthread_t *tid, const os_pthread_attr_t *attr, void *(*func) (void *), void *arg)
{
	pthread_t id = 0;
	int ret = -1;
	ret = pthread_create(&id,NULL,func,arg);
	RETURN_VAL_IF_FAIL(0 == ret, -1);
	*tid = (os_pthread_t)id;
	return 0;
}

int os_pthread_join (os_pthread_t tid, void ** status)
{
	return pthread_join((pthread_t)tid,status);
}

os_pthread_t os_pthread_self (void)
{
	return (os_pthread_t)pthread_self();
}

int os_pthread_detach (os_pthread_t tid)
{
	return pthread_detach((pthread_t)tid);
}

void os_pthread_exit (void *status)
{
	pthread_exit(status);
}

///////////////////////////////////////////////////////////////////
int os_pthread_mutex_init(os_pthread_mutex_t *mutex,const os_pthread_mutexattr_t *attr)
{
	int ret = -1;
	pthread_mutex_t *p = malloc(sizeof(pthread_mutex_t));
	RETURN_VAL_IF_FAIL(p, -1);
	ret = pthread_mutex_init(p,NULL);
	GOTO_LABEL_IF_FAIL(0 == ret, fail);
	*mutex = (os_pthread_mutex_t)p;
	return ret;
fail:
	if(p) free(p);
	return -1;
}
int os_pthread_mutex_destroy(os_pthread_mutex_t *mutex){
	return pthread_mutex_destroy((pthread_mutex_t *)(*mutex));
}
int os_pthread_mutex_lock(os_pthread_mutex_t *mutex){
	pthread_mutex_t *p = (pthread_mutex_t *)(*mutex);
	return pthread_mutex_lock(p);
}
int os_pthread_mutex_trylock(os_pthread_mutex_t *mutex){
	pthread_mutex_t *p = (pthread_mutex_t *)(*mutex);
	return pthread_mutex_trylock(p);
}
int os_pthread_mutex_unlock(os_pthread_mutex_t *mutex){
	pthread_mutex_t *p = (pthread_mutex_t *)(*mutex);
	return pthread_mutex_unlock(p);
}

///////////////////////////////////////////////////////////////////
int os_pthread_cond_init(os_pthread_cond_t * cond,
       const os_pthread_condattr_t * attr){
	int ret = -1;
	pthread_cond_t *p = malloc(sizeof(pthread_cond_t));
	RETURN_VAL_IF_FAIL(p, -1);
	ret = pthread_cond_init(p,NULL);
	GOTO_LABEL_IF_FAIL(0 == ret, fail);
	*cond = (os_pthread_cond_t)p;
	return ret;
fail:
	if(p) free(p);
	return -1;
}

int os_pthread_cond_destroy(os_pthread_cond_t *cond) {
   return pthread_cond_destroy((pthread_cond_t *)(*cond));
}
int os_pthread_cond_timedwait(os_pthread_cond_t * cond,
       os_pthread_mutex_t * mutex,
       const os_timespec_t * abstime) {
	pthread_cond_t *c = (pthread_cond_t *)(*cond);
	pthread_mutex_t *m = (pthread_mutex_t *)(*mutex);
	struct timespec time;
	memset(&time,0,sizeof(time));
	//TODO: timer
	return pthread_cond_timedwait(c,m,&time);
}
int os_pthread_cond_wait(os_pthread_cond_t * cond,
       os_pthread_mutex_t *mutex) {
	pthread_cond_t *c = (pthread_cond_t *)(*cond);
	pthread_mutex_t *m = (pthread_mutex_t *)(*mutex);
	return pthread_cond_wait(c,m);
}
int os_pthread_cond_broadcast(os_pthread_cond_t *cond) {
	pthread_cond_t *c = (pthread_cond_t *)(*cond);
	return pthread_cond_broadcast(c);
}
int os_pthread_cond_signal(os_pthread_cond_t *cond) {
	pthread_cond_t *c = (pthread_cond_t *)(*cond);
	return pthread_cond_signal(c);
}


///////////////////////////////////////////////////////////////////
int os_sem_init(os_sem_t *sem, int pshared, unsigned int value){
	int ret = -1;
	sem_t *p = malloc(sizeof(sem_t));
	RETURN_VAL_IF_FAIL(p, -1);
	ret = sem_init(p,pshared,value);
	GOTO_LABEL_IF_FAIL(0 == ret, fail);
	*sem = (os_sem_t)p;
	return ret;
fail:
	if(p) free(p);
	return -1;
}

int os_sem_wait(os_sem_t *sem){
	sem_t *s = (sem_t *)(*sem);
	return sem_wait(s);
}

int os_sem_trywait(os_sem_t *sem){
	sem_t *s = (sem_t *)(*sem);
	return sem_trywait(s);
}

int os_sem_post(os_sem_t * sem){
	sem_t *s = (sem_t *)(*sem);
	return sem_post(s);
}

int os_sem_destroy(os_sem_t * sem){
	sem_t *s = (sem_t *)(*sem);
	return sem_destroy(s);
}



