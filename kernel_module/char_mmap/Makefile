SRCS = char_ks2us.c

mmap_test-objs := $(SRCS:.c=.o)

obj-m := char_ks2us.o

KERNSRC := /lib/modules/$(shell uname -r)/build

all: 
	make -C $(KERNSRC) M=$(PWD) modules
clean: 
	make -C $(KERNSRC) M=$(PWD) clean