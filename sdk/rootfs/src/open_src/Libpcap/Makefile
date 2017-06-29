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
CSIDDIR     = $(SRC_TOP_DIR)/include/csid
LIBDIR      = $(SRC_TOP_DIR)/$(TARGET)-lib
#UCLIBCDIR 	= $(subst buildroot-gcc342/bin/,buildroot-gcc342/mipsel-linux-uclibc/lib,$(TOOLPATH))

CFLAGS     += -I$(INCLUDEDIR)
CFLAGS     += -I$(CSIDDIR)                                                                       
CFLAGS     += -I$(KERNEL_DIR)/include

AMIT_CFLAGS = ${CFLAGS}
CXXFLAGS    = ${CFLAGS}
CPPFLAGS    = ${CFLAGS}
LDFLAGS    += -L${TOPDIR}/rootfs/src/mipsel-linux-uclibc-lib

export AMIT_CFLAGS CFLAGS LIBDIR KERNEL_DIR TARGET CXXFLAGS CPPFLAGS LDFLAGS LIBS

all: precheck uninstall
	@rm -rf $(INSTALL_DIR) ; mkdir -p $(INSTALL_DIR)
	@if [ ! -e src/Makefile ]; then \
		cd src; ac_cv_header_libusb_1_0_libusb_h=no \
			./configure --prefix=$(INSTALL_DIR) \
			--host=$(TARGET) \
			--disable-protochain  \
			--without-flex \
			--without-bison \
			--disable-universal \
			--disable-bluetooth  \
			--disable-largefile \
			--with-pcap=linux \
			CC=$(CROSS_COMPILE)gcc \
			CXX=$(CROSS_COMPILE)g++ \
			STRIP=$(CROSS_COMPILE)strip;\
	fi; 
	@make -C src &&  make install -C src

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
	@mkdir -p $(SRC_TOP_DIR)/include/pcap
	@cp -af install/include/*.h   				$(SRC_TOP_DIR)/include
	@cp -ar install/include/pcap   				$(SRC_TOP_DIR)/include
	@cp -af install/lib/libpcap*so*   			$(SRC_TOP_DIR)/$(TARGET)-lib/ 
	@cp -af install/lib/libpcap*so*  			$(INSTALL_ROOT)/lib

uninstall: postcheck
	@-rm -rf $(SRC_TOP_DIR)/include/pcap*
	@-rm -f $(SRC_TOP_DIR)/$(TARGET)-lib/libpcap*so*
	@-rm -f	$(INSTALL_ROOT)/lib/libpcap*so*

.PHONY: all clean

clean: uninstall 
	@-rm -rf $(INSTALL_DIR)
	@if [ -e src/Makefile ]; then make clean -C src; rm src/Makefile; fi

distclean: uninstall
	@-rm -rf $(INSTALL_DIR)
	@if [ -e src/Makefile ]; then make distclean -C src; rm src/Makefile; fi


