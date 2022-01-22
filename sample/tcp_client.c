#include "multitask.h"

int main(int argc, char *argv[])
{
	os_tcp_client_t *c = NULL;
	const char *str = NULL;
	int ret = -1;
	char buf[1024] = {0};
	if (argc != 2) {
	   MLOGE("Usage: %s <string>\n", argv[0]);
	   return -1;
	}
	str = argv[1];
	c = os_tcp_client_create("tcp_client", "127.0.0.1",6666);
	RETURN_VAL_IF_FAIL(c, -1);
	MLOGD("os_tcp_client_create succ!!!");
	ret = os_tcp_client_write(c,(void *)str, os_strlen(str));
	RETURN_VAL_IF_FAIL(ret > 0, -1);	
	ret = os_tcp_client_read(c,(void *)buf, sizeof(buf));
	RETURN_VAL_IF_FAIL(ret > 0, -1);
	MLOGD("read %d byte: %s",ret,buf);
	os_tcp_client_destroy(c);
	return 0;
}

