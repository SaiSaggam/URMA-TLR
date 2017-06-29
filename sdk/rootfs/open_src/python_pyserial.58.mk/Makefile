#
# Makefile to compile Python - PySerial extension
#

include ../../makefile.envars

COMPO_NAME := $(word 1,$(subst ., ,$(notdir $(CURDIR))))
VERSION := $(shell cd $(GLUE); sh version.sh $(COMPO_NAME))
ifeq ($(VERSION),0)
COMPO_REV :=
else
COMPO_REV := -r $(VERSION)
endif

COMPO_DIR := $(COMPO_BASE)/open_src/python_pyserial

include ../../makefile.compo
PRIV_TOOLS := $(TOPDIR)/tools/priv
OPEN_TOOLS := $(TOPDIR)/tools/open

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

copying: 

