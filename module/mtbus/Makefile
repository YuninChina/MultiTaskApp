
obj-y :=
obj-y += mtbus.o

ifeq ($(CONFIG_MTBUS_DBUS),y)
	obj-y += dbus/
else ifeq ($(CONFIG_MTBUS_UBUS),y)
	obj-y += ubus/
else
	obj-y += 
endif


