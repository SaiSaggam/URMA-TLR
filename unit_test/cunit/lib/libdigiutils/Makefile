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

TLR_BASE ?= $(shell cd ../../../..; pwd)
export TLR_BASE
include $(TLR_BASE)/build/makefile.globals

SRC_DIR = $(LIB_BASE)/libdigiutils
INC_DIR = -I. -I$(SRC_DIR) -I$(LIB_BASE)/config_mgr -I$(LIB_BASE)/libdigiutils/include -I$(TLR_BASE)/include -I$(ROOTFS_BASE)/src/include -I/usr/local/include/uci
INC_DIR += -I$(TLR_BASE)/sdk/rootfs/src/open_src/uci/

CC ?= gcc
CFLAGS = -g -Wall -Werror -fdata-sections -ffunction-sections --std=gnu99 -D_GNU_SOURCE -D_CUNIT_ $(INC_DIR)
CFLAGS += -fprofile-arcs -ftest-coverage
LDFLAGS = -L/usr/local/lib -lcunit -Wl,--gc-sections
LDFLAGS += -fprofile-arcs

LIBDIGIUTILS_TEST_EXE = test_libdigiutils
LIBDIGIUTILS_TEST_OBJS =test_libdigiutils.o \
			test_get_field.o \
			test_strip_whitespace.o \
			system_command.o \
			test_security.o \
			strlcpy.o \
			tlr_print.o \
			test_element.o \
			test_show_parse_helper.o

LIBDIGIUTILS_SRC_OBJS =get_field.o \
			strip_whitespace.o \
			security.o \
			element.o \
			show_parse_helper.o

CONFIG_FILE_TEST_EXE = test_config_file
CONFIG_FILE_TEST_OBJS = test_config_file.o
CONFIG_FILE_SRC_OBJS = config_file.o

CONFIG_FILE_HELPER_TEST_EXE = test_config_file_helper
CONFIG_FILE_HELPER_TEST_OBJS = test_config_file_helper.o
CONFIG_FILE_HELPER_SRC_OBJS = config_file_helper.o

ODM_SPECIFIC_TEST_EXE = odm_specific_test
ODM_SPECIFIC_TEST_OBJS = odm_specific_test.o
ODM_SPECIFIC_SRC_OBJS = odm_specific.o

FIRMWARE_VERSION_TEST_EXE = firmware_version_test
FIRMWARE_VERSION_TEST_OBJS = firmware_version_test.o strip_whitespace.o
FIRMWARE_VERSION_SRC_OBJS = firmware_version.o

FORMAT_STRING_TEST_EXE = test_format_string
FORMAT_STRING_TEST_OBJS = test_format_string.o
FORMAT_STRING_SRC_OBJS = tlr_format_string.o

ALL_EXE = \
		$(LIBDIGIUTILS_TEST_EXE) \
		$(CONFIG_FILE_TEST_EXE) \
		$(CONFIG_FILE_HELPER_TEST_EXE) \
		$(ODM_SPECIFIC_TEST_EXE) \
		$(FIRMWARE_VERSION_TEST_EXE) \
		$(FORMAT_STRING_TEST_EXE)

.PHONY: all
all: $(ALL_EXE) run coverage

.PHONY: run
run: $(ALL_EXE)
	@for exe in $^ ; do \
		./$$exe || exit 1; \
	done

$(LIBDIGIUTILS_TEST_EXE): $(LIBDIGIUTILS_TEST_OBJS) $(LIBDIGIUTILS_SRC_OBJS)
	@$(CC) -o $@ $^ $(LDFLAGS)

$(CONFIG_FILE_TEST_EXE): $(CONFIG_FILE_TEST_OBJS) $(CONFIG_FILE_SRC_OBJS)
	@$(CC) -o $@ $^ $(LDFLAGS)

$(CONFIG_FILE_HELPER_TEST_EXE): $(CONFIG_FILE_HELPER_TEST_OBJS) $(CONFIG_FILE_HELPER_SRC_OBJS)
	@$(CC) -o $@ $^ $(LDFLAGS)

$(ODM_SPECIFIC_TEST_EXE): $(ODM_SPECIFIC_TEST_OBJS) $(ODM_SPECIFIC_SRC_OBJS)
	@$(CC) -o $@ $^ $(LDFLAGS)

$(FIRMWARE_VERSION_TEST_EXE): $(FIRMWARE_VERSION_TEST_OBJS) $(FIRMWARE_VERSION_SRC_OBJS)
	@$(CC) -o $@ $^ $(LDFLAGS)

$(FORMAT_STRING_TEST_EXE): $(FORMAT_STRING_TEST_OBJS) $(FORMAT_STRING_SRC_OBJS)
	@$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	@$(CC) $(CFLAGS) -c -o $@ $<
%.o: $(SRC_DIR)/%.c
	@$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: coverage
coverage:
	gcov $(LIBDIGIUTILS_SRC_OBJS) $(CONFIG_FILE_SRC_OBJS) $(CONFIG_FILE_HELPER_SRC_OBJS) $(ODM_SPECIFIC_SRC_OBJS) $(FIRMWARE_VERSION_SRC_OBJS) $(FORMAT_STRING_SRC_OBJS)

.PHONY: clean
clean:
	@rm -f *.o $(ALL_EXE)
