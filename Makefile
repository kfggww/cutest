
BUILD_SHARED := yes
DEBUG := yes

Q := @
SHELL := /bin/bash

SRC_DIR := src
TEST_SRC_DIR := test
BUILD_DIR := build
INSTALL_DIR := install

SRCS := $(shell find $(SRC_DIR) -name *.c)
TEST_SRCS := $(shell find $(TEST_SRC_DIR) -name *.c)

SRC_OBJS := $(addprefix $(BUILD_DIR)/, $(patsubst %.c, %.o, $(SRCS)))
TEST_SRC_OBJS := $(addprefix $(BUILD_DIR)/, $(patsubst %.c, %.o, $(TEST_SRCS)))

COMMON_INC := include
CUTEST_INC := $(COMMON_INC) $(shell find $(SRC_DIR) -type d)
TEST_INC := $(COMMON_INC) $(shell find $(TEST_SRC_DIR) -type d)

CFLAGS :=
ifeq ($(DEBUG), yes)
	CFLAGS += -g
endif

DEPFLAGS := -MM
DEPS := $(BUILD_DIR)/.deps

TEST_TARGETS := $(patsubst %.o, %, $(TEST_SRC_OBJS))

ifeq ($(BUILD_SHARED), yes)
CUTEST_TARGET := $(BUILD_DIR)/$(SRC_DIR)/libcutest.so
else
CUTEST_TARGET := $(BUILD_DIR)/$(SRC_DIR)/libcutest.a
endif

$(SRC_OBJS): CFLAGS += $(addprefix -I, $(CUTEST_INC))
$(TEST_SRC_OBJS): CFLAGS += $(addprefix -I, $(TEST_INC))

ifeq ($(BUILD_SHARED), yes)
$(CUTEST_TARGET): CFLAGS += -shared
$(SRC_OBJS): CFLAGS += -fPIC
endif

$(TEST_TARGETS): CFLAGS += -L$(dir $(CUTEST_TARGET)) -lcutest

all: cutest tests

cutest: $(CUTEST_TARGET)

tests: $(TEST_TARGETS)

install: all
	$(Q)mkdir -p $(INSTALL_DIR)/{lib,include}
	$(Q)cp $(COMMON_INC)/*.h $(INSTALL_DIR)/include
	$(Q)cp $(CUTEST_TARGET) $(INSTALL_DIR)/lib

clean:
	$(Q)rm -rf $(CUTEST_TARGET) $(TEST_TARGETS) $(SRC_OBJS) $(TEST_SRC_OBJS) $(DEPS)

.PHONY: all clean cutest tests install

$(CUTEST_TARGET): $(SRC_OBJS)
	$(Q)$(CC) $(CFLAGS) -o $@ $(SRC_OBJS)

$(TEST_TARGETS): $(TEST_SRC_OBJS)

$(BUILD_DIR)/%.o: %.c
	$(Q)mkdir -p $(dir $@)
	$(Q)$(CC) -c $(CFLAGS) $(DEPFLAGS) -MT $@ $< >> $(BUILD_DIR)/.deps
	$(Q)$(CC) -c $(CFLAGS) -o $@ $<

%: %.o
	$(Q)$(CC) $(CFLAGS) -o $@ $<

-include $(BUILD_DIR)/.deps