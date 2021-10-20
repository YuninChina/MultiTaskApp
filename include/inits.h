
#ifndef __INITS_H
#define __INITS_H

#ifdef  __cplusplus
extern "C" {
#endif


typedef int (*initcall_t)(void);
typedef void (*exitcall_t)(void);

#define __define_pre_initcall(fn) \
	static initcall_t __pre_initcall_##fn __attribute__((__used__)) \
	__attribute__((__section__(".preinit_array"))) = fn

#define __define_initcall(level,fn,id) \
	static initcall_t __initcall_##fn##id __attribute__((__used__)) \
	__attribute__((__section__(".init_array." level ".init"))) = fn

#define __define_fini_initcall(fn) \
	static exitcall_t __fini_initcall_##fn __attribute__((__used__)) \
	__attribute__((__section__(".fini_array"))) = fn


#define earliest_init(x)	__define_pre_initcall(x)
#define latest_init(x)		__define_fini_initcall(x)

/************************************************************
 * pure->os->osa->core->hal->hala->module->service->app
 ************************************************************/
#define pure_initcall(fn)		__define_initcall("0",fn,0)
#define osa_initcall(fn)		__define_initcall("1",fn,1)
#define os_initcall(fn)			__define_initcall("2",fn,2)
#define core_initcall(fn)		__define_initcall("3",fn,3)
#define hala_initcall(fn)		__define_initcall("4",fn,4)
#define hal_initcall(fn)		__define_initcall("5",fn,5)
#define modulea_initcall(fn)	__define_initcall("6",fn,6)
#define module_initcall(fn)		__define_initcall("7",fn,7)
#define service_initcall(fn)	__define_initcall("8",fn,8)
#define app_initcall(fn)		__define_initcall("9",fn,9)

/***********************************************************/
#define pure_init(x)	pure_initcall(x)
#define core_init(x)	core_initcall(x)
#define osla_init(x)	osa_initcall(x)
#define core_init(x)	core_initcall(x)
#define hal_init(x)		hal_initcall(x)
#define module_init(x)	module_initcall(x)
#define modulea_init(x)	modulea_initcall(x)
#define service_init(x)	service_initcall(x)
#define app_init(x)		app_initcall(x)

#define module_exit(fn)	latest_init(fn)


#ifdef  __cplusplus
}
#endif

#endif  /* __INITS_H */


