#
# Makefile to compile popt lib
#

include ../../makefile.envars

#CHECKOUT_PREFIX ?= svn://svn.atc.amit.com.tw
#COMPO_URL := $(CHECKOUT_PREFIX)/Sahara/OWget/trunk

COMPO_NAME := $(word 1,$(subst ., ,$(notdir $(CURDIR))))
VERSION := $(shell cd $(GLUE); sh version.sh $(COMPO_NAME))
ifeq ($(VERSION),0)
COMPO_REV :=
else
COMPO_REV := -r $(VERSION)
endif

COMPO_DIR := $(COMPO_BASE)/open_src/OWget

include ../../makefile.compo

#
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

build:
	make -C $(COMPO_DIR) ${PREFIX}

install: 
	make -C $(COMPO_DIR) install ${PREFIX}

clean:
	-make -C $(COMPO_DIR) clean ${PREFIX}

distclean:
	-make -C $(COMPO_DIR) distclean ${PREFIX}

uninstall: 
	-make -C $(COMPO_DIR) uninstall ${PREFIX}

copying:

