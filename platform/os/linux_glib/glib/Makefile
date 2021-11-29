
current_dir := ${CURDIR}/platform/os/linux_glib/glib

configure_zlib_args := --static --enable-shared --prefix=${CURDIR}/target

configure_libffi_args := --enable-static --host=${HOST_NAME} --prefix=${CURDIR}/target LDFLAGS=""

configure_glib_args := --enable-silent-rules --enable-static --enable-shared --with-pcre=internal --disable-gtk-doc-html --disable-man 
configure_glib_args += glib_cv_stack_grows=no glib_cv_uscore=no ac_cv_func_posix_getpwuid_r=no ac_cv_func_posix_getgrgid_r=no
configure_glib_args += --host=${HOST_NAME} --prefix=${CURDIR}/target  CFLAGS="${CFLAGS} -Wno-format-nonliteral -Wno-format-overflow -Wno-format-security"

lib_name_zlib := zlib-1.2.11
lib_name_libffi := libffi-3.2.1
lib_name_glib := glib-2.40.2
lib_name_suffix :=.tar.gz

sub_target:	
	$(call auto_make_install_compressed_lib,${current_dir},${lib_name_zlib},$(lib_name_suffix),$(configure_zlib_args))
	$(call auto_make_install_compressed_lib,${current_dir},${lib_name_libffi},$(lib_name_suffix),$(configure_libffi_args))
	$(call auto_make_compressed_lib,${current_dir},${lib_name_glib},$(lib_name_suffix),$(configure_glib_args))
	$(call auto_install_lib,${current_dir},${lib_name_glib},$(lib_name_suffix),$(configure_glib_args))


