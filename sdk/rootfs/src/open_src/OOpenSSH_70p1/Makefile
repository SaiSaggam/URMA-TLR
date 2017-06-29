# RDS/00008: OASIS Makefile Conventions

#for test
TOPDIR := $(shell cd ../../../../; pwd)
TARGET ?= mipsel-linux-uclibc
KERNEL_DIR ?= ${TOPDIR}/linux/src/

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
LDFLAGS    += -lcrypto -lssl -lpam
LDLIB		= -L$(SRC_TOP_DIR)/$(TARGET)-lib
CFLAGS += -I$(INCLUDEDIR)

all: precheck
	echo "PATH="$(PATH)
	if [ ! -e src/Makefile ]; then \
	cd src;  ./configure --host=$(TARGET) \
	CC=$(CROSS_COMPILE)gcc \
	CXX=$(CROSS_COMPILE)g++ \
	LD=$(CROSS_COMPILE)gcc \
	STRIP=$(CROSS_COMPILE)strip \
	--prefix="/usr" \
	--with-ldflags="$(LDLIB)" \
	--with-ssl-dir=$(INCLUDEDIR) \
	--with-cflags="$(CFLAGS)" \
	--disable-lastlog --disable-utmp --disable-utmpx --disable-wtmp --disable-wtmpx \
	--with-pam; \
	fi;
	make -C src 
	
install: postcheck
	@mkdir -p $(INSTALL_ROOT)/usr/bin
	@cp -a src/ssh				$(INSTALL_ROOT)/usr/bin
	@chmod 755 $(INSTALL_ROOT)/usr/bin/ssh
	@cp -a src/scp				$(INSTALL_ROOT)/usr/bin
	@chmod 755 $(INSTALL_ROOT)/usr/bin/scp
	@cp -a src/ssh-add			$(INSTALL_ROOT)/usr/bin
	@chmod 755 $(INSTALL_ROOT)/usr/bin/ssh-add
	@cp -a src/ssh-agent			$(INSTALL_ROOT)/usr/bin
	@chmod 755 $(INSTALL_ROOT)/usr/bin/ssh-agent
	@cp -a src/ssh-keygen			$(INSTALL_ROOT)/usr/bin
	@chmod 755 $(INSTALL_ROOT)/usr/bin/ssh-keygen
	@cp -a src/ssh-keyscan			$(INSTALL_ROOT)/usr/bin
	@chmod 755 $(INSTALL_ROOT)/usr/bin/ssh-keyscan
	@cp -a src/sftp			$(INSTALL_ROOT)/usr/bin
	@chmod 755 $(INSTALL_ROOT)/usr/bin/sftp
	@mkdir -p $(INSTALL_ROOT)/usr/sbin
	@cp -a src/sshd			$(INSTALL_ROOT)/usr/sbin
	@chmod 755 $(INSTALL_ROOT)/usr/sbin/sshd
	@mkdir -p $(INSTALL_ROOT)/usr/libexec
	@cp -a src/ssh-keysign			$(INSTALL_ROOT)/usr/libexec
	@chmod 4711 $(INSTALL_ROOT)/usr/libexec/ssh-keysign
	@cp -a src/ssh-pkcs11-helper		$(INSTALL_ROOT)/usr/libexec
	@chmod 755 $(INSTALL_ROOT)/usr/libexec/ssh-pkcs11-helper
	@cp -a src/sftp-server			$(INSTALL_ROOT)/usr/libexec
	@chmod 755 $(INSTALL_ROOT)/usr/libexec/sftp-server
	@cp -a src/contrib/sshd.pam.generic		$(INSTALL_ROOT)/usr/libexec
	
	# Create /var/empty directory for privilege separation
	@install -d -m 0755 $(INSTALL_ROOT)/usr/mv2ram/var/empty
	#@mkdir -p ${INSTALL_ROOT}/usr/etc/init.d

	# DIGI init scripts
	@mkdir -p ${INSTALL_ROOT}/usr/etc/init.d
	@install -m 0755 files/init_sshd.sh	$(INSTALL_ROOT)/usr/etc/init.d/
	@mkdir -p ${INSTALL_ROOT}/usr/etc/ssh
	@install -m 0644 files/custom_banner	$(INSTALL_ROOT)/usr/etc/ssh/custom_banner
	@mkdir -p $(INSTALL_ROOT)/usr/mv2ram/etc/pam.d
	@install -m 0644 files/sshd.pam		$(INSTALL_ROOT)/usr/mv2ram/etc/pam.d/sshd
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
	if [ -e src/Makefile ]; then make -C ./src clean; fi;

distclean:
	if [ -e src/Makefile ]; then make -C ./src distclean; fi;

