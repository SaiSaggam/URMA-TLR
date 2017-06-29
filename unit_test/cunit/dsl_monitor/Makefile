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

TLR_BASE ?= $(shell cd ../../..; pwd)
export TLR_BASE
include $(TLR_BASE)/build/makefile.globals

SRC_DIR = $(TOOLS_BASE)/dsl_monitor
CONFIG_MGR_DIR = $(LIB_BASE)/config_mgr
INC_DIR = -I. -I$(SRC_DIR) -I$(LIB_BASE)/config_mgr -I$(TLR_BASE)/include -I$(MESSAGE_BASE)/include -I$(UTILS_BASE)/include

CC ?= gcc
CFLAGS = -g -Wall -Werror --std=gnu99 -D_GNU_SOURCE -D_CUNIT_ $(INC_DIR)
LDFLAGS = -L/usr/local/lib -lcunit

EXE = test_dsl_monitor
TEST_OBJS = main.o test_config_dsl_to_modem.o test_dsl_led.o test_modem_to_config_state_dsl.o
SRC_OBJS = config_dsl_to_modem.o dsl_led.o modem_to_config_state_dsl.o
CONFIG_MGR_OBJS = config_table.o config_valid.o

.PHONY: all
all: $(EXE) run

run: $(EXE)
	@for exe in $^ ; do \
		./$$exe || exit 1 ; \
	done

$(EXE): $(TEST_OBJS) $(SRC_OBJS) $(CONFIG_MGR_OBJS)
	@$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	@$(CC) $(CFLAGS) -c -o $@ $<

%.o: $(SRC_DIR)/%.c
	@$(CC) $(CFLAGS) -c -o $@ $<

%.o: $(CONFIG_MGR_DIR)/%.c
	@$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean all run

clean:
	@rm -f *.o $(EXE)
