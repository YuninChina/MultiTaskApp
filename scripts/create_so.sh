#!/bin/sh -e

source ./script/common.sh

######################################################################
# Parameter Verification   ${cross-compiler} ${so_name} ${dir}
######################################################################
if [ $# -lt 3 ];then
	ERR [$FUNCNAME-$LINENO] "usage: $0 [cross-compiler] [so_name] [dir]"
	exit 1
fi

LOG "[cross-compiler]: $1 , [so_name]: $2 , [dir]: $3"

# cross_compiler
cross_compiler=$1
so_name=$2
dir=$3

#link libs
LOG "link_libs: `cd ${dir} && ls *.a | sed 's/lib\(.*\)\.a/-l\1/' | xargs `"

${cross_compiler} -Wall -fPIC -rdynamic -Wstrict-prototypes -O2 -fomit-frame-pointer  \
-t -z defs -z muldefs -undefined -Bsymbolic -shared -L ${dir} \
-o ${dir}/${so_name} -Wl,-Bstatic -Wl,--whole-archive `cd ${dir} && ls *.a | sed 's/lib\(.*\)\.a/-l\1/' ` -Wl,--no-whole-archive \
-Wl,-Bdynamic -lpthread -ldl -lm
