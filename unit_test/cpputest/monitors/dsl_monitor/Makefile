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

# Define the location of the cpputest framework Makefile.
CPPUTEST_TOPDIR ?= $(CURDIR)/../../../test_frameworks/cpputest

# Define the directory where cpputest.mk can be found.
UNIT_TEST_ROOT = $(CURDIR)/../..

include $(UNIT_TEST_ROOT)/cpputest.mk

TLR_BASE ?= $(shell cd ../../../..; pwd)
export TLR_BASE
include $(TLR_BASE)/build/makefile.globals

SRC_DIR = $(TOOLS_BASE)/dsl_monitor
INC_DIR = -I. -I$(SRC_DIR) -I$(LIB_BASE)/config_mgr -I$(LIB_BASE)/libdigiutils/include -I$(TLR_BASE)/include -I$(MESSAGE_BASE)/include -I$(ROOTFS_BASE)/src/include

CC = gcc
CPP = g++
CPPFLAGS += -g -Wall -Werror $(INC_DIR)
CFLAGS += $(CPPFLAGS) -fdata-sections -ffunction-sections --std=gnu99 -D_GNU_SOURCE -fprofile-arcs -ftest-coverage -DDSL_LOG_MODE=0
LDFLAGS += -Wl,--gc-sections
LIBS += -lgcov

TEST_DSL_EVENT_HANDLERS_TESTEXE = test_dsl_event_handlers
TEST_DSL_EVENT_HANDLERS_TESTOBJS = main.o test_dsl_event_handlers.o
TEST_DSL_EVENT_HANDLERS_SRCOBJS = dsl_event_handlers.o

ALL_EXE = $(TEST_DSL_EVENT_HANDLERS_TESTEXE)

.PHONY: all
all: $(ALL_EXE) run coverage

.PHONY: run
run: $(ALL_EXE)
	@for exe in $^ ; do \
		./$$exe || exit 1 ; \
	done

$(TEST_DSL_EVENT_HANDLERS_TESTEXE): $(TEST_DSL_EVENT_HANDLERS_TESTOBJS) $(TEST_DSL_EVENT_HANDLERS_SRCOBJS)
	@$(CPP) $(LDFLAGS) -o $@ $^ $(LIBS)

%.o: %.c
	@$(CC) $(CFLAGS) -c -o $@ $<
%.o: $(SRC_DIR)/%.c
	@$(CC) $(CFLAGS) -c -o $@ $<
%.o: %.cpp
	@$(CPP) $(CPPFLAGS) -c -o $@ $<

.PHONY: coverage
coverage:
	gcov $(TEST_DSL_EVENT_HANDLERS_SRCOBJS)

.PHONY: clean
clean:
	@rm -f *.o *.c.gcov *.gcda *.gcno $(ALL_EXE)
