#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <assert.h>
#include <unistd.h>

#include <errno.h>
#include <fcntl.h>		/* O_CLOEXEC */
#include <string.h>		/* memset() */
#include <sys/epoll.h>
#include <sys/ioctl.h>
#include <sys/select.h>		/* for select() workaround */
#include <sys/signalfd.h>	/* struct signalfd_siginfo */
#include <sys/timerfd.h>
#include <signal.h>

#include "multitask.h"
#include <glib.h>
#include <glib-unix.h>


typedef struct glib_evdriver_timer_s {
	os_evdriver_node_t *evnode;
	gboolean loop;
	os_evdriver_callback_t cb;
	void *userdata;
}glib_evdriver_timer_t;

typedef struct glib_evdriver_signal_s {
	os_evdriver_node_t *evnode;
	os_evdriver_callback_t cb;
	void *userdata;
}glib_evdriver_signal_t;

typedef struct glib_evdriver_io_s {
	os_evdriver_node_t *evnode;
	os_evdriver_callback_t cb;
	void *userdata;
}glib_evdriver_io_t;


static gboolean
__evdriver_timeout_callback (gpointer data)
{
	gboolean loop = G_SOURCE_REMOVE;
	glib_evdriver_timer_t *timer = (glib_evdriver_timer_t *)data;
	loop = timer->loop;
	if(timer->cb) timer->cb(timer->evnode,timer->userdata);
	FREE(timer);
	timer = NULL;
	
    return loop;
}

static gboolean
__evdriver_on_sig_callback (gpointer data)
{
	glib_evdriver_signal_t *signal = (glib_evdriver_signal_t *)data;
	if(signal->cb) signal->cb(signal->evnode,signal->userdata);
	FREE(signal);
	signal = NULL;
    return G_SOURCE_REMOVE;
}

static gboolean
__evdriver_on_io_callback (gpointer data)
{
	glib_evdriver_io_t *io = (glib_evdriver_io_t *)data;
	if(io->cb) io->cb(io->evnode,io->userdata);
	FREE(io);
	io = NULL;
    return G_SOURCE_CONTINUE;
}



os_evdriver_t *os_evdriver_create2(const char *name)
{
	os_evdriver_t *evdriver = NULL;
	evdriver = (os_evdriver_t *)g_main_new(TRUE);
	return evdriver;
}

os_evdriver_t *os_evdriver_create(void)
{
	return os_evdriver_create2("evdriver");
}


void os_evdriver_destroy(os_evdriver_t *evdriver)
{
	g_main_loop_unref((GMainLoop *)evdriver);
}

int os_evdriver_run2(os_evdriver_t *evdriver,int flags)
{
	g_main_loop_run((GMainLoop *)evdriver);
	return 0;
}

int os_evdriver_run(os_evdriver_t *evdriver)
{
	return os_evdriver_run2(evdriver,0);
}

os_evdriver_node_t *os_evdriver_add(os_evdriver_t *evdriver,os_event_t *ev,os_evdriver_callback_t cb,void *user_data)
{
	GMainContext* context = NULL;
	GSource *source = NULL;
	RETURN_VAL_IF_FAIL(evdriver && ev && cb, NULL);
	RETURN_VAL_IF_FAIL(ev->event < OS_EVDRIVER_BUTT, NULL);

	context = g_main_loop_get_context((GMainLoop *)evdriver);
	RETURN_VAL_IF_FAIL(context, NULL);
	
	switch (ev->event)
	{
		case OS_EVDRIVER_IO: 
		{
			glib_evdriver_timer_t *io = NULL;
			io = MALLOC(sizeof(*io));
			RETURN_VAL_IF_FAIL(io, NULL);
			io->evnode = (os_evdriver_node_t *)context;
			io->cb = cb;
			io->userdata = user_data;			
			source = g_unix_fd_source_new (ev->io.fd, G_IO_IN);
			g_source_set_priority (source, G_PRIORITY_DEFAULT);
			g_source_set_callback (source, (GSourceFunc) __evdriver_on_io_callback,io , NULL);
			g_source_attach (source, NULL);
			g_source_unref (source);
		}break;
		case OS_EVDRIVER_TIMER: 
		{
			glib_evdriver_timer_t *timer = NULL;
			unsigned long timeout = 0;
			timer = MALLOC(sizeof(*timer));
			RETURN_VAL_IF_FAIL(timer, NULL);
			timer->evnode = (os_evdriver_node_t *)context;
			timer->loop = G_SOURCE_REMOVE;
			timer->cb = cb;
			timer->userdata = user_data;
			if(ev->timer.interval > 0) 
			{
				timeout = ev->timer.interval;
				timer->loop = G_SOURCE_CONTINUE;
			}
			else 
			{
				timeout = ev->timer.when;
			}
			source = g_timeout_source_new(timeout);
			g_source_set_callback (source, (GSourceFunc)__evdriver_timeout_callback, timer, NULL);
			g_source_set_priority (source, G_PRIORITY_DEFAULT);
			g_source_attach (source, context);
			g_source_unref (source);
		}break;
		case OS_EVDRIVER_SIGNAL: 
		{
			glib_evdriver_signal_t *signal = NULL;
			signal = MALLOC(sizeof(*signal));
			RETURN_VAL_IF_FAIL(signal, NULL);
			signal->evnode = (os_evdriver_node_t *)context;
			signal->cb = cb;
			signal->userdata = user_data;			
			source = g_unix_signal_source_new (ev->signal.signo);
			g_source_set_priority (source, G_PRIORITY_DEFAULT);		
			g_source_set_callback (source, (GSourceFunc)__evdriver_on_sig_callback, user_data, NULL);
		 	g_source_attach (source, context);
			g_source_unref (source);
		}break;
		default:
		{
			GOTO_LABEL_IF_FAIL(0,fail);
		}break;
	}

	return (os_evdriver_node_t *)context;
	
fail:

	return NULL;
}


void os_evdriver_del(os_evdriver_node_t *evnode)
{

}




