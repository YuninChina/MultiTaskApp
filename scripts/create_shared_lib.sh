#!/bin/sh

# ANSI COLORS
COLOR_CRE="[K"
COLOR_NORMAL="[0;39m"
COLOR_RED="[1;31m"
COLOR_GREEN="[1;32m"
COLOR_YELLOW="[1;33m"
COLOR_BLUE="[1;34m"
COLOR_MAGENTA="[1;35m"
COLOR_CYAN="[1;36m"
COLOR_WHITE="[1;37m"

#$1: string
#$2: color
ECHO()
{
	[ -n "$2" ] && eval echo -n \"\${${2}}\";
	echo "${1}${COLOR_NORMAL}"
}

#$1 FUNC-LINE
#$2 string
ERR()
{
	echo "${COLOR_RED} ERR${1}: ${2}${COLOR_NORMAL}" >&2
}

#$1 FUNC-LINE
#$2 string
WARN()
{
	echo "${COLOR_YELLOW}WARN${1}: ${2}${COLOR_NORMAL}" >&2
}

#$1 FUNC-LINE
#$2 string
CMD()
{
	echo "${COLOR_GREEN} CMD${1}: ${2}${COLOR_NORMAL}" >&2
}

# $1: string
LOG()
{
	echo "${COLOR_CYAN} LOG: ${1}${COLOR_NORMAL}" >&2
}


#$1: string
#$2: level
DEBUG()
{
	local level=$2
	[ -z "$level" ] && { level=0; }
	[ $level -lt $DEBUG_LOG_LEVEL ] && return 0;

	echo "$COLOR_WHITE$1$COLOR_NORMAL" > $DEBUG_LOG_FILE
}

CUR_DIR=$PWD

if [ $# -lt 5 ];then
	ERR [$FUNCNAME-$LINENO] "usage: $0 [cross_compiler] [shared_lib_name] [root_path] [static_librarys ...]"
	CMD [$FUNCNAME-$LINENO] "soc options: [x86/x64/hi3518ev200]"
	CMD [$FUNCNAME-$LINENO] "cross_compiler options: [gcc/g++/arm-hisiv400-linux-gcc/arm-hisiv400-linux-g++]"
	CMD [$FUNCNAME-$LINENO] "shared_lib_name options: [test.so]"
	CMD [$FUNCNAME-$LINENO] "root_path options: [/home/leon/install]"
	CMD [$FUNCNAME-$LINENO] "static_librarys options: [libtest.a/libglib-2.0.a]"
	exit 1
fi

soc=$1
cross_compiler=$2
shared_lib_name=$3
root_path=$4

#
static_libs="$(until [ -z "$5" ]; do echo "$5" ; shift ;done)"
static_libs="`echo ${static_libs} | xargs`"
link_staticlibs=`for i in ${static_libs};do echo $i | sed 's/.*\///g' | sed 's/lib\(.*\)\.a/\-l\1/g' ;done  | xargs`

CMD [$FUNCNAME-$LINENO] "$0 ${cross_compiler} ${shared_lib_name} ${root_path} ${link_staticlibs} "

cflags="-rdynamic -fPIC -pipe -O3 -Wall "
ld_flags="${new_symbols} -t -z defs -z muldefs -undefined -Bsymbolic -shared "
link_static="-Wl,-Bstatic -Wl,--whole-archive"
link_shared="-Wl,--no-whole-archive -Wl,-Bdynamic"


#dynamic library
if [[ "$soc" == "x86" ]] ; then
	link_sharedlibs="-lgtk-3 -lgdk-3 -lgdk_pixbuf-2.0 -lcairo -lgstreamer-1.0 -lgstapp-1.0 -lgstriff-1.0 -lgstpbutils-1.0 "
	link_sharedlibs="${link_sharedlibs}  -lpng -lpthread -ldl -lresolv -lz -lstdc++"
elif [[ "$soc" == "x64" ]] ; then
	link_sharedlibs="-lpthread -ldl -lresolv"
else
	link_sharedlibs="-lpthread -ldl -lresolv"
fi
######################################################################
# fuctions
######################################################################
#
do_create_shared_lib()
{

	# Compiled dynamic linked library
	CMD [$FUNCNAME-$LINENO] "${cross_compiler} ${cflags} ${ld_flags} -o ${shared_lib_name} -L ${root_path}/lib ${link_static} ${link_staticlibs} ${link_shared} ${link_sharedlibs}"
	${cross_compiler} ${cflags} ${ld_flags} -o ${shared_lib_name} -L ${root_path}/lib ${link_static} ${link_staticlibs} ${link_shared} ${link_sharedlibs}
}

do_create_shared_lib
