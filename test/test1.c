#include "cutest.h"

CUTEST_SUIT(test_suit1)

CUTEST_CASE(test_suit1, test1) {
    int a = 10;
    int b = 10;
    CUT_EXPECT_EQ(a, b);
}

CUTEST_CASE(test_suit1, test2) {
    int a = 10;
    int b = 20;
    CUT_EXPECT_GT(a, b);
}