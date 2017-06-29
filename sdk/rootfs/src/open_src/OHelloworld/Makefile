# RDS/00008: OASIS Makefile Conventions

#for test
TOPDIR := $(shell cd ../../../../; pwd)
TARGET ?= mips-linux-uclibc
KERNEL_DIR ?= ${TOPDIR}/linux/src

ifeq ($(TARGET), mips-linux-uclibc)
	ARCH = mips
	STRIP_TAG = MIPS
	LDFLAGS+=-L$(COMPO_BASE)/$(TARGET)-lib
endif
ifeq ($(TARGET), mipsel-linux-uclibc)
	ARCH = mips
	STRIP_TAG = MIPS
	LDFLAGS+=-L$(COMPO_BASE)/$(TARGET)-lib
endif

ifndef TOOLPATH
    export TOOLPATH := $(shell cat $(TOPDIR)/TOOLPATH2)
	export PATH     := $(TOOLPATH):$(PATH)
endif

include rds00008.mk

INCLUDEDIR= ../../include/
CSIDDIR= ../../include/csid

CFLAGS += -I$(INCLUDEDIR)
CFLAGS += -I$(CSIDDIR)
LFLAGS += $(LIBPATH) -lcsman

export LDFLAGS LFLAGS
export CFLAGS = -I$(COMPO_BASE)/include

all: precheck
	if [ ! -e src/Makefile ]; then \
		cd src; ./configure --with-kernel-dir=$(KERNEL_DIR) --host=$(TARGET) CC=$(CROSS_COMPILE)gcc STRIP=$(CROSS_COMPILE)strip;\
	fi; 
	make -C ./src
	
precheck:
	@echo "Build OHelloworld for $(PROJECT_NAME)"
ifeq ($(PROJECT_CUSTOMER), ) 
	@$(error You need specify PROJECT_CUSTOMER for Makefile)
endif 

postcheck:
	@echo "Install OHelloworld to $(INSTALL_ROOT)" 
ifeq ($(INSTALL_ROOT), ) 
	@$(error You need specify INSTALL_ROOT for Makefile)
endif 

install: postcheck
	@cp -a src/helloworld					$(INSTALL_ROOT)/usr/bin
	@cp -a helloworld-action				$(INSTALL_ROOT)/usr/bin

uninstall: postcheck
	@rm -f $(INSTALL_ROOT)/usr/bin/helloworld
	@rm -f $(INSTALL_ROOT)/usr/bin/helloworld-action


.PHONY: all clean
	make -C src module minstall programs install

clean:
	if [ -e src/Makefile ]; then make -C ./src clean; fi;

distclean:
	if [ -e src/Makefile ]; then make -C ./src distclean; fi;

