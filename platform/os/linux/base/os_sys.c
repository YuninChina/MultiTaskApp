#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "multitask.h"


int os_open(const char *pathname, int flags)
{
	return open(pathname,flags);
}

int os_open2(const char *pathname, int flags, int mode)
{
	return open(pathname,flags,mode);
}

int os_close(int fd)
{
	return close(fd);
}

int os_read(int fd, void *buf, unsigned int count)
{
	return read(fd,buf,count);
}

int os_write(int fd, const void *buf, unsigned int count)
{
	return write(fd,buf,count);
}




