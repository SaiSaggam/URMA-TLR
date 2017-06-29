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

CFLAGS+=-DGET_FW_BEFORE_UPG -DENABLE_FWUPG_FILE_CHECKSUM

ifeq ($(DEBUG),true)
CFLAGS+=-g -O0
endif

INCPATH=-I. -I$(KERNEL_DIR)/include -I$(INSTALL_ROOT)/usr/include -I$(LIB_BASE)/config_mgr -I$(ROOTFS_BASE)/src/include -I$(UTILS_BASE)/include

CFLAGS+=$(INCPATH)

LFLAGS+=$(LIBPATH) -lupgbuf -ldigiutils  -lconfig_mgr -luci -lubox -lm -ljson-c -lcrypto

OBJS = update_firmware.o
ACTION = action_update_firmware

all: $(ACTION)

$(ACTION): $(OBJS)
	$(CC) -o $@ $^ $(LFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<
%.o: %.c %.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o $(ACTION)

install:
	-mkdir -p $(INSTALL_ROOT)/usr/bin
	install -m 0777 $(ACTION) $(INSTALL_ROOT)/usr/bin

.PHONY: all clean install

