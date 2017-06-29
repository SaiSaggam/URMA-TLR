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

TOPDIR := $(shell cd ../../../; pwd)
TARGET ?= mips-linux-uclibc
KERNEL_DIR ?= $(SDK_BASE)/linux/src/

include $(TLR_BASE)/build/rds00008.mk

INCPATH = -I. -I$(INSTALL_ROOT)/usr/include -I$(KERNEL_DIR)/include -I$(OPENSRC_BASE)/json-c/src/
INCPATH += -I$(ROOTFS_BASE)/src/include -I$(TLR_BASE)/include
CFLAGS += $(INCPATH)

LFLAGS+=$(LIBPATH) -lmessage -lifuptime -lconfig_mgr -ldigiutils -luci -lubox -lpthread -lm -ljson-c -lcrypto

OBJS = dsl_config.o dsl_event.o dsl_main.o dsl_message.o dsl_modem.o dsl_monitor.o dsl_ppp.o dsl_led.o dsl_event_handlers.o
OBJS += config_dsl_to_modem.o modem_to_config_state_dsl.o dsl_status.o dsl_state.o dsl_timer.o
OBJS += dsl_show.o dsl_show_ascii.o dsl_show_json.o

EXE = dsl_monitor

all: $(EXE)

$(EXE): $(OBJS)
	$(CC) -o $@ $^ $(LFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<
%.o: %.c %.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o $(EXE)

install:
	-mkdir -p $(INSTALL_ROOT)/usr/bin
	install -m 0777 $(EXE) $(INSTALL_ROOT)/usr/bin

.PHONY: all clean install

