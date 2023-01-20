CFLAGS := -I include

build_dir := build

lib_cutest_srcs := $(wildcard src/*.c)
lib_cutest := $(build_dir)/libcutest.so

$(lib_cutest): CFLAGS +=  --shared -fPIC

cutest_test_srcs := $(wildcard test/*.c)
cutest_test_exes := $(addprefix $(build_dir)/, $(patsubst %.c, %, $(cutest_test_srcs)))

all: $(lib_cutest) $(cutest_test_exes)

create_build_dir:
	mkdir -p $(build_dir)/test

$(lib_cutest): $(lib_cutest_srcs) create_build_dir
	$(CC) $(CFLAGS) -o $@ $(lib_cutest_srcs)

$(cutest_test_exes): $(libcutest)

$(build_dir)/test/%: test/%.c
	$(CC) $(CFLAGS) -L $(build_dir) -lcutest $< -o $@

clean:
	rm -rf $(lib_cutest) $(cutest_test_exes)

.PHONY: all clean create_build_dir