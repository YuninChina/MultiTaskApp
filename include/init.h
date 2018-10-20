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
#define primary_initcall(fn)	__define_initcall("2",fn,2)
#define bsp_initcall(fn)		__define_initcall("3",fn,3)
#define subsys_initcall(fn)		__define_initcall("4",fn,4)
#define module_initcall(fn)		__define_initcall("5",fn,5)
#define service_initcall(fn)	__define_initcall("6",fn,6)
#define late_initcall(fn)		__define_initcall("7",fn,7)

#define __purecall(fn) 		pure_initcall(fn)
#define __corecall(fn) 		core_initcall(fn)
#define __primarycall(fn) 	primary_initcall(fn)
#define __bspcall(fn) 		bsp_initcall(fn)
#define __subsyscall(fn) 	subsys_initcall(fn)
#define __modulecall(fn) 	module_initcall(fn)
#define __servicecall(fn) 	service_initcall(fn)
#define __latecall(fn) 		late_initcall(fn)

#define __exitcall(fn) \
	static exitcall_t __exitcall_##fn __exit_call = fn

//
#define pures_init(x)	__purecall(x)
#define pures_exit(x)	__exitcall(x)

//核心模块初始化
#define cores_init(x)	__corecall(x)
#define cores_exit(x)	__exitcall(x)

//初级模块初始化
#define primarys_init(x)	__primarycall(x)
#define primarys_exit(x)	__exitcall(x)

//BSP模块初始化
#define bsps_init(x)	__bspcall(x)
#define bsps_exit(x)	__exitcall(x)

//子系统模块初始化
#define subsys_init(x)	__subsyscall(x)
#define cores_exit(x)	__exitcall(x)

// 模块初始化
#define modules_init(x)	__modulecall(x)
#define modules_exit(x)	__exitcall(x)

//业务初始化
#define services_init(x)	__servicecall(x)
#define services_exit(x)	__exitcall(x)

//
#define lates_init(x)	__latecall(x)
#define lates_exit(x)	__exitcall(x)


#ifdef  __cplusplus
}
#endif

#endif  /* __INIT_H */


