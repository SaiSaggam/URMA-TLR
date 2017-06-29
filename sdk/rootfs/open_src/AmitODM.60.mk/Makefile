#
# Makefile to compile OBridgeUtil
#

include ../../makefile.envars

CHECKOUT_PREFIX ?= svn://svn.atc.amit.com.tw
COMPO_URL := $(CHECKOUT_PREFIX)/Sahara/AmitODM/trunk

COMPO_NAME := $(word 1,$(subst ., ,$(notdir $(CURDIR))))
ifeq ($(VERSION),0)
COMPO_REV :=
else
COMPO_REV := -r $(VERSION)
endif

COMPO_DIR := $(COMPO_BASE)/open_src/AmitODM

include ../../makefile.compo
PRIV_TOOLS := $(TOPDIR)/tools/priv
OPEN_TOOLS := $(TOPDIR)/tools/open

MAIL_ALERT := 1
PREFIX += PROJECT_NAME=${PROJECT_NAME} 
PREFIX += PROJECT_CUSTOMER=${PROJECT_CUSTOMER} 
PREFIX += PRIV_TOOLS=${PRIV_TOOLS} 
PREFIX += INSTALL_ROOT=$(INSTALL_ROOT) 
PREFIX += ENDIAN=$(ENDIAN)   
PREFIX += MAIL_ALERT=$(MAIL_ALERT)
PREFIX += KERNEL_DIR=${TOPDIR}/linux/src

ifeq (${PROJECT_NAME}, CDS530AM)
TARGET_CDS :=mipsel-linux-gcc
endif


build: 
	@make -C $(COMPO_DIR) ${PREFIX}

install:
	@make -C $(COMPO_DIR) install ${PREFIX}


clean:
	if test -e "$(COMPO_DIR)/config.status"; then make -C $(COMPO_DIR) clean ${PREFIX}; fi;
#	@make -C $(COMPO_DIR) clean ${PREFIX}

copying:         
