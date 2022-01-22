#ifndef __OS_TCP_CLIENT_H
#define __OS_TCP_CLIENT_H

#ifdef  __cplusplus
extern "C" {
#endif

typedef void (*os_tcp_client_callback_t)(int fd,void *user_data);

typedef struct os_tcp_client_s os_tcp_client_t;

os_tcp_client_t *os_tcp_client_create(const char *name,const char *ip,unsigned short port);
void os_tcp_client_destroy(os_tcp_client_t *c);
int os_tcp_client_write(os_tcp_client_t *c,void *data,unsigned int size);
int os_tcp_client_read(os_tcp_client_t *c,void *data,unsigned int size);




#ifdef  __cplusplus
}
#endif

#endif  /* __OS_TCP_CLIENT_H */


