PROJECT := cutest
VERSION := 0.1
ARCH ?= amd64

pwd := $(PWD)
build_dir := $(pwd)/build

CC := gcc

objs := src/cutest.o \
		src/json.o \
		src/option.o
objs := $(objs:%.o=$(build_dir)/%.o)
deps := $(objs:%.o=%.d)
libcutest = $(build_dir)/src/libcutest.so
example-objs := test/example.o
example-objs := $(example-objs:%.o=$(build_dir)/%.o)
example := $(build_dir)/test/example

CFLAGS := -Iinclude
$(objs): CFLAGS += -Isrc -fPIC
MAKEFLAGS += -rR

all: $(libcutest) $(example)

$(libcutest): $(objs)
	$(CC) $(CFLAGS) -shared -o $@ $^

$(example): $(example-objs) $(libcutest)
	$(CC) $(example-objs) -L$(build_dir)/src -lcutest -Wl,-rpath=$(build_dir)/src -o $(example)

$(build_dir)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -MM -MT $@ $< > $(@:%.o=%.d)
	$(CC) -c $(CFLAGS) -o $@ $<

run_example: $(example)
	$(example)

deb-pkg: $(libcutest)
	package=$(PROJECT) version=$(VERSION) arch=$(ARCH) build=$(build_dir) src=$(pwd) scripts/mkdebian

clean:
	rm -rf $(objs) $(deps) $(libcutest) $(example-objs) $(example)

.PHONY: all run_example clean

-include $(deps)