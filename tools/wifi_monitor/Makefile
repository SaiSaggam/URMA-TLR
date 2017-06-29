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

TOPDIR := $(shell cd ../../../; pwd)
TARGET ?= mips-linux-uclibc
KERNEL_DIR ?= $(SDK_BASE)/linux/src/

include $(TLR_BASE)/build/rds00008.mk

INCPATH =-I. -I$(INSTALL_ROOT)/usr/include -I$(KERNEL_DIR)/include
INCPATH +=-I$(ROOTFS_BASE)/src/include -I/$(TLR_BASE)/include -I$(UTILS_BASE)/include
CFLAGS += $(INCPATH)

LFLAGS+=$(LIBPATH) -lmessage -lifuptime -lconfig_mgr -ldigiutils -luci -lubox -lpthread -lm -ljson-c -lcrypto

COMMON_SRCS=$(wildcard *.c)
COMMON_OBJS=$(patsubst %.c,%.o,$(COMMON_SRCS))

flagwifi = -DRADIO_24
flagwifi5g = -DRADIO_5G

WIFI24_DIR = wifi24
WIFI5G_DIR = wifi5g
WIFI_24_COMMON_OBJS_LIST = $(addprefix $(WIFI24_DIR)/, $(COMMON_OBJS))
WIFI_5G_COMMON_OBJS_LIST = $(addprefix $(WIFI5G_DIR)/, $(COMMON_OBJS))

SRCSwifi24=$(wildcard $(WIFI24_DIR)/*.c)
WIFI_24_SPECIFIC_OBJS_LIST=$(patsubst %.c,%.o,$(SRCSwifi24))
SRCSwifi5g=$(wildcard $(WIFI5G_DIR)/*.c)
WIFI_5G_SPECIFIC_OBJS_LIST=$(patsubst %.c,%.o,$(SRCSwifi5g))

EXE = wifi_monitor wifi5g_monitor

all: $(EXE)

wifi_monitor: $(WIFI_24_COMMON_OBJS_LIST) $(WIFI_24_SPECIFIC_OBJS_LIST)
	$(CC) -o $@ $^ $(LFLAGS)

wifi5g_monitor: $(WIFI_5G_COMMON_OBJS_LIST) $(WIFI_5G_SPECIFIC_OBJS_LIST)
	$(CC) -o $@ $^ $(LFLAGS)

## wifi_monitor compiles
$(WIFI24_DIR)/%.o: %.c
	$(CC) $(flagwifi) $(CFLAGS) -c -o $@ $<
$(WIFI24_DIR)/%.o: %.c %.h
	$(CC) $(flagwifi) $(CFLAGS) -c -o $@ $<
$(WIFI24_DIR)/%.o: $(WIFI24_DIR)/%.c
	$(CC) $(flagwifi) $(CFLAGS) -c -o $@ $<

## wifi5g_monitor compiles
$(WIFI5G_DIR)/%.o: %.c
	$(CC) $(flagwifi5g) $(CFLAGS) -c -o $@ $<
$(WIFI5G_DIR)/%.o: %.c %.h
	$(CC) $(flagwifi5g) $(CFLAGS) -c -o $@ $<
$(WIFI5G_DIR)/%.o: $(WIFI5G_DIR)/%.c
	$(CC) $(flagwifi5g) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o $(EXE)
	rm -f $(WIFI24_DIR)/*.o
	rm -f $(WIFI5G_DIR)/*.o 

install:
	-mkdir -p $(INSTALL_ROOT)/usr/bin
	install -m 0777 $(EXE) $(INSTALL_ROOT)/usr/bin

.PHONY: all clean install


