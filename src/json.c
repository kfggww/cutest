#include <stdio.h>

#include "formatter.h"

extern char *result_file;

static FILE *json_fout = NULL;
static int indent_level = 0;

static inline void json_print_indent(int indent)
{
    for (int i = 0; i < indent; i++) {
        fprintf(json_fout, "    ");
    }
}

void json_print_registry(const struct test_registry *registry)
{
    if (result_file == NULL)
        result_file = "results.json";

    json_fout = fopen(result_file, "w");
    if (json_fout == NULL) {
        perror("Fail to open result_file");
        return;
    }

    // print start marker
    json_print_indent(indent_level);
    fprintf(json_fout, "{\n");

    // print properties of test_registry
    json_print_indent(indent_level + 1);
    fprintf(json_fout, "\"pass:\": %d,\n", registry->pass);

    json_print_indent(indent_level + 1);
    fprintf(json_fout, "\"fail:\": %d,\n", registry->fail);

    json_print_indent(indent_level + 1);
    fprintf(json_fout, "\"ignore:\": %d,\n", registry->ignore);

    json_print_indent(indent_level + 1);
    fprintf(json_fout, "\"total:\": %d,\n", registry->total);

    json_print_indent(indent_level + 1);
    fprintf(json_fout, "\"test_results\": [");

    if (registry->total == 0) {
        fprintf(json_fout, "]\n");
    } else {
        indent_level++;
        fprintf(json_fout, "\n");
        struct test_suite *suite = registry->first_suite;
        do {
            indent_level++;
            json_print_suite(suite);
            indent_level--;
            fprintf(json_fout, "%s",
                    suite->next != registry->first_suite ? ",\n" : "\n");
            suite = suite->next;
        } while (suite != registry->first_suite);
        indent_level--;
    }

    json_print_indent(indent_level + 1);
    fprintf(json_fout, "]\n");

    // print end marker
    json_print_indent(indent_level);
    fprintf(json_fout, "}");

    fclose(json_fout);
}

void json_print_suite(const struct test_suite *suite)
{
    json_print_indent(indent_level);
    fprintf(json_fout, "{\n");

    json_print_indent(indent_level + 1);
    fprintf(json_fout, "\"suite_name\": \"%s\",\n", suite->name);

    json_print_indent(indent_level + 1);
    fprintf(json_fout, "\"pass\": %d,\n", suite->pass);

    json_print_indent(indent_level + 1);
    fprintf(json_fout, "\"fail\": %d,\n", suite->fail);

    json_print_indent(indent_level + 1);
    fprintf(json_fout, "\"ignore\": %d,\n", suite->ignore);

    json_print_indent(indent_level + 1);
    fprintf(json_fout, "\"total\": %d,\n", suite->total);

    json_print_indent(indent_level + 1);
    fprintf(json_fout, "\"tests\": [");

    if (suite->total == 0) {
        fprintf(json_fout, "]\n");
    } else {
        indent_level++;
        fprintf(json_fout, "\n");
        struct test_case *caze = suite->first_case;
        do {
            indent_level++;
            json_print_case(caze);
            fprintf(json_fout, "%s",
                    caze->next != suite->first_case ? ",\n" : "\n");
            indent_level--;
            caze = caze->next;
        } while (caze != suite->first_case);
        indent_level--;
    }

    json_print_indent(indent_level + 1);
    fprintf(json_fout, "]\n");

    json_print_indent(indent_level);
    fprintf(json_fout, "}");
}

void json_print_case(const struct test_case *caze)
{
    json_print_indent(indent_level);
    fprintf(json_fout, "{\n");

    json_print_indent(indent_level + 1);
    fprintf(json_fout, "\"case_name\": \"%s\",\n", caze->name);

    json_print_indent(indent_level + 1);
    fprintf(json_fout, "\"result\": \"%s\"\n",
            caze->result == TEST_PASS ?
                "PASS" :
                (caze->result == TEST_FAIL ? "FAIL" : "IGNORE"));

    json_print_indent(indent_level);
    fprintf(json_fout, "}");
}
