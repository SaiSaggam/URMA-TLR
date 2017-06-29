# RDS/00008: OASIS Makefile Conventions

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

INSTALL_DIR = $(shell pwd)/install
SRC_TOP_DIR = $(shell cd ../../; pwd)
INCLUDEDIR  = $(SRC_TOP_DIR)/include
LIBDIR      = $(SRC_TOP_DIR)/$(TARGET)-lib
UCLIBCDIR 	= $(subst buildroot-gcc342/bin/,buildroot-gcc342/mipsel-linux-uclibc/lib,$(TOOLPATH))

CFLAGS     += -I$(INCLUDEDIR)                                                                
CFLAGS     += -I$(KERNEL_DIR)/usr/include

CFLAGS     += -I$(KERNEL_DIR)/drivers/char
CFLAGS     += -I$(KERNEL_DIR)/drivers/net/rt_rdm
CFLAGS     += -I$(KERNEL_DIR)/drivers/net/raeth
CFLAGS     += -I$(KERNEL_DIR)/arch/mips/include/asm/mach-ralink

AMIT_CFLAGS = ${CFLAGS}
CXXFLAGS    = ${CFLAGS}
CPPFLAGS    = ${CFLAGS}
LDFLAGS    += -L${TOPDIR}/rootfs/src/mipsel-linux-uclibc-lib
LDFLAGS    += -L${INSTALL_ROOT}/lib
LDFLAGS    += -lcsman -lpthread

export AMIT_CFLAGS CFLAGS LIBDIR KERNEL_DIR TARGET CXXFLAGS CPPFLAGS LDFLAGS LIBS

all: precheck
	if [ ! -e src/Makefile ]; then \
		cd src; ./configure --host=$(TARGET) CC=$(CROSS_COMPILE)gcc CXX=$(CROSS_COMPILE)g++ STRIP=$(CROSS_COMPILE)strip;\
	fi; 
	@make -C src progs

precheck:
	@echo "Build $(shell basename `pwd`) for $(PROJECT_NAME)"
ifeq ($(PROJECT_CUSTOMER), ) 
	@$(error You need specify PROJECT_CUSTOMER for Makefile)
endif 

postcheck:
	@echo "Install $(shell basename `pwd`) to $(INSTALL_ROOT)" 
ifeq ($(INSTALL_ROOT), ) 
	@$(error You need specify INSTALL_ROOT for Makefile)
endif 

install: postcheck
	@cp -a src/procan	  $(INSTALL_ROOT)/usr/bin/
	@cp -a src/filan	  $(INSTALL_ROOT)/usr/bin/
	@cp -a src/socat	  $(INSTALL_ROOT)/usr/bin/

uninstall: postcheck 
	@rm -f $(INSTALL_ROOT)/usr/bin/procan
	@rm -f $(INSTALL_ROOT)/usr/bin/filan
	@rm -f $(INSTALL_ROOT)/usr/bin/socat

.PHONY: all clean

clean:
	if [ -e src/Makefile ]; then \
		make -C ./src clean; \
		rm src/Makefile; \
	fi;

distclean:
	if [ -e src/Makefile ]; then \
		make -C ./src distclean; \
		rm src/Makefile; \
	fi;
