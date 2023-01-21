#include <stdio.h>

#include "cutest.h"

test_suit __cutest_test_suit_head__ = {.test_suit_name =
                                           "__cutest_test_suit_head__",
                                       .test_case_list = NULL,
                                       .test_suit_next = NULL};
test_suit *__cutest_current_test_suit__ = NULL;
test_case *__cutest_current_test_case__ = NULL;

static void __cutest_run_one_suit(test_suit *suit) {
    if (suit->test_case_list == NULL)
        return;

    test_case *test_case = suit->test_case_list;
    while (test_case) {
        __cutest_current_test_case__ = test_case;
        test_case->test_func();
        test_case = test_case->test_case_next;
    }
    __cutest_current_test_case__ = NULL;
}

static void __cutest_show_test_results() {
    test_suit *suit = &__cutest_test_suit_head__;
    printf("cutest summary:\n");
    while (suit) {
        if (suit->test_case_total == 0) {
            suit = suit->test_suit_next;
            continue;
        }
        printf("\t[%s] suit result: %d/%d\n", suit->test_suit_name,
               suit->test_case_passed, suit->test_case_total);
        test_case *test_case = suit->test_case_list;
        while (test_case) {
            printf("\t[%s] case result: %s\n", test_case->test_name,
                   test_case->test_result == CUTEST_PASS ? "Pass" : "Fail");
            test_case = test_case->test_case_next;
        }
        suit = suit->test_suit_next;
    }
}

void cutest_run_all() {
    test_suit *suit = &__cutest_test_suit_head__;

    while (suit) {
        __cutest_current_test_suit__ = suit;
        __cutest_run_one_suit(suit);
        suit = suit->test_suit_next;
    }

    __cutest_show_test_results();
}

int main(int argc, char **argv) {
    cutest_run_all();
    return 0;
}