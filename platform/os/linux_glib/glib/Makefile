
current_dir := ${CURDIR}/platform/os/linux_glib/glib

configure_args := --static 
configure_args += --enable-shared

sub_target:
	@echo "[1;33m" ">>>>>>> Enter DIR:${current_dir}" "[0;39m"
	@echo "[1;33m" "Make sub_target: zlib-1.2.11 " "[0;39m"
	${shell tar -zxf ${current_dir}/zlib-1.2.11.tar.gz -C ${current_dir}/ }
	cd ${current_dir}/zlib-1.2.11 && ./configure ${configure_args} && make
	cp ${current_dir}/zlib-1.2.11/libz.a ${CURDIR}/libs
	@echo "[1;32m" "cp ${current_dir}/zlib-1.2.11/libz.a ${CURDIR}/libs" "[0;39m"

	@echo "[1;33m" "Make sub_target: libffi " "[0;39m"


