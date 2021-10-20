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

if [ $# -lt 1 ];then
	ERR [$FUNCNAME-$LINENO] "usage: $0 [soc]"
	CMD [$FUNCNAME-$LINENO] "soc options: [x86]"
	exit 1
fi

soc=$1
LOG "soc : $soc"


if [[ "$soc" == "x86" ]] ; then
	cross_compiler="g++"
	shared_lib_name="./3th/lib/lib3th-${soc}.so"
	root_path="./3th/lib/x86/${soc}"
	static_librarys="libgio-2.0.a libgmodule-2.0.a libgobject-2.0.a libgthread-2.0.a libglib-2.0.a libffi.a libdbus-1.a libexpat.a"
	static_librarys="${static_librarys} libIlmImf.a libittnotify.a liblibjasper.a liblibjpeg.a liblibprotobuf.a liblibtiff.a liblibwebp.a libopencv_calib3d.a "
	static_librarys="${static_librarys} libopencv_calib3d_pch_dephelp.a libopencv_core.a libopencv_core_pch_dephelp.a libopencv_dnn.a libopencv_dnn_pch_dephelp.a libopencv_features2d.a libopencv_features2d_pch_dephelp.a "
	static_librarys="${static_librarys} libopencv_flann.a libopencv_flann_pch_dephelp.a libopencv_highgui.a libopencv_highgui_pch_dephelp.a libopencv_imgcodecs.a libopencv_imgcodecs_pch_dephelp.a libopencv_imgproc.a "
	static_librarys="${static_librarys} libopencv_imgproc_pch_dephelp.a libopencv_ml.a libopencv_ml_pch_dephelp.a libopencv_objdetect.a libopencv_objdetect_pch_dephelp.a libopencv_perf_calib3d_pch_dephelp.a "
	static_librarys="${static_librarys} libopencv_perf_core_pch_dephelp.a libopencv_perf_dnn_pch_dephelp.a libopencv_perf_features2d_pch_dephelp.a libopencv_perf_imgcodecs_pch_dephelp.a libopencv_perf_imgproc_pch_dephelp.a "
	static_librarys="${static_librarys} libopencv_perf_objdetect_pch_dephelp.a libopencv_perf_photo_pch_dephelp.a libopencv_perf_stitching_pch_dephelp.a libopencv_perf_superres_pch_dephelp.a libopencv_perf_videoio_pch_dephelp.a "
	static_librarys="${static_librarys} libopencv_perf_video_pch_dephelp.a libopencv_photo.a libopencv_photo_pch_dephelp.a libopencv_shape.a libopencv_shape_pch_dephelp.a libopencv_stitching.a libopencv_stitching_pch_dephelp.a "
	static_librarys="${static_librarys} libopencv_superres.a libopencv_superres_pch_dephelp.a libopencv_test_calib3d_pch_dephelp.a libopencv_test_core_pch_dephelp.a libopencv_test_dnn_pch_dephelp.a libopencv_test_features2d_pch_dephelp.a "
	static_librarys="${static_librarys} libopencv_test_flann_pch_dephelp.a libopencv_test_highgui_pch_dephelp.a libopencv_test_imgcodecs_pch_dephelp.a libopencv_test_imgproc_pch_dephelp.a libopencv_test_ml_pch_dephelp.a "
	static_librarys="${static_librarys} libopencv_test_objdetect_pch_dephelp.a libopencv_test_photo_pch_dephelp.a libopencv_test_shape_pch_dephelp.a libopencv_test_stitching_pch_dephelp.a libopencv_test_superres_pch_dephelp.a "
	static_librarys="${static_librarys} libopencv_test_videoio_pch_dephelp.a libopencv_test_video_pch_dephelp.a libopencv_test_videostab_pch_dephelp.a libopencv_ts.a libopencv_ts_pch_dephelp.a libopencv_video.a libopencv_videoio.a "
	static_librarys="${static_librarys} libopencv_videoio_pch_dephelp.a libopencv_video_pch_dephelp.a libopencv_videostab.a libopencv_videostab_pch_dephelp.a "

elif [[ "$soc" == "x64" ]] ; then
	cross_compiler="g++"
	shared_lib_name="./3th/lib/lib3th-${soc}.so"
	root_path="./3th/lib/x64/${soc}"
	static_librarys="libgio-2.0.a libgmodule-2.0.a libgobject-2.0.a libgthread-2.0.a libglib-2.0.a libffi.a libdbus-1.a libexpat.a libz.a"
elif [[ "$soc" == "hi3518ev200" ]] ; then
	cross_compiler=
	shared_lib_name=
	root_path=
	static_librarys=
	ERR [$FUNCNAME-$LINENO] "soc: $soc unsupport..."
	exit 1	
else
	ERR [$FUNCNAME-$LINENO] "soc: $soc unsupport..."
	exit 1
fi


######################################################################
# fuctions
######################################################################
#
do_create_3th_shared_lib()
{
	LOG "./script/create_shared_lib.sh ${soc} ${cross_compiler} ${shared_lib_name} ${root_path} ${static_librarys}"

	./script/create_shared_lib.sh ${soc} ${cross_compiler} ${shared_lib_name} ${root_path} ${static_librarys}
}

do_create_3th_shared_lib


