
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



#ifdef  __cplusplus
}
#endif

#endif 

