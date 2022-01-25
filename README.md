# 多任务通讯系统.     
架构设计核心思想: 统一、抽象、下沉、上升、量化、闭环。     
统一：做到通用，标准化，如posix标准，高可移植性。   
抽象：将具体功能业务抽象化，减少依赖      
下沉：下沉组件，组件化。工具化、自动化。     
上升：上升业务逻辑，打薄业务层，专注于业务。    
量化：对系统性能、算法等能够量化分析。
闭环：软件闭环，逻辑闭环。     



> 嵌入式系统职能划分    
![image](https://note.youdao.com/yws/public/resource/53020d3e2dd5db8dc377d086688552e5/4BBF58B17FF04ABFADD990E7F0B74BB2?ynotemdtimestamp=1637413162344)

> 嵌入式系统架构   
![image](https://note.youdao.com/yws/public/resource/53020d3e2dd5db8dc377d086688552e5/9390BA86588D4B6AAE386BBB79C1BDD3?ynotemdtimestamp=1636789562673)

> 进程间通讯框架    
![image](https://note.youdao.com/yws/public/resource/53020d3e2dd5db8dc377d086688552e5/020D72D6CAD443FA84B916764DC84018?ynotemdtimestamp=1636902903987)

# 跨平台编译指令.
make distclean   
#####################################################################
# auto make x86
#####################################################################
make BUILD_FILE=build/ubuntu.make

#####################################################################
# auto make hi3518ev200 (for glibc)
#####################################################################
make BUILD_FILE=build/hi3518ev200.make

#####################################################################
# auto make hi3518ev200 (for glibc)
#####################################################################
make BUILD_FILE=build/jz2440.make


