
#include "multitask.h"


#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <assert.h>


unsigned int os_file_write(int fd, const void* buf, unsigned int count)
{
    unsigned int nwritten = 0;
    while (count > 0)
    {
        ssize_t r = write(fd, buf, count);

        if (r < 0 && errno == EINTR) continue;
        if (r < 0) return r;
        if (r == 0) return nwritten;
        buf = (const char*)buf + r;
        count -= r;
        nwritten += r;
    }
    return nwritten;
}

int os_file_append(const char* path, unsigned char* data,unsigned int size)
{
    int fd;

	fd = open(path, O_WRONLY | O_APPEND | O_CREAT, 0777);
    if (fd == -1) 
    	return -1;
    if (os_file_write(fd, data, size) < 0)
    {
        close(fd);
        return -1;
    }
    /* Use errno from failed close only if there was no write error.  */
    if (close(fd) != 0) return -1;

    return 0;
}


int os_file_write_str(const char* path, const char* str, int mode)
{
    int fd;

    if (mode)
        fd = open(path, O_WRONLY | O_APPEND | O_CREAT, mode);
    else
        fd = open(path, O_WRONLY | O_APPEND);
    if (fd == -1) 
    	return -1;

    if (os_file_write(fd, str, strlen(str)) < 0)
    {
        close(fd);
        return -1;
    }

    /* Use errno from failed close only if there was no write error.  */
    if (close(fd) != 0) 
    	return -1;

    return 0;
}



int os_file_rewrite(const char *path, const void *data, int len)
{
    int fd;

	fd = open(path, O_WRONLY|O_CREAT, 0777);
    if (fd == -1)
        return -1;

    if (os_file_write(fd, data, len) < 0) {
        close(fd);
        return -1;
    }

    /* Use errno from failed close only if there was no write error.  */
    if (close(fd) != 0)
        return -1;

    return 0;
}

unsigned int os_file_read(int fd, void *buf, unsigned int count)
{
    unsigned int nread = 0;
    while (count > 0) {
        ssize_t r = read(fd, buf, count);
        if (r < 0 && errno == EINTR)
            continue;
        if (r < 0)
            return r;
        if (r == 0)
            return nread;
        buf = (char *)buf + r;
        count -= r;
        nread += r;
    }
    return nread;
}

int os_file_read2(const char *file,void *data, int len)
{
    int fd;
    ssize_t sz;

    fd = open(file, O_RDONLY);
    if (fd < 0)
        return -errno;

    sz = os_file_read(fd, data, len);
    close(fd);
    if (sz < 0)
        return -errno;

    return sz;
}


void os_trace2file(const char* filename, const char* fmt, ...)
{
    char buffer[1024 * 4] = {0};
    int size = 0;
    if (filename)
    {
        va_list args;
        va_start(args, fmt);
        size = vsnprintf(buffer, sizeof(buffer), fmt, args);
        va_end(args);
        buffer[size] = '\n';
        os_file_write_str(filename, buffer, 0777);
    }
}

int os_file_exist(const char *filename)
{
	return (access(filename,F_OK) != -1);
}

int os_file_size(const char* filename)
{
    struct stat statbuf;
    int size = 0;
    stat(filename,&statbuf);
    size = statbuf.st_size;
    return size;
}

int os_file_key_write(const char *filename,const char *key,const char *val)
{
	char buf[512] = {0};
	
	snprintf(buf,sizeof(buf),"%s=%s\n",key,val);
	return os_file_write_str(filename, buf, 0777);
}


