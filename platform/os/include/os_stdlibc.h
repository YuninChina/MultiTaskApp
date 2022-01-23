
#ifndef __OS_STDLIBC__H
#define __OS_STDLIBC__H
#include "os_type.h"

#ifdef  __cplusplus
extern "C" {
#endif


void *os_malloc(os_size_t size);
void os_free(void *ptr);
void *os_calloc(os_size_t nmemb, os_size_t size);
void *os_realloc(void *ptr, os_size_t size);

int os_system(const char *command);
unsigned int os_sleep(unsigned int seconds);
void *os_memset(void *s, int c, os_size_t n);
char *os_strcpy(char *dest, const char *src);
char *os_strncpy(char *dest, const char *src, os_size_t n);

int os_scanf(const char *format, ...);
int os_sscanf(const char *str, const char *format, ...);
os_size_t os_strlen(const char *s);
int os_strcmp(const char *s1, const char *s2);
int os_strncmp(const char *s1, const char *s2, unsigned int n);



#ifdef  __cplusplus
}
#endif

#endif 

