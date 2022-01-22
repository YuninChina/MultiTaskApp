
#ifndef __OS_SYS__H
#define __OS_SYS__H
#include "os_type.h"

#ifdef  __cplusplus
extern "C" {
#endif

int os_open(const char *pathname, int flags);
int os_open2(const char *pathname, int flags, int mode);
int os_close(int fd);
int os_read(int fd, void *buf, unsigned int count);
int os_write(int fd, const void *buf, unsigned int count);



#ifdef  __cplusplus
}
#endif

#endif 

