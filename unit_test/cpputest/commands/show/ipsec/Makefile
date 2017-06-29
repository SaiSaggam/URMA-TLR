##############################################################################
# Copyright (c) 2016 Digi International Inc., All Rights Reserved
#
# This software contains proprietary and confidential information of Digi
# International Inc.  By accepting transfer of this copy, Recipient agrees
# to retain this software in confidence, to prevent disclosure to others,
# and to make no use of this software other than that for which it was
# delivered.  This is an unpublished copyrighted work of Digi International
# Inc.  Except as permitted by federal law, 17 USC 117, copying is strictly
# prohibited.
#
# Restricted Rights Legend
#
# Use, duplication, or disclosure by the Government is subject to
# restrictions set forth in sub-paragraph (c)(1)(ii) of The Rights in
# Technical Data and Computer Software clause at DFARS 252.227-7031 or
# subparagraphs (c)(1) and (2) of the Commercial Computer Software -
# Restricted Rights at 48 CFR 52.227-19, as applicable.
#
# Digi International Inc. 11001 Bren Road East, Minnetonka, MN 55343
#
##############################################################################
#define the location of the cpputest framework Makefile
CPPUTEST_TOPDIR ?= $(CURDIR)/../../../../test_frameworks/cpputest
#define the directory where cpputest.mk can be found
UNIT_TEST_ROOT = $(CURDIR)/../../..

include $(UNIT_TEST_ROOT)/cpputest.mk

TLR_BASE ?= $(shell cd ../../../../..; pwd)
export TLR_BASE
include $(TLR_BASE)/build/makefile.globals

CODE_DIR = $(COMMANDS_BASE)/show/ipsec
INC_DIR = -I$(CODE_DIR) -I$(LIB_BASE)/config_mgr -I$(LIB_BASE)/libifuptime -I$(LIB_BASE)/libdigiutils/include 
INC_DIR += -I$(TLR_BASE)/include -I$(ROOTFS_BASE)/src/include -I/usr/local/include/uci -I$(OPENSRC_BASE)/json-c/src/

CC = gcc
CPPFLAGS += -g -Wall -Werror $(INC_DIR)
CFLAGS = $(CPPFLAGS) -DUNIT_TEST -fdata-sections -ffunction-sections --std=gnu99 -D_GNU_SOURCE -fprofile-arcs -ftest-coverage
LDFLAGS += -Wl,--gc-sections
LIBS += -lgcov
GCOV_ARTIFACTS=*.gcov *.gcda *.gcno
ALL_TEST_OBJECTS=*.to
ALL_PRODUCTION_OBJS=*.o

# define the test code
UTILS_TESTSRCS=AllTests.cpp test_utils.cpp
UTILS_TESTOBJS=$(subst .cpp,.to,$(UTILS_TESTSRCS))
UTILS_TESTOBJS:=$(subst .c,.o,$(UTILS_TESTOBJS))
# define the code to test
UTILS_SRCS=utils.c
UTILS_OBJS=$(subst .c,.o,$(UTILS_SRCS))
# other dependency sources
UTILS_TEST_DEPENDENCY_SRCS=get_field.c strip_whitespace.c
UTILS_TEST_DEPENDENCY_OBJS=$(subst .c,.o,$(UTILS_TEST_DEPENDENCY_SRCS))
# define the test executable name
UTILS_TEST_EXE=test_utils


# define the test code
SHOW_IPSEC_JSON_TESTSRCS=AllTests.cpp test_show_ipsec_json.cpp
SHOW_IPSEC_JSON_TESTOBJS=$(subst .cpp,.to,$(SHOW_IPSEC_JSON_TESTSRCS))
SHOW_IPSEC_JSON_TESTOBJS:=$(subst .c,.o,$(SHOW_IPSEC_JSON_TESTOBJS))
# define the code to test
SHOW_IPSEC_JSON_SRCS=show_ipsec_json.c
SHOW_IPSEC_JSON_OBJS=$(subst .c,.o,$(SHOW_IPSEC_JSON_SRCS))
# define the test executable name
SHOW_IPSEC_JSON_TEST_EXE=show_ipsec_json_utils

ALL_PRODUCTION_SRCS = \
					$(UTILS_SRCS) \
					$(SHOW_IPSEC_JSON_SRCS)

ALL_EXE = $(UTILS_TEST_EXE) \
		$(SHOW_IPSEC_JSON_TEST_EXE)

.PHONY: all
all: run coverage

.PHONY: run
run: $(ALL_EXE)
	@for exe in $^ ; do \
		./$$exe || exit 1 ; \
	done

$(SHOW_IPSEC_JSON_TEST_EXE): $(SHOW_IPSEC_JSON_TESTOBJS) $(SHOW_IPSEC_JSON_OBJS)
	@$(CXX) $(LDFLAGS) -o $@ $(SHOW_IPSEC_JSON_TESTOBJS) $(SHOW_IPSEC_JSON_OBJS) $(LIBS)

$(UTILS_TEST_EXE): $(UTILS_TESTOBJS) $(UTILS_OBJS) $(UTILS_TEST_DEPENDENCY_OBJS)
	@$(CXX) $(LDFLAGS) -o $@ $(UTILS_TESTOBJS) $(UTILS_OBJS) $(UTILS_TEST_DEPENDENCY_OBJS) $(LIBS)

%.o: %.c
	@$(CC) $(CFLAGS) -c -o $@ $<

%.o: $(CODE_DIR)/%.c
	@$(CC) $(CFLAGS) -c -o $@ $<

%.o: $(UTILS_BASE)/%.c
	@$(CC) $(CFLAGS) -c -o $@ $<

%.to: %.cpp
	$(CXX) -c $(CPPFLAGS) $(CODE_INC) $< -o $@

.PHONY: coverage
coverage:
	@gcov $(ALL_PRODUCTION_SRCS)

.PHONY: clean
clean:
	@rm -f  $(ALL_EXE) $(ALL_TEST_OBJECTS) $(ALL_PRODUCTION_OBJS) $(GCOV_ARTIFACTS)
