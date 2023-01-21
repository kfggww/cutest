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

CUTEST_SUIT(test_suit2)

CUTEST_CASE(test_suit2, test1) {
    CUT_EXPECT_GE(10, 8);
    CUT_SUCCESS();
}

CUTEST_CASE(test_suit2, test2) {
    CUT_EXPECT_EQ(10, 10);
    CUT_FAIL();
}