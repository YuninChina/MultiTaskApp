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

#undef RETURN_IF_FAIL_WITH_ERRNO
#define RETURN_IF_FAIL_WITH_ERRNO(expr,errno)  ({\
	if (!(expr))\
	{\
		log_assert(NULL,#expr,__FILE__, __LINE__,"with errno: %f(%d,%x)",(float)errno,errno,errno);\
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


#undef RETURN_VAL_IF_FAIL_WITH_ERRNO
#define RETURN_VAL_IF_FAIL_WITH_ERRNO(expr,return_val,errno)  ({\
	if (!(expr))\
	{\
		log_assert(NULL,#expr,__FILE__, __LINE__,"with errno: %f(%d,%x)",(float)errno,errno,errno);\
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


#undef GOTO_LABEL_IF_FAIL_WITH_ERRNO
#define GOTO_LABEL_IF_FAIL_WITH_ERRNO(expr,label,errno)  ({\
	if (!(expr))\
	{\
		log_assert(NULL,#expr,__FILE__, __LINE__,"with errno: %f(%d,%x)",(float)errno,errno,errno);\
		goto label;\
	}\
})



#ifdef  __cplusplus
}
#endif

#endif  /* __DIAGNOSIS_H */


