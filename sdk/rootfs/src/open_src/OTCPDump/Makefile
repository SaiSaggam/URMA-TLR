# RDS/00008: OASIS Makefile Conventions

TOPDIR := $(shell cd ../../../../; pwd)
TARGET ?= mips-linux-uclibc
KERNEL_DIR ?= ${TOPDIR}/linux/src/
INSTALL_DIR := $(shell pwd)/_install

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
ifeq ($(ENABLE_ANALYSER),1)
CFLAGS	   += -DENABLE_ANALYSER
endif
AMIT_CFLAGS = ${CFLAGS}
CXXFLAGS    = ${CFLAGS}
LDFLAGS    += -L${LIBDIR}
CPPFLAGS   += -I${INCLUDEDIR}
CFLAGS =
PCAP_CONFIG = $(SRC_TOP_DIR)/open_src/Libpcap/install/bin/pcap-config

export CFLAGS

all: precheck
	@if [ ! -e src/Makefile ]; then \
		cd src; chmod +x configure; \
		./configure --prefix=/usr --host=$(TARGET) --build=$(HOST) \
			--target=$(TARGET) --disable-ipv6 CPPFLAGS=$(CPPFLAGS) \
			LDFLAGS=$(LDFLAGS) LIBS=$(LIBS) CC=$(CROSS_COMPILE)gcc \
			CFLAGS=$(CFLAGS) PCAP_CONFIG=$(PCAP_CONFIG) ;\
	fi;
	@make -C src
	
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
	@cd src;make install DESTDIR=$(INSTALL_DIR)

	@mkdir -p $(INSTALL_ROOT)/usr/sbin
	@cp -a $(INSTALL_DIR)/usr/sbin/tcpdump $(INSTALL_ROOT)/usr/sbin/
	
uninstall:
.PHONY: all clean

clean:
	@if [ -e src/Makefile ]; then \
		make -C ./src clean; \
		rm -Rf $(INSTALL_DIR); \
		rm src/Makefile; \
	fi;

distclean:
	@if [ -e src/Makefile ]; then \
		make -C ./src clean; \
		rm -Rf $(INSTALL_DIR); \
		rm src/Makefile; \
	fi;
