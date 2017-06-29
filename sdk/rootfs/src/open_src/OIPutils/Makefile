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
include ../../../makefile.envars

SRC_TOP_DIR = $(shell cd ../../; pwd)
INCLUDEDIR  = $(SRC_TOP_DIR)/include
LIBDIR      = $(SRC_TOP_DIR)/$(TARGET)-lib
LIBS       += -lcsman
AMIT_CFLAGS = ${CFLAGS}
CXXFLAGS    = ${CFLAGS}
LDFLAGS    += -L${TOPDIR}/rootfs/src/mipsel-linux-uclibc-lib
CPPFLAGS   += -I${TOPDIR}/rootfs/src/include

export CFLAGS

.PHONY: all install clean

all: precheck
	@make -C src
	@make -C src/traceroute-2.0.21
	@$(STRIP) src/ping
	@$(STRIP) src/ping6
	@$(STRIP) src/traceroute-2.0.21/traceroute/traceroute

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
	@cp -a src/ping                                       $(INSTALL_ROOT)/bin
	@cp -a src/ping6                                      $(INSTALL_ROOT)/bin
	@cp -a src/traceroute-2.0.21/traceroute/traceroute    $(INSTALL_ROOT)/usr/sbin

clean:
	make clean -C src
	make clean -C src/traceroute-2.0.21
