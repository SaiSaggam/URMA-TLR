
#define     USE_BASIC_STYLE     1
#define     USE_AUTOMATED_STYLE     0
#define     USE_CONSOLE_STYLE     0

#if USE_BASIC_STYLE
#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#elif USE_AUTOMATED_STYLE
#include "CUnit/Automated.h"
#else
#include "CUnit/Console.h"
#endif

#include <stdio.h>
#include <string.h>


/** Cli definitions we'll need for our tests **/
#include    "cli.h"
#include    "line_ed.h"
#include    "cli_timeout.h"

cli_t *init_cli (void);
void   add_command_to_history(cli_t *context);


/* Test Suite setup and cleanup functions: */

int init_suite(void) { return 0; }
int clean_suite(void) { return 0; }


/************* Test case functions ****************/
static cli_t *cli;


void cli_init(void)
{
if ((cli = init_cli ()) == NULL)
  {
  CU_FAIL_FATAL("Failed to init cli");
  }
}

void cli_history_test(void)
{
    // cli should be non null, and history null to start with.
    CU_ASSERT_PTR_NOT_EQUAL_FATAL(cli, NULL);
    CU_ASSERT_PTR_NULL(cli->history[0]);

    // Right , lets test we remember commands in history correctly
    strcpy ( cli->line, "Gidday" );
    add_command_to_history(cli);
    CU_ASSERT_PTR_NOT_EQUAL_FATAL(cli->history[0], NULL);
    CU_ASSERT_STRING_EQUAL(cli->history[0], "Gidday" );

    // Add something else..
    strcpy ( cli->line, "Hello World" );
    add_command_to_history(cli);
    CU_ASSERT_PTR_NOT_EQUAL_FATAL(cli->history[0], NULL);
    CU_ASSERT_PTR_NOT_EQUAL_FATAL(cli->history[1], NULL);
    CU_ASSERT_PTR_EQUAL_FATAL(cli->history[2], NULL);
    CU_ASSERT_STRING_EQUAL(cli->history[0], "Hello World" );    // checking latest one is first.
    CU_ASSERT_STRING_EQUAL(cli->history[1], "Gidday" );


    // Ok, now add twice as many as we have history size, and check we have a full remaining history
    {
        int     idx, chkidx;
        char    buffer[40];

        for ( idx=0; idx< 2*MAX_HISTORY; idx++ )
        {
            sprintf ( buffer, "cmd %d", idx );
            strcpy ( cli->line, buffer );
            add_command_to_history(cli);

            if(idx >= DEFAULT_HISTORY && idx < MAX_HISTORY)
                CU_ASSERT_PTR_EQUAL_FATAL(cli->history[idx], NULL);
        }

        // Now check history is what it should be
        chkidx = 2*MAX_HISTORY - 1;     // As last one added should be the first.
        for ( idx=0; idx< DEFAULT_HISTORY; idx++ )
        {
            sprintf ( buffer, "cmd %d", chkidx-- );
            CU_ASSERT_PTR_NOT_NULL_FATAL(cli->history[idx]);
            CU_ASSERT_STRING_EQUAL(cli->history[idx],buffer);
        }

        // test clear_command_history()
        clear_command_history(cli);
        for ( idx=0; idx< DEFAULT_HISTORY; idx++ )
        {
            CU_ASSERT_PTR_NULL_FATAL(cli->history[idx]);
        }
    }
}


// To test the cli function "line_edit()" we need to simulate the
// call it makes to 'os_getc()' to provide it with the simulated test input keystrokes.
static char        charinput[40];
static char        *pinput = NULL;
static char         inlen=0;

// Define the test input keystrokes we want to sumulate
void setcharinput (char *s, int len)
{
    // if len is 0 just use the string length
    if ( len == 0 )
        len = strlen(s);
    inlen = len;
    memcpy (charinput, s, len);
    pinput = charinput;
}

// Simulated character input (called by line_edit function under test).
int os_getc (cli_t *cli)
{
    int     ret=0;

    if ( inlen && pinput != NULL)
        {
        ret = (int) *pinput++;
        inlen--;
        if (inlen == 0 )
            pinput = NULL;
        }
    else
        CU_FAIL_FATAL("no input keystrokes");

    return ret;
}

