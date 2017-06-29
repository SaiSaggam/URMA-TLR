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

LIBOBJS = cellular_status.o cellular_csid.o cellular_count.o
LIBRARY = libcellular.so

INCLUDE_HDR = cellular_status.h
INCLUDE = -I. -I$(COMPO_BASE)/include -I$(INSTALL_ROOT)/usr/include -I$(ROOTFS_SRC)/src/include -I$(TLR_BASE)/include

ifeq ($(DEBUG),true)
CFLAGS += -g -O0
endif

CFLAGS += -fPIC $(INCLUDE)

.PHONY: all clean install

all: $(LIBRARY)

$(LIBRARY): $(LIBOBJS)
	$(CC) -Wl,-soname,$(LIBRARY) -shared $^ -o $@

# Dependancy rule
-include $(LIBOBJS:.o=.d)

$(LIBOBJS): %.o: %.c
	$(COMPILE.c) $(DEFINES) -fPIC -o $@ $<

install: $(LIBRARY)
	-mkdir -p $(INSTALL_ROOT)/usr/{include,lib}
	install -m 0755 $^ $(INSTALL_ROOT)/usr/lib
#	install -m 0644 $(INCLUDE_HDR) $(INSTALL_ROOT)/usr/include

clean:
	-rm -f $(LIBRARY) $(LIBOBJS) *.d

