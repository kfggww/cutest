#ifndef CUTEST_H
#define CUTEST_H
#include <stddef.h>

#include "cutest-internal.h"

#define __init __attribute__((constructor))

/* Macros for test_suite */

#define __REGISTER_TEST_SUITE(suite_name)                           \
    static void __init __register_suite_##suite_name(void)          \
    {                                                               \
        extern struct test_registry cutest_registry;                \
        struct test_suite *psuite = &__TEST_SUITE_NAME(suite_name); \
        if (cutest_registry.first_suite == NULL) {                  \
            cutest_registry.first_suite = psuite;                   \
            psuite->next = psuite;                                  \
            psuite->prev = psuite;                                  \
        } else {                                                    \
            psuite->next = cutest_registry.first_suite;             \
            psuite->prev = cutest_registry.first_suite->prev;       \
            cutest_registry.first_suite->prev->next = psuite;       \
            cutest_registry.first_suite->prev = psuite;             \
        }                                                           \
    }

#define __TEST_SUITE_INITIALIZER(suite_name, init_fn, cleanup_fn)           \
    {                                                                       \
        .name = #suite_name, .total = 0, .pass = 0, .fail = 0, .ignore = 0, \
        .init = init_fn, .cleanup = cleanup_fn, .next = NULL, .prev = NULL, \
        .first_case = NULL,                                                 \
    }

#define __TEST_SUITE_NAME(suite_name) __cutest_suite_##suite_name

#define __DEFINE_TEST_SUITE(suite_name, init_fn, cleanup_fn) \
    static struct test_suite __TEST_SUITE_NAME(suite_name) = \
        __TEST_SUITE_INITIALIZER(suite_name, init_fn, cleanup_fn);

#define TEST_SUITE(suite_name)                   \
    __DEFINE_TEST_SUITE(suite_name, NULL, NULL); \
    __REGISTER_TEST_SUITE(suite_name)

#define TEST_SUITE_WITH(suite_name, init_fn, cleanup_fn) \
    __DEFINE_TEST_SUITE(suite_name, init_fn, cleanup_fn) \
    __REGISTER_TEST_SUITE(suite_name)

/* Macros for test_case */

#define __DEFINE_TEST_CASE_FUNC(suite_name, test_name) \
    static enum test_result __TEST_CASE_FUNC_NAME(suite_name, test_name)(void)

#define __REGISTER_TEST_CASE(suite_name, test_name)                          \
    static void __init __register_test_case_##suite_name##_##test_name(void) \
    {                                                                        \
        extern struct test_registry cutest_registry;                         \
        struct test_suite *psuite = &__TEST_SUITE_NAME(suite_name);          \
        struct test_case *pcase = &__TEST_CASE_NAME(suite_name, test_name);  \
        if (psuite->first_case == NULL) {                                    \
            psuite->first_case = pcase;                                      \
            pcase->suite = psuite;                                           \
            pcase->next = pcase;                                             \
            pcase->prev = pcase;                                             \
        } else {                                                             \
            pcase->suite = psuite;                                           \
            pcase->next = psuite->first_case;                                \
            pcase->prev = psuite->first_case->prev;                          \
            psuite->first_case->prev->next = pcase;                          \
            psuite->first_case->prev = pcase;                                \
        }                                                                    \
        psuite->total += 1;                                                  \
        cutest_registry.total += 1;                                          \
    }

#define __TEST_CASE_INITIALIZER(suite_name, test_name)                \
    {                                                                 \
        .name = #test_name,                                           \
        .test_fn = __TEST_CASE_FUNC_NAME(suite_name, test_name),      \
        .result = TEST_PASS, .suite = &__TEST_SUITE_NAME(suite_name), \
        .next = NULL, .prev = NULL,                                   \
    }

#define __TEST_CASE_NAME(suite_name, test_name) \
    __cutest_case_##suite_name##_##test_name

#define __TEST_CASE_FUNC_NAME(suite_name, test_name) \
    __cutest_case_fn_##suite_name##_##test_name

#define __DEFINE_TEST_CASE(suite_name, test_name)                     \
    static enum test_result __TEST_CASE_FUNC_NAME(suite_name,         \
                                                  test_name)(void);   \
    static struct test_case __TEST_CASE_NAME(suite_name, test_name) = \
        __TEST_CASE_INITIALIZER(suite_name, test_name);

#define TEST_CASE(suite_name, test_name)        \
    __DEFINE_TEST_CASE(suite_name, test_name)   \
    __REGISTER_TEST_CASE(suite_name, test_name) \
    __DEFINE_TEST_CASE_FUNC(suite_name, test_name)

/* APIs for client program */
void cutest_init(int argc, char *argv[]);
void cutest_run_all();

#endif