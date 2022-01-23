#include "multitask.h"

#define TCP_CLINET2_PORT  6666

typedef struct test_ip_info_s {
	const char *ip;
	unsigned short port;
	unsigned char mac[6];
}test_ip_info_t;

static void *task_routine_tcp_client2(void *arg)
{
	test_ip_info_t *ipinfo = NULL;
	unsigned int size = 0;	
	static os_tcp_client_t *c = NULL;
	const char *str = "i'm task_routine_tcp_client2...";
	int ret = -1;
	char buf[1024] = {0};
	static const char *old_ip = NULL;
	while(1)
	{
		os_memset(buf, 0, sizeof(buf));
		ipinfo = os_msg_recv2(&size);
		RETURN_VAL_IF_FAIL(ipinfo, NULL);
		if(NULL == old_ip 
		|| 0 != os_strcmp(old_ip,ipinfo->ip))
		{
			old_ip = ipinfo->ip;
			if(c) 
				os_tcp_client_destroy(c);
			c = os_tcp_client_create("tcp_client", ipinfo->ip,ipinfo->port);
			RETURN_VAL_IF_FAIL(c, NULL);
			MLOGD("os_tcp_client_create succ!!!");
		}
		if(c)
		{
			ret = os_tcp_client_write(c,(void *)str, os_strlen(str));
			RETURN_VAL_IF_FAIL(ret > 0, NULL);	
			ret = os_tcp_client_read(c,(void *)buf, sizeof(buf));
			RETURN_VAL_IF_FAIL(ret > 0, NULL);
			MLOGD("read %d byte: %s",ret,buf);
		}
		
		FREE(ipinfo);
	}
	return NULL;
}


static void *task_routine_udp_client2(void *arg)
{
	os_broadcast_t *b = NULL;
	char *pjson = NULL;
	unsigned char buf[1024] = {0};
	int ret = -1;
	test_ip_info_t *ipinfo = NULL;
	
	b = os_broadcast_create(BROADCAST_TYPE_CLIENT,TCP_CLINET2_PORT);
	ASSERT(b);
	
	while(1)
	{
		ret = os_broadcast_recv(b, buf, sizeof(buf));
		//json parse

		//
		os_msg_send2("tcp_client2", 0, ipinfo, sizeof(*ipinfo));
		/////////////////////////////
		os_sleep(1);
	}
	
	os_broadcast_destroy(b);
	return NULL;
}


int main(int argc, char *argv[])
{
	os_task_create("tcp_client2",0,0, task_routine_tcp_client2, (void *)NULL);
	os_task_create("udp_client2",0,0, task_routine_udp_client2, (void *)NULL);
	
	while(1) 
		os_sleep(5);
	return 0;
}



