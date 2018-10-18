/**
  ******************************************************************************
  * @file init.h 
  * @author leon.xie
  * @version v1.0.0
  * @date 2018-10-18 21:45:46
  * @brief This file contains all the functions prototypes for the BOARD 
  *  init 
  ******************************************************************************
  * @attention
  *
  * File For Yunin Software Team Only
  *
  * Copyright (C), 2017-2027, Yunin Software Team
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __INIT_H
#define __INIT_H

/* Includes ------------------------------------------------------------------*/

#include "type.h" 

#ifdef  __cplusplus
extern "C" {
#endif


#ifndef __attribute_used__
#define __attribute_used__	/* deprecated */
#endif

#ifndef __cold
#define __cold			__attribute__((__cold__))
#endif
/* These are for everybody (although not all archs will actually
   discard it in modules) */
#define __init		__attribute__ ((__section__ (".init.text"))) __cold
#define __initdata	__attribute__ ((__section__ (".init.data")))
#define __exitdata	__attribute__ ((__section__(".exit.data")))
#define __exit_call	__attribute_used__ __attribute__ ((__section__ (".exitcall.exit")))


/*
 * Used for initialization calls..
 */
typedef int (*initcall_t)(void);
typedef void (*exitcall_t)(void);


/* These are for everybody (although not all archs will actually
   discard it in modules) */
#define __init		__attribute__ ((__section__ (".init.text"))) __cold
#define __initdata	__attribute__ ((__section__ (".init.data")))
#define __exitdata	__attribute__ ((__section__(".exit.data")))
#define __exit_call	__attribute_used__ __attribute__ ((__section__ (".exitcall.exit")))


#define __define_initcall(level,fn,id) \
	static initcall_t __initcall_##fn##id __attribute_used__ \
	__attribute__((__section__(".initcall" level ".init"))) = fn

/*
 * A "pure" initcall has no dependencies on anything else, and purely
 * initializes variables that couldn't be statically initialized.
 *
 * This only exists for built-in code, not for modules.
 */
#define pure_initcall(fn)		__define_initcall("0",fn,0)

#define core_initcall(fn)		__define_initcall("1",fn,1)
#define core_initcall_sync(fn)		__define_initcall("1s",fn,1s)
#define postcore_initcall(fn)		__define_initcall("2",fn,2)
#define postcore_initcall_sync(fn)	__define_initcall("2s",fn,2s)
#define arch_initcall(fn)		__define_initcall("3",fn,3)
#define arch_initcall_sync(fn)		__define_initcall("3s",fn,3s)
#define subsys_initcall(fn)		__define_initcall("4",fn,4)
#define subsys_initcall_sync(fn)	__define_initcall("4s",fn,4s)
#define fs_initcall(fn)			__define_initcall("5",fn,5)
#define fs_initcall_sync(fn)		__define_initcall("5s",fn,5s)
#define module_initcall(fn)		__define_initcall("6",fn,6)
#define module_initcall_sync(fn)	__define_initcall("6s",fn,6s)
#define late_initcall(fn)		__define_initcall("7",fn,7)
#define late_initcall_sync(fn)		__define_initcall("7s",fn,7s)

#define __initcall(fn) module_initcall(fn)
#define __servicecall(fn) late_initcall(fn)

#define __exitcall(fn) \
	static exitcall_t __exitcall_##fn __exit_call = fn


// 模块初始化
#define modules_init(x)	__initcall(x);
#define modules_exit(x)	__exitcall(x);
//业务初始化
#define services_init(x)	__servicecall(x);
#define services_exit(x)	__exitcall(x);


#ifdef  __cplusplus
}
#endif

#endif  /* __INIT_H */


