#ifndef __MM__H
#define __MM__H

#ifdef  __cplusplus
extern "C" {
#endif

#define CONFIG_MM_TRACE 1

void *mm_malloc(const char *func,unsigned long line,unsigned long size);
void mm_free(void *addr);
void mm_show(void);
void mm_show2(void (*show)(const char *));

#define MALLOC(size)	mm_malloc(__func__,__LINE__,size)
#define FREE(addr)		mm_free(addr)

#ifdef  __cplusplus
}
#endif

#endif
