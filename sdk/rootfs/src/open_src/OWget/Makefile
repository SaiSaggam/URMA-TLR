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
#CFLAGS	   += $(GNUTLS_CFLAGS)
#CFLAGS     += -I$(INCLUDEDIR)/nettle
AMIT_CFLAGS = ${CFLAGS}
CXXFLAGS    = ${CFLAGS}
#GNUTLS_CFLAGS = -I$(SRC_TOP_DIR)/include/gnutls
#GNUTLS_LIBS = -L${TOPDIR}/rootfs/src/mipsel-linux-uclibc-lib
LDFLAGS    += -L${TOPDIR}/rootfs/src/mipsel-linux-uclibc-lib
CPPFLAGS   += -I${TOPDIR}/rootfs/src/include
OPENSSL_CFLAGS = -I$(INCLUDEDIR)/openssl
OPENSSL_LIBS = -L${LIBDIR}/ -lssl
#LIBS = -lgnutls -lnettle

export AMIT_CFLAGS CFLAGS LIBDIR KERNEL_DIR TARGET CXXFLAGS CPPFLAGS LDFLAGS LIBS

all: precheck
	@if [ ! -e src/Makefile ]; then \
		cd src; ./configure --host=$(TARGET) \
				    CC=$(CROSS_COMPILE)gcc \
				    CFLAGS="$(CFLAGS)" \
				    CXX=$(CROSS_COMPILE)g++ \
				    STRIP=$(CROSS_COMPILE)strip \
				    OPENSSL_CFLAGS="$(OPENSSL_CFLAGS)" \
			    	    OPENSSL_LIBS="$(OPENSSL_LIBS)" \
				    --with-libssl-prefix="$(LIBDIR)" \
	  			    --without-libgnutls-prefix \
				    --with-ssl=openssl \
				    --prefix=/usr \
				    --sysconfdir=/etc; \
	fi;
	@make -C src && make -C src install DESTDIR=$(INSTALL_DIR)
	
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
	@cp install/usr/bin/wget	$(INSTALL_ROOT)/usr/bin/
uninstall: postcheck
	@-rm -f $(INSTALL_ROOT)/usr/bin/wget

.PHONY: all clean

clean: uninstall
	@-rm -rf $(INSTALL_DIR)
	@rm -f $(INSTALL_ROOT)/usr/bin/wget
	make clean -C src
	@rm -f src/Makefile

distclean: uninstall
	@-rm -rf $(INSTALL_DIR)
	@rm -f $(INSTALL_ROOT)/usr/bin/wget
	make distclean -C src
	@rm -f src/Makefile
