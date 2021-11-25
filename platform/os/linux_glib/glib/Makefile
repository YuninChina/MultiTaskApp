
current_dir := ${CURDIR}/platform/os/linux_glib/glib

sub_target:
	@echo "[1;31m" "Make sub_target: Glib ; current_dir:${current_dir}" "[0;39m"
	${shell tar -zxf ${current_dir}/zlib-1.2.11.tar.gz -C ${current_dir}/ }
	cd ${current_dir}/zlib-1.2.11 && ./configure && make

