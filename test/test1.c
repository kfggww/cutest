#include <stdio.h>
#include "cutest.h"

CUTEST_SUIT(test_suit1)
CUTEST_CASE(test_suit1, test1) { printf("hello from test1\n"); }

int main(int argc, char **argv) {
    cutest_run_all();
    return 0;
}