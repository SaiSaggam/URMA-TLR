#ifndef _TEST_IFMANAGER_H_
#define _TEST_IFMANAGER_H_
#include <syslog.h>

int add_ifmanager_tests(void);
int add_ifmessage_tests(void);
int add_failover_tests(void);
int add_route_tests(void);

extern int if_manager_start(void);
extern int ifm_unit_test_result;

#endif

