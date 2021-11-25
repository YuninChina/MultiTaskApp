
current_dir := ${CURDIR}/platform/os/linux_glib/glib

configure_zlib_args := --static --enable-shared --prefix=${CURDIR}/target

configure_libffi_args := --enable-silent-rules --enable-static --enable-shared --host=${HOST_NAME} --prefix=${CURDIR}/target

configure_glib_args := --enable-silent-rules --enable-static --enable-shared --with-pcre=internal --disable-gtk-doc-html --disable-man 
configure_glib_args += "glib_cv_stack_grows=no glib_cv_uscore=no ac_cv_func_posix_getpwuid_r=no ac_cv_func_posix_getgrgid_r=no"
configure_glib_args += --host=${HOST_NAME} --prefix=${CURDIR}/target 


lib_name_zlib := zlib-1.2.11
lib_name_libffi := libffi-3.2.1
lib_name_glib := glib-2.40.2

sub_target:
	@echo "[1;33m" ">>>>>>> Enter DIR:${current_dir}" "[0;39m"
#	@echo "[1;33m" "++++ Make sub_target: ${lib_name_zlib} " "[0;39m"
#	${shell tar -zxf ${current_dir}/${lib_name_zlib}.tar.gz -C ${current_dir}/ }
#	cd ${current_dir}/${lib_name_zlib} && ./configure ${configure_zlib_args} && make && make install
#	@echo "[1;32m" "make ${lib_name_zlib} Succ!!!" "[0;39m"

#	@echo "[1;33m" "++++ Make sub_target: ${lib_name_libffi} " "[0;39m"
#	${shell tar -zxf ${current_dir}/${lib_name_libffi}.tar.gz -C ${current_dir}/ }
#	cd ${current_dir}/${lib_name_libffi} && ./configure ${configure_libffi_args} && make && make install
#	@echo "[1;32m" "make ${lib_name_libffi} Succ!!!" "[0;39m"
	
	@echo "[1;33m" "++++ Make sub_target: ${lib_name_glib} " "[0;39m"
	${shell tar -zxf ${current_dir}/${lib_name_glib}.tar.gz -C ${current_dir}/ }
	cd ${current_dir}/${lib_name_glib} && ./configure ${configure_glib_args} && make && make install
	@echo "[1;32m" "make ${lib_name_glib} Succ!!!" "[0;39m"