void cli_edit_test()
{
    char    sim_in[40];

    setcharinput ( "Hello world\n", 0 );      // Our simulated input
    cli->line[0] = 0;
    switch ( line_edit (cli) )
        {
        case INPUT_ENTER:
            CU_ASSERT_STRING_EQUAL(cli->line, "Hello world" );
            break;
        case INPUT_QUESTIONMARK:  CU_FAIL("?");      break;
        case INPUT_TAB:           CU_FAIL("tab");    break;
        case INPUT_CTRL_Z:        CU_FAIL("Ctrl-z"); break;
        }

    // Test a backspace.
    setcharinput ( "\b\n", 0 );
    switch ( line_edit (cli) )
        {
        case INPUT_ENTER:
            CU_ASSERT_STRING_EQUAL(cli->line, "Hello worl" );
            break;
        case INPUT_QUESTIONMARK:  CU_FAIL("?");      break;
        case INPUT_TAB:           CU_FAIL("tab");    break;
        case INPUT_CTRL_Z:        CU_FAIL("Ctrl-z"); break;
        }

    // Test Ctrl-U delete the line
    sim_in[0] = CTRL_U;
    sim_in[1] = '\n';
    setcharinput ( sim_in, 2 );

    switch ( line_edit (cli) )
        {
        case INPUT_ENTER:
           CU_ASSERT_STRING_EQUAL(cli->line, "" );
           break;
        case INPUT_QUESTIONMARK:  CU_FAIL("?");      break;
        case INPUT_TAB:           CU_FAIL("tab");    break;
        case INPUT_CTRL_Z:        CU_FAIL("Ctrl-z"); break;
        }

    // etc, etc



}


void cli_history_edit_test()
{
    char    sim_in[40];

    // Stick a few commands in history first
    strcpy ( cli->line, "edittest cmd 1\n" );
    add_command_to_history(cli);
    strcpy ( cli->line, "edittest cmd 2\n" );
    add_command_to_history(cli);
    strcpy ( cli->line, "edittest cmd 3\n" );
    add_command_to_history(cli);


    // Recall history once
    cli->line[0] = 0;
    sim_in[0] = CTRL_P;
    sim_in[1] = '\n';
    setcharinput ( sim_in, 2 );
    switch ( line_edit (cli) )
        {
        case INPUT_ENTER:
           // Li.... to be consistant we normal returns.. we shouldn't provide the '\n'
           // at end just because it was a history recall I reckon.
           // Nick.... This is exactly what was stored in the history.
           // In the real cli application, only the successful command will be added in to the history,
           // those commands will not have '\n' at end.
           CU_ASSERT_STRING_EQUAL(cli->line, "edittest cmd 3\n" );
           break;
        case INPUT_QUESTIONMARK:  CU_FAIL("?");      break;
        case INPUT_TAB:           CU_FAIL("tab");    break;
        case INPUT_CTRL_Z:        CU_FAIL("Ctrl-z"); break;
        }

    // Test for standard input after a history recall.
    setcharinput ( "Hello world\n", 0 );
    cli->line[0] = 0;
    cli->editpos = 0;
    switch ( line_edit (cli) )
        {
        case INPUT_ENTER:
           CU_ASSERT_STRING_EQUAL(cli->line, "Hello world" );
           break;
        case INPUT_QUESTIONMARK:  CU_FAIL("?");      break;
        case INPUT_TAB:           CU_FAIL("tab");    break;
        case INPUT_CTRL_Z:        CU_FAIL("Ctrl-z"); break;
        }

    // Recall history twice
    cli->line[0] = 0;
    sim_in[0] = CTRL_P;
    sim_in[1] = CTRL_P;
    sim_in[2] = '\n';
    setcharinput ( sim_in, 3 );
    switch ( line_edit (cli) )
        {
        case INPUT_ENTER:
           // Li.... to be consistent we normal returns.. we shouldn't provide the '\n' at end just
           // because it was a history recall I reckon. Nick.... This is exactly what was stored in
           // the history.
           CU_ASSERT_STRING_EQUAL(cli->line, "edittest cmd 2\n" );
           break;
        case INPUT_QUESTIONMARK:  CU_FAIL("?");      break;
        case INPUT_TAB:           CU_FAIL("tab");    break;
        case INPUT_CTRL_Z:        CU_FAIL("Ctrl-z"); break;
        }

    // OK go back in history three times in history and forward once.
    // Should get to the same command.
    cli->line[0] = 0;
    sim_in[0] = CTRL_P;
    sim_in[1] = CTRL_P;
    sim_in[2] = CTRL_P;
    sim_in[3] = CTRL_N;
    sim_in[4] = '\n';
    setcharinput ( sim_in, 5 );
    switch ( line_edit (cli) )
        {
        case INPUT_ENTER:
           // Li.... to be consistant we normal returns.. we shouldn't provide the '\n'
           // at end just because it was a history recall I reckon.
           // Nick.... This is exactly what was stored in the history.
           // In the real cli application, only the successful command will be added in to the history,
           // those commands will not have '\n' at end.
           CU_ASSERT_STRING_EQUAL(cli->line, "edittest cmd 2\n" );
           break;
        case INPUT_QUESTIONMARK:  CU_FAIL("?");      break;
        case INPUT_TAB:           CU_FAIL("tab");    break;
        case INPUT_CTRL_Z:        CU_FAIL("Ctrl-z"); break;
        }


}

