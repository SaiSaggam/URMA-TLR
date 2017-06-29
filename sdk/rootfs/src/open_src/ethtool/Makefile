#
#  open_src/ethtool/Makefile
#
#  Copyright (C) 2015 by Digi International Inc. All rights reserved.
#

TOPDIR := $(shell cd ../../../../; pwd)
TARGET ?= mipsel-linux-uclibc
KERNEL_DIR ?= ${TOPDIR}/linux/src/

ifeq ($(TARGET), mips-linux-uclibc)
	ARCH = mips
endif

ifndef TOOLPATH
	export TOOLPATH := $(shell cat $(TOPDIR)/TOOLPATH2)
	export PATH     := $(TOOLPATH):$(PATH)
endif

include ./rds00008.mk

CFLAGS += -I$(INCLUDEDIR)
CFLAGS += -I$(CSIDDIR)
CFLAGS += -I$(KERNEL_DIR)
ifeq ($(TOOLCHAIN432), 1)
	CFLAGS += -fPIC
endif

export CFLAGS KERNEL_DIR TOPDIR LIBFL LIBFL_SHARED LIBFL_SHARED_FULLNAME MAJOR_VERSION MINOR_VERSION SUBLEVEL VERSION

all:
	echo
	echo "Building Ethtool... "
	cd src; ./configure --prefix=$(INSTALL_ROOT) --host=$${TARGET} --quiet --enable-silent-rules
	cd src; make; make install;
	$(STRIP) $(INSTALL_ROOT)/sbin/ethtool
	echo "Done."
	echo

.PHONY: all clean

clean:
	make -C ./src clean

distclean:
	make -C ./src distclean

install:
	make -C ./src install
