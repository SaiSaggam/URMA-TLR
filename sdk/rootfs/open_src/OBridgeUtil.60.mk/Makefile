#
# Makefile to compile OBridgeUtil
#

include ../../makefile.envars

CHECKOUT_PREFIX ?= svn://svn.atc.amit.com.tw
COMPO_URL := $(CHECKOUT_PREFIX)/Sahara/OBridgeUtil/trunk

COMPO_NAME := $(word 1,$(subst ., ,$(notdir $(CURDIR))))
ifeq ($(VERSION),0)
COMPO_REV :=
else
COMPO_REV := -r $(VERSION)
endif

COMPO_DIR := $(COMPO_BASE)/open_src/OBridgeUtil

include ../../makefile.compo
PRIV_TOOLS := $(TOPDIR)/tools/priv
OPEN_TOOLS := $(TOPDIR)/tools/open

PREFIX += PROJECT_NAME=${PROJECT_NAME} 
PREFIX += PROJECT_CUSTOMER=${PROJECT_CUSTOMER} 
PREFIX += PRIV_TOOLS=${PRIV_TOOLS} 
PREFIX += INSTALL_ROOT=$(INSTALL_ROOT) 
PREFIX += ENDIAN=$(ENDIAN) 
PREFIX += KERNEL_DIR=${TOPDIR}/linux/src

KERNEL_INC_DIR = $(KERNEL_DIR)/usr/include

build:
	@if [ ! -e $(COMPO_DIR)/configure ]; then \
	    cd $(COMPO_DIR); autoconf -o configure configure.in; \
	fi;
	@if [ ! -e $(COMPO_DIR)/Makefile ]; then \
	    cd $(COMPO_DIR); ./configure \
		--host=$(TARGET) \
		--with-linux-headers=$(KERNEL_INC_DIR) \
		CC=$(CROSS_COMPILE)gcc \
		STRIP=$(CROSS_COMPILE)strip;\
	fi;
	@make -C $(COMPO_DIR) ${PREFIX}

ifeq ($(shell if test $(word 2, ${COMPO_REV}) -lt 5 2>/dev/null; then echo 1; else echo 0; fi), 1)
include ../../makefile.install
install: INSTALL.copy_OBridgeUtil
 
else
install:
#	@make -C $(COMPO_DIR) install ${PREFIX}
	@cp -f $(COMPO_DIR)/brctl/brctl    $(INSTALL_ROOT)/usr/sbin

endif        

clean:
	@make -C $(COMPO_DIR) clean ${PREFIX}
	@rm -f $(COMPO_DIR)/brctl/Makefile
	@rm -f $(COMPO_DIR)/Makefile
	@rm -f $(COMPO_DIR)/configure

distclean:
	@make -C $(COMPO_DIR) distclean ${PREFIX}
	@rm -f $(COMPO_DIR)/brctl/Makefile
	@rm -f $(COMPO_DIR)/Makefile
	@rm -f $(COMPO_DIR)/configure

copying: 

