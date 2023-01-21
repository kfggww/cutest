# cutest

![ubuntu-latest](https://github.com/kfggww/cutest/actions/workflows/cutest.yml/badge.svg)

A simple unit test framework for c programming language.

## Features
- Define new test suit
- Define new test case
- Using assertions in test case function
- Show test results statistics
- No need to write a 'main' function
- Manage all the user defined test cases automaticlly

## Usages
1. write your test case

    Say you write two test cases in the file 'test1.c'.
    ```c
    /*test1.c*/

    /*include cutest header file*/
    #include "cutest.h"

    /*define a new test suit with the name 'test_suit1'*/
    CUTEST_SUIT(test_suit1)

    /*define first test case in test_suit1*/
    CUTEST_CASE(test_suit1, test1) {
        int a = 10;
        int b = 10;
        CUT_EXPECT_EQ(a, b);
    }

    /*define the second test case in test_suit1*/
    CUTEST_CASE(test_suit1, test2) {
        int a = 10;
        int b = 20;
        CUT_EXPECT_GT(a, b);
    }
    ```
2. compile and link
   
   You should tell gcc compiler that where the libcutest.so is if it is not in the standard system library path.
   ```shell
   $ gcc -L$(CUTEST_LIB_DIR) -lcutest -o test1 test1.c
   ```
3. run the executable file

    Just run the 'test1' in your shell, again you should set LD_LIBRARY_PATH if libcutest.so is not installed in your standard system library path.
   ```shell
   $ ./test1
   cutest summary:
        [test_suit1] suit result: 1/2
                [test_suit1::test2] case result: Fail
                [test_suit1::test1] case result: Pass
   ```

## TODOs
- [ ] running cutest in multi-threads environment
- [ ] using more efficient data structures to manage test cases
- [ ] cross-platform