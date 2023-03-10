#ifndef CUTEST_H
#define CUTEST_H

typedef void (*test_func)(void);

typedef struct test_case {
    char *test_name;
    test_func test_func;
    struct test_case *test_case_next;
    int test_result;
} test_case;

typedef struct test_suit {
    char *test_suit_name;
    test_case *test_case_list;
    int test_case_total;
    int test_case_passed;
    struct test_suit *test_suit_next;
} test_suit;

extern test_suit __cutest_test_suit_head__;
extern test_suit *__cutest_current_test_suit__;
extern test_case *__cutest_current_test_case__;

#define NULL ((void *)0)
#define CUTEST_PASS 1
#define CUTEST_FAIL 0

/*macros used by test suit*/
#define __CUTEST_SUIT_NAME(suit_name) __cutest_test_suit_##suit_name
#define __CUTEST_INIT_SUIT(suit_name)                                          \
    {                                                                          \
        .test_suit_name = #suit_name, .test_case_list = NULL,                  \
        .test_case_total = 0, .test_case_passed = 0, .test_suit_next = NULL    \
    }

#define __CUTEST_INSERT_TEST_SUIT(suit_name)                                   \
    do {                                                                       \
        __CUTEST_SUIT_NAME(suit_name).test_suit_next =                         \
            __cutest_test_suit_head__.test_suit_next;                          \
        __cutest_test_suit_head__.test_suit_next =                             \
            &(__CUTEST_SUIT_NAME(suit_name));                                  \
    } while (0)

#define __REGISTER_CUTEST_SUIT(suit_name)                                      \
    void __attribute__((constructor))                                          \
    __cutest_register_test_suit_##suit_name() {                                \
        __CUTEST_INSERT_TEST_SUIT(suit_name);                                  \
    }

#define __DEFINE_CUTEST_SUIT(suit_name)                                        \
    test_suit __CUTEST_SUIT_NAME(suit_name) = __CUTEST_INIT_SUIT(suit_name);

#define CUTEST_SUIT(suit_name)                                                 \
    __DEFINE_CUTEST_SUIT(suit_name)                                            \
    __REGISTER_CUTEST_SUIT(suit_name)

/*macros used by test case*/
#define __CUTEST_CASE_FUNC_NAME(suit_name, case_name)                          \
    __cutest_func_##suit_name##_##case_name

#define __CUTEST_CASE_FUNC(suit_name, case_name)                               \
    void __CUTEST_CASE_FUNC_NAME(suit_name, case_name)()

#define __CUTEST_CASE_NAME(suit_name, case_name)                               \
    __cutest_test_case_##suit_name##_##case_name

#define __CUTEST_INIT_CASE(suit_name, case_name)                               \
    {                                                                          \
        .test_name = #suit_name "::" #case_name,                               \
        .test_func = __CUTEST_CASE_FUNC_NAME(suit_name, case_name),            \
        .test_case_next = NULL, .test_result = CUTEST_PASS                     \
    }

#define __DEFINE_CUTEST_CASE(suit_name, case_name)                             \
    __CUTEST_CASE_FUNC(suit_name, case_name);                                  \
    test_case __CUTEST_CASE_NAME(suit_name, case_name) =                       \
        __CUTEST_INIT_CASE(suit_name, case_name);

#define __CUTEST_INSERT_TEST_CASE(suit_name, case_name)                        \
    do {                                                                       \
        extern test_suit __CUTEST_SUIT_NAME(suit_name);                        \
        __CUTEST_CASE_NAME(suit_name, case_name).test_case_next =              \
            __CUTEST_SUIT_NAME(suit_name).test_case_list;                      \
        __CUTEST_SUIT_NAME(suit_name).test_case_list =                         \
            &(__CUTEST_CASE_NAME(suit_name, case_name));                       \
        __CUTEST_SUIT_NAME(suit_name).test_case_total += 1;                    \
        __CUTEST_SUIT_NAME(suit_name).test_case_passed += 1;                   \
    } while (0)

#define __REGISTER_CUTEST_CASE(suit_name, case_name)                           \
    void __attribute__((constructor))                                          \
    __cutest_register_test_case_##suit_name##_##case_name() {                  \
        __CUTEST_INSERT_TEST_CASE(suit_name, case_name);                       \
    }

#define CUTEST_CASE(suit_name, case_name)                                      \
    __DEFINE_CUTEST_CASE(suit_name, case_name)                                 \
    __REGISTER_CUTEST_CASE(suit_name, case_name)                               \
    __CUTEST_CASE_FUNC(suit_name, case_name)

#define CUT_EXPECT_TRUE(c)                                                     \
    do {                                                                       \
        if (!(c)) {                                                            \
            __cutest_current_test_case__->test_result = CUTEST_FAIL;           \
            __cutest_current_test_suit__->test_case_passed -= 1;               \
            return;                                                            \
        }                                                                      \
    } while (0)

#define CUT_EXPECT_FALSE(c) CUT_EXPECT_TRUE(!(a))

#define CUT_EXPECT_EQ(a, b) CUT_EXPECT_TRUE((a) == (b))
#define CUT_EXPECT_NQ(a, b) CUT_EXPECT_TRUE((a) != (b))
#define CUT_EXPECT_LT(a, b) CUT_EXPECT_TRUE((a) < (b))
#define CUT_EXPECT_LE(a, b) CUT_EXPECT_TRUE((a) <= (b))
#define CUT_EXPECT_GT(a, b) CUT_EXPECT_TRUE((a) > (b))
#define CUT_EXPECT_GE(a, b) CUT_EXPECT_TRUE((a) >= (b))

#define CUT_SUCCESS() return
#define CUT_FAIL() CUT_EXPECT_TRUE(0)

void cutest_run_all();

#endif