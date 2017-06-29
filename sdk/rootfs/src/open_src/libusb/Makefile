# RDS/00008: OASIS Makefile Conventions

#for test
TOPDIR := $(shell cd ../../../../; pwd)
TARGET ?= mipsel-linux-uclibc
KERNEL_DIR ?= ${TOPDIR}/linux/src/
INSTALL_DIR = $(shell pwd)/_install

PREFIX += PROJECT_NAME=${PROJECT_NAME} 
PREFIX += PROJECT_CUSTOMER=${PROJECT_CUSTOMER} 
PREFIX += PRIV_TOOLS=${PRIV_TOOLS} 
PREFIX += INSTALL_ROOT=$(INSTALL_ROOT)
PREFIX += ENDIAN=$(ENDIAN)

ifeq ($(TARGET), mips-linux-uclibc)
	ARCH = mips
endif

ifndef TOOLPATH
    export TOOLPATH := $(shell cat $(TOPDIR)/TOOLPATH2)
    export PATH     := $(TOOLPATH):$(PATH)
endif

ifndef OINTLTOOL_PATH
    export INTLTOOL_PATH := $(TOPDIR)/tools/open/bin
    export PATH     := $(INTLTOOL_PATH):$(PATH)
endif


#include rds00008.mk

SRC_TOP_DIR	=	$(shell cd ../../; pwd)
INCLUDEDIR	=	$(SRC_TOP_DIR)/include
LIBDIR		=	$(SRC_TOP_DIR)/$(TARGET)-lib
LDLIB		=	-L$(SRC_TOP_DIR)/$(TARGET)-lib -L/opt/buildroot-gcc463/usr/mipsel-unknown-linux-uclibc/sysroot/usr/lib
CFLAGS		+=	-I$(INCLUDEDIR)

all: precheck
	echo "PATH="$(PATH)
	if [ ! -e src/Makefile ]; then \
		cd src; ./configure --host=$(TARGET) \
		CC=$(CROSS_COMPILE)gcc \
		CXX=$(CROSS_COMPILE)g++ \
		LD=$(CROSS_COMPILE)ld \
		STRIP=$(CROSS_COMPILE)strip \
		LDFLAGS="$(LDLIB)" \
		--enable-shared \
		--disable-static \
		--enable-udev=no \
		--prefix=/usr \
		--sysconfdir=/etc; \
	fi;
	make -C src
	
install: postcheck
	@mkdir -p $(INSTALL_DIR)
	@$(MAKE) -C src DESTDIR=$(INSTALL_DIR) install

	@mkdir -p $(INSTALL_ROOT)/usr/lib
	@cp -a $(INSTALL_DIR)/usr/lib/*.so* $(INSTALL_ROOT)/usr/lib/

precheck:
	@echo ""
	@echo "Build $(shell basename `pwd`) for $(PROJECT_NAME) ($(PROJECT_CUSTOMER))"
ifeq ($(PROJECT_CUSTOMER), ) 
	@echo ""
	@echo "No specify PROJECT_CUSTOMER in Makefile. "
	@echo "Maybe you should update the components Makefile of the Project"
	@echo ""
	@exit 1
endif 

postcheck:                                   
	@echo ""
	@echo "Install $(shell basename `pwd`) to $(INSTALL_ROOT)" 
ifeq ($(INSTALL_ROOT), ) 
	@echo ""
	@echo "No specify INSTALL_ROOT in Makefile. "
	@echo "Maybe you should update the components Makefile of the Project"
	@echo ""
	@exit 1
endif 

clean:
	@rm -Rf $(INSTALL_DIR)
	if [ -e src/Makefile ]; then make -C ./src clean; rm src/Makefile; fi;

distclean:
	@rm -Rf $(INSTALL_DIR)
	if [ -e src/Makefile ]; then make -C ./src distclean; rm src/Makefile; fi;

