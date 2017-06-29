#define the location of the cpputest framework Makefile
CPPUTEST_TOPDIR ?= $(CURDIR)/../../../../test_frameworks/cpputest
#define the directory where cpputest.mk can be found
UNIT_TEST_ROOT = $(CURDIR)/../../..

CURRENT_DIR := $(shell pwd)

include $(UNIT_TEST_ROOT)/cpputest.mk

# base location of production code
TLR_BASE ?= $(shell cd ../../../../..; pwd)
include $(TLR_BASE)/build/makefile.globals

CODE_DIR=$(TOOLS_BASE)/snmp/sub_agent

CC=gcc
CXX=g++
RM=rm -f
CPPFLAGS+=-g \
			-include CppUTest/MemoryLeakDetectorNewMacros.h \
			-DUNIT_TEST

LIBPATH=
LIBS += -lgcov
LDFLAGS += -Wl,--gc-sections $(LIBPATH)
GCOV_ARTIFACTS=*.gcov *.gcda *.gcno
ALL_TEST_OBJECTS=*.to
ALL_PRODUCTION_OBJS=*.o


CODE_INC = -I$(TLR_BASE)/include \
			-I$(UTILS_BASE)/include \
			-I$(CODE_DIR) \
			-I$(LIB_BASE)/config_mgr \
			-I$(TLR_BASE)/sdk/rootfs/src/open_src/OSNMP/net-snmp-5.7.3/include \
			-I$(TLR_BASE)/sdk/rootfs/src/open_src/OSNMP/net-snmp-5.7.3/include/net-snmp

#C flags for the code under test

CFLAGS = $(CODE_INC) \
		-g \
		-Wall -Werror \
		--std=gnu99 \
		-fdata-sections -ffunction-sections \
		-D_GNU_SOURCE \
		-DUNIT_TEST \
		-fprofile-arcs -ftest-coverage
# XXX FIXME. with warnings treated as errors, the redefinition of config_error causes a problem. Redefine config_error -> tlr_config_error?
CFLAGS = $(CODE_INC) \
		-g \
		-Wall \
		--std=gnu99 \
		-fdata-sections -ffunction-sections \
		-D_GNU_SOURCE \
		-DUNIT_TEST \
		-fprofile-arcs -ftest-coverage


# define the test code
SUB_AGENT_TESTSRCS=AllTests.cpp test_sub_agent.cpp
SUB_AGENT_TESTOBJS=$(subst .cpp,.to,$(SUB_AGENT_TESTSRCS))
SUB_AGENT_TESTOBJS:=$(subst .c,.o,$(SUB_AGENT_TESTOBJS))
# define the code to test
SUB_AGENT_SRCS=sub_agent.c
SUB_AGENT_OBJS=$(subst .c,.o,$(SUB_AGENT_SRCS))

# define the test executable name
SUB_AGENT_TEST_EXE=test_sub_agent

# define the test code
REQUEST_HANDLERS_TESTSRCS=AllTests.cpp test_request_handlers.cpp
REQUEST_HANDLERS_TESTOBJS=$(subst .cpp,.to,$(REQUEST_HANDLERS_TESTSRCS))
REQUEST_HANDLERS_TESTOBJS:=$(subst .c,.o,$(REQUEST_HANDLERS_TESTOBJS))
# define the code to test
REQUEST_HANDLERS_SRCS=request_handlers.c
REQUEST_HANDLERS_OBJS=$(subst .c,.o,$(REQUEST_HANDLERS_SRCS))

# define the test executable name
REQUEST_HANDLERS_TEST_EXE=test_request_handlers

