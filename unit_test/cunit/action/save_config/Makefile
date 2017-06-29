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

#Makefile for show_config unit test application
TLR_BASE ?= $(shell cd ../../../..; pwd)
export TLR_BASE
include $(TLR_BASE)/build/makefile.globals

CODE_DIR = $(COMMANDS_BASE)/action/save_config
CONFIG_MGR_DIR = $(LIB_BASE)/config_mgr
LIBDIGIUTILS_INC = $(UTILS_BASE)/include

CC ?= gcc
CFLAGS = -g -Wall -Werror --std=gnu99 -D_CUNIT_ -D_GNU_SOURCE

#standard CUNIT settings
USE_BASIC_STYLE=1
USE_AUTOMATED_STYLE=0
USE_CONSOLE_STYLE=0
CFLAGS+=-DUSE_BASIC_STYLE=$(USE_BASIC_STYLE) -DUSE_AUTOMATED_STYLE=$(USE_AUTOMATED_STYLE) -DUSE_CONSOLE_STYLE$=$(USE_CONSOLE_STYLE)

CFLAGS += -fprofile-arcs -ftest-coverage

LDFLAGS = -lcunit

CODE_INC = -I$(CODE_DIR) -I$(CONFIG_MGR_DIR) -I$(LIBDIGIUTILS_INC) -I/usr/local/include/uci -I$(TLR_BASE)/include

SAVE_CONFIG_MAIN_OBJS=test_save_config_main.o save_config_work.o tlr_print.o strlcpy.o
CONFIG_SAVE_PROCESS_OBJS=test_config_save_process.o config_save_process.o strlcpy.o tlr_print.o config_file_write_firewall.o

SAVE_CONFIG_MAIN_COMMAND_TEST_EXE=save_config_main_test
CONFIG_SAVE_PROCESS_TEST_EXE=config_save_process_test

ALL_EXE=$(SAVE_CONFIG_MAIN_COMMAND_TEST_EXE) $(CONFIG_SAVE_PROCESS_TEST_EXE)

all: $(ALL_EXE) run coverage

run: $(ALL_EXE)
	@for exe in $^ ; do \
		./$$exe || exit 1 ; \
	done

$(CONFIG_SAVE_PROCESS_TEST_EXE): $(CONFIG_SAVE_PROCESS_OBJS)
	@$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(SAVE_CONFIG_MAIN_COMMAND_TEST_EXE): $(SAVE_CONFIG_MAIN_OBJS)
	@$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	@$(CC) $(CODE_INC) $(CFLAGS) -c -o $@ $<
%.o: $(CODE_DIR)/%.c $(CODE_DIR)/%.h
	@$(CC) $(CODE_INC) $(CFLAGS) -c -o $@ $<
%.o: $(CODE_DIR)/%.c
	@$(CC) $(CODE_INC) $(CFLAGS) -c -o $@ $<
%.o: $(UTILS_BASE)/%.c $(CODE_DIR)/%.h
	@$(CC) $(CODE_INC) $(CFLAGS) -c -o $@ $<
%.o: $(UTILS_BASE)/%.c
	@$(CC) $(CODE_INC) $(CFLAGS) -c -o $@ $<

clean:
	@rm -f *.o *.g* $(ALL_EXE)
	@rm -f tmpconfigfile

coverage:
	gcov config_save_process.c save_config_work.c

.PHONY: all clean run coverage
