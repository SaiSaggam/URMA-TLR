#ifndef __TEST_INTERFACES_HELPER_H__
#define __TEST_INTERFACES_HELPER_H__

#include <stddef.h>

void test_bridge_interface_name_set(char const * const name);
void test_system_command_result_set(int result);
void test_system_commands_cleanup(void);
void test_system_command_call_count_init(size_t const expected_call_count);
char const * test_system_command_get(size_t const index);
size_t test_system_command_call_count_get(void);


#endif /* __TEST_INTERFACES_HELPER_H__ */
