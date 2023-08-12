#ifndef CUTEST_H
#define CUTEST_H
#include <stddef.h>

/* Basic data structures */

enum test_result {
    TEST_PASS,
    TEST_FAIL,
    TEST_IGNORE,
};

typedef void (*test_func)(void);

struct test_suite;

struct test_case {
    const char *name;
    test_func test_fn;
    enum test_result result;

    struct test_suite *suite;
    struct test_case *next;
    struct test_case *prev;
};

struct test_suite {
    const char *name;

    int total;
    int pass;
    int fail;
    int ignore;

    void (*init)(void);
    void (*cleanup)(void);

    struct test_suite *next;
    struct test_suite *prev;
    struct test_case *first_case;
};

struct test_registry {
    int total;
    int pass;
    int fail;
    int ignore;

    struct test_suite *first_suite;
};

#define __init __attribute__((constructor))

/* Macros for test_suite */

#define __REGISTER_TEST_SUITE(suite_name)                           \
    static void __init __register_suite_##suite(void)               \
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
    static void __TEST_CASE_FUNC_NAME(suite_name, test_name)(void)

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
    static void __TEST_CASE_FUNC_NAME(suite_name, test_name)(void);   \
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