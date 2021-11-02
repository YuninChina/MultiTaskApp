#ifndef __MT_MSG_H
#define __MT_MSG_H

#ifdef  __cplusplus
extern "C" {
#endif

typedef struct os_msg_s {
	const char *src;
	const char *dst;
	int priority;
	unsigned int size;
	unsigned char data[0];
}os_msg_t;


int os_msg_send(os_msg_t *msg);
os_msg_t *os_msg_recv(void);
int os_msg_send2(const char *dst,int priority,void *data,unsigned int size);
void *os_msg_recv2(unsigned int *p_size);


#ifdef  __cplusplus
}
#endif



#endif
