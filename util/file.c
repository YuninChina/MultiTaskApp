/**
  ******************************************************************************
  * @file file.c
  * @author  leon.xie
  * @version v1.0.0
  * @date 2018-10-20 13:30:26
  * @brief  This file provides all the file functions. 
  ******************************************************************************
  * @attention
  *
  * File For Yunin Software Team Only
  *
  * Copyright (C), 2017-2027, Yunin Software Team
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/

#include "diagnosis.h"
#include "file.h"
#include "util.h"

#ifdef  __cplusplus
extern "C" {
#endif


/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#undef S_ISREG
#define S_ISREG(m) (((m) & S_IFMT) == S_IFREG)
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

ssize_t
file_read(int fd, void *buf, size_t count)
{
    size_t nread = 0;
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


ssize_t
file_write(int fd, const void *buf, size_t count)
{
    size_t nwritten = 0;
    while (count > 0) {
        ssize_t r = write(fd, buf, count);

        if (r < 0 && errno == EINTR)
            continue;
        if (r < 0)
            return r;
        if (r == 0)
            return nwritten;
        buf = (const char *)buf + r;
        count -= r;
        nwritten += r;
    }
    return nwritten;
}


int
file_write_atomic(const char *path, const void *data, int len, mode_t mode)
{
    int fd;

    if (mode)
        fd = open(path, O_WRONLY|O_APPEND|O_CREAT, mode);
    else
        fd = open(path, O_WRONLY|O_APPEND);
    if (fd == -1)
        return -1;

    if (file_write(fd, data, len) < 0) {
        close(fd);
        return -1;
    }

    /* Use errno from failed close only if there was no write error.  */
    if (close(fd) != 0)
        return -1;

    return 0;
}

int
file_rewrite_atomic(const char *path, const void *data, int len, mode_t mode)
{
    int fd;

    if (mode)
        fd = open(path, O_WRONLY|O_CREAT, mode);
    else
        fd = open(path, O_WRONLY);
    if (fd == -1)
        return -1;

    if (file_write(fd, data, len) < 0) {
        close(fd);
        return -1;
    }

    /* Use errno from failed close only if there was no write error.  */
    if (close(fd) != 0)
        return -1;

    return 0;
}


int
file_read_atomic(const char *file,void *data, int len)
{
    int fd;
    ssize_t sz;

    fd = open(file, O_RDONLY);
    if (fd < 0)
        return -errno;

    sz = file_read(fd, data, len);
    close(fd);
    if (sz < 0)
        return -errno;

    return sz;
}



int
file_write_str(const char *path, const char *str, mode_t mode)
{
    int fd;

    if (mode)
        fd = open(path, O_WRONLY|O_APPEND|O_CREAT, mode);
    else
        fd = open(path, O_WRONLY|O_APPEND);
    if (fd == -1)
        return -1;

    if (file_write(fd, str, strlen(str)) < 0) {
        close(fd);
        return -1;
    }

    /* Use errno from failed close only if there was no write error.  */
    if (close(fd) != 0)
        return -1;

    return 0;
}


int
file_read_buf(const char *file, char *buf, int len)
{
    int fd;
    ssize_t sz;

    fd = open(file, O_RDONLY);
    if (fd < 0)
        return -errno;

    sz = file_read(fd, buf, len - 1);
    close(fd);
    if (sz < 0)
        return -errno;

    buf[sz] = '\0';
    return sz;
}


off_t 
file_size_get(const char *path, int fd)
{
    struct stat s;

    if (fd >= 0) {
        if (fstat(fd, &s) < 0)
            return -1;
    } else {
        if (stat(path, &s) < 0)
            return -1;
    }

    if (!S_ISREG(s.st_mode))
        return -1;

    return s.st_size;

}

void 
trace2file(const char *filename,const char *fmt,...)
{
	char buffer[1024*4] = {0};
	
	if(filename)
	{
		va_list args;
		va_start(args, fmt);
		vsnprintf(buffer, sizeof(buffer), fmt, args);
		va_end(args);
	 	file_write_str(filename,buffer,0777);
	}
}



int 
read_txt_string(const char *path,const char *member,
	char *outputValue,unsigned int size)
{
	FILE *stream = NULL;
	char memberName[256];
	char buf[1024],*pBuf = NULL;
	char nodeStr[256],*pNode = NULL;
	int ret = -1;
	RETURN_VAL_IF_FAIL(path && member && outputValue, -1);
	stream = fopen(path,"rt");
	RETURN_VAL_IF_FAIL(stream, -1);
	while(!feof(stream))
	{
		memset(buf,0,sizeof(buf));
		memset(nodeStr,0,sizeof(nodeStr));
		pNode = nodeStr;
		pBuf = buf;
		GOTO_LABEL_IF_FAIL(NULL != fgets(buf,sizeof(buf),stream),fail);
		skip_spaces((const char **)&pBuf);
		ret = sscanf(pBuf,"%[0-9A-Za-z_. ]:%[0-9A-Za-z_. ]",memberName,nodeStr);
		GOTO_LABEL_IF_FAIL(2 == ret,fail);
		truncated_spaces(memberName);
		if(0 != strcmp(memberName,member))
		{
			continue;
		}
		skip_spaces((const char **)&pNode);
		truncated_spaces(pNode);
		strncpy(outputValue,pNode,size);
		fclose(stream);
		return 0;
	}

fail:	
	fclose(stream);
	return -1;
}


#ifdef  __cplusplus
}
#endif


