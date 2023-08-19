#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cutest.h"
#include "formatter.h"
#include "option.h"

struct test_registry cutest_registry = {
    .total = 0,
    .pass = 0,
    .fail = 0,
    .ignore = 0,
    .first_suite = NULL,
};

static struct test_suite *current_suite;
static struct test_case *current_case;

static int cutest_pattern_match(const char *s, const char *pattern)
{
    /* No wildcard, compare directly */
    char *pstar = strchr(pattern, '*');
    if (pstar == NULL)
        return strcmp(s, pattern) == 0;

    /* Compare left part of pattern with s */
    const char *pi = s;
    const char *pj = pattern;

    while (*pi == *pj && *pj != '*') {
        pi++;
        pj++;
    }
    if (*pj != '*')
        return 0;

    /* Compare right part of pattern with s */
    pi = s + strlen(s);
    pj = pattern + strlen(pattern);

    while (*pi == *pj && *pj != '*') {
        pi--;
        pj--;
    }

    if (*pj != '*')
        return 0;

    return 1;
}

static int cutest_filter(const char *name, const char *filter_pattern,
                         const char *filterout_pattern)
{
    if (filter_pattern != NULL) {
        return cutest_pattern_match(name, filter_pattern);
    } else if (filterout_pattern != NULL) {
        return !cutest_pattern_match(name, filterout_pattern);
    }

    return 1;
}

static inline int cutest_filter_suite(struct test_suite *suite)
{
    return cutest_filter(suite->name, suite_filter_pattern,
                         suite_filterout_pattern);
}

static inline int cutest_filter_case(struct test_case *caze)
{
    return cutest_filter(caze->name, case_filter_pattern,
                         case_filterout_pattern);
}

static void cutest_run_one_case(struct test_case *caze)
{
    if (cutest_filter_case(caze) == 0) {
        caze->result = TEST_IGNORE;
        caze->suite->ignore++;
        return;
    }

    printf("[RUN...]: %s::%s\n", caze->suite->name, caze->name);

    if (caze->suite->init != NULL)
        caze->suite->init();

    caze->result = caze->test_fn();

    if (caze->suite->cleanup != NULL)
        caze->suite->cleanup();

    printf("[...%s]: %s::%s\n",
           caze->result == TEST_PASS ?
               "OK" :
               (caze->result == TEST_FAIL ? "FAIL" : "IGNORE"),
           caze->suite->name, caze->name);

    if (caze->result == TEST_PASS)
        caze->suite->pass++;
    else if (caze->result == TEST_FAIL)
        caze->suite->fail++;
    else
        caze->suite->ignore++;
}

static void cutest_ignore_suite(struct test_suite *suite)
{
    struct test_case *caze = suite->first_case;
    if (caze == NULL)
        return;

    do {
        caze->result = TEST_IGNORE;
        caze = caze->next;
    } while (caze != suite->first_case);
}

static void cutest_run_one_suite(struct test_suite *suite)
{
    if (cutest_filter_suite(suite) == 0) {
        cutest_ignore_suite(suite);
        suite->ignore = suite->total;
        return;
    }

    current_case = suite->first_case;
    struct test_case *start_case = current_case;

    if (current_case != NULL) {
        do {
            cutest_run_one_case(current_case);
            current_case = current_case->next;
        } while (current_case != start_case);
    }
}

static void cutest_save_results()
{
    if (result_outformat == NULL)
        return;

    if (strcmp(result_outformat, "json") == 0) {
        json_print_registry(&cutest_registry);
    } else {
        printf("Output file format NOT supported: %s\n", result_outformat);
    }
}

static inline void cutest_report_suite(struct test_suite *suite)
{
    printf("[SUITE RESULT: %s]: ", suite->name);
    printf("pass=%d/%d, fail=%d/%d, ignore=%d/%d\n\n", suite->pass,
           suite->total, suite->fail, suite->total, suite->ignore,
           suite->total);
}

static inline void cutest_report_all()
{
    printf("[ALL RESULT]: pass=%d/%d, fail=%d/%d, ignore=%d/%d\n",
           cutest_registry.pass, cutest_registry.total, cutest_registry.fail,
           cutest_registry.total, cutest_registry.ignore,
           cutest_registry.total);
    cutest_save_results();
}

void cutest_run_all()
{
    current_suite = cutest_registry.first_suite;
    struct test_suite *start_suite = current_suite;

    if (current_suite != NULL) {
        do {
            cutest_run_one_suite(current_suite);
            cutest_report_suite(current_suite);

            cutest_registry.pass += current_suite->pass;
            cutest_registry.fail += current_suite->fail;
            cutest_registry.ignore += current_suite->ignore;

            current_suite = current_suite->next;
        } while (current_suite != start_suite);
    }

    cutest_report_all();
}
