#ifndef __MULTICASR_H
#define __MULTICASR_H

#ifdef  __cplusplus
extern "C" {
#endif

typedef struct os_multicast_s os_multicast_t;


typedef enum {
	MULTICAST_TYPE_SERVER = 0,
	MULTICAST_TYPE_CLIENT,
	MULTICAST_TYPE_BUTT	
}os_multicast_type_e;

//
os_multicast_t *os_multicast_create(os_multicast_type_e type,const char *addr,unsigned short port);
void os_multicast_destroy(os_multicast_t *m);
int os_multicast_send(os_multicast_t *m,unsigned char *data,unsigned int size);
int os_multicast_recv(os_multicast_t *m,unsigned char *data,unsigned int size);


#ifdef  __cplusplus
}
#endif


#endif  //__MULTICASR_H

