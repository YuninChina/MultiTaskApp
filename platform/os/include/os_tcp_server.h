#ifndef __OS_TCP_SERVER_H
#define __OS_TCP_SERVER_H

#ifdef  __cplusplus
extern "C" {
#endif

#define TASK_TCP_SERVER	"tcp_server_node"

#define TCP_SERVER_QUQUE_MAX 10

typedef void (*os_tcp_server_callback_t)(int fd,void *user_data);

typedef struct os_tcp_server_s os_tcp_server_t;

os_tcp_server_t *os_tcp_server_create(int threads,unsigned int listen_fds,int port,os_tcp_server_callback_t cb,void *user_data);
void os_tcp_server_destroy(os_tcp_server_t *s);



#ifdef  __cplusplus
}
#endif

#endif  /* __OS_TCP_SERVER_H */


