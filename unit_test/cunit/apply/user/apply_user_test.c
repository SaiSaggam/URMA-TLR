#define	USE_BASIC_STYLE		1
#define	USE_AUTOMATED_STYLE	0
#define	USE_CONSOLE_STYLE	0

#if USE_BASIC_STYLE
#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#elif USE_AUTOMATED_STYLE
#include "CUnit/Automated.h"
#else
#include "CUnit/Console.h"
#endif
#include <apply_user.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>

/********************
	Header Files
********************/

#define MSG_LEN 64
#include "config_def.h"
#include "config_status.h"
#include "config_mgr.h"

void expect_system (char *cmd );
void expect_cnt(int cnt );

static char *expected_cmd = NULL;
static int  any_old_commands=0;

// Functions to test...
bool is_entry_found(char const * const filename, char const * const entryname);
void delete_user(char const * const username);
void add_group(void);
void add_user(char const * const user, size_t const instance);
int apply_user_config_change(size_t instance, char const * const cur_login, config_mgr_access_level_t const access);

typedef enum
{
    apply_user_test_case_super_add_user,
    apply_user_test_case_super_add_others,
    apply_user_test_case_invalid_instance,
    apply_user_test_case_user_root,
    apply_user_test_case_access_readonly,
    apply_user_test_case_user_exists,
    apply_user_test_case_rw_change_user,
    apply_user_test_case_rw_change_access,
    apply_user_test_case_rw_user_empty,
    apply_user_test_case_rw_others_password_empty,
    apply_user_test_case_rw_password_others,
    apply_user_test_case_rw_change_password,
    apply_user_test_case_super_user_empty,
    apply_user_test_case_super_password_empty,
    apply_user_test_case_last
} apply_user_test_case_t;

typedef struct
{
    char name[32];
    char passwd[32];
    char access[16];
} user_test_info_t;

typedef struct
{
    size_t instance;
    char const * login_name;
    user_test_info_t user[2];
    config_mgr_access_level_t access;
    int expected_result;
} apply_user_test_info_t;

static apply_user_test_info_t test_info;

/********************
	Test Suite Setup
********************/

int init_suite(void) { return 0; }
int clean_suite(void) { return 0; }

/********************
	Test Case Functions
********************/

static void update_test_user(size_t const instance, char const * const name, char const * const pw, char const * const access)
{
    user_test_info_t * puser = &test_info.user[instance - 1];

    test_info.instance = instance;

    if (name == NULL)
        puser->name[0] = '\0';
    else
        strcpy(puser->name, name);

    if (pw == NULL)
        puser->passwd[0] = '\0';
    else
        strcpy(puser->passwd, pw);

    strcpy(puser->access, access);
}


static apply_user_test_info_t * get_test_info(apply_user_test_case_t case_id)
{
    static char const user1[] = "user1";
    static char const user2[] = "user2";
    static char const pwd1[] = "passwd1";
    static char const pwd2[] = "passwd2";
    static char const super[] = "super";
    static char const rw[] = "rw";

    switch (case_id)
    {
        case apply_user_test_case_super_add_user:
            test_info.access = config_mgr_access_level_super;
            update_test_user(1, user1, pwd1, super);
            test_info.login_name = user1;
            test_info.expected_result = 0;
            break;

        case apply_user_test_case_super_add_others:
            test_info.access = config_mgr_access_level_super;
            update_test_user(2, user2, pwd2, rw);
            test_info.login_name = user1;
            test_info.expected_result = 0;
            break;

        case apply_user_test_case_invalid_instance:
            test_info.instance = 11;
            test_info.access = config_mgr_access_level_super;
            test_info.login_name = user1;
            test_info.expected_result = 1;
            break;

        case apply_user_test_case_user_root:
            test_info.access = config_mgr_access_level_super;
            update_test_user(1, "root", "passwd", super);
            test_info.login_name = user1;
            test_info.expected_result = 1;
            break;

        case apply_user_test_case_access_readonly:
            test_info.access = config_mgr_access_level_read_only;
            update_test_user(1, user1, pwd2, super);
            test_info.login_name = user1;
            test_info.expected_result = 1;
            break;

        case apply_user_test_case_user_exists:
            test_info.access = config_mgr_access_level_super;
            update_test_user(2, user1, pwd1, super);
            test_info.login_name = user1;
            test_info.expected_result = 1;
            break;

        case apply_user_test_case_rw_change_user:
            test_info.access = config_mgr_access_level_read_write;
            update_test_user(2, "usernew", pwd2, rw);
            test_info.login_name = user2;
            test_info.expected_result = 1;
            break;

        case apply_user_test_case_rw_change_access:
            test_info.access = config_mgr_access_level_read_write;
            update_test_user(2, user2, pwd2, super);
            test_info.login_name = user2;
            test_info.expected_result = 1;
            break;

        case apply_user_test_case_rw_user_empty:
            test_info.access = config_mgr_access_level_read_write;
            update_test_user(2, NULL, pwd2, rw);
            test_info.login_name = user2;
            test_info.expected_result = 1;
            break;

        case apply_user_test_case_rw_others_password_empty:
            test_info.access = config_mgr_access_level_read_write;
            update_test_user(1, user1, NULL, super);
            test_info.login_name = user2;
            test_info.expected_result = 1;
            break;

        case  apply_user_test_case_rw_password_others:
            test_info.access = config_mgr_access_level_read_write;
            update_test_user(1, user1, "test1", super);
            test_info.login_name = user2;
            test_info.expected_result = 1;
            break;

        case apply_user_test_case_rw_change_password:
            test_info.access = config_mgr_access_level_read_write;
            update_test_user(2, user2, "test2", rw);
            test_info.login_name = user2;
            test_info.expected_result = 0;
            break;

        case apply_user_test_case_super_user_empty:
            test_info.access = config_mgr_access_level_super;
            update_test_user(2, NULL, "test2", rw);
            test_info.login_name = user1;
            test_info.expected_result = 0;
            break;

        case apply_user_test_case_super_password_empty:
            test_info.access = config_mgr_access_level_super;
            update_test_user(2, user2, NULL, rw);
            test_info.login_name = user1;
            test_info.expected_result = 0;
            break;

        default:
            break;
    }

    return &test_info;
}

