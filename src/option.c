#include <getopt.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "cutest-internal.h"

#define HELP_INFO                                                            \
    "Usage of cutest:\n"                                                     \
    "/path/to/test_executable [--help]\n"                                    \
    "                         [--filter_suite suite_pattern]\n"              \
    "                         [--filterout_suite suite_filterout_pattern]\n" \
    "                         [--filter_case case_pattern]\n"                \
    "                         [--filterout_case case_filterout_pattern]\n"   \
    "                         [--outformat json|xml]\n"                      \
    "                         [--outfile result_file_name]\n"

/* Options that cutest accepts */
enum cutest_option_value {
    EMPTY_OPTION = -1,
    FILTER_SUITE_OPTION = 1,
    FILTEROUT_SUITE_OPTION,
    FILTER_CASE_OPTION,
    FILTEROUT_CASE_OPTION,
    OUTFORMAT_OPTION,
    OUTFILE_OPTION,
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
    LONG_ARG_OPTION("filterout_suite", FILTEROUT_SUITE_OPTION),
    LONG_ARG_OPTION("filter_case", FILTER_CASE_OPTION),
    LONG_ARG_OPTION("filterout_case", FILTEROUT_CASE_OPTION),
    LONG_ARG_OPTION("outformat", OUTFORMAT_OPTION),
    LONG_ARG_OPTION("outfile", OUTFILE_OPTION),
    LONG_OPTION("help", HELP_OPTION),
};

char *suite_filter_pattern = NULL;
char *suite_filterout_pattern = NULL;

char *case_filter_pattern = NULL;
char *case_filterout_pattern = NULL;

char *result_outformat = NULL;
char *result_file = NULL;


void cutest_init(int argc, char *argv[])
{
    int index = -1;
    int ret = 0;
    while ((ret = getopt_long(argc, argv, "", init_options, &index)) > 0) {
        switch (ret) {
        case FILTER_SUITE_OPTION:
            suite_filter_pattern = optarg;
            break;
        case FILTEROUT_SUITE_OPTION:
            suite_filterout_pattern = optarg;
            break;
        case FILTER_CASE_OPTION:
            case_filter_pattern = optarg;
            break;
        case FILTEROUT_CASE_OPTION:
            case_filterout_pattern = optarg;
            break;
        case OUTFORMAT_OPTION:
            result_outformat = optarg;
            break;
        case OUTFILE_OPTION:
            result_file = optarg;
            break;
        default:
            printf("error use of cutest\n");
        case HELP_OPTION:
            printf(HELP_INFO);
            exit(0);
        }
    }
}