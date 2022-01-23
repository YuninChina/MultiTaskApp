#include "multitask.h"
#include "parson.h"

#define TCP_SERVER2_PORT  6666
#define TCP_SERVER2_IP  "127.0.0.1"
#define TCP_SERVER2_MAC  "ff:ff:ff:ff:ff:ff"

#define UDP_SERVER2_PORT  9999

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
	
	b = os_broadcast_create(BROADCAST_TYPE_SERVER,UDP_SERVER2_PORT);
	RETURN_VAL_IF_FAIL(b,NULL);
	PJSON_Value *jvalRoot = pjson_value_init_object();
	RETURN_VAL_IF_FAIL(jvalRoot,NULL);
	PJSON_Object *jobjRoot = pjson_value_get_object(jvalRoot);
	RETURN_VAL_IF_FAIL(jobjRoot,NULL);
	pjson_object_dotset_string(jobjRoot,"ip",TCP_SERVER2_IP);
	pjson_object_dotset_number(jobjRoot,"port",TCP_SERVER2_PORT);
	pjson_object_dotset_string(jobjRoot,"mac",TCP_SERVER2_MAC);
	pjson = pjson_serialize_to_string(jvalRoot);
	RETURN_VAL_IF_FAIL(pjson,NULL);
	while(1)
	{
		MLOGD("json: %s\n",pjson);
		os_broadcast_send(b, (unsigned char *)pjson, os_strlen(pjson));
		/////////////////////////////
		/////////////////////////////
		os_sleep(1);
	}
	
	os_broadcast_destroy(b);
	pjson_value_free(jvalRoot);
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