void is_entry_found_test(void)
{
    bool    result;

    // Test when file cannot open
    result = is_entry_found("passwd", "user1");
    CU_ASSERT_FALSE(result);

    // Test OPens but fails read
    result = is_entry_found("test_passwd", "nick");
    CU_ASSERT_FALSE(result);

    // And now succeeding.
    result = is_entry_found("test_passwd", "user1");
    CU_ASSERT_TRUE(result);
}

void delete_user_test(void)
{
    expect_system ("/usr/sbin/deluser user2");
    delete_user("user2");
    // and test we have had a system() call so we could check for the deluser command
    CU_ASSERT_EQUAL(expected_cmd, NULL);


    // Check when cannot find the name nothing happens.
    expect_system("/usr/sbin/deluser harry");
    delete_user("harry");
    // And test we have NOT had a system call.
    CU_ASSERT_NOT_EQUAL(expected_cmd, NULL);
}

void add_group_test(void)
{
    expect_system ("/usr/sbin/addgroup -g 2000 testers");
    add_group();
    // and test we have had a system() call
    CU_ASSERT_EQUAL(expected_cmd, NULL);
}

void add_user_test(void)
{
    int     instance=0;
    int     user_id = 2000 + instance;
    char    expect_buf[100];

    snprintf(expect_buf, sizeof expect_buf, "/usr/sbin/adduser -DHh /home/digi/user -s /bin/sh_wrapper -u %d nick -G testers", user_id);
    expect_system(expect_buf);
    add_user("nick", instance);
    // and test we have had a system() call
    CU_ASSERT_EQUAL(expected_cmd, NULL);
}

static void apply_user_config_change_test(void)
{
    apply_user_test_case_t case_id;
    int actual_result;

    memset(&test_info, 0, sizeof test_info);
    any_old_commands = apply_user_test_case_last;
    for (case_id = apply_user_test_case_super_add_user; case_id < apply_user_test_case_last; case_id++)
    {
        apply_user_test_info_t const * const pinfo = get_test_info(case_id);

        actual_result = apply_user_config_change(pinfo->instance, pinfo->login_name, pinfo->access);
        if (pinfo->expected_result != actual_result)
        {
            printf("Failed:%d, [%d/%d]\n", case_id, pinfo->expected_result, actual_result);
        }

        CU_ASSERT_EQUAL(pinfo->expected_result, actual_result);
    }

    return;

}

/********************
	Test Runner
********************/
int main() {
	unsigned int number_of_failures;

	//	Initialize the CUnit test registry
	if ( CUE_SUCCESS != CU_initialize_registry() )
		return CU_get_error();

	//	Add a suite to the registry
	CU_pSuite pSuite = CU_add_suite( "apply_user_test_suite", init_suite, clean_suite );
	if ( pSuite == NULL ) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	// Add the tests to the suite
	if (
		(CU_add_test(pSuite, "is_entry_found_test", is_entry_found_test)==NULL) ||
		(CU_add_test(pSuite, "delete_user_test", delete_user_test)==NULL) ||
		(CU_add_test(pSuite, "add_group_test", add_group_test)==NULL) ||
		(CU_add_test(pSuite, "add_user_test", add_user_test)==NULL) ||
		(CU_add_test(pSuite, "apply_user_config_change_test", apply_user_config_change_test)==NULL)

		)
	{
		CU_cleanup_registry();
		return CU_get_error();
	}

	#if USE_BASIC_STYLE
		//	Run all tests using the basic interface
		CU_basic_set_mode(CU_BRM_VERBOSE);
		CU_basic_run_tests();
		CU_basic_show_failures(CU_get_failure_list());
	#elif USE_AUTOMATED_STYLE
		//	Run all tests using the automated interface
		CU_automated_run_tests();
		CU_list_tests_to_file();
	#else
		//	Run all tests using the console interface
		CU_console_run_tests();
	#endif

	//	Clean up registry and return
	number_of_failures = CU_get_number_of_failures();
	CU_cleanup_registry();

	//	Ensure we return an error if any tests failed
	if (number_of_failures)
		return -1;

	return CU_get_error();
}

