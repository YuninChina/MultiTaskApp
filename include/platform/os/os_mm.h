#ifndef __MM__H
#define __MM__H

#ifdef  __cplusplus
extern "C" {
#endif

#define CONFIG_OS_MM_TRACE 1

void *mt_mm_malloc(const char *func,unsigned long line,unsigned long size);
void mt_mm_free(void *addr);
void mt_mm_show(void);
void mt_mm_show2(void (*show)(const char *));

#define MALLOC(size)	mt_mm_malloc(__func__,__LINE__,size)
#define FREE(addr)		mt_mm_free(addr)

#ifdef  __cplusplus
}
#endif

#endif