# define the test code
OID_TO_PARAMETER_TESTSRCS=AllTests.cpp test_oid_to_parameter.cpp
OID_TO_PARAMETER_TESTOBJS=$(subst .cpp,.to,$(OID_TO_PARAMETER_TESTSRCS))
OID_TO_PARAMETER_TESTOBJS:=$(subst .c,.o,$(OID_TO_PARAMETER_TESTOBJS))
# define the code to test
OID_TO_PARAMETER_SRCS=oid_to_parameter.c
OID_TO_PARAMETER_OBJS=$(subst .c,.o,$(OID_TO_PARAMETER_SRCS))

# define the test executable name
OID_TO_PARAMETER_TEST_EXE=test_oid_to_parameter

# define the test code
GET_PARAMETER_VALUE_TESTSRCS=AllTests.cpp test_get_parameter_value.cpp
GET_PARAMETER_VALUE_TESTOBJS=$(subst .cpp,.to,$(GET_PARAMETER_VALUE_TESTSRCS))
GET_PARAMETER_VALUE_TESTOBJS:=$(subst .c,.o,$(GET_PARAMETER_VALUE_TESTOBJS))
# define the code to test
GET_PARAMETER_VALUE_SRCS=get_parameter_value.c
GET_PARAMETER_VALUE_OBJS=$(subst .c,.o,$(GET_PARAMETER_VALUE_SRCS))

# define the test executable name
GET_PARAMETER_VALUE_TEST_EXE=test_get_parameter

# define the test code
FIND_ELEMENT_TESTSRCS=AllTests.cpp test_find_element.cpp
FIND_ELEMENT_TESTOBJS=$(subst .cpp,.to,$(FIND_ELEMENT_TESTSRCS))
FIND_ELEMENT_TESTOBJS:=$(subst .c,.o,$(FIND_ELEMENT_TESTOBJS))
# define the code to test
FIND_ELEMENT_SRCS=find_element.c
FIND_ELEMENT_OBJS=$(subst .c,.o,$(FIND_ELEMENT_SRCS))

# define the test executable name
FIND_ELEMENT_TEST_EXE=test_find_element

# define the test code
OID_TO_GROUP_TESTSRCS=AllTests.cpp test_oid_to_group.cpp
OID_TO_GROUP_TESTOBJS=$(subst .cpp,.to,$(OID_TO_GROUP_TESTSRCS))
OID_TO_GROUP_TESTOBJS:=$(subst .c,.o,$(OID_TO_GROUP_TESTOBJS))
# define the code to test
OID_TO_GROUP_SRCS=oid_to_group.c
OID_TO_GROUP_OBJS=$(subst .c,.o,$(OID_TO_GROUP_SRCS))

# define the test executable name
OID_TO_GROUP_TEST_EXE=test_oid_to_group

# define the test code
CREATE_NEW_OID_TESTSRCS=AllTests.cpp test_create_new_oid.cpp
CREATE_NEW_OID_TESTOBJS=$(subst .cpp,.to,$(CREATE_NEW_OID_TESTSRCS))
CREATE_NEW_OID_TESTOBJS:=$(subst .c,.o,$(CREATE_NEW_OID_TESTOBJS))
# define the code to test
CREATE_NEW_OID_SRCS=create_new_oid.c
CREATE_NEW_OID_OBJS=$(subst .c,.o,$(CREATE_NEW_OID_SRCS))

# define the test executable name
CREATE_NEW_OID_TEST_EXE=test_create_new_oid

# define the test code
GET_CONFIG_VALUE_TESTSRCS=AllTests.cpp test_get_config_value.cpp
GET_CONFIG_VALUE_TESTOBJS=$(subst .cpp,.to,$(GET_CONFIG_VALUE_TESTSRCS))
GET_CONFIG_VALUE_TESTOBJS:=$(subst .c,.o,$(GET_CONFIG_VALUE_TESTOBJS))
# define the code to test
GET_CONFIG_VALUE_SRCS=get_config_value.c
GET_CONFIG_VALUE_OBJS=$(subst .c,.o,$(GET_CONFIG_VALUE_SRCS))

# define the test executable name
GET_CONFIG_VALUE_TEST_EXE=test_get_config_value

