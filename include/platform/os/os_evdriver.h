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
}mt_evdriver_type_e;

/* Run flags */
#define EVDIRVER_ONCE        1		/**< run loop once    */
#define EVDIRVER_NONBLOCK    2		/**< exit if no event */


typedef struct mt_event_s {
	mt_evdriver_type_e event;
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
}mt_event_t;


typedef struct mt_evdriver_s mt_evdriver_t;
typedef struct mt_evdriver_node_s mt_evdriver_node_t;

typedef void (*mt_evdriver_callback_t)(mt_evdriver_node_t *evnode,void *user_data,int events);

//////////////////////////////////////////////////////////////////////////////
mt_evdriver_t *mt_evdriver_create(void);
mt_evdriver_t *mt_evdriver_create2(const char *name);
void mt_evdriver_destroy(mt_evdriver_t *evdriver);
int mt_evdriver_run(mt_evdriver_t *evdriver);
int mt_evdriver_run2(mt_evdriver_t *evdriver,int flag);

mt_evdriver_node_t *mt_evdriver_add(mt_evdriver_t *evdriver,mt_event_t *ev,mt_evdriver_callback_t cb,void *user_data);
void mt_evdriver_del(mt_evdriver_node_t *evnode);

#ifdef  __cplusplus
}
#endif


#endif

