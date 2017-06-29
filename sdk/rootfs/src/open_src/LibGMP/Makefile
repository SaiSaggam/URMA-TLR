# RDS/00008: OASIS Makefile Conventions

#for test
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

INCLUDEDIR= ../../amit_priv/include/
CSIDDIR= ../../amit_priv/include/csid

CFLAGS += -I$(INCLUDEDIR)
CFLAGS += -I$(CSIDDIR)

USE_SHARE_OBJ = 1

all: precheck
	if [ ! -e src/Makefile ]; then \
		cd src; ./configure --host=$(TARGET) CC=$(CROSS_COMPILE)gcc CXX=$(CROSS_COMPILE)g++ STRIP=$(CROSS_COMPILE)strip;\
	fi; 
	make -C ./src
	cp src/gmp.h ../../include
ifeq ($(USE_SHARE_OBJ), 1)
	$(STRIP) --strip-unneeded --strip-debug src/.libs/libgmp.so
	cp src/.libs/libgmp.so ../../$(TARGET)-lib
else
	$(STRIP) --strip-debug src/.libs/libgmp.a
	cp src/.libs/libgmp.a ../../$(TARGET)-lib
endif	

precheck:
	@echo "Build LibGMP for $(PROJECT_NAME)"
ifeq ($(PROJECT_CUSTOMER), ) 
	@$(error You need specify PROJECT_CUSTOMER for Makefile)
endif 

postcheck:
	@echo "Install LibGMP to $(INSTALL_ROOT)" 
ifeq ($(INSTALL_ROOT), ) 
	@$(error You need specify INSTALL_ROOT for Makefile)
endif 

install: postcheck
	@cp -a src/.libs/libgmp.so*	  $(INSTALL_ROOT)/lib/

uninstall: postcheck
	@rm -f $(INSTALL_ROOT)/lib/libgmp.so 

.PHONY: all clean
	make KERNELSRC=$(KERNEL_DIR) -C src module minstall programs install

clean:
	if [ -e src/Makefile ]; then make -C ./src clean; fi;

distclean:
	if [ -e src/Makefile ]; then make -C ./src distclean; fi;

