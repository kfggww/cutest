#ifndef FORMATTER_H
#define FORMATTER_H
#include "cutest-internal.h"

/* JSON formatter APIs */
void json_print_registry(const struct test_registry *registry);
void json_print_suite(const struct test_suite *suite);
void json_print_case(const struct test_case *caze);

#endif