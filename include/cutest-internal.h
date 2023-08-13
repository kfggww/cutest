#ifndef CUTEST_INTERNAL_H
#define CUTEST_INTERNAL_H

/* Internal data structures used to manage user tests*/

enum test_result {
    TEST_PASS,
    TEST_FAIL,
    TEST_IGNORE,
};

typedef enum test_result (*test_func)(void);

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

#endif