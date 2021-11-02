#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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

