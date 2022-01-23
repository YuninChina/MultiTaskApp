#include "multitask.h"
#include "parson.h"

#define TCP_CLINET2_PORT  6666
#define UDP_CLINET2_PORT  9999

typedef struct test_ip_info_s {
	char ip[32];
	unsigned short port;
	unsigned char mac[6];
}test_ip_info_t;

static test_ip_info_t g_test_ip_info = {0};

static void *task_routine_tcp_client2(void *arg)
{
	static os_tcp_client_t *c = NULL;
	const char *str = "i'm task_routine_tcp_client2...";
	int ret = -1;
	char buf[1024] = {0};
	
	while(1)
	{
		os_memset(buf, 0, sizeof(buf));
		if(0 != g_test_ip_info.ip[0])
		{
			MLOGI("ip:%s , port:%d , mac[%02x:%02x:%02x:%02x:%02x:%02x]\n",
			g_test_ip_info.ip,g_test_ip_info.port,g_test_ip_info.mac[0],g_test_ip_info.mac[1],
			g_test_ip_info.mac[2],g_test_ip_info.mac[3],g_test_ip_info.mac[4],g_test_ip_info.mac[5]);
			c = os_tcp_client_create("tcp_client", g_test_ip_info.ip,g_test_ip_info.port);
			
			ret = os_tcp_client_write(c,(void *)str, os_strlen(str));
			RETURN_VAL_IF_FAIL(ret > 0, NULL);	
			ret = os_tcp_client_read(c,(void *)buf, sizeof(buf));
			RETURN_VAL_IF_FAIL(ret > 0, NULL);
			MLOGD("read %d byte: %s",ret,buf);
			os_tcp_client_destroy(c);
			c = NULL;
			os_sleep(1);
		}
		else
		{
			os_sleep(1);
		}
	}
	return NULL;
}


static void *task_routine_udp_client2(void *arg)
{
	os_broadcast_t *b = NULL;
	char *pjson = NULL;
	unsigned char buf[1024] = {0};
	int ret = -1;
	PJSON_Value *jvalRoot = NULL;
	PJSON_Object *jobjRoot = NULL;
	
	b = os_broadcast_create(BROADCAST_TYPE_CLIENT,UDP_CLINET2_PORT);
	ASSERT(b);
	
	while(1)
	{
		ret = os_broadcast_recv(b, buf, sizeof(buf));
		CONTINUE_IF_FAIL(ret > 0);
		//json parse
		MLOGD("buf: %s",buf);
		pjson = (char *)buf;
		jvalRoot = pjson_parse_string(pjson);
		RETURN_VAL_IF_FAIL(jvalRoot,NULL);
		jobjRoot = pjson_value_get_object(jvalRoot);
		RETURN_VAL_IF_FAIL(jobjRoot,NULL);
		os_strcpy(g_test_ip_info.ip,pjson_object_dotget_string(jobjRoot, "ip"));
		g_test_ip_info.port = (unsigned short)pjson_object_dotget_number(jobjRoot, "port");
		/*
		const char *mac = pjson_object_dotget_string(jobjRoot, "mac");
		os_sscanf(mac, "%x:%02x:%02x:%02x:%02x:%02x",&ipinfo->mac[0],&ipinfo->mac[1]
		,&ipinfo->mac[2],&ipinfo->mac[3],&ipinfo->mac[4],&ipinfo->mac[5]);
		*/
		pjson_value_free(jvalRoot);
		/////////////////////////////
		//os_sleep(1);
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



