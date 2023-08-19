# cutest

![ubuntu-22.04](https://github.com/kfggww/cutest/actions/workflows/ci.yml/badge.svg)

## Introduction

A simple unit test framework for c programming language. It supports the following features:

- [x] Declare test suite
- [x] Define test cases under certain test suite
- [x] Filter or filter out test cases/suites
- [x] Output test results in json format

Here is the explaination in more detail:

- A test suite basicly is consist of a set of related test cases, you can also declare a test suite with initialization and cleanup functions, these two functions will be called each time before and after a test case separately, so you have a change to initialize and release some common resources used by the test cases in that suite;
- You define a test case using the macro provided by cutest with a suite name and a case name, then you implement your own test case function;
- When you are about to run the test cases, pass the --filter or --filterout options if needed, this will allow you to run a subset of all the test cases, the others will be makred as IGNORE in the result;

## Install

### Build from source
```shell
$ git clone https://github.com/kfggww/cutest.git
$ cd cutest
$ make
$ sudo make install
```
### Binary installation

```shell
$ sudo add-apt-repository ppa:kfggww/cutest
$ sudo apt update
$ sudo apt install cutest
```

### Public APIs
- TEST_SUITE(suite_name)
- TEST_SUITE_WITH(suite_name, init, cleanup)
- TEST_CASE(suite_name, case_name)
- enum test_result
- cutest_init(int argc, char *argv[])
- cutest_run_all()

Read cutest document in "/usr/share/doc/cutest/index.html" for more details after installation.

## Example

The code below defines two test suites, each one has three test cases, the second suite has init and cleanup functions with it.

example.c:

```c
#include <stdio.h>
/* include cutest header file */
#include "cutest.h"

/* declare the first test suite named "example_suite" */
TEST_SUITE(example_suite)
TEST_CASE(example_suite, test1) // define "test1" under "example_suite"
{
    printf("hello from test1\n");
    return TEST_PASS;
}

TEST_CASE(example_suite, test2) // define "test2" under "example_suite"
{
    printf("hello from test2\n");
    return TEST_FAIL;
}

TEST_CASE(example_suite, test3) // define "test3" under "example_suite"
{
    printf("hello from test3\n");
    return TEST_IGNORE; // it's up to you to return the test result, if you wanna comment out this case, just move this statement to the first line of current test function
}

/* 
 * initialization and cleanup functions for "example2_suite" 
 */
static void example2_suite_init(void)
{
    printf("%s done\n", __func__);
}

static void example2_suite_exit(void)
{
    printf("%s done\n", __func__);
}

/* declare the second test suite "example2_suite" with initialization and cleanup functions */
TEST_SUITE_WITH(example2_suite, example2_suite_init, example2_suite_exit)
TEST_CASE(example2_suite, test1)
{
    printf("hello from test1\n");
    return TEST_PASS;
}

TEST_CASE(example2_suite, test2)
{
    printf("hello from test2\n");
    return TEST_FAIL;
}

TEST_CASE(example2_suite, test3)
{
    printf("hello from test3\n");
    return TEST_IGNORE;
}

/* main routine, "cutest_init" handles the options, "cutest_run_all" runs all the tests */
int main(int argc, char *argv[])
{
    cutest_init(argc, argv);
    cutest_run_all();
    return 0;
}
```

compile and run:

```shell
$ gcc example.c -I/usr/include/cutest -L/usr/lib/cutest -lcutest -o example -Wl,-rpath=/usr/lib/cutest
$
$
$ # show help info
$ ./example --help
Usage of cutest:
/path/to/test_executable [--help]
                         [--filter_suite suite_pattern]
                         [--filterout_suite suite_filterout_pattern]
                         [--filter_case case_pattern]
                         [--filterout_case case_filterout_pattern]
                         [--outformat json|xml(not supported yet)]
                         [--outfile result_file_name]
$
$
$ # run all the tests
$ ./example
[RUN...]: example_suite::test1
hello from test1
[...OK]: example_suite::test1
[RUN...]: example_suite::test2
hello from test2
[...FAIL]: example_suite::test2
[RUN...]: example_suite::test3
hello from test3
[...IGNORE]: example_suite::test3
[SUITE RESULT: example_suite]: pass=1/3, fail=1/3, ignore=1/3

[RUN...]: example2_suite::test1
example2_suite_init done
hello from test1
example2_suite_exit done
[...OK]: example2_suite::test1
[RUN...]: example2_suite::test2
example2_suite_init done
hello from test2
example2_suite_exit done
[...FAIL]: example2_suite::test2
[RUN...]: example2_suite::test3
example2_suite_init done
hello from test3
example2_suite_exit done
[...IGNORE]: example2_suite::test3
[SUITE RESULT: example2_suite]: pass=1/3, fail=1/3, ignore=1/3

[ALL RESULT]: pass=2/6, fail=2/6, ignore=2/6
$
$
$ # run only "example_suite"
$ ./example --filter_suite example_*
[RUN...]: example_suite::test1
hello from test1
[...OK]: example_suite::test1
[RUN...]: example_suite::test2
hello from test2
[...FAIL]: example_suite::test2
[RUN...]: example_suite::test3
hello from test3
[...IGNORE]: example_suite::test3
[SUITE RESULT: example_suite]: pass=1/3, fail=1/3, ignore=1/3

[SUITE RESULT: example2_suite]: pass=0/3, fail=0/3, ignore=3/3

[ALL RESULT]: pass=1/6, fail=1/6, ignore=4/6
$
$
$ # filterout "test1" in the two suites, and print result in json format
$ ./example --filterout_case test1 --outformat json --outfile results.json
[RUN...]: example_suite::test2
hello from test2
[...FAIL]: example_suite::test2
[RUN...]: example_suite::test3
hello from test3
[...IGNORE]: example_suite::test3
[SUITE RESULT: example_suite]: pass=0/3, fail=1/3, ignore=2/3

[RUN...]: example2_suite::test2
example2_suite_init done
hello from test2
example2_suite_exit done
[...FAIL]: example2_suite::test2
[RUN...]: example2_suite::test3
example2_suite_init done
hello from test3
example2_suite_exit done
[...IGNORE]: example2_suite::test3
[SUITE RESULT: example2_suite]: pass=0/3, fail=1/3, ignore=2/3

[ALL RESULT]: pass=0/6, fail=2/6, ignore=4/6
$ cat results.json
{
    "pass:": 0,
    "fail:": 2,
    "ignore:": 4,
    "total:": 6,
    "test_results": [
        {
            "suite_name": "example_suite",
            "pass": 0,
            "fail": 1,
            "ignore": 2,
            "total": 3,
            "tests": [
                {
                    "case_name": "test1",
                    "result": "IGNORE"
                },
                {
                    "case_name": "test2",
                    "result": "FAIL"
                },
                {
                    "case_name": "test3",
                    "result": "IGNORE"
                }
            ]
        },
        {
            "suite_name": "example2_suite",
            "pass": 0,
            "fail": 1,
            "ignore": 2,
            "total": 3,
            "tests": [
                {
                    "case_name": "test1",
                    "result": "IGNORE"
                },
                {
                    "case_name": "test2",
                    "result": "FAIL"
                },
                {
                    "case_name": "test3",
                    "result": "IGNORE"
                }
            ]
        }
    ]
}
```