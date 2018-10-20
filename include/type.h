/**
  ******************************************************************************
  * @file type.h 
  * @author leon.xie
  * @version v1.0.0
  * @date 2018-10-19 21:34:49
  * @brief This file contains all the functions prototypes for the BOARD 
  *  type 
  ******************************************************************************
  * @attention
  *
  * File For Yunin Software Team Only
  *
  * Copyright (C), 2017-2027, Yunin Software Team
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __TYPE_H
#define __TYPE_H

/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>

#ifdef  __cplusplus
extern "C" {
#endif

/* Exported typedef ----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#undef FAIL
#define FAIL -1

#undef SUCC
#define SUCC 0

#ifndef BOOL
#define BOOL bool
#endif

#undef TRUE
#define TRUE 1

#undef FALSE
#define FALSE 0

#define COLOUR_NONE		"\e[0m"
#define COLOUR_BLACK	"\e[0;30m"
#define COLOUR_L_BLACK	"\e[1;30m"
#define COLOUR_RED		"\e[0;31m"
#define COLOUR_L_RED	"\e[1;31m"
#define COLOUR_GREEN	"\e[0;32m"
#define COLOUR_L_GREEN	"\e[1;32m"
#define COLOUR_BROWN	"\e[0;33m"
#define COLOUR_YELLOW	"\e[1;33m"
#define COLOUR_BLUE		"\e[0;34m"
#define COLOUR_L_BLUE	"\e[1;34m"
#define COLOUR_PURPLE	"\e[0;35m"
#define COLOUR_L_PURPLE	"\e[1;35m"
#define COLOUR_CYAN		"\e[0;36m"
#define COLOUR_L_CYAN	"\e[1;36m"
#define COLOUR_GRAY		"\e[0;37m"
#define COLOUR_WHITE	"\e[1;37m"


typedef char S8;
typedef unsigned char U8;
typedef short S16;
typedef unsigned short U16;
typedef int S32;
typedef unsigned int U32;
typedef long L32;
typedef unsigned long UL32;
typedef signed long long L64;
typedef unsigned long long UL64;
typedef float F32;
typedef double D64;


/* Exported functions ------------------------------------------------------- */






#ifdef  __cplusplus
}
#endif

#endif  /* __TYPE_H */


