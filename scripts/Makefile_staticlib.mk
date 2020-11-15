SHELL=/bin/bash
TOP_DIR=$(shell TOP_DIR=Unable_To_Find_Top_Dir; \
				CUR_DIR=$$(pwd); \
				while [ "$$CUR_DIR" != "/" ]; \
				do { \
					if [ -a $$CUR_DIR/BOBI ]; then \
					TOP_DIR=$$CUR_DIR; \
					fi; \
				    CUR_DIR=$$(dirname $$CUR_DIR); } \
				done;\
				echo $$TOP_DIR)
$(info $(TOP_DIR))
$(info $(CURDIR))
BUILD_DIR := build_x86_64
OBJDIR:= $(CURDIR)/$(BUILD_DIR)
TARGET=libhw_ssvusb.a
#include $(TOP_DIR)/modules-path_var.mk
INCLUDE_DIRS := -I.
INCLUDE_DIRS += -I$(TOP_DIR)/kspace/include/


# Source code
C_SOURCES := $(TOP_DIR)/cmnlib/kspace_components/libhw_ssvusb.c

# OBJECTS HANDLE
OBJ_FILES = $(C_SOURCES:%.c=$(BUILD_DIR)/%.o)
#OBJ_FILES = $(patsubst %.c, $(BUILD_DIR)/%.o, $(C_SOURCES))

# DEPENDENT HANDLE
# DEPENDS = $(BUILD_DIR)/$(notdir $@).d  # METHOD 1
# DEP_FILES=$(wildcard $(BUILD_DIR)/*.d) # METHOD 1
# METHOD 2
DEP_FILES = $(OBJ_FILES:%.o=%.d)

# FLAGS HANDLE
CFLAGS = -Wall -g -fPIC -std=gnu99 -pedantic -W -Werror -D_GNU_SOURCE
CFLAGS += -Wno-unused-function
# For *.s and *.i
CFLAGS += -save-temps=obj

# AUTO_DEPENDS = -MMD -MF $(DEPENDS) # METHOD 1
AUTO_DEPENDS = -MMD -MF $(DEP_FILES)

CFLAGS += $(AUTO_DEPENDS)
-include ${DEP_FILES}

LDFLAGS =
${TARGET} : $(BUILD_DIR)/$(TARGET)

${BUILD_DIR}/${TARGET} : ${OBJ_FILES}
	-mkdir -p ${@D}
	$(AR) rcs $@ $^

${BUILD_DIR}/%.o : %.c
	-mkdir -p $(@D)
	$(CC) $(CFLAGS) ${INCLUDE_DIRS} -c $< -o $@
####
# all: $(TARGET) $(C_SOURCES)

# %.o: %.c %.h
# 	$(CC) -c $(CFLAGS) -o $@ $<

# $(TARGET): $(OBJECTS)
# 	$(AR) rcs $@ $^

# -include $(OBJECTS:.o=.d)
# test: $(TARGET) $(C_SOURCES)
# 	mkdir -p build
# 	cd build; cmake -S $(TOP_DIR)/modules/dcmbus/tests/ ;\
# 	make
# 	./build/runTests

#	g++ -Wall -g tests/tests_ringbuffer.c  $< -I. -o $@
#	./test
.PHONY : clean
clean:
	-rm -rf $(BUILD_DIR)