# define the test code
SNMP_UTILS_TESTSRCS=AllTests.cpp test_snmp_utils.cpp
SNMP_UTILS_TESTOBJS=$(subst .cpp,.to,$(SNMP_UTILS_TESTSRCS))
SNMP_UTILS_TESTOBJS:=$(subst .c,.o,$(SNMP_UTILS_TESTOBJS))
# define the code to test
SNMP_UTILS_SRCS=snmp_utils.c
SNMP_UTILS_OBJS=$(subst .c,.o,$(SNMP_UTILS_SRCS))

# define the test executable name
SNMP_UTILS_TEST_EXE=test_snmp_utils

# define the test code
OID_TO_ELEMENT_TESTSRCS=AllTests.cpp test_oid_to_element.cpp
OID_TO_ELEMENT_TESTOBJS=$(subst .cpp,.to,$(OID_TO_ELEMENT_TESTSRCS))
OID_TO_ELEMENT_TESTOBJS:=$(subst .c,.o,$(OID_TO_ELEMENT_TESTOBJS))
# define the code to test
OID_TO_ELEMENT_SRCS=oid_to_element.c
OID_TO_ELEMENT_OBJS=$(subst .c,.o,$(OID_TO_ELEMENT_SRCS))

# define the test executable name
OID_TO_ELEMENT_TEST_EXE=test_oid_to_element

GET_SHOW_VALUE_TESTSRCS=AllTests.cpp test_get_show_value.cpp
GET_SHOW_VALUE_TESTOBJS=$(subst .cpp,.to,$(GET_SHOW_VALUE_TESTSRCS))
GET_SHOW_VALUE_TESTOBJS:=$(subst .c,.o,$(GET_SHOW_VALUE_TESTOBJS))
# define the code to test
GET_SHOW_VALUE_SRCS=get_show_value.c
GET_SHOW_VALUE_OBJS=$(subst .c,.o,$(GET_SHOW_VALUE_SRCS))

# define the test executable name
GET_SHOW_VALUE_TEST_EXE=test_get_show_value

READ_SHOW_ELEMENT_TESTSRCS=AllTests.cpp test_read_show_element.cpp
READ_SHOW_ELEMENT_TESTOBJS=$(subst .cpp,.to,$(READ_SHOW_ELEMENT_TESTSRCS))
READ_SHOW_ELEMENT_TESTOBJS:=$(subst .c,.o,$(READ_SHOW_ELEMENT_TESTOBJS))
# define the code to test
READ_SHOW_ELEMENT_SRCS=read_show_element.c
READ_SHOW_ELEMENT_OBJS=$(subst .c,.o,$(READ_SHOW_ELEMENT_SRCS))

# define the test code
SET_PARAMETER_VALUE_TESTSRCS=AllTests.cpp test_set_parameter_value.cpp
SET_PARAMETER_VALUE_TESTOBJS=$(subst .cpp,.to,$(SET_PARAMETER_VALUE_TESTSRCS))
SET_PARAMETER_VALUE_TESTOBJS:=$(subst .c,.o,$(SET_PARAMETER_VALUE_TESTOBJS))
# define the code to test
SET_PARAMETER_VALUE_SRCS=set_parameter_value.c
SET_PARAMETER_VALUE_OBJS=$(subst .c,.o,$(SET_PARAMETER_VALUE_SRCS))

# define the test executable name
SET_PARAMETER_VALUE_TEST_EXE=test_set_parameter

# define the test code
APPLY_CONFIG_TESTSRCS=AllTests.cpp test_apply_config.cpp
APPLY_CONFIG_TESTOBJS=$(subst .cpp,.to,$(APPLY_CONFIG_TESTSRCS))
APPLY_CONFIG_TESTOBJS:=$(subst .c,.o,$(APPLY_CONFIG_TESTOBJS))
# define the code to test
APPLY_CONFIG_SRCS=apply_config.c
APPLY_CONFIG_OBJS=$(subst .c,.o,$(APPLY_CONFIG_SRCS))

