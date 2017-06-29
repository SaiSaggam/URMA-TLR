# RDS/00008: OASIS Makefile Conventions

#for test
TOPDIR := $(shell cd ../../../../; pwd)
TARGET ?= mips-linux-uclibc
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

# The rpl_alloc/rpl_realloc are addressed by the variables ac_cv_func_malloc_0_nonnull/ac_cv_func_realloc_0_nonnull
all:
	@echo "Building all in src"
	export ac_cv_func_malloc_0_nonnull=yes
	export ac_cv_func_realloc_0_nonnull=yes
	cd src;./configure ac_cv_func_malloc_0_nonnull=yes ac_cv_func_realloc_0_nonnull=yes --with-gnu-ld --prefix=/usr --host=$${TARGET} --quiet;make
	
.PHONY: all clean

clean:
	make -C ./src clean

distclean:
	make -C ./src distclean

install:
	-mkdir -p $(INSTALL_ROOT)/lib
	cp -a src/.libs/libjson-c.so* $(INSTALL_ROOT)/lib
