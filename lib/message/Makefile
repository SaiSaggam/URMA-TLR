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

LIBOBJS=event_manager.o \
	args.o \
	event_queue.o \
	messages.o \
	queue.o \
	timer_timval.o \
	message_codes.o \
	interfaces.o \
	parsing.o \
	parsing_get_wan_message_info.o \
	socket.o \
	plain_socket.o \
	update_message.o

LIBRARY=libmessage.so

INCLUDES_DIR=./include


# INCLUDE is required when building for the host.
INCLUDE=-I$(INCLUDES_DIR) \
		-I$(INSTALL_ROOT)/usr/include \
		-I$(ROOTFS_BASE)/src/include \
		-I$(TLR_BASE)/include \
		-I$(LIB_BASE)/config_mgr \
		-I$(UTILS_BASE)/include

ifeq ($(DEBUG),true)
CFLAGS += -g -O0
endif

CFLAGS += -fPIC $(INCLUDE) -D_GNU_SOURCE

LDFLAGS=$(LIBPATH) -lrt -lpthread -ldigiutils -lconfig_mgr -lcrypto


.PHONY: all clean install

all: $(LIBRARY)

$(LIBRARY): $(LIBOBJS)
	echo "install root $(INSTALL_ROOT)"
	$(CC) -Wl,-soname,$(LIBRARY) $(LDFLAGS) -shared $^ -o $@

# Dependancy rule
-include $(LIBOBJS:.o=.d)

$(LIBOBJS): %.o: %.c
	$(COMPILE.c) $(DEFINES) -fPIC -o $@ $<

install: $(LIBRARY)
	-mkdir -p $(INSTALL_ROOT)/usr/{include,lib}
	install -m 0755 $^ $(INSTALL_ROOT)/usr/lib
	install -m 0644 $(INCLUDES_DIR)/* $(INSTALL_ROOT)/usr/include

clean:
	-rm -f $(LIBRARY) $(LIBOBJS) *.d

