
SHELL=/bin/bash
TOP_DIR=$(shell TOP_DIR=Unable_To_Find_Top_Dir; \
				CUR_DIR=$$(pwd); \
				while [ "$$CUR_DIR" != "/" ]; \
				do { \
					if [ -a $$CUR_DIR/.git ]; then \
					TOP_DIR=$$CUR_DIR; \
					fi; \
				    CUR_DIR=$$(dirname $$CUR_DIR); } \
				done;\
				echo $$TOP_DIR)
$(info TOP_DIR = $(TOP_DIR))

MKFILE_PATH := $(abspath $(lastword $(MAKEFILE_LIST)))
ROOT_DIR := $(patsubst %/tests/Makefile, %, $(MKFILE_PATH))


$(info ROOT_DIR = $(ROOT_DIR))
$(info MKFILE_PATH = $(MKFILE_PATH))
INCS := -I$(ROOT_DIR)
CFLAGS := $(INCS)
CFLAGS += -Wall -g
CFLAGS += -MD -save-temps


##### C Source #####

C_SOURCES += sendRawEth.c
C_SOURCES += sockun_client.c
C_SOURCES += send_inband_cmd.c
C_SOURCES += inband_utils.c
##### OBJECTS #####
OBJECTS += $(patsubst %.c, %.o, $(C_SOURCES))

deps += $(OBJECTS:%.o=%.d)

all: binary $(C_SOURCES)

binary: sendRawEth sockun_client send_inband_cmd inband_utils

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

sendRawEth: $(OBJECTS)
	$(CC) sendRawEth.o -o $@ $(CFLAGS)
sockun_client: $(OBJECTS)
	$(CC) sockun_client.o -o $@ $(CFLAGS)
send_inband_cmd: $(OBJECTS)
	$(CC) send_inband_cmd.o -o $@ $(CFLAGS)
inband_utils: $(OBJECTS)
	$(CC) inband_utils.o -o $@ $(CFLAGS)

-include $(deps)

.PHONY: clean
clean:
	rm sendRawEth sockun_client send_inband_cmd inband_utils
	find . -name "*.o" -type f -delete
	find . -name "*.d" -type f -delete
	find . -name "*.i" -type f -delete
	find . -name "*.s" -type f -delete
