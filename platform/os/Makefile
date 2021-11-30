
obj-y :=

ifeq ($(strip $(CONFIG_OS)),linux)
	obj-y += linux/
else ifeq ($(strip $(CONFIG_OS)),freertos)
	obj-y += freertos/
else ifeq ($(strip $(CONFIG_OS)),linux_glib)
	obj-y += linux_glib/
else ifeq ($(strip $(CONFIG_OS)),ros)
	obj-y += ros/
else
	obj-y += linux/
endif

