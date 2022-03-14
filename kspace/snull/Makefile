
KERNEL_DIR ?= /lib/modules/$(shell uname -r)/build
PWD := $(CURDIR)
VERBOSE   ?= 0

C_SOURCES = snull_main.c
# Source name do not same as module name
obj-m := snull.o
snull-objs := $(C_SOURCES:.c=.o)

ccflags-y := -std=gnu99 -Wno-declaration-after-statement

all:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) clean
	rm -f *.o *.ko *.mod.c *.symvers *.order .snull*
	rm -fr .tmp_versions

depend .depend dep:
	$(CC) $(EXTRA_CFLAGS) -M *.c > .depend

ifeq (.depend,$(wildcard .depend))
include .depend
endif