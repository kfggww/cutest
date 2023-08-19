@page Implementation
@tableofcontents

## Internal data structures

Each test suits has one or more test cases, these cases is managed by test suite using double linked list, the same method is used by test registry to manage its test suites.

- struct test_case
- struct test_suite
- struct test_registry

## Test suite and case management
The magic happens when you declare test suites or define test cases using TEST_XXX macros. when you declare a test suite by TEST_SUITE(suite_name), you actually define a varible of type "struct test_suite" and a static function with "constructor" attribute, the name of the variable and function is determined by the parameter of TEST_SUITE macro, since the function is a "constructor" it will be called before the main routine, inside this function the test_suite is inserted into the linked list of test_registry. The TEST_CASE macro is almost the same, read cutest.h for more details.