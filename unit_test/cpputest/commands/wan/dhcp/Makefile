##############################################################################
# Copyright (c) 2015 Digi International Inc., All Rights Reserved
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

SRC_DIR  = $(TLR_BASE)/tools/ifmanager
SRC_DIR1 = $(TLR_BASE)/tools/eth_monitor
SRC_DIR2 = $(TLR_BASE)/lib/message

INC_DIR = -I. -I$(SRC_DIR) -I$(SRC_DIR1) -I$(SRC_DIR2) -I$(TLR_BASE)/lib/message/include -I$(LIB_BASE)/config_mgr -I$(LIB_BASE)/libifuptime -I$(LIB_BASE)/libdigiutils/include 
INC_DIR += -I$(TLR_BASE)/include -I$(ROOTFS_BASE)/src/include -I/usr/local/include/uci -I$(OPENSRC_BASE)/json-c/src/
INC_DIR += -I$(TLR_BASE)/commands/show/eth_stats -I$(TLR_BASE)/lib_config_mgr

CC = gcc
CPP = g++
CPPFLAGS += -g -Wall -Werror $(INC_DIR)
#CPPFLAGS += -g $(INC_DIR)
CFLAGS = $(CPPFLAGS) -DUNIT_TEST -fdata-sections -ffunction-sections --std=gnu99 -D_GNU_SOURCE -fprofile-arcs -ftest-coverage
LDFLAGS += -Wl,--gc-sections
LIBS += -lgcov

IFMWAN_TEST_EXE      = ifmwan_test
IFMWAN_TEST_OBJS     = ifmwan_test.o
IFMWAN_TEST_SRC_OBJS = ifmwan.o

DHCP_CLIENT_TEST_EXE      = dhcp_client_test
DHCP_CLIENT_TEST_OBJS     = dhcp_client_test.o
DHCP_CLIENT_TEST_SRC_OBJS = dhcp_client.o

PARSING_TEST_EXE      = get_wan_message_info_test
PARSING_TEST_OBJS     = get_wan_message_info_test.o
PARSING_TEST_SRC_OBJS = parsing_get_wan_message_info.o

APPLY_TEST_EXE      = apply_eth_func_test
APPLY_TEST_OBJS     = apply_eth_func_test.o
APPLY_TEST_SRC_OBJS = apply_eth_func.o

ALL_EXE = $(IFMWAN_TEST_EXE) $(DHCP_CLIENT_TEST_EXE) $(PARSING_TEST_EXE) $(APPLY_TEST_EXE)

.PHONY: all
all: $(ALL_EXE) run coverage

.PHONY: run
run: $(ALL_EXE)
	@for exe in $^ ; do \
		./$$exe || exit 1 ; \
	done

$(IFMWAN_TEST_EXE): $(IFMWAN_TEST_OBJS) $(IFMWAN_TEST_SRC_OBJS)
	@$(CPP) $(LDFLAGS) -o $@ $^ $(LIBS)

$(DHCP_CLIENT_TEST_EXE): $(DHCP_CLIENT_TEST_OBJS) $(DHCP_CLIENT_TEST_SRC_OBJS)
	@$(CPP) $(LDFLAGS) -o $@ $^ $(LIBS)

$(PARSING_TEST_EXE): $(PARSING_TEST_OBJS) $(PARSING_TEST_SRC_OBJS)
	@$(CPP) $(LDFLAGS) -o $@ $^ $(LIBS)

$(APPLY_TEST_EXE): $(APPLY_TEST_OBJS) $(APPLY_TEST_SRC_OBJS)
	@$(CPP) $(LDFLAGS) -o $@ $^ $(LIBS)

%.o: %.c
	@$(CC) $(CFLAGS) -c -o $@ $<
%.o: $(SRC_DIR)/%.c
	@$(CC) $(CFLAGS) -c -o $@ $<
%.o: $(SRC_DIR1)/%.c
	@$(CC) $(CFLAGS) -c -o $@ $<
%.o: $(SRC_DIR2)/%.c
	@$(CC) $(CFLAGS) -c -o $@ $<
%.o: %.cpp
	@$(CPP) $(CPPFLAGS) -c -o $@ $<

.PHONY: coverage
coverage:
	gcov $(IFMWAN_TEST_SRC_OBJS)
	gcov $(DHCP_CLIENT_TEST_SRC_OBJS)
	gcov $(PARSING_TEST_SRC_OBJS)
	gcov $(APPLY_TEST_SRC_OBJS)

.PHONY: clean
clean:
	@rm -f *~ *.o *.c.gcov *.gcda *.gcno $(ALL_EXE)
