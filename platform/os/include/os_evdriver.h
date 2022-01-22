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

/* Run flags */
#define EVDIRVER_ONCE        1		/**< run loop once    */
#define EVDIRVER_NONBLOCK    2		/**< exit if no event */


typedef struct os_event_s {
	os_evdriver_type_e event;
	union {
		struct __evdriver_io_s {
			int fd;
		}io;
		
		struct __evdriver_signal_s {
			int signo;
		}signal;

		struct __evdriver_timer_s {
			unsigned long when;
			unsigned long interval;
		}timer;
	};
}os_event_t;


typedef struct os_evdriver_s os_evdriver_t;
typedef struct os_evdriver_node_s os_evdriver_node_t;

typedef void (*os_evdriver_callback_t)(os_evdriver_node_t *evdriver,void *user_data);

//////////////////////////////////////////////////////////////////////////////
os_evdriver_t *os_evdriver_create(void);
os_evdriver_t *os_evdriver_create2(const char *name);
void os_evdriver_destroy(os_evdriver_t *evdriver);
int os_evdriver_run(os_evdriver_t *evdriver);
int os_evdriver_run2(os_evdriver_t *evdriver,int flag);

os_evdriver_node_t *os_evdriver_add(os_evdriver_t *evdriver,os_event_t *ev,os_evdriver_callback_t cb,void *user_data);
void os_evdriver_del(os_evdriver_node_t *evnode);

#ifdef  __cplusplus
}
#endif


#endif