size_t get_string_line_length(char const * const str, size_t const max_str_length);

void cli_get_string_line_length_test()
{

    char string[] = "12 45 78 AB DE ";

    size_t length;
    size_t max_width = 10;

    length = get_string_line_length("", max_width);
    CU_ASSERT_EQUAL(length, 0);

    /* This should be less than max_width.
     * It should the length for "12 45 78 "
     */
    max_width = 10;
    length = get_string_line_length(string, max_width);
    CU_ASSERT_EQUAL(length, 9);

    /* Make max_width as same the string length.
     * Should return the length of the string.
     */
    max_width = strlen(string);
    length = get_string_line_length(string, max_width);
    CU_ASSERT_EQUAL(length, max_width);

    {
        /*
         * No spaces so expect the return length same
         * as max_width.
         */
        char no_space_string[] = "1234567890";
        max_width = strlen(no_space_string);

        length = get_string_line_length(no_space_string, max_width);
        CU_ASSERT_EQUAL(length, max_width);
    }

}

/************* Test Runner Code goes here **************/

int main ( void )
{
   CU_pSuite pSuite = NULL;
   unsigned int number_of_failures;

   /* initialize the CUnit test registry */
   if ( CUE_SUCCESS != CU_initialize_registry() )
      return CU_get_error();

   /* add a suite to the registry */
   pSuite = CU_add_suite( "cli_test_suite", init_suite, clean_suite );
   if ( NULL == pSuite ) {
      CU_cleanup_registry();
      return CU_get_error();
   }

   /* add the tests to the suite */
   if ( (NULL == CU_add_test(pSuite, "cli_init", cli_init)) ||
        (NULL == CU_add_test(pSuite, "cli_history", cli_history_test)) ||
        (NULL == CU_add_test(pSuite, "cli_edit", cli_edit_test)) ||
        (NULL == CU_add_test(pSuite, "cli_history", cli_history_edit_test)) ||
        (NULL == CU_add_test(pSuite, "cli_get_string_line_length_test", cli_get_string_line_length_test))

        )
   {
      CU_cleanup_registry();
      return CU_get_error();
   }

#if USE_BASIC_STYLE
   // Run all tests using the basic interface
   CU_basic_set_mode(CU_BRM_VERBOSE);
   CU_basic_run_tests();
   printf("\n");
   CU_basic_show_failures(CU_get_failure_list());
   printf("\n\n");

#endif


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




/****************  Stubs   ***************************/
void os_end(cli_t * const context)
{
}

cli_status_t cli_get_string_value(cli_tree_type_t const tree_type, char const * const group_name,
                                   size_t const instance, char const * const element_name, char const ** value)
{
    return cli_config_error;
}




// Output functions.. Can be used to check for correct printing...from functions under test.
void os_putc (char c, cli_t *context)
{
}

void os_flush (cli_t *context)
{
}

void cli_puts (char *string, cli_t *context)
{

}



/*************** Auto complete stubs   ****************/

void const * tlr_get_table(config_mgr_table_type_t const type)
{
    return NULL;
}


void cli_get_timeout(cli_t * const cli)
{
    cli->cli_timeout_secs = 180;
}

void calculate_next_logout_secs (cli_t * const cli)
{
    cli->next_logout_secs = 0;
}



bool tlr_user_is_authorized(char const * const username, char const * const password, config_mgr_access_level_t * level, bool const user_is_logged_in)
{
    return true;
}



config_status_t tlr_apply_read(tlr_handle_t const handle, char * const data, size_t const data_length, size_t const timeout)
{
    return config_status_success;
}

int tlr_apply_close(tlr_handle_t const handle)
{
    return 0;
}

tlr_handle_t tlr_table_hook_open(size_t const id, size_t const instance, size_t const element_id , char const * value, char const *pre_post,
    char    const * const username, config_mgr_access_level_t  cur_access )
{
    return NULL;
}

config_status_t tlr_table_hook_read(tlr_handle_t const handle, char * const data, size_t const data_length, size_t const timeout)
{
    return config_status_success;
}

int tlr_table_hook_close(tlr_handle_t const handle)
{
    return 0;
}


config_status_t tlr_get_element_value(size_t const group_id,
									  size_t const instance,
									  size_t const element_id,
									  void * const value,
									  size_t const bytes,
									  config_mgr_element_type_t const type,
									  config_mgr_access_level_t const access)
{
    return config_status_success;
}

bool tlr_user_is_pwdset (char const * const password)
{
    return true;
}

size_t tlr_get_element_size(config_mgr_element_t const * const element)
{
    return 1024;
}

