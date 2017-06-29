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

include $(TLR_BASE)/build/rds00008.mk

INCPATH =-I. -I$(INSTALL_ROOT)/usr/include -I$(KERNEL_DIR)/include -I$(ROOTFS_BASE)/src/include -I$(TLR_BASE)/include
INCPATH +=-I$(TOPDIR)/sdk/rootfs/src/include -I$(LIB_BASE)/libcellular -I$(LIB_BASE)/config_mgr

CFLAGS += $(INCPATH)

LFLAGS += $(LIBPATH) -lmessage -lpthread -lrt -lcsman -lconfig_mgr -luci -lubox -lcellular -ldigiutils -lm -ljson-c -lcrypto -lcellular2 -l_3g

OBJS = main.o cellular_monitor.o cellular_message.o cellular_config.o cellular_state.o cellular_modem.o cellular_led.o
OBJS += cellular_passthrough.o cellular_odis.o

EXE = cellular_monitor

SHARED_LIBS=$(INSTALL_ROOT)/usr/lib/libcellular.so $(INSTALL_ROOT)/usr/lib/libconfig_mgr.so $(INSTALL_ROOT)/usr/lib/libdigiutils.so
SHARED_LIBS+=$(INSTALL_ROOT)/usr/lib/libuci.so $(INSTALL_ROOT)/usr/lib/libubox.so

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
	-mkdir -p $(INSTALL_ROOT)/usr/{include,bin}
	install -m 0777 $(EXE) $(INSTALL_ROOT)/usr/bin

.PHONY: all clean install

