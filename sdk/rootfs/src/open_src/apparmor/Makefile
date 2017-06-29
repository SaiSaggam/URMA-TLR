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
LDLIB		=	-L$(SRC_TOP_DIR)/$(TARGET)-lib -L/opt/buildroot-gcc463/usr/mipsel-unknown-linux-uclibc/sysroot/usr/lib -L$(INSTALL_DIR)/lib
CFLAGS		+=	-I$(INCLUDEDIR) -I/opt/buildroot-gcc463/usr/mipsel-unknown-linux-uclibc/sysroot/usr/include
CFLAGS		+=	-g -O2 -pipe

export CC=$(CROSS_COMPILE)gcc
export CXX=$(CROSS_COMPILE)g++
export LD=$(CROSS_COMPILE)ld
export STRIP=$(CROSS_COMPILE)strip

all: libapparmor parser

install: libapparmor_install parser_install profiles_install scripts_install utils_install

clean: libapparmor_clean parser_clean
	@rm -Rf $(INSTALL_DIR)

distclean: clean


libapparmor:
	if [ ! -e src/libraries/libapparmor/Makefile ]; then \
		cd src/libraries/libapparmor; \
		./configure --host=$(TARGET) \
			CC=$(CROSS_COMPILE)gcc \
			CXX=$(CROSS_COMPILE)g++ \
			LD=$(CROSS_COMPILE)ld \
			STRIP=$(CROSS_COMPILE)strip \
			LDFLAGS="$(LDLIB)" \
			--prefix=/usr \
			--sysconfdir=/etc; \
	fi
	make -C src/libraries/libapparmor

libapparmor_install:
	make -C src/libraries/libapparmor DESTDIR=$(INSTALL_DIR) install

	@cp -a $(INSTALL_DIR)/usr/lib/*.so*	$(INSTALL_ROOT)/usr/lib/

libapparmor_clean:
	if [ -e src/libraries/libapparmor ]; then \
		make -C src/libraries/libapparmor clean; \
		rm -f src/libraries/libapparmor/src/af_protos.h \
		rm src/libraries/libapparmor/Makefile; \
	fi

parser:
	export CFLAGS="$(CFLAGS)"; \
	make -C src/parser arch

parser_install:
	make -C src/parser DESTDIR=$(INSTALL_DIR) install-arch

	@cp -a $(INSTALL_DIR)/sbin/*			$(INSTALL_ROOT)/sbin/

	@install -m 755 -d $(INSTALL_ROOT)/usr/mv2ram/etc/apparmor
	@install -m 644 src/parser/subdomain.conf	$(INSTALL_ROOT)/usr/mv2ram/etc/apparmor/
	@install -m 644 src/parser/parser.conf		$(INSTALL_ROOT)/usr/mv2ram/etc/apparmor/
	@install -m 755 -d $(INSTALL_ROOT)/usr/mv2ram/var/lib/apparmor
	@install -m 755 -d $(INSTALL_ROOT)/lib/apparmor
	@install -m 755 files/rc.apparmor.functions $(INSTALL_ROOT)/lib/apparmor/

parser_clean:
	make -C src/parser clean

PROFILES_SOURCE=files/profiles/apparmor.d
SUBDIRS_MUST_BE_SKIPPED=${PROFILES_SOURCE}/abstractions ${PROFILES_SOURCE}/tunables ${PROFILES_SOURCE}/local
PROFILES_TO_COPY=$(filter-out ${SUBDIRS_MUST_BE_SKIPPED}, $(wildcard ${PROFILES_SOURCE}/*))
TUNABLES_TO_COPY=$(filter-out ${PROFILES_SOURCE}/tunables/home.d, $(wildcard ${PROFILES_SOURCE}/tunables/*))
ABSTRACTIONS_TO_COPY=$(filter-out , $(wildcard ${PROFILES_SOURCE}/abstractions/*))

profiles_install:
	@install -v -m 755 -d $(INSTALL_ROOT)/usr/mv2ram/etc/apparmor.d \
			  $(INSTALL_ROOT)/usr/mv2ram/etc/apparmor.d/abstractions \
			  $(INSTALL_ROOT)/usr/mv2ram/etc/apparmor.d/tunables

	@install -m 644 $(PROFILES_TO_COPY)		$(INSTALL_ROOT)/usr/mv2ram/etc/apparmor.d/
	@install -m 644 $(ABSTRACTIONS_TO_COPY)		$(INSTALL_ROOT)/usr/mv2ram/etc/apparmor.d/abstractions
	@install -m 644 $(TUNABLES_TO_COPY)		$(INSTALL_ROOT)/usr/mv2ram/etc/apparmor.d/tunables

scripts_install:
	@install -m 775 files/rc.apparmor		$(INSTALL_ROOT)/usr/etc/init.d/init_apparmor.sh
	@install -m 644 files/rc.apparmor.functions	$(INSTALL_ROOT)/lib/apparmor/

utils_install:
	@install -m 755 src/utils/aa-status		$(INSTALL_ROOT)/usr/sbin/
