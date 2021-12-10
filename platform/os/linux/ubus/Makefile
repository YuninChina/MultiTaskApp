
current_dir := ${CURDIR}/platform/os/linux/ubus

configure_jsonc_args := --enable-static --enable-shared  --host=${HOST_NAME} --prefix=${CURDIR}/target 
configure_jsonc_args += CFLAGS="${CFLAGS} -Drpl_realloc=realloc -Drpl_malloc=malloc"

configure_ubox_args := -DBUILD_LUA=OFF -DBUILD_EXAMPLES=OFF -DCMAKE_INSTALL_PREFIX=${CURDIR}/target 
configure_ubox_args += -DPKG_CONFIG_USE_CMAKE_PREFIX_PATH=${CURDIR}/target 
configure_ubox_args += -DCMAKE_C_FLAGS="${CFLAGS} -DJSONC -I ${CURDIR}/target/include/json-c"

configure_ubus_args := -DBUILD_LUA=OFF -DBUILD_EXAMPLES=OFF -DBUILD_STATIC=ON -DCMAKE_INSTALL_PREFIX=${CURDIR}/target 
configure_ubus_args += -DPKG_CONFIG_USE_CMAKE_PREFIX_PATH=${CURDIR}/target -DCMAKE_EXE_LINKER_FLAGS="${LD_LIBS}"

lib_name_jsonc := json-c
lib_name_ubox := libubox
lib_name_ubus := ubus

lib_name_suffix :=.tar.gz

sub_target:	
	$(call auto_make_install_compressed_lib_once,${current_dir},${lib_name_jsonc},$(lib_name_suffix),$(configure_jsonc_args))
	$(call auto_cmake_install_compressed_lib_once,${current_dir},${lib_name_ubox},$(lib_name_suffix),$(configure_ubox_args))
	$(call auto_cmake_install_compressed_lib_once,${current_dir},${lib_name_ubus},$(lib_name_suffix),$(configure_ubus_args))

