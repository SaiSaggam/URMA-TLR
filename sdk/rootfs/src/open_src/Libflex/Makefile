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

include rds00008.mk

INCLUDEDIR= ../../amit_priv/include/
CSIDDIR= ../../amit_priv/include/csid

CFLAGS += -I$(INCLUDEDIR) 
CFLAGS += -I$(CSIDDIR)
CFLAGS += -I$(KERNEL_DIR)
ifeq ($(TOOLCHAIN432), 1)
CFLAGS += -fPIC
endif

MAJOR_VERSION := 0
MINOR_VERSION := 9
SUBLEVEL      := 28
VERSION       := $(MAJOR_VERSION).$(MINOR_VERSION).$(SUBLEVEL)

LIBFL=libfl.a
LIBFL_SHARED=libfl.so
LIBFL_SHARED_FULLNAME=libfl-$(MAJOR_VERSION).$(MINOR_VERSION).$(SUBLEVEL).so

export CFLAGS KERNEL_DIR TOPDIR LIBFL LIBFL_SHARED LIBFL_SHARED_FULLNAME MAJOR_VERSION MINOR_VERSION SUBLEVEL VERSION

all: precheck
	LFLAGS="" make -C ./src
	LFLAGS="" make -C ./src shared
	$(STRIP) --strip-unneeded --strip-debug src/$(LIBFL_SHARED_FULLNAME)
	
precheck:
	@echo "Build Libflex for $(PROJECT_NAME)"
ifeq ($(PROJECT_CUSTOMER), ) 
	@$(error You need specify PROJECT_CUSTOMER for Makefile)
endif 

postcheck:
	@echo "Install Libflex to $(INSTALL_ROOT)" 
ifeq ($(INSTALL_ROOT), ) 
	@$(error You need specify INSTALL_ROOT for Makefile)
endif 

install: postcheck
	@cp -a src/$(LIBFL_SHARED_FULLNAME) ../../$(TARGET)-lib
	@cp -a src/$(LIBFL_SHARED) 		    ../../$(TARGET)-lib
	@cp -a src/$(LIBFL_SHARED_FULLNAME) $(INSTALL_ROOT)/lib/
	@cp -a src/$(LIBFL_SHARED) 		    $(INSTALL_ROOT)/lib/

uninstall: postcheck
	rm -f $(INSTALL_ROOT)/lib/$(LIBFL_SHARED)*
	rm -f ../../$(TARGET)-lib/$(LIBFL_SHARED)*

.PHONY: all clean

clean:
	make -C ./src clean

distclean:
	make -C ./src distclean

