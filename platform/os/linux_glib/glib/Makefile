
current_dir := ${CURDIR}/platform/os/linux_glib/glib

configure_args := --static 
configure_args += --enable-shared

sub_target:
	@echo "[1;32m" "Make sub_target: Glib ; current_dir:${current_dir}" "[0;39m"
	${shell tar -zxf ${current_dir}/zlib-1.2.11.tar.gz -C ${current_dir}/ }
	cd ${current_dir}/zlib-1.2.11 && ./configure ${configure_args} && make