/********************
	Stub Functions
********************/

config_status_t tlr_get_element_value(size_t const group_id,
                                      size_t const instance,
                                      size_t const element_id,
                                      void * const value,
                                      size_t const bytes,
                                      config_mgr_element_type_t const type,
                                      config_mgr_access_level_t const access)
{
    config_status_t status = config_status_success;
    user_test_info_t * puser;
    char * const data = value;

    if ((instance > 10) || (instance == 0))
    {
        status = config_status_instance_not_found;
        goto done;
    }

    if (group_id != config_user)
    {
        status = config_status_group_not_found;
        goto done;
    }

    if (instance > 2)
    {
        *data = '\0';
        goto done;
    }

    puser = &test_info.user[instance - 1];

    if (group_id != config_user)
    {
        status = config_status_group_not_found;
        goto done;
    }

    switch (element_id)
    {
        case config_user_name:
            strcpy(data, puser->name);
            break;

        case config_user_password:
            strcpy(data, puser->passwd);
            break;

        case config_user_access:
            strcpy(data, puser->access);
            break;

        default:
            status = config_status_element_not_found;
            break;
    }

done:
    return status;
}

void const * tlr_get_table(config_mgr_table_type_t const type)
{
    void const * table = NULL;

    switch (type)
    {
        case config_mgr_table_type_show:
            table = &config_mgr_group_table[config_mgr_get_group];
            break;

        case config_mgr_table_type_config:
            table = &config_mgr_group_table[config_mgr_set_group];
            break;

        case config_mgr_table_type_action:
            table = &config_mgr_action_table[0];
            break;

    }

    return table;
}

bool tlr_printf(int type, char const * const format, ...)
{
    (void)type;
    (void)format;

    return true;
}

config_status_t tlr_set_element_value(size_t const group_id,
                                      size_t const instance,
                                      size_t const element_id,
                                      char const * const value,
                                      config_mgr_access_level_t const access)
{

    config_status_t status = config_status_success;
    user_test_info_t * puser;

    if ((instance > 10) || (instance == 0))
    {
        status = config_status_instance_not_found;
        goto done;
    }

    if (group_id != config_user)
    {
        status = config_status_group_not_found;
        goto done;
    }

    if (instance > 2)
    {
        goto done;
    }

    puser = &test_info.user[instance - 1];

    switch (element_id)
    {
        case config_user_name:
            strcpy(puser->name, value);
            break;

        case config_user_password:
            strcpy(puser->passwd, value);
            break;

        case config_user_access:
            strcpy(puser->access, value);
            break;

        default:
            status = config_status_element_not_found;
            break;
    }

done:
    return status;
}

void expect_system (char *cmd )
{
    any_old_commands=0;

    if ( expected_cmd != NULL )
        free(expected_cmd);
    if ( cmd!=NULL && *cmd != 0 )
        {
        expected_cmd = strdup(cmd);
        }
    else
        expected_cmd = NULL;
}

void expect_cnt(int cnt )
{
    any_old_commands=cnt;
}


int system_stub (char const * const cmd )
{

    if ( any_old_commands != 0 )
        {
        any_old_commands--;
        }
    else
        {
        if ( expected_cmd != NULL )
            {
            if ( strcmp(expected_cmd, cmd ) != 0 )
                printf ( "expected:%s. got:%s\n", expected_cmd, cmd );
            CU_ASSERT_STRING_EQUAL( expected_cmd, cmd );
            free(expected_cmd);
            expected_cmd = NULL;
            }
        else
            CU_FAIL("not expecting a system command." );
        }

    return 0;
}

void tlr_popen()
{
}

void tlr_pclose()
{
}

void tlr_system_command (char const * const format, ...)
{
    char * command = NULL;
    va_list args;

    va_start(args, format);
    if (vasprintf(&command, format, args) > 0)
    {
        system_stub(command);
        free(command);
    }
    va_end(args);
}

void system_command (char const * const format, ...)
{
    char * command = NULL;
    va_list args;

    va_start(args, format);
    if (vasprintf(&command, format, args) > 0)
    {
        system_stub(command);
        free(command);
    }
    va_end(args);
}


