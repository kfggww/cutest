#include <stdio.h>

#include "cutest.h"

TEST_SUITE(example_suite)
TEST_CASE(example_suite, test1)
{
    printf("hello from test1\n");
    return TEST_PASS;
}

TEST_CASE(example_suite, test2)
{
    printf("hello from test2\n");
    return TEST_FAIL;
}

TEST_CASE(example_suite, test3)
{
    printf("hello from test3\n");
    return TEST_IGNORE;
}

int main(int argc, char *argv[])
{
    cutest_init(argc, argv);
    cutest_run_all();
    return 0;
}