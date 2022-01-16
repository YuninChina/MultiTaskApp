# 多任务通讯系统.

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


