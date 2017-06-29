

#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"

#include <stdio.h>
#include <string.h>


/** Cli definitions we'll need for our tests **/
#include    "cli.h"
#include    "line_ed.h"
#include    "cli_timeout.h"

cli_t * init_cli(void);

static char empty_show_command[] = "show";
static char show_action_argument[] = "show_argument";
static char unknown_action_argument[] = "unknown_argument";
char * cli_tree_find_match_string;
char * cli_tree_find_subtree_match_string;

/* Test Suite setup and cleanup functions: */

int init_suite(void)
{
    return 0;
}
int clean_suite(void)
{
    return 0;
}

/****************  Stubs   ***************************/
int cli_tree_find(cli_tree_type_t tree_type, char const * name, void ** data, cli_tree_data_type_t * const data_type)
{
    void * return_data;
    if (cli_tree_find_match_string != NULL && strcasecmp(name, cli_tree_find_match_string) == 0)
    {
        static cli_tree_data_t tree =
        {
            .tree_data_type = cli_tree_data_type_show,
            .name = "show",
            .data = "dummy_data"
        };

        return_data = &tree;
    }
    else
    {
        return_data = NULL;
    }
    *data = return_data;
    return 0;
}

int cli_tree_find_subtree(void * const search_tree, char const * name, void ** data, cli_tree_data_type_t * const data_type)
{
    if (cli_tree_find_subtree_match_string != NULL && strcasecmp(name, cli_tree_find_subtree_match_string) == 0)
    {
        static cli_tree_data_t tree =
        {
            .tree_data_type = cli_tree_data_type_group,
            .name = "show_argument",
            .data = "dummy_data"
        };
        *data_type = tree.tree_data_type;
        *data = &tree;
    }
    else
    {
        *data = NULL;
    }

    return 0;

}

void cli_printf(cli_t * cli, char const * const format, ...)
{
}

int process_show_command(cli_t * const context, size_t const group_id, size_t const instance, char const * const parameter)
{
    return 0;
}

int process_action_command(cli_t * const context, size_t const action_id, char const * const parameter)
{
    return 0;
}

cli_status_t cli_cmd_set_element(cli_t * const context, config_mgr_group_t const * const group, size_t const instance, size_t const element_id, char const * const element_value, char const ** const error_string)
{
    return cli_success;
}

int cli_apply_config(cli_t * const context, size_t const group_id, size_t const instance)
{
    return 0;
}

cli_status_t cli_cmd_get_element(cli_t * const context, config_mgr_group_t const * const group, size_t const instance, size_t const element_id, char * const element_value, size_t const size)
{
    return cli_success;
}

void cli_cmd_print_all_elements_in_group(cli_t * const context, cli_cmd_t * const cmd)
{
}

int autocomplete_cmd(cli_t * const context, cli_cmd_t * const cmd, char * const buffer, int autohelp)
{
    return 0;
}

void autocomplete_element(cli_t * const context, cli_cmd_t * const cmd, cli_tree_data_t const * const command, char * const buffer)
{
}

void autocomplete_instance(cli_t * const context, cli_cmd_t * const cmd)
{
}

void autocomplete_value(cli_t * const context, cli_cmd_t * const cmd, char * const buffer)
{
}

void autocomplete_multi_value(cli_t * const context, cli_cmd_t * const cmd, char * const buffer)
{
}

void autocomplete_on_off(cli_t * const context, cli_cmd_t * const cmd, char * const buffer)
{
}

void autocomplete_boolean(cli_t * const context, cli_cmd_t * const cmd, char * const buffer)
{
}

void help_action_element(cli_t * const context, cli_cmd_t * const cmd)
{
}

void help_action(cli_t * const context, cli_cmd_t * const cmd, char const * const buf)
{
}

void help_group(cli_t * const context, cli_cmd_t * const cmd, char const * const buf)
{
}

void help_group_element(cli_t * const context, cli_cmd_t * const cmd)
{
}

void help_all(cli_t * const context, cli_cmd_t * const cmd, char const * const buf)
{
}

/************* Test case functions ****************/
static cli_t * cli;


static void base_test_setup(char * const main_tree_match_string, char * const sub_tree_match_string)
{
    if ((cli = init_cli()) == NULL)
    {
        CU_FAIL_FATAL("Failed to init cli");
    }
    cli_tree_find_match_string = main_tree_match_string;
    cli_tree_find_subtree_match_string = sub_tree_match_string;
}

static void base_test_teardown(void)
{
    free(cli);
    cli = NULL;
}

static void empty_command_gives_empty_result(void)
{
    cli_parse_status_t parse_input_status;
    int position;

    /* setup */
    base_test_setup(NULL, NULL);
    strcpy(cli->line, "");

    /* perform test */
    parse_input_status = parse_input(cli, &position);

    /* check result */
    CU_ASSERT_EQUAL(parse_input_status, PARSE_EMPTY_CMD);

    /* cleanup */
    base_test_teardown();
}

