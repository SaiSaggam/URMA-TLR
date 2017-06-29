#This is for RELEASE IN SDK COPY
#
# Makefile to compile OHelloworld
#

include ../../makefile.envars

CHECKOUT_PREFIX ?= svn://svn.atc.amit.com.tw
COMPO_URL := $(CHECKOUT_PREFIX)/Sahara/OHelloworld/trunk
COMPO_REV := -r 1
COMPO_DIR := $(COMPO_BASE)/open_src/OHelloworld

include ../../makefile.compo
PRIV_TOOLS := $(TOPDIR)/tools/priv
OPEN_TOOLS := $(TOPDIR)/tools/open

ifeq ($(BIG_ENDIAN),1)
ENDIAN := -b
else
ENDIAN :=
endif

PREFIX += PROJECT_NAME=${PROJECT_NAME} 
PREFIX += PROJECT_CUSTOMER=${PROJECT_CUSTOMER} 
PREFIX += PRIV_TOOLS=${PRIV_TOOLS} 
PREFIX += INSTALL_ROOT=$(INSTALL_ROOT) 
PREFIX += ENDIAN=$(ENDIAN) 
PREFIX += MAIL_ALERT=$(MAIL_ALERT) 
PREFIX += KERNEL_DIR=${TOPDIR}/linux/src

build:
	@make -C $(COMPO_DIR) ${PREFIX}

install:
	@make -C $(COMPO_DIR) install ${PREFIX}

clean:
	@make -C $(COMPO_DIR) clean ${PREFIX}

distclean:
	@make -C $(COMPO_DIR) distclean

copying: 

