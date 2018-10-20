/**
  ******************************************************************************
  * @file util.h 
  * @author leon.xie
  * @version v1.0.0
  * @date 2018-10-20 13:18:42
  * @brief This file contains all the functions prototypes for the BOARD 
  *  util 
  ******************************************************************************
  * @attention
  *
  * File For Yunin Software Team Only
  *
  * Copyright (C), 2017-2027, Yunin Software Team
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __UTIL_H
#define __UTIL_H

/* Includes ------------------------------------------------------------------*/

#include "configs.h" 
#include "type.h" 

#ifdef  __cplusplus
extern "C" {
#endif

#ifndef MIN
#define MIN(x,y) ({ \
	typeof(x) _x = (x);	\
	typeof(y) _y = (y);	\
	(void) (&_x == &_y);		\
	_x < _y ? _x : _y; })
#endif

#ifndef MAX
#define MAX(x,y) ({ \
	typeof(x) _x = (x);	\
	typeof(y) _y = (y);	\
	(void) (&_x == &_y);		\
	_x > _y ? _x : _y; })
#endif

#define MIN_T(type,x,y) \
	({ type __x = (x); type __y = (y); __x < __y ? __x: __y; })
#define MAX_T(type,x,y) \
	({ type __x = (x); type __y = (y); __x > __y ? __x: __y; })
	

#undef ARRAY_CARDINALITY
#define ARRAY_CARDINALITY(Array) (sizeof(Array) / sizeof(*(Array)))
#undef VERIFY
#define VERIFY(name,cond)	static char name ## dummy [sizeof(struct { unsigned char dummy:((cond)?1:-1);})] __attribute__ ((unused))

#define ENUM_IMPL(name, lastVal, ...)                               \
	static const char *const name ## TypeList[] = { __VA_ARGS__ };		\
	VERIFY(name,(ARRAY_CARDINALITY(name ## TypeList) == lastVal)); 	\
	const char *name ## _type2str(int type) {						\
		return  enum2str(name ## TypeList,						\
							   ARRAY_CARDINALITY(name ## TypeList), 	\
							   type);									\
	}																	\
	int name ## _str2type(const char *type) {						\
		return str2enum(name ## TypeList,						\
								 ARRAY_CARDINALITY(name ## TypeList),	\
								 type); 								\
	}
							
#define ENUM_DECL(name)                             \
	const char *name ## _type2str(int type); 		\
	int name ## _str2type(const char*type);


//enum to string
#undef  E2S
#define E2S(e)	#e



//====================================================================
int  str2enum(const char *const*types,
				unsigned int ntypes,
				const char *type);

const char * enum2str(const char *const*types,
						unsigned int ntypes,
						int type);
void skip_spaces(const char **str);
void truncated_spaces(char *str);
void screen_clear(void);


#ifdef  __cplusplus
}
#endif

#endif  /* __UTIL_H */


