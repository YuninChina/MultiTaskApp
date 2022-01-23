#include "multitask.h"

#define TCP_SERVER2_PORT  6666

static void __tcp_server_callback2(int fd,void *user_data)
{
	char buf[1024] = {0};
	int ret = -1;
	ret = os_read(fd,buf, sizeof(buf));
	RETURN_IF_FAIL(ret > 0);
	MLOGD("os_read: %s",buf);
	ret = os_write(fd,buf, ret);
	RETURN_IF_FAIL(ret > 0);
	MLOGD("os_write: %d byte",ret);
}

static void *task_routine_tcp_server2(void *arg)
{
	os_tcp_server_t *s = NULL;
	s = os_tcp_server_create(5, 5, TCP_SERVER2_PORT, __tcp_server_callback2, NULL);
	RETURN_VAL_IF_FAIL(s, NULL);
	MLOGD("os_tcp_server_create succ!!!");
	os_tcp_server_destroy(s);
	return NULL;
}


static void *task_routine_udp_server2(void *arg)
{
	os_broadcast_t *b = NULL;
	char *pjson = NULL;
	
	b = os_broadcast_create(BROADCAST_TYPE_SERVER,TCP_SERVER2_PORT);
	ASSERT(b);
	
	while(1)
	{
		if(0 == os_task_mm_json_get(&pjson))
		{
			MLOGD("\n%s\n",pjson);
			os_broadcast_send(b, (unsigned char *)pjson, os_strlen(pjson));
			/////////////////////////////
			FREE(pjson);
		}
		/////////////////////////////
		os_sleep(1);
	}
	
	os_broadcast_destroy(b);
	return NULL;
}


int main(int argc, char *argv[])
{
	os_task_create("tcp_server2",0,0, task_routine_tcp_server2, (void *)NULL);
	os_task_create("udp_server2",0,0, task_routine_udp_server2, (void *)NULL);
	
	while(1) 
		os_sleep(5);
	return 0;
}



