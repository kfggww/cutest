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

static void example2_suite_init(void)
{
    printf("%s done\n", __func__);
}

static void example2_suite_exit(void)
{
    printf("%s done\n", __func__);
}

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

int main(int argc, char *argv[])
{
    cutest_init(argc, argv);
    cutest_run_all();
    return 0;
}