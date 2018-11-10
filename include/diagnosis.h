/**
  ******************************************************************************
  * @file diagnosis.h 
  * @author leon.xie
  * @version v1.0.0
  * @date 2018-10-19 21:37:59
  * @brief This file contains all the functions prototypes for the BOARD 
  *  diagnosis 
  ******************************************************************************
  * @attention
  *
  * File For Yunin Software Team Only
  *
  * Copyright (C), 2017-2027, Yunin Software Team
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __DIAGNOSIS_H
#define __DIAGNOSIS_H

/* Includes ------------------------------------------------------------------*/

#include "type.h" 

#ifdef  __cplusplus
extern "C" {
#endif

/* Exported typedef ----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
extern void log_assert(const char *source
   ,const char *expr
   ,const char *filename
   ,int linenr
   ,const char *fmt, ...);


#undef ASSERT
#define ASSERT(expr) if(!(expr)) log_assert(NULL,#expr,__FILE__, __LINE__,NULL)

#undef ASSERT_ERRNO
#define ASSERT_ERRNO(expr,errno) if(!(expr)) log_assert(NULL,#expr,__FILE__, __LINE__,"with errno: %f(%d,%x)",(float)errno,errno,errno)



#undef RETURN_IF_FAIL
#define RETURN_IF_FAIL(expr)  ({\
	if (!(expr))\
	{\
		log_assert(NULL,#expr,__FILE__, __LINE__,NULL);\
		return;\
	}\
})

#undef RETURN_IF_FAIL_ARGS
#define RETURN_IF_FAIL_ARGS(expr,args...)  ({\
	if (!(expr))\
	{\
		log_assert(NULL,#expr,__FILE__, __LINE__,args);\
		return;\
	}\
})


#undef RETURN_VAL_IF_FAIL
#define RETURN_VAL_IF_FAIL(expr,return_val)  ({\
	if (!(expr))\
	{\
		log_assert(NULL,#expr,__FILE__, __LINE__,NULL);\
		return return_val;\
	}\
})


#undef RETURN_VAL_IF_FAIL_ARGS
#define RETURN_VAL_IF_FAIL_ARGS(expr,return_val,args...)  ({\
	if (!(expr))\
	{\
		log_assert(NULL,#expr,__FILE__, __LINE__,args);\
		return return_val;\
	}\
})


#undef GOTO_LABEL_IF_FAIL
#define GOTO_LABEL_IF_FAIL(expr,label)  ({\
	if (!(expr))\
	{\
		log_assert(NULL,#expr,__FILE__, __LINE__,NULL);\
		goto label;\
	}\
})


#undef GOTO_LABEL_IF_FAIL_ARGS
#define GOTO_LABEL_IF_FAIL_ARGS(expr,label,args...)  ({\
	if (!(expr))\
	{\
		log_assert(NULL,#expr,__FILE__, __LINE__,args);\
		goto label;\
	}\
})


#define DIAGNO_STR_EINVAL 		"Invalid argument"
#define DIAGNO_STR_EUNINIT 		"Uninitialized"
#define DIAGNO_STR_EMISUSE 		"Misuse"
#define DIAGNO_STR_ENOMEM 		"Out of memory"
#define DIAGNO_STR_EFAULT 		"Bad address"
#define DIAGNO_STR_EAGAIN 		"Try again"

#ifdef  __cplusplus
}
#endif

#endif  /* __DIAGNOSIS_H */


