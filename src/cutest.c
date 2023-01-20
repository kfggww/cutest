#include <stdio.h>

#include "cutest.h"

test_suit __cutest_test_suit_head__ = {.test_suit_name =
                                           "__cutest_test_suit_head__",
                                       .test_case_list = NULL,
                                       .test_suit_next = NULL};

static void __cutest_run_one_suit(test_suit *suit) {
    if (suit->test_case_list == NULL)
        return;

    printf("Start running test suit [%s]...\n", suit->test_suit_name);

    test_case *test_case = suit->test_case_list;
    while (test_case) {
        printf("Start running [%s][%s]...\n", suit->test_suit_name,
               test_case->test_name);
        test_case->test_func();
        printf("Finish running [%s][%s]...\n", suit->test_suit_name,
               test_case->test_name);
        test_case = test_case->test_case_next;
    }

    printf("Finish test suit [%s].\n", suit->test_suit_name);
}

void cutest_run_all() {
    test_suit *suit = &__cutest_test_suit_head__;

    while (suit) {
        __cutest_run_one_suit(suit);
        suit = suit->test_suit_next;
    }
}