#include "multitask.h"


static void __tcp_server_callback(int fd,void *user_data)
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


int main(int argc, char *argv[])
{
	os_tcp_server_t *s = NULL;
	s = os_tcp_server_create(5, 5, 6666, __tcp_server_callback, NULL);
	RETURN_VAL_IF_FAIL(s, -1);
	MLOGD("os_tcp_server_create succ!!!");
	os_tcp_server_destroy(s);
	return 0;
}


