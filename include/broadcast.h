#ifndef __BROADCAST_H
#define __BROADCAST_H


typedef struct broadcast_s broadcast_t;

typedef enum {
	BROADCAST_TYPE_SERVER = 0,
	BROADCAST_TYPE_CLIENT,
	BROADCAST_TYPE_BUTT	
}broadcast_type_e;

//
broadcast_t *broadcast_create(broadcast_type_e type,unsigned short port);
void broadcast_destroy(broadcast_t *b);
int broadcast_send(broadcast_t *b,unsigned char *data,unsigned int size);
int broadcast_recv(broadcast_t *b,unsigned char *data,unsigned int size);


#endif  //__BROADCAST_H
