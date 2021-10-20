#ifndef __MULTICASR_H
#define __MULTICASR_H


typedef struct multicast_s multicast_t;


typedef enum {
	MULTICAST_TYPE_SERVER = 0,
	MULTICAST_TYPE_CLIENT,
	MULTICAST_TYPE_BUTT	
}multicast_type_e;

//
multicast_t *multicast_create(multicast_type_e type,const char *addr,unsigned short port);
void multicast_destroy(multicast_t *m);
int multicast_send(multicast_t *m,unsigned char *data,unsigned int size);
int multicast_recv(multicast_t *m,unsigned char *data,unsigned int size);

#endif  //__MULTICASR_H

