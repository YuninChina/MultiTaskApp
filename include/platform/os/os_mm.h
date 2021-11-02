#ifndef __MM__H
#define __MM__H

#ifdef  __cplusplus
extern "C" {
#endif

#define CONFIG_OS_MM_TRACE 1

void *os_mm_malloc(const char *func,unsigned long line,unsigned long size);
void os_mm_free(void *addr);
void os_mm_show(void);
void os_mm_show2(void (*show)(const char *));

#define MALLOC(size)	os_mm_malloc(__func__,__LINE__,size)
#define FREE(addr)		os_mm_free(addr)

#ifdef  __cplusplus
}
#endif

#endif
