#!/bin/sh

#set -e

# for debug
DEBUG_LOG_FILE='&2'
DEBUG_LOG_LEVEL=0

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

# Shell command
TAR=tar
CP=/bin/cp
RM=/bin/rm
GREP=grep
SED=sed
MKDIR=mkdir
CHMOD=chmod
MV=mv
CD=cd
LN=ln
MAKE=make
MKNOD=mknod
PUSHD=pushd 
POPD=popd 
RMDIR=rmdir
DEPMOD=/sbin/depmod
RMDIR=rmdir
MKIMG=mkimage
PATCH=patch
DIFF=diff
TOUCH=touch
CAT=cat
PWD=`pwd`



#unset linux environment variable
ENV_VAR_UNSET()
{
	# cross compile
	unset CC
	unset AR
	unset RANLIB
	unset CFLAGS
	unset LDFLAGS
	unset CROSS_COMPILE
	unset PROCESSOR

	# path
	unset DESTDIR
	unset LIBDIR
	unset INCDIR
	unset BINDIR
	unset LIBPATH
	unset INCPATH
	unset BINPATH
	unset DATAPATH
	unset PREFIX
	
	#misc
	unset PKG_CONFIG
	unset PKG_CONFIG_PATH
}

e_blank='[        ][      ]*'
e_year='20[0-9][0-9]'
e_month='([1-9]|0[1-9]|1[0-2])'
e_day='([1-9]|0[1-9]|[12][0-9]|3[0-1])'
e_time='([01][0-9]|2[0-3]):[0-5][0-9]'
e_employid='[a-zA-Z][a-zA-Z]*[0-9]{4,}'

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


#$1: self script name
dependencies_info()
{
	ERR [$FUNCNAME-$LINENO] "usage: $1 [language] [action] [arch] [soc] [dirname] [clibs]"
	CMD [$FUNCNAME-$LINENO] "language options: [c/cpp]"
	CMD [$FUNCNAME-$LINENO] "action options: [make/clean/install/uninstall]"
	CMD [$FUNCNAME-$LINENO] "arch options: [arm/x86/mips]"
	CMD [$FUNCNAME-$LINENO] "soc options: [goke/hi3518e/hi3518ev200/xilink]"
	CMD [$FUNCNAME-$LINENO] "clibs options: [uclibc/glibc]"
}


#$1: language
#$2: action
#$3: arch
#$4: soc
#$5: clibs
dependencies_assert()
{
	if [[ "$1" != "c" ]] \
	&& [[ "$1" != "cpp" ]] ;then
		 ERR [$FUNCNAME-$LINENO] "language: $1 unsupport..."
		 exit 1
	fi

	if [ "$2" != "make" ] \
	&& [ "$2" != "clean" ] \
	&& [ "$2" != "install" ] \
	&& [ "$2" != "uninstall" ] ;then
		 ERR [$FUNCNAME-$LINENO] "action: $2 unsupport..."
		 exit 1
	fi	

	if [ "$3" != "arm" ] \
	&& [ "$3" != "x86" ] \
	&& [ "$3" != "mips" ] ;then
		 ERR [$FUNCNAME-$LINENO] "arch: $3 unsupport..."
		 exit 1
	fi	
	
	if [[ "$4" != "goke" ]] \
	&& [[ "$4" != "hi3518e" ]] \
	&& [[ "$4" != "hi3518ev200" ]] \
	&& [[ "$4" != "ingenic_t31" ]] \
	&& [[ "$4" != "x86" ]] \
	&& [[ "$4" != "xilink" ]] ;then
		ERR [$FUNCNAME-$LINENO] "soc: $4 unsupport..."
		exit 1	 
	fi
	
	if [ "$5" != "uclibc" ] \
	&& [ "$5" != "glibc" ] ;then
		 ERR [$FUNCNAME-$LINENO] "clibs: $5 unsupport..."
		 exit 1
	fi
	
}