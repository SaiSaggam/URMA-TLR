# RDS/00008: OASIS Makefile Conventions

TOPDIR := $(shell cd ../../../../; pwd)
#TARGET = mipsel-linux
#TARGET ?= mips-linux-uclibc
#TARGET ?= mips-linux-uclibc
KERNEL_DIR ?= ${TOPDIR}/linux/src/

ifndef CROSS_COMPILE
    CROSS_COMPILE = $(TARGET)-
endif

ifeq ($(TARGET), mips-linux)
	ARCH = mips
endif

ifndef TOOLPATH
    export TOOLPATH := $(shell cat $(TOPDIR)/TOOLPATH2)
	export PATH     := $(TOOLPATH):$(PATH)
endif

#include rds00008.mk

INSTALL_DIR = $(shell pwd)/install
SRC_TOP_DIR = $(shell cd ../../; pwd)
INCLUDEDIR  = $(SRC_TOP_DIR)/include
LIBDIR      = $(COMPO_BASE)/mipsel-linux-uclibc-lib
UCLIBCDIR 	= $(subst buildroot-gcc342/bin/,buildroot-gcc342/mipsel-linux-uclibc/lib,$(TOOLPATH))
INSTALL_PAM = $(INSTALL_ROOT)/lib/security

AR = $(CROSS_COMPILE)ar
CC = $(CROSS_COMPILE)gcc
RANLIB = $(CROSS_COMPILE)ranlib
STRIP=$(CROSS_COMPILE)strip

CFLAGS     += -I$(INCLUDEDIR)                                                                    
CFLAGS     += -I$(KERNEL_DIR)/include
#CFLAGS     += -I$(KERNEL_DIR)/usr/include

#CFLAGS	   += -DTIOCSRS485

AMIT_CFLAGS = ${CFLAGS}
CXXFLAGS    = ${CFLAGS}
CPPFLAGS    = ${CFLAGS}
#LDFLAGS    += -L${TOPDIR}/rootfs/src/mipsel-linux-uclibc-lib
LDFLAGS    += -lcrypto -lssl
#LIBS	   += -lssl -lcrypto

#export AMIT_CFLAGS CFLAGS LIBDIR KERNEL_DIR TARGET CXXFLAGS CPPFLAGS LDFLAGS LIBS