# define the test executable name
APPLY_CONFIG_TEST_EXE=test_apply_config


# define the test executable name
READ_SHOW_ELEMENT_TEST_EXE=test_read_show_element


ALL_PRODUCTION_SRCS = \
				$(SUB_AGENT_SRCS) \
				$(REQUEST_HANDLERS_SRCS) \
				$(OID_TO_PARAMETER_SRCS) \
				$(CREATE_NEW_OID_SRCS) \
				$(GET_CONFIG_VALUE_SRCS) \
				$(SNMP_UTILS_SRCS) \
				$(GET_PARAMETER_VALUE_SRCS) \
				$(SET_PARAMETER_VALUE_SRCS) \
				$(FIND_ELEMENT_SRCS) \
				$(OID_TO_GROUP_SRCS) \
				$(OID_TO_ELEMENT_SRCS) \
				$(GET_SHOW_VALUE_SRCS) \
				$(READ_SHOW_ELEMENT_SRCS) \
				$(APPLY_CONFIG_SRCS)

EXES=$(SUB_AGENT_TEST_EXE) \
				$(REQUEST_HANDLERS_TEST_EXE) \
				$(OID_TO_PARAMETER_TEST_EXE) \
				$(CREATE_NEW_OID_TEST_EXE) \
				$(GET_CONFIG_VALUE_TEST_EXE) \
				$(SNMP_UTILS_TEST_EXE) \
				$(GET_PARAMETER_VALUE_TEST_EXE) \
				$(SET_PARAMETER_VALUE_TEST_EXE) \
				$(FIND_ELEMENT_TEST_EXE) \
				$(OID_TO_GROUP_TEST_EXE) \
				$(OID_TO_ELEMENT_TEST_EXE) \
				$(GET_SHOW_VALUE_TEST_EXE) \
				$(READ_SHOW_ELEMENT_TEST_EXE) \
				$(APPLY_CONFIG_TEST_EXE) 
				
all: $(EXES) run coverage

$(SNMP_UTILS_TEST_EXE): $(SNMP_UTILS_TESTOBJS) $(SNMP_UTILS_OBJS)
	@$(CXX)  $(LDFLAGS) -o $@ $(SNMP_UTILS_TESTOBJS) $(SNMP_UTILS_OBJS) $(LIBS)

$(READ_SHOW_ELEMENT_TEST_EXE): $(READ_SHOW_ELEMENT_TESTOBJS) $(READ_SHOW_ELEMENT_OBJS)
	@$(CXX)  $(LDFLAGS) -o $@ $(READ_SHOW_ELEMENT_TESTOBJS) $(READ_SHOW_ELEMENT_OBJS) $(LIBS)

$(GET_SHOW_VALUE_TEST_EXE): $(GET_SHOW_VALUE_TESTOBJS) $(GET_SHOW_VALUE_OBJS)
	@$(CXX)  $(LDFLAGS) -o $@ $(GET_SHOW_VALUE_TESTOBJS) $(GET_SHOW_VALUE_OBJS) $(LIBS)

$(GET_CONFIG_VALUE_TEST_EXE): $(GET_CONFIG_VALUE_TESTOBJS) $(GET_CONFIG_VALUE_OBJS)
	@$(CXX)  $(LDFLAGS) -o $@ $(GET_CONFIG_VALUE_TESTOBJS) $(GET_CONFIG_VALUE_OBJS) $(LIBS)

$(CREATE_NEW_OID_TEST_EXE): $(CREATE_NEW_OID_TESTOBJS) $(CREATE_NEW_OID_OBJS)
	@$(CXX)  $(LDFLAGS) -o $@ $(CREATE_NEW_OID_TESTOBJS) $(CREATE_NEW_OID_OBJS) $(LIBS)

