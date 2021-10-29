#ifndef __BROADCAST_H
#define __BROADCAST_H


#ifdef  __cplusplus
extern "C" {
#endif

typedef struct os_broadcast_s os_broadcast_t;

typedef enum {
	BROADCAST_TYPE_SERVER = 0,
	BROADCAST_TYPE_CLIENT,
	BROADCAST_TYPE_BUTT	
}os_broadcast_type_e;

//
os_broadcast_t *os_broadcast_create(os_broadcast_type_e type,unsigned short port);
void os_broadcast_destroy(os_broadcast_t *b);
int os_broadcast_send(os_broadcast_t *b,unsigned char *data,unsigned int size);
int os_broadcast_recv(os_broadcast_t *b,unsigned char *data,unsigned int size);


#ifdef  __cplusplus
}
#endif



#endif  //__BROADCAST_H
