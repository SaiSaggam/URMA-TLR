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
KERNEL_DIR ?= ${SDK_BASE}/linux/src/

include $(TLR_BASE)/build/rds00008.mk

INC_DIR = -I. -I$(KERNEL_DIR)/include -I$(INSTALL_ROOT)/usr/include -I$(LIB_BASE)/config_mgr -I$(ROOTFS_BASE)/src/include -I$(UTILS_BASE)/include
INC_DIR += -I$(UTILS_BASE)/include -I$(TLR_BASE)/include

CFLAGS += $(INC_DIR)
LDFLAGS = $(LIBPATH) -ldigiutils  -lconfig_mgr -luci -lubox -lm -ljson-c -lcrypto

OBJS = show_firewall.o show_firewall_work.o
EXE = show_firewall

.PHONY: all
all: $(EXE)

$(EXE): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c %.h
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	rm -f *.o $(EXE)

.PHONY: install
install:
	-mkdir -p $(INSTALL_ROOT)/usr/bin
	install -m 0777 $(EXE) $(INSTALL_ROOT)/usr/bin

