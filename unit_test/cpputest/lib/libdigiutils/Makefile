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
CPPUTEST_TOPDIR ?= $(CURDIR)/../../../test_frameworks/cpputest
#define the directory where cpputest.mk can be found
UNIT_TEST_ROOT = $(CURDIR)/../..

include $(UNIT_TEST_ROOT)/cpputest.mk

TLR_BASE ?= $(shell cd ../../../..; pwd)
export TLR_BASE
include $(TLR_BASE)/build/makefile.globals

SRC_DIR = $(LIB_BASE)/libdigiutils
INC_DIR = -I. -I$(SRC_DIR) -I$(LIB_BASE)/config_mgr -I$(LIB_BASE)/libdigiutils/include -I$(TLR_BASE)/include -I$(ROOTFS_BASE)/src/include -I/usr/local/include/uci

CC = gcc
CPP = g++
CPPFLAGS += -g -Wall -Werror $(INC_DIR)
CFLAGS += $(CPPFLAGS) -fdata-sections -ffunction-sections --std=gnu99 -D_CUNIT_ -D_GNU_SOURCE -fprofile-arcs -ftest-coverage
LDFLAGS += -Wl,--gc-sections 
LIBS += -lgcov -lpthread

LIBDIGIUTILS_TEST_EXE = test_libdigiutils
LIBDIGIUTILS_TEST_OBJS = main.o test_bank.o test_tlr_print.o test_tlr_popen.o test_show_status_helper.o
LIBDIGIUTILS_SRC_OBJS = bank.o tlr_print.o tlr_popen.o strip_whitespace.o show_status_helper.o

CREATE_FILE_LIST_TEST_EXE = test_create_file_list
CREATE_FILE_LIST_TEST_OBJS = main.o test_create_file_list.o
CREATE_FILE_LIST_SRC_OBJS = create_file_list.o strlcpy.o

ALL_EXE = $(LIBDIGIUTILS_TEST_EXE) $(CREATE_FILE_LIST_TEST_EXE)

.PHONY: all
all: $(ALL_EXE) run coverage

.PHONY: run
run: $(ALL_EXE)
	@for exe in $^ ; do \
		./$$exe || exit 1 ; \
	done

$(LIBDIGIUTILS_TEST_EXE): $(LIBDIGIUTILS_TEST_OBJS) $(LIBDIGIUTILS_SRC_OBJS)
	@$(CPP) $(LDFLAGS) -o $@ $^ $(LIBS)
	
$(CREATE_FILE_LIST_TEST_EXE): $(CREATE_FILE_LIST_TEST_OBJS) $(CREATE_FILE_LIST_SRC_OBJS)
	@$(CPP) $(LDFLAGS) -o $@ $^ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<
%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<
%.o: %.cpp
	$(CPP) $(CPPFLAGS) -c -o $@ $<

.PHONY: coverage
coverage:
	gcov $(LIBDIGIUTILS_SRC_OBJS) $(CREATE_FILE_LIST_SRC_OBJS)

.PHONY: clean
clean:
	@rm -f *.o *.c.gcov *.gcda *.gcno $(ALL_EXE)