static void whitespace_command_gives_empty_result(void)
{
    cli_parse_status_t parse_input_status;
    int position;

    /* setup */
    base_test_setup(NULL, NULL);
    strcpy(cli->line, " ");

    /* perform test */
    parse_input_status = parse_input(cli, &position);

    /* check result */
    CU_ASSERT_EQUAL(parse_input_status, PARSE_EMPTY_CMD);

    /* cleanup */
    base_test_teardown();
}

static void hash_comment_gives_comment_result(void)
{
    cli_parse_status_t parse_input_status;
    int position;

    /* setup */
    base_test_setup(NULL, NULL);
    strcpy(cli->line, "# some comment using a hash");

    /* perform test */
    parse_input_status = parse_input(cli, &position);

    /* check result */
    CU_ASSERT_EQUAL(parse_input_status, PARSE_COMMENT);

    /* cleanup */
    base_test_teardown();
}

static void bang_comment_gives_comment_result(void)
{
    cli_parse_status_t parse_input_status;
    int position;

    /* setup */
    base_test_setup(NULL, NULL);
    strcpy(cli->line, "! some comment using a bang");

    /* perform test */
    parse_input_status = parse_input(cli, &position);

    /* check result */
    CU_ASSERT_EQUAL(parse_input_status, PARSE_COMMENT);

    /* cleanup */
    base_test_teardown();
}

static void leading_whitespace_comment_gives_comment_result(void)
{
    cli_parse_status_t parse_input_status;
    int position;

    /* setup */
    base_test_setup(NULL, NULL);
    strcpy(cli->line, "   # some comment with leading whitespace");

    /* perform test */
    parse_input_status = parse_input(cli, &position);

    /* check result */
    CU_ASSERT_EQUAL(parse_input_status, PARSE_COMMENT);

    /* cleanup */
    base_test_teardown();
}

static void firewall_tag_gives_firewall_result(void)
{
    cli_parse_status_t parse_input_status;
    int position;

    /* setup */
    base_test_setup(NULL, NULL);
    strcpy(cli->line, "[FIREWALL]");
    // XXX currently need to have boot mode flag set before firewall
    // tags are processed. Why not process these tags all the time?
    cli->flag |= CLI_FLAG_BOOT;

    /* perform test */
    parse_input_status = parse_input(cli, &position);

    /* check result */
    CU_ASSERT_EQUAL(parse_input_status, PARSE_FIREWALL);

    /* cleanup */
    base_test_teardown();
}

static void end_firewall_tag_gives_firewall_result(void)
{
    cli_parse_status_t parse_input_status;
    int position;

    /* setup */
    base_test_setup(NULL, NULL);
    strcpy(cli->line, "[FIREWALL_END]");
    // XXX currently need to have boot mode flag set before firewall
    // tags are processed. Why not process these tags all the time?
    cli->flag |= CLI_FLAG_BOOT;

    /* perform test */
    parse_input_status = parse_input(cli, &position);

    /* check result */
    CU_ASSERT_EQUAL(parse_input_status, PARSE_FIREWALL);

    /* cleanup */
    base_test_teardown();
}

static void lines_after_opening_firewall_tag_give_firewall_result(void)
{
    cli_parse_status_t parse_input_status;
    int position;

    /* setup */
    base_test_setup(NULL, NULL);
    cli->flag |= CLI_FLAG_BOOT;

    /* first step pass the opening tag */
    strcpy(cli->line, "[FIREWALL]");
    // XXX currently need to have boot mode flag set before firewall
    // tags are processed. Why not process these tags all the time?
    (void)parse_input(cli, &position);

    /* now check that some random command returns PARSE_FIREWALL */
    strcpy(cli->line, "some random string");
    parse_input_status = parse_input(cli, &position);

    /* check result */
    CU_ASSERT_EQUAL(parse_input_status, PARSE_FIREWALL);

    /* cleanup */
    base_test_teardown();
}

static void lines_after_firewall_end_tag_doesnt_give_firewall_result(void)
{
    cli_parse_status_t parse_input_status;
    int position;

    /* setup */
    base_test_setup(NULL, NULL);
    cli->flag |= CLI_FLAG_BOOT;

    /* first step pass the opening tag */
    strcpy(cli->line, "[FIREWALL]");
    // XXX currently need to have boot mode flag set before firewall
    // tags are processed. Why not process these tags all the time?
    (void)parse_input(cli, &position);

    /* send the end tag */
    strcpy(cli->line, "[FIREWALL_END]");
    // XXX currently need to have boot mode flag set before firewall
    // tags are processed. Why not process these tags all the time?
    (void)parse_input(cli, &position);

    /* check that the result is _not_ PARSE_FIREWALL*/
    /* now check that some random command returns PARSE_FIREWALL */
    strcpy(cli->line, "dummy_command");
    parse_input_status = parse_input(cli, &position);

    /* check result */
    CU_ASSERT_NOT_EQUAL(parse_input_status, PARSE_FIREWALL);


    /* cleanup */
    base_test_teardown();
}

