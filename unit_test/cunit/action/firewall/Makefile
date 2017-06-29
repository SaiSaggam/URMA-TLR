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

TLR_BASE ?= $(shell cd ../../../..; pwd)
export TLR_BASE
include $(TLR_BASE)/build/makefile.globals

SRC_DIR = $(COMMANDS_BASE)/action/firewall
INC_DIR = -I. -I$(SRC_DIR) -I$(LIB_BASE)/config_mgr -I$(TLR_BASE)/include -I$(UTILS_BASE)/include

CC ?= gcc
CFLAGS = -g -fdata-sections -ffunction-sections -Wall -Werror --std=gnu99 -D_GNU_SOURCE -D_CUNIT_ $(INC_DIR)
LDFLAGS = -Wl,--gc-sections -zmuldefs -lcunit

EXE = test_firewall
EXE1 = test_restore_firewall
TEST_OBJS  = test_firewall.o 
TEST_OBJS1 = test_restore_firewall.o 
SRC_OBJS = tlr_print.o strlcpy.o

.PHONY: all
all: $(EXE) $(EXE1) run

run: $(EXE) $(EXE1)
	@for exe in $^ ; do \
		./$$exe || exit 1 ; \
	done

$(EXE): $(TEST_OBJS) $(SRC_OBJS)
	@$(CC) -o $@ $^ $(LDFLAGS)
	
$(EXE1): $(TEST_OBJS1) $(SRC_OBJS)
	@$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	@$(CC) $(CFLAGS) -c -o $@ $<
%.o: $(SRC_DIR)/%.c
	@$(CC) $(CFLAGS) -c -o $@ $<
%.o: $(UTILS_BASE)/%.c
	@$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean run
clean:
	@rm -f *.o $(EXE) $(EXE1)
