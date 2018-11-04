# LinuxMultitaskApp
> 基于Linux系统的应用程序,旨在搭建一套完整的多进程多线程通讯的消息框架. 支持多SOC的嵌入式APP.

# 1. 目录组织

> 目录组织是工程管理的第一步.

![image](https://github.com/YuninChina/LinuxMultitaskApp/blob/develop_local/docs/project.png)


# 2.项目架构图

> 从嵌入式Linux的整体角度去看框架. 

![image](https://github.com/YuninChina/LinuxMultitaskApp/blob/develop_local/docs/arch.png)


# 3. 进程/线程间通讯
> 通讯机制构成嵌入式系统多任务系统的交流骨架.

![image](https://github.com/YuninChina/LinuxMultitaskApp/blob/develop_local/docs/communication.png)


# 4. 程序段
> 段内容,分析段错误可能造成的因素.

![image](https://github.com/YuninChina/LinuxMultitaskApp/blob/develop_local/docs/segment.png)

# 5. CPU资源
> 嵌入式系统资源非常有限,CPU性能,进程/线程间调度,也是尤为重要.

![image](https://github.com/YuninChina/LinuxMultitaskApp/blob/develop_local/docs/cpu.png)

# 6. 模块
> 同层模块之间尽量避免直接交互,或接口调用.

![image](https://github.com/YuninChina/LinuxMultitaskApp/blob/develop_local/docs/module_callback.png)
