CRAZYFLIE_BASE := ../crazyflie-firmware/
#
# We override the default OOT_CONFIG here, we could also name
# our config to oot-config and that would be the default.
#
OOT_CONFIG := $(PWD)/config
# To include C file
obj-y += src/myapp.o
# To add some includes
EXTRA_CFLAGS += -I$(PWD)/src/includes
include $(CRAZYFLIE_BASE)/tools/make/oot.mk