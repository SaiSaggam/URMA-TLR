#
# Makefile to compile socat - Multipurpose relay (SOcket CAT)
#                     (netcat++)

include ../../makefile.envars

#CHECKOUT_PREFIX ?= svn://svn.atc.amit.com.tw
#COMPO_URL := $(CHECKOUT_PREFIX)/Sahara/Socat/trunk

COMPO_NAME := $(word 1,$(subst ., ,$(notdir $(CURDIR))))
ifeq ($(VERSION),0)
COMPO_REV :=
else
COMPO_REV := -r $(VERSION)
endif

COMPO_DIR := $(COMPO_BASE)/open_src/Socat/

include ../../makefile.compo
PRIV_TOOLS := $(TOPDIR)/tools/priv
OPEN_TOOLS := $(TOPDIR)/tools/open

PREFIX += PROJECT_NAME=${PROJECT_NAME} 
PREFIX += PROJECT_CUSTOMER=${PROJECT_CUSTOMER} 
PREFIX += PRIV_TOOLS=${PRIV_TOOLS} 
PREFIX += INSTALL_ROOT=$(INSTALL_ROOT) 
PREFIX += ENDIAN=$(ENDIAN) 

ifeq ($(BIG_ENDIAN),1)
ENDIAN := -b
else
ENDIAN :=
endif

build:
	@make -C $(COMPO_DIR) all ${PREFIX}

ifeq ($(shell if test $(word 2, ${COMPO_REV}) -lt 5 2>/dev/null; then echo 1; else echo 0; fi), 1)
include ../../makefile.install
install: INSTALL.copy_Socat
 
else
install:
	@make -C $(COMPO_DIR) install ${PREFIX}

endif

uninstall:
	@make -C $(COMPO_DIR) uninstall ${PREFIX}

clean:
	@make -C $(COMPO_DIR) clean ${PREFIX}

distclean:
	@make -C $(COMPO_DIR) distclean ${PREFIX}
copying:

