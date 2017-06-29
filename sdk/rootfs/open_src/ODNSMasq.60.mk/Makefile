#
# Makefile to compile popt lib
#

include ../../makefile.envars

#CHECKOUT_PREFIX ?= svn://svn.atc.amit.com.tw
#COMPO_URL := $(CHECKOUT_PREFIX)/Sahara/OKrb5/trunk
#COMPO_REV := -r 6
COMPO_DIR := $(COMPO_BASE)/open_src/ODNSMasq

include ../../makefile.compo

#
ifeq ($(BIG_ENDIAN),1)
ENDIAN := -b
else
ENDIAN :=
endif

PRIV_TOOLS := $(TOPDIR)/tools/priv

PREFIX += PROJECT_NAME=${PROJECT_NAME} 
PREFIX += PROJECT_CUSTOMER=${PROJECT_CUSTOMER} 
PREFIX += PRIV_TOOLS=${PRIV_TOOLS} 
PREFIX += INSTALL_ROOT=$(INSTALL_ROOT) 
PREFIX += ENDIAN=$(ENDIAN)    

build:
	make -C $(COMPO_DIR) ${PREFIX} COPTS=-DDIGI_RESOLV_SYMLINK_EXTENSION

install: 
	make -C $(COMPO_DIR) install ${PREFIX}

uninstall:
	make -C $(COMPO_DIR) uninstall ${PREFIX}

clean:
	make -C $(COMPO_DIR) clean ${PREFIX}

distclean:
	make -C $(COMPO_DIR) distclean ${PREFIX}

copying:
