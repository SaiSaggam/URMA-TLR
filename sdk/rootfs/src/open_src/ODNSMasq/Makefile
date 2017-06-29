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

SRC_TOP_DIR = $(shell cd ../../; pwd)
INCLUDEDIR  = $(SRC_TOP_DIR)/include
LIBDIR      = $(SRC_TOP_DIR)/$(TARGET)-lib

INSTALL_DIR = $(shell pwd)/install
CFLAGS     += -I$(INCLUDEDIR)
CXXFLAGS    = ${CFLAGS}
LDFLAGS    += -L${TOPDIR}/rootfs/src/mipsel-linux-uclibc-lib
CPPFLAGS   += -I${TOPDIR}/rootfs/src/include
OPENSSL_CFLAGS = -I$(INCLUDEDIR)/openssl
OPENSSL_LIBS = -L${LIBDIR}/ -lssl
#LIBS = -lgnutls -lnettle

export AMIT_CFLAGS CFLAGS LIBDIR KERNEL_DIR TARGET CXXFLAGS CPPFLAGS LDFLAGS LIBS

all: precheck
	@make -C src PREFIX=/usr
	@make -C src install DESTDIR=$(INSTALL_DIR) PREFIX=/usr
	
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
	@mkdir -p $(INSTALL_ROOT)/usr/sbin
	@install -m 0755 $(INSTALL_DIR)/usr/sbin/dnsmasq $(INSTALL_ROOT)/usr/sbin/
	@install -m 0755 files/init_dnsmasq.sh $(INSTALL_ROOT)/usr/etc/init.d/

uninstall: postcheck
	@rm -rf $(INSTALL_DIR)
	@rm -f $(INSTALL_ROOT)/usr/sbin/dnsmasq

.PHONY: all clean

clean: uninstall
	make clean -C src

distclean: uninstall
	make clean -C src
