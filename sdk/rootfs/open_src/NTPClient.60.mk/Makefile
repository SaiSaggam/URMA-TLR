
include ../../makefile.envars

CHECKOUT_PREFIX ?= svn://svn.atc.amit.com.tw
COMPO_URL := $(CHECKOUT_PREFIX)/Sahara/NTPClient/trunk

COMPO_NAME := $(word 1,$(subst ., ,$(notdir $(CURDIR))))
VERSION := $(shell cd $(GLUE); sh version.sh $(COMPO_NAME))
ifeq ($(VERSION),0)
COMPO_REV :=
else
COMPO_REV := -r $(VERSION)
endif

COMPO_DIR := $(COMPO_BASE)/open_src/NTPClient

include ../../makefile.compo
PRIV_TOOLS := $(TOPDIR)/tools/priv
OPEN_TOOLS := $(TOPDIR)/tools/open

PREFIX += PROJECT_NAME=${PROJECT_NAME} 
PREFIX += PROJECT_CUSTOMER=${PROJECT_CUSTOMER} 
PREFIX += PRIV_TOOLS=${PRIV_TOOLS} 
PREFIX += INSTALL_ROOT=$(INSTALL_ROOT) 
PREFIX += ENDIAN=$(ENDIAN) 

build:
	@make -C $(COMPO_DIR) ${PREFIX}

ifeq ($(shell if test $(word 2, ${COMPO_REV}) -lt 9 2>/dev/null; then echo 1; else echo 0; fi), 1)
include ../../makefile.install
install: INSTALL.copy_NTPClient
 
else
install:
	@make -C $(COMPO_DIR) install ${PREFIX}

endif        

clean:
	@make -C $(COMPO_DIR) clean ${PREFIX}

copying:     

