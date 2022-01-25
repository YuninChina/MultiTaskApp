#ifndef __OS_FILE_H
#define __OS_FILE_H

#ifdef  __cplusplus
extern "C" {
#endif

unsigned int os_file_write(int fd, const void* buf, unsigned int count);
int os_file_append(const char* path, unsigned char* data,unsigned int size);
int os_file_write_str(const char* path, const char* str, int mode);
int os_file_rewrite(const char *path, const void *data, int len);
unsigned int os_file_read(int fd, void *buf, unsigned int count);
int os_file_read2(const char *file,void *data, int len);
void os_trace2file(const char* filename, const char* fmt, ...);
int os_file_exist(const char *filename);
int os_file_size(const char* filename);
int os_file_key_write(const char *filename,const char *key,const char *val);




#ifdef  __cplusplus
}
#endif

#endif  /* __OS_FILE_H */