all: precheck
	if [ ! -e src-1.2.1/Makefile ]; then \
		cd src-1.2.1; ./configure --host=$(TARGET) \
		CC=$(CROSS_COMPILE)gcc \
		CXX=$(CROSS_COMPILE)g++ \
		--program-prefix="" \
		--target=$(TARGET) \
		--enable-shared \
		--libdir=/lib \
		STRIP=$(CROSS_COMPILE)strip; \
	fi; 
	@make -C src-1.2.1

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
	@cp -af src-1.2.1/libpam/include/security/* $(SRC_TOP_DIR)/include/

	@cp -a src-1.2.1/libpam/.libs/libpam.so*  $(SRC_TOP_DIR)/$(TARGET)-lib/
	@mkdir -p $(SRC_TOP_DIR)/include/security
	@mkdir -p $(SRC_TOP_DIR)/include/pam
	@cp -af src-1.2.1/libpam/include/security/* $(SRC_TOP_DIR)/include/security/
	@cp -af src-1.2.1/libpam/include/security/* $(SRC_TOP_DIR)/include/pam/
	@cp -a src-1.2.1/libpam/.libs/libpam.so* $(INSTALL_ROOT)/lib
	@mkdir -p $(INSTALL_PAM)
	@cp -a src-1.2.1/modules/pam_stress/.libs/pam_stress.so $(INSTALL_PAM)
	@cp -a src-1.2.1/modules/pam_permit/.libs/pam_permit.so $(INSTALL_PAM)
	@cp -a src-1.2.1/modules/pam_debug/.libs/pam_debug.so $(INSTALL_PAM)
	@cp -a src-1.2.1/modules/pam_filter/.libs/pam_filter.so $(INSTALL_PAM)
	@cp -a src-1.2.1/modules/pam_deny/.libs/pam_deny.so $(INSTALL_PAM)
	@cp -a src-1.2.1/modules/pam_tally/.libs/pam_tally.so $(INSTALL_PAM)
	@cp -a src-1.2.1/modules/pam_timestamp/.libs/pam_timestamp.so $(INSTALL_PAM)
	@cp -a src-1.2.1/modules/pam_securetty/.libs/pam_securetty.so $(INSTALL_PAM)
	@cp -a src-1.2.1/modules/pam_rhosts/.libs/pam_rhosts.so $(INSTALL_PAM)
	@cp -a src-1.2.1/modules/pam_keyinit/.libs/pam_keyinit.so $(INSTALL_PAM)
	@cp -a src-1.2.1/modules/pam_issue/.libs/pam_issue.so $(INSTALL_PAM)
	@cp -a src-1.2.1/modules/pam_exec/.libs/pam_exec.so $(INSTALL_PAM)
	@cp -a src-1.2.1/modules/pam_warn/.libs/pam_warn.so $(INSTALL_PAM)
	@cp -a src-1.2.1/modules/pam_mkhomedir/.libs/pam_mkhomedir.so $(INSTALL_PAM)
	@cp -a src-1.2.1/modules/pam_rootok/.libs/pam_rootok.so $(INSTALL_PAM)
	@cp -a src-1.2.1/modules/pam_limits/.libs/pam_limits.so $(INSTALL_PAM)
	@cp -a src-1.2.1/modules/pam_faildelay/.libs/pam_faildelay.so $(INSTALL_PAM)
	@cp -a src-1.2.1/modules/pam_shells/.libs/pam_shells.so $(INSTALL_PAM)
	@cp -a src-1.2.1/modules/pam_group/.libs/pam_group.so $(INSTALL_PAM)
	@cp -a src-1.2.1/modules/pam_env/.libs/pam_env.so $(INSTALL_PAM)
	@cp -a src-1.2.1/modules/pam_tally2/.libs/pam_tally2.so $(INSTALL_PAM)
	@cp -a src-1.2.1/modules/pam_succeed_if/.libs/pam_succeed_if.so $(INSTALL_PAM)
	@cp -a src-1.2.1/modules/pam_echo/.libs/pam_echo.so $(INSTALL_PAM)
	@cp -a src-1.2.1/modules/pam_time/.libs/pam_time.so $(INSTALL_PAM)
	@cp -a src-1.2.1/modules/pam_wheel/.libs/pam_wheel.so $(INSTALL_PAM)
	@cp -a src-1.2.1/modules/pam_umask/.libs/pam_umask.so $(INSTALL_PAM)
	@cp -a src-1.2.1/modules/pam_access/.libs/pam_access.so $(INSTALL_PAM)
	@cp -a src-1.2.1/modules/pam_lastlog/.libs/pam_lastlog.so $(INSTALL_PAM)
	@cp -a src-1.2.1/modules/pam_xauth/.libs/pam_xauth.so $(INSTALL_PAM)
	@cp -a src-1.2.1/modules/pam_unix/.libs/pam_unix.so $(INSTALL_PAM)
	@cp -a src-1.2.1/modules/pam_nologin/.libs/pam_nologin.so $(INSTALL_PAM)
	@cp -a src-1.2.1/modules/pam_pwhistory/.libs/pam_pwhistory.so $(INSTALL_PAM)
	@cp -a src-1.2.1/modules/pam_loginuid/.libs/pam_loginuid.so $(INSTALL_PAM)
	@cp -a src-1.2.1/modules/pam_ftp/.libs/pam_ftp.so $(INSTALL_PAM)
	@cp -a src-1.2.1/modules/pam_motd/.libs/pam_motd.so $(INSTALL_PAM)
	@cp -a src-1.2.1/modules/pam_listfile/.libs/pam_listfile.so $(INSTALL_PAM)
	@cp -a src-1.2.1/modules/pam_mail/.libs/pam_mail.so $(INSTALL_PAM)
	@cp -a src-1.2.1/modules/pam_localuser/.libs/pam_localuser.so $(INSTALL_PAM)

	#cp -a src/lib/.libs/libfreeradius-client.so* $(INSTALL_ROOT)/lib
	#cp -a src/src/.libs/* $(INSTALL_ROOT)/sbin/
	@mkdir -p $(INSTALL_ROOT)/usr/mv2ram/etc/pam.d
	@install -m 0644 files/other_secure.pam		$(INSTALL_ROOT)/usr/mv2ram/etc/pam.d/other

uninstall:

.PHONY: all clean

clean:
	if [ -e src-1.2.1/Makefile ]; then make -C ./src-1.2.1 clean; fi;

distclean:
	if [ -e src-1.2.1/Makefile ]; then make -C ./src-1.2.1 distclean; fi;
