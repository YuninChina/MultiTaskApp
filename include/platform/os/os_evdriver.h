#ifndef __OS_EVDRIVER__H
#define __OS_EVDRIVER__H


#ifdef  __cplusplus
extern "C" {
#endif

typedef enum {
	OS_EVDRIVER_IO,
	OS_EVDRIVER_TIMER,
	OS_EVDRIVER_SIGNAL,
	OS_EVDRIVER_BUTT
}os_evdriver_type_e;

typedef struct os_evdriver_s os_evdriver_t;

typedef void (*os_evdriver_callback_t)(os_evdriver_t *handle,int event,void *user_data);

os_evdriver_t *os_evdriver_create(void);
os_evdriver_t *os_evdriver_create2(const char *name);
void os_evdriver_destroy(os_evdriver_t *evdriver);
int os_evdriver_run(os_evdriver_t *evdriver);
int os_evdriver_add(os_evdriver_t *evdriver,int event,int fd,os_evdriver_callback_t cb,void *user_data);
void os_evdriver_del(os_evdriver_t *evdriver,int fd);

#ifdef  __cplusplus
}
#endif


#endif

