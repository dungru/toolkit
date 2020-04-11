CSRCS = snull_netdev.c

KERNEL_DIR = /lib/modules/$(shell uname -r)/build
BUILD_DIR := $(shell pwd)
VERBOSE   := 1

# Source name do not same as module name
obj-m := sn_netdev.o
sn_netdev-objs := $(CSRCS:.c=.o)

all:
	make -C $(KERNEL_DIR) SUBDIRS=$(BUILD_DIR) KBUILD_VERBOSE=$(VERBOSE) modules

clean:
	make -C $(KERNEL_DIR) SUBDIRS=$(BUILD_DIR) KBUILD_VERBOSE=$(VERBOSE) clean
	rm -f *.o *.ko *.mod.c *.symvers *.order .sn_netdev*
	rm -fr .tmp_versions