
obj-y :=

ifeq (${CONFIG_OS_LINUX},y)
	obj-y += linux/
else ifeq (${CONFIG_OS_LINUX_GLIB},y)
	obj-y += freertos/
else ifeq (${CONFIG_OS_FREERTOS},y)
	obj-y += freertos/
else ifeq (${CONFIG_OS_ROS},y)
	obj-y += ros/
else
	obj-y += linux_glib/
endif

