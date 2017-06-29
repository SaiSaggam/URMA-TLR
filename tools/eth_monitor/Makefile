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

TARGET ?= mips-linux-uclibc
KERNEL_DIR ?= $(SDK_BASE)/linux/src/
SHOW_DIR = $(COMMANDS_BASE)/show/eth_stats
include $(TLR_BASE)/build/rds00008.mk
UPTIMES_DIR = $(LIB_BASE)/libifuptime

INCPATH= -I. -I$(KERNEL_DIR)/include -I/usr/local/include/uci -I$(INSTALL_ROOT)/usr/include -I$(SHOW_DIR) -I$(UPTIMES_DIR)
CFLAGS += $(INCPATH)
CFLAGS+=-DETH_MONITOR_DEBUG=0

LFLAGS+=$(LIBPATH) -lmessage -lpthread -lconfig_mgr -luci -lubox -ldigiutils -lm -ljson-c -lcrypto

OBJS = eth_monitor_main.o eth_monitor.o apply_eth_func.o poll_eth_func.o ethregs.o dhcp_client.o

EXE = eth_monitor

all: $(EXE)

$(EXE): $(OBJS)
	$(CC) -o $@ $^ $(LFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<
%.o: %.c %.h
	$(CC) $(CFLAGS) -c -o $@ $<
%.o: $(SHOW_DIR)/%.c $(SHOW_DIR)/%.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o $(EXE)

install:
	-mkdir -p $(INSTALL_ROOT)/usr/bin
	install -m 0777 $(EXE) $(INSTALL_ROOT)/usr/bin

.PHONY: all clean install

