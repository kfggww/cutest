PROJECT := cutest
VERSION := 0.1

build_dir := build
install_prefix ?= /usr

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

doc: all
	rm -rf Documentation
	doxygen Doxyfile

install: all doc
	install -D -m=0644 -t $(DESTDIR)/$(install_prefix)/include/cutest include/*.h
	install -D -m=0755 -t $(DESTDIR)/$(install_prefix)/lib/cutest $(libcutest)
	mkdir -p $(DESTDIR)/$(install_prefix)/share/doc/cutest
	cp -r Documentation/html $(DESTDIR)/$(install_prefix)/share/doc/cutest

clean:
	rm -rf $(objs) $(deps) $(libcutest) $(example-objs) $(example) Documentation build

.PHONY: all run_example doc install clean

-include $(deps)
