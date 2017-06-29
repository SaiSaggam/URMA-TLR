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
KERNEL_DIR ?= $(SDK_BASE)/linux/src

include $(TLR_BASE)/build/rds00008.mk

ifeq ($(TARGET), mipsel-linux-uclibc)
DESTDIR =
else
DESTDIR = $(INSTALL_ROOT)
endif
CFLAGS+=-DDESTDIR=$(DESTDIR)


LIBOBJS = config_table.o config_valid.o config_cmd.o config_last.o apply.o show.o status.o action.o validate.o custom_valid.o password.o uci_interface.o config_util.o access.o tlr_show_object.o
LIBOBJS += base32.o password_gen.o tlr_backup.o
LIBRARY = libconfig_mgr.so

CONFIGMGR_HDR = config_mgr.h config_status.h
TOOL_HDR = config_def.h config_table.h
UCI_DEFAULT=./uci_default/*
MIB_DEFAULT=./mib_default/*
JSON_DEFAULT=./json_default/*
REFERENCE_DOCS=./reference_docs/*

# INCLUDE is required when building for the host.
INCLUDE = -I. -I$(COMPO_BASE)/include -I$(INSTALL_ROOT)/usr/include -I$(ROOTFS_BASE)/src/include -I$(OPENSRC_BASE)/json-c/src/ -I$(TLR_BASE)/include -I/usr/local/include/uci -I$(OPENSRC_BASE)/uci
INCLUDE += -I$(UTILS_BASE)/include

CFLAGS += -fPIC $(INCLUDE)

ifeq ($(DEBUG),true)
CFLAGS  += -g -O0
else
CFLAGS  += -O2
endif

LFLAGS += $(LIBPATH) -luci -lpthread -ldigiutils -lubox -lm -ljson-c -lcrypto

.PHONY: all clean install

all: $(LIBRARY)

$(LIBRARY): $(LIBOBJS)
	echo $(INSTALL_ROOT)
	$(CC) -Wl,-soname,$(LIBRARY) -shared $^ -o $@ $(LFLAGS)

# Dependancy rule
-include $(LIBOBJS:.o=.d)

$(LIBOBJS): %.o: %.c
	$(COMPILE.c) $(DEFINES) -fPIC -o $@ $<

install: $(LIBRARY)
	-mkdir -p $(INSTALL_ROOT)/usr/{include,lib,bin,config}
	install -m 0755 $^ $(INSTALL_ROOT)/usr/lib
	# Remove all existing configurations from the default config dir
	rm -f $(INSTALL_ROOT)/usr/config/*
	install -m 0644 $(UCI_DEFAULT) $(INSTALL_ROOT)/usr/config
	install -m 0644 $(CONFIGMGR_HDR) $(INSTALL_ROOT)/usr/include
	install -m 0644 $(TOOL_HDR) $(INSTALL_ROOT)/usr/include

clean:
	-rm -f $(LIBRARY) $(LIBOBJS) $(UCI_DEFAULT) $(MIB_DEFAULT) $(JSON_DEFAULT) $(REFERENCE_DOCS) *.d
