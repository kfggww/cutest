#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cutest.h"

#define HELP_INFO                                               \
    "Usage of cutest:\n"                                        \
    "/path/to/test_executable [--help]\n"                       \
    "                         [--filter_suite suite_pattern]\n" \
    "                         [--filter_case case_pattern]\n"

/* Options that cutest accepts */
enum cutest_option_value {
    EMPTY_OPTION = -1,
    FILTER_SUITE_OPTION = 1,
    FILTER_CASE_OPTION,
    HELP_OPTION,
};

#define LONG_OPTION(opt_name, value)                                \
    {                                                               \
        .name = opt_name, .has_arg = 0, .flag = NULL, .val = value, \
    }

#define LONG_ARG_OPTION(opt_name, value)                            \
    {                                                               \
        .name = opt_name, .has_arg = 1, .flag = NULL, .val = value, \
    }

static struct option init_options[] = {
    LONG_ARG_OPTION("filter_suite", FILTER_SUITE_OPTION),
    LONG_ARG_OPTION("filter_case", FILTER_CASE_OPTION),
    LONG_OPTION("help", HELP_OPTION),
};

struct test_registry cutest_registry = {
    .total = 0,
    .pass = 0,
    .fail = 0,
    .ignore = 0,
    .first_suite = NULL,
};

static struct test_suite *current_suite;
static struct test_case *current_case;

static char *cutest_suite_pattern = NULL;
static char *cutest_case_pattern = NULL;

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

static int cutest_filter_suite(struct test_suite *psuite)
{
    if (psuite == NULL)
        return 0;
    if (cutest_suite_pattern == NULL)
        return 1;
    return cutest_pattern_match(psuite->name, cutest_suite_pattern);
}

static int cutest_filter_case(struct test_case *pcase)
{
    if (pcase == NULL)
        return 0;
    if (cutest_case_pattern == NULL)
        return 1;
    return cutest_pattern_match(pcase->name, cutest_case_pattern);
}

static void cutest_run_one_case(struct test_case *pcase)
{
    if (cutest_filter_case(pcase) == 0) {
        pcase->suite->ignore++;
        return;
    }

    printf("[RUN...]: %s::%s\n", pcase->suite->name, pcase->name);

    if (pcase->suite->init != NULL)
        pcase->suite->init();

    pcase->test_fn();

    if (pcase->suite->cleanup != NULL)
        pcase->suite->cleanup();

    printf("[...%s]: %s::%s\n", pcase->result == TEST_PASS ? "OK" : "FAIL",
           pcase->suite->name, pcase->name);

    if (pcase->result == TEST_PASS)
        pcase->suite->pass++;
    else
        pcase->suite->fail++;
}

static void cutest_run_one_suite(struct test_suite *psuite)
{
    if (cutest_filter_suite(psuite) == 0) {
        psuite->ignore = psuite->total;
        return;
    }

    current_case = psuite->first_case;
    struct test_case *start_case = current_case;

    if (current_case != NULL) {
        do {
            cutest_run_one_case(current_case);
            current_case = current_case->next;
        } while (current_case != start_case);
    }
}

static void cutest_report_suite(struct test_suite *psuite)
{
    printf("[SUITE RESULT: %s]: ", psuite->name);
    printf("pass=%d/%d, fail=%d/%d, ignore=%d/%d\n\n", psuite->pass,
           psuite->total, psuite->fail, psuite->total, psuite->ignore,
           psuite->total);
}

static void cutest_report_all()
{
    printf("[ALL RESULT]: pass=%d/%d, fail=%d/%d, ignore=%d/%d\n",
           cutest_registry.pass, cutest_registry.total, cutest_registry.fail,
           cutest_registry.total, cutest_registry.ignore,
           cutest_registry.total);
}

void cutest_init(int argc, char *argv[])
{
    int index = -1;
    int ret = 0;
    while ((ret = getopt_long(argc, argv, "", init_options, &index)) > 0) {
        switch (ret) {
        case FILTER_SUITE_OPTION:
            cutest_suite_pattern = optarg;
            break;
        case FILTER_CASE_OPTION:
            cutest_case_pattern = optarg;
            break;
        default:
            printf("error use of cutest\n");
        case HELP_OPTION:
            printf(HELP_INFO);
            exit(0);
        }
    }
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
