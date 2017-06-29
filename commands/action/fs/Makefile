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

INCPATH= -I./ -I$(KERNEL_DIR)/include -I$(OPENSRC_BASE)/json-c/src/ -I$(LIB_BASE)/config_mgr -I$(UTILS_BASE)/include
CFLAGS += $(INCPATH)

LFLAGS += $(LIBPATH) -ljson-c -lconfig_mgr -ldigiutils -luci -lubox -lpthread -lcrypto

SRCS=digi_cd_work.c  digi_pwd_work.c digi_more_work.c  digi_del_work.c  digi_mkdir_work.c  digi_rmdir_work.c  fs_lib.c digi_copy_work.c  digi_dir_work.c  digi_ren_work.c    fs.c
OBJS=$(SRCS:.c=.o)
$(warning OBJS=$(OBJS))

MAIN = digi_fs
EXES = action_cd action_pwd action_del action_mkdir action_rmdir action_copy action_dir action_rename action_more
LINKED_FILES=$(foreach EXE, $(EXES), $(INSTALL_ROOT)/usr/bin/$(EXE))
$(warning LINKED_FILES=$(LINKED_FILES))

all: $(MAIN)

$(MAIN): $(OBJS)
	echo $(CC) -o $@ $^ $(LFLAGS)
	$(CC) -o $@ $^ $(LFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<
%.o: %.c %.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o $(MAIN) $(EXES) $(LINKED_FILES)

$(LINKED_FILES): $(MAIN)
	echo ln -s /usr/bin/digi_fs $@
	ln -s /usr/bin/digi_fs $@

$(INSTALLED_MAIN): $(MAIN)
	-mkdir -p $(INSTALL_ROOT)/usr/bin
	install -m 0777 $(MAIN)      $(INSTALL_ROOT)/usr/bin

install: $(INSTALLED_MAIN) $(LINKED_FILES)
	install -m 0777 $(MAIN)      $(INSTALL_ROOT)/usr/bin

.PHONY: all clean install
