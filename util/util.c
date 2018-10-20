/**
  ******************************************************************************
  * @file util.c
  * @author  leon.xie
  * @version v1.0.0
  * @date 2018-10-18 21:49:21
  * @brief  This file provides all the util functions. 
  ******************************************************************************
  * @attention
  *
  * File For Yunin Software Team Only
  *
  * Copyright (C), 2017-2027, Yunin Software Team
  ******************************************************************************
  */ 

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
#include <ctype.h>

#include "init.h"

#include "util.h"
#include "log.h"
#include "file.h"

#ifdef  __cplusplus
extern "C" {
#endif

/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


int str2enum(const char *const*types,
				unsigned int ntypes,
				const char *type)
{
	size_t i;
	if (!type)
		return -1;

	for (i = 0; i < ntypes; i++){
		if (0 == strcmp(types[i], type))
			return i;
	}

	return -1;
}

const char * enum2str(const char *const*types,
						unsigned int ntypes,
						int type)
{
	if (type < 0 || type >= ntypes)
		return NULL;

	return types[type];
}


void skip_spaces(const char **str)
{
    const char *cur = *str;

    while (isspace(*cur))
        cur++;
    *str = cur;
}

void truncated_spaces(char *str)
{
    char *cur = str;
    while (!isspace(*cur))
        cur++;
    *cur=0;
}


void screen_clear(void)
{
#define SCREEN_CLEAR "\033[H""\033[J"
	file_write(STDOUT_FILENO,(void *)SCREEN_CLEAR,strlen(SCREEN_CLEAR));
}


#ifdef  __cplusplus
}
#endif


