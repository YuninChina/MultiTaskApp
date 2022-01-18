#include "multitask.h"


#ifdef  __cplusplus
extern "C" {
#endif




int os_io_register(const char *name,os_io_callback_t cb,void *user_data)
{
	return 0;
}

void os_io_unregister(const char *name)
{

}

int os_io_node_add(os_io_node *node)
{
	return 0;
}

void os_io_node_del(const char *name)
{

}

void os_io_node_del2(int fd)
{

}




#ifdef  __cplusplus
}
#endif

