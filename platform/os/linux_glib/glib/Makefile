
current_dir := ${CURDIR}/platform/os/linux_glib/glib

configure_zlib_args := --static --enable-shared --prefix=${CURDIR}/target

sub_target:
	@echo "[1;33m" ">>>>>>> Enter DIR:${current_dir}" "[0;39m"
	@echo "[1;33m" "++++ Make sub_target: zlib-1.2.11 " "[0;39m"
	${shell tar -zxf ${current_dir}/zlib-1.2.11.tar.gz -C ${current_dir}/ }
	cd ${current_dir}/zlib-1.2.11 && ./configure ${configure_zlib_args} && make && make install
	@echo "[1;32m" "make zlib-1.2.11 Succ!!!" "[0;39m"

	@echo "[1;33m" "++++ Make sub_target: libffi-3.2.1 " "[0;39m"
	
	@echo "[1;33m" "++++ Make sub_target: glib-2.40.2 " "[0;39m"


