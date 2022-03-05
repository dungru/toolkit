SRCS = simple_pcie.c

mmap_test-objs := $(SRCS:.c=.o)

obj-m := simple_pcie.o

KERNSRC := /lib/modules/$(shell uname -r)/build

all: 
	make -C $(KERNSRC) M=$(PWD) modules
clean: 
	make -C $(KERNSRC) M=$(PWD) clean