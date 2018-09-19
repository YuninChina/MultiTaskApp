# LinuxMultitaskApp
> 基于Linux系统的应用程序,旨在搭建一套完整的多进程多线程通讯的消息框架. 支持多SOC的嵌入式APP.

# 1. 目录组织
```
├── 3th				第三方库/文件
├── bsp				板级相关代码,与SOC/硬件强相关,提供多平台支持
├── configs		提供配置文件,对于不同的BOARD或者OEM/ODM客户提供不同的配置文件
├── daemon		守护进程,负责管理其他进程
├── include		全局头文件,只放置全局性的头文件
├── init			app入口
├── kernel		核心组件
├── module		模块化代码
├── script		Makefile/shell等相关编译脚本
├── service		业务逻辑相关任务代码
├── tests			测试代码
├── tools			工具集
└── util			纯功能相关代码
```


