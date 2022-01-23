#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>


#include "os_stdlibc.h"


void *os_malloc(os_size_t size)
{
	return malloc((size_t)size);
}

void os_free(void *ptr)
{
	free(ptr);
}

void *os_calloc(os_size_t nmemb, os_size_t size)
{
	return calloc((size_t)nmemb,(size_t)size);
}

void *os_realloc(void *ptr, os_size_t size)
{
	return realloc(ptr,(size_t)size);
}

int os_system(const char *command)
{
	return system(command);
}

unsigned int os_sleep(unsigned int seconds)
{
	return sleep(seconds);
}


void *os_memset(void *s, int c, os_size_t n)
{
	return memset(s, c, (size_t)n);
}


char *os_strcpy(char *dest, const char *src)
{
	return strcpy(dest,src);
}

char *os_strncpy(char *dest, const char *src, os_size_t n)
{
	return strncpy(dest,src,n);
}

int os_scanf(const char *format, ...)
{
	int ret = -1;
	va_list args;
	va_start(args, format);
	ret = scanf(format,args);
	va_end(args);
	return ret;
}

int os_sscanf(const char *str, const char *format, ...)
{
	int ret = -1;
	va_list args;
	va_start(args, format);
	ret = sscanf(str,format,args);
	va_end(args);
	return ret;
}

os_size_t os_strlen(const char *s)
{
	return strlen(s);
}


int os_strcmp(const char *s1, const char *s2)
{
	return strcmp(s1,s2);
}


int os_strncmp(const char *s1, const char *s2, unsigned int n)
{
	return strncmp(s1,s2,n);
}


