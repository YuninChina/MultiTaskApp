/**
  ******************************************************************************
  * @file yin_file.h 
  * @author leon.xie
  * @version v1.0.0
  * @date 2018-7-12 22:19:25
  * @brief This file contains all the functions prototypes for the BOARD 
  *  yin_file 
  ******************************************************************************
  * @attention
  *
  * File For Yunin Software Team Only
  *
  * Copyright (C), 2017-2027, Yunin Software Team
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __YIN_FILE_H
#define __YIN_FILE_H

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdarg.h>

#ifdef  __cplusplus
extern "C" {
#endif

/* Exported typedef ----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
ssize_t file_read(int fd, void *buf, size_t count);
ssize_t file_write(int fd, const void *buf, size_t count);
int file_write_str(const char *path, const char *str, mode_t mode);
int file_read_buf(const char *file, char *buf, int len);
off_t file_size_get(const char *path, int fd);
int file_write_atomic(const char *path, const void *data, int len, mode_t mode);
int file_rewrite_atomic(const char *path, const void *data, int len, mode_t mode);
int file_read_atomic(const char *file,void *data, int len);
void trace2file(const char *filename,const char *fmt,...);
int read_txt_string(const char *path,const char *member,char *outputValue,unsigned int size);

#ifdef  __cplusplus
}
#endif

#endif  /* __YIN_FILE_H */