$(GET_PARAMETER_VALUE_TEST_EXE): $(GET_PARAMETER_VALUE_TESTOBJS) $(GET_PARAMETER_VALUE_OBJS)
	$(CXX)  $(LDFLAGS) -o $@ $(GET_PARAMETER_VALUE_TESTOBJS) $(GET_PARAMETER_VALUE_OBJS) $(LIBS)
	
$(SET_PARAMETER_VALUE_TEST_EXE): $(SET_PARAMETER_VALUE_TESTOBJS) $(SET_PARAMETER_VALUE_OBJS)
	$(CXX)  $(LDFLAGS) -o $@ $(SET_PARAMETER_VALUE_TESTOBJS) $(SET_PARAMETER_VALUE_OBJS) $(LIBS)
	
$(FIND_ELEMENT_TEST_EXE): $(FIND_ELEMENT_TESTOBJS) $(FIND_ELEMENT_OBJS)
	$(CXX)  $(LDFLAGS) -o $@ $(FIND_ELEMENT_TESTOBJS) $(FIND_ELEMENT_OBJS) $(LIBS)

$(OID_TO_GROUP_TEST_EXE): $(OID_TO_GROUP_TESTOBJS) $(OID_TO_GROUP_OBJS)
	$(CXX)  $(LDFLAGS) -o $@ $(OID_TO_GROUP_TESTOBJS) $(OID_TO_GROUP_OBJS) $(LIBS)

$(OID_TO_PARAMETER_TEST_EXE): $(OID_TO_PARAMETER_TESTOBJS) $(OID_TO_PARAMETER_OBJS)
	@$(CXX)  $(LDFLAGS) -o $@ $(OID_TO_PARAMETER_TESTOBJS) $(OID_TO_PARAMETER_OBJS) $(LIBS)

$(SUB_AGENT_TEST_EXE): $(SUB_AGENT_TESTOBJS) $(SUB_AGENT_OBJS)
	@$(CXX)  $(LDFLAGS) -o $@ $(SUB_AGENT_TESTOBJS) $(SUB_AGENT_OBJS) $(LIBS)

$(REQUEST_HANDLERS_TEST_EXE): $(REQUEST_HANDLERS_TESTOBJS) $(REQUEST_HANDLERS_OBJS)
	@$(CXX)  $(LDFLAGS) -o $@ $(REQUEST_HANDLERS_TESTOBJS) $(REQUEST_HANDLERS_OBJS) $(LIBS)
	
$(OID_TO_ELEMENT_TEST_EXE): $(OID_TO_ELEMENT_TESTOBJS) $(OID_TO_ELEMENT_OBJS)
	$(CXX)  $(LDFLAGS) -o $@ $(OID_TO_ELEMENT_TESTOBJS) $(OID_TO_ELEMENT_OBJS) $(LIBS)	

$(APPLY_CONFIG_TEST_EXE): $(APPLY_CONFIG_TESTOBJS) $(APPLY_CONFIG_OBJS)
	$(CXX)  $(LDFLAGS) -o $@ $(APPLY_CONFIG_TESTOBJS) $(APPLY_CONFIG_OBJS) $(LIBS)

run: $(EXES)
	@for exe in $^ ; do \
		./$$exe || exit 1 ; \
	done

coverage:
	@gcov $(ALL_PRODUCTION_SRCS)

clean:
	@$(RM) $(EXES) $(ALL_TEST_OBJECTS) $(ALL_PRODUCTION_OBJS) $(GCOV_ARTIFACTS)

dist-clean: clean

%.o: %.c
	@$(CC) $(CFLAGS) -c -o $@ $<

%.o: $(CODE_DIR)/%.c
	@$(CC) $(CFLAGS) -c -o $@ $<

%.to: %.cpp
	@$(CXX) -c $(CPPFLAGS) $(CODE_INC) $< -o $@

.PHONY: all clean coverage run dist-clean

