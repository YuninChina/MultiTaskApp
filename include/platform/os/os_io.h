#ifndef __OS_IO_H
#define __OS_IO_H

#ifdef  __cplusplus
extern "C" {
#endif

typedef void (*os_io_callback_t)(const char *name,void *data,unsigned int size,void *user_data);

typedef struct {
	const char *name;
	int fd;
	int (*send)(const char *name,void *data,unsigned int size,long long timeout);
}os_io_node;


int os_io_register(const char *name,os_io_callback_t cb,void *user_data);
void os_io_unregister(const char *name);

int os_io_node_add(os_io_node *node);
void os_io_node_del(const char *name);
void os_io_node_del2(int fd);



#ifdef  __cplusplus
}
#endif

#endif  /* __OS_IO_H */



