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

BIN_IMAGE = quick-sort
MKFILE_PATH := $(abspath $(lastword $(MAKEFILE_LIST)))
ROOT_DIR := $(patsubst %/examples/Makefile, %, $(MKFILE_PATH))

$(info ROOT_DIR = $(ROOT_DIR))
$(info MKFILE_PATH = $(MKFILE_PATH))
###### C flags #####
CC = gcc
CFLAGS = -Wall -g -MD -save-temps
CFLAGS += -I./ \
	-I$(ROOT_DIR)/include \
	-I$(ROOT_DIR)/private

##### C Source #####

C_SOURCES += quick-sort.c
##### OBJECTS #####
OBJECTS += $(patsubst %.c, %.o, $(C_SOURCES))
all: $(BIN_IMAGE) $(C_SOURCES)

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

$(BIN_IMAGE): $(OBJECTS)
	$(CC) -Wall -g $(C_SOURCES) -o $@ $(CFLAGS)

-include $(OBJECTS:.o=.d)

.PHONY : clean
clean:
	rm -f $(BIN_IMAGE)
	find . -name "*.o" -type f -delete
	find . -name "*.d" -type f -delete
	find . -name "*.i" -type f -delete