static void one_word_show_command_returns_incomplete(void)
{
    cli_parse_status_t parse_input_status;
    int position;

    /* setup */
    base_test_setup(empty_show_command, NULL);

    strcpy(cli->line, empty_show_command);
    parse_input_status = parse_input(cli, &position);

    /* check result */
    CU_ASSERT_EQUAL(parse_input_status, PARSE_INCOMPLETE);


    /* cleanup */
    base_test_teardown();
}

static void valid_two_word_show_command(void)
{
    cli_parse_status_t parse_input_status;
    int position;

    /* setup */
    base_test_setup(empty_show_command, show_action_argument);

    snprintf(cli->line, sizeof cli->line, "%s %s", empty_show_command, show_action_argument);
    parse_input_status = parse_input(cli, &position);

    /* check result */
    CU_ASSERT_EQUAL(parse_input_status, PARSE_OK);


    /* cleanup */
    base_test_teardown();
}

static void valid_two_word_show_command_with_trailing_whitespace(void)
{
    cli_parse_status_t parse_input_status;
    int position;

    /* setup */
    base_test_setup(empty_show_command, show_action_argument);

    snprintf(cli->line, sizeof cli->line, "%s %s ", empty_show_command, show_action_argument);
    parse_input_status = parse_input(cli, &position);

    /* check result */
    CU_ASSERT_EQUAL(parse_input_status, PARSE_OK);


    /* cleanup */
    base_test_teardown();
}

static void unknown_show_argument_gives_unknown_param_result(void)
{
    cli_parse_status_t parse_input_status;
    int position;

    /* setup */
    base_test_setup(empty_show_command, show_action_argument);

    snprintf(cli->line, sizeof cli->line, "%s %s", empty_show_command, unknown_action_argument);
    parse_input_status = parse_input(cli, &position);

    /* check result */
    CU_ASSERT_EQUAL(parse_input_status, PARSE_INVALID_PARAM);


    /* cleanup */
    base_test_teardown();
}

/************* Test Runner Code goes here **************/

int main(void)
{
    CU_pSuite pSuite = NULL;
    unsigned int number_of_failures;

    /* initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    /* add a suite to the registry */
    pSuite = CU_add_suite("cli_test_suite", init_suite, clean_suite);
    if (NULL == pSuite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* add the tests to the suite */
    if (0
        || NULL == CU_add_test(pSuite, "empty_command_gives_empty_result", empty_command_gives_empty_result)
        || NULL == CU_add_test(pSuite, "whitespace_command_gives_empty_result", whitespace_command_gives_empty_result)
        || NULL == CU_add_test(pSuite, "hash_comment_gives_comment_result", hash_comment_gives_comment_result)
        || NULL == CU_add_test(pSuite, "bang_comment_gives_comment_result", bang_comment_gives_comment_result)
        || NULL == CU_add_test(pSuite, "leading_whitespace_comment_gives_comment_result", leading_whitespace_comment_gives_comment_result)
        || NULL == CU_add_test(pSuite, "firewall_tag_gives_firewall_result", firewall_tag_gives_firewall_result)
        || NULL == CU_add_test(pSuite, "end_firewall_tag_gives_firewall_result", end_firewall_tag_gives_firewall_result)
        || NULL == CU_add_test(pSuite, "lines_after_opening_firewall_tag_give_firewall_result", lines_after_opening_firewall_tag_give_firewall_result)
        || NULL == CU_add_test(pSuite, "lines_after_firewall_end_tag_doesnt_give_firewall_result", lines_after_firewall_end_tag_doesnt_give_firewall_result)
        || NULL == CU_add_test(pSuite, "one_word_show_command_returns_incomplete", one_word_show_command_returns_incomplete)
        || NULL == CU_add_test(pSuite, "valid_two_word_show_command", valid_two_word_show_command)
        || NULL == CU_add_test(pSuite, "valid_two_word_show_command_with_trailing_whitespace", valid_two_word_show_command_with_trailing_whitespace)
        || NULL == CU_add_test(pSuite, "unknown_show_argument_gives_unknown_param_result", unknown_show_argument_gives_unknown_param_result)
       )
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Run all tests using the basic interface
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    printf("\n");
    CU_basic_show_failures(CU_get_failure_list());
    printf("\n\n");

/*
   // Run all tests using the automated interface
   CU_automated_run_tests();
   CU_list_tests_to_file();

   // Run all tests using the console interface
   CU_console_run_tests();
*/

    /* Clean up registry and return */
    number_of_failures = CU_get_number_of_failures();
    CU_cleanup_registry();

    // Ensure we return an error if any tests failed.
    if (number_of_failures)
        return -1;

    return CU_get_error();
}

