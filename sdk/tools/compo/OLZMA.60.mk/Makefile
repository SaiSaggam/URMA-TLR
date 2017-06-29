#
# Makefile to compile OBridgeUtil
#


CHECKOUT_PREFIX ?= svn://svn.atc.amit.com.tw
COMPO_URL := $(CHECKOUT_PREFIX)/Sahara/OLZMA/trunk
COMPO_REV := -r 9
COMPO_DIR := $(COMPO_BASE)/OLZMA
GPL_LEVEL := 2

include ../makefile.common

build:
	make -C $(COMPO_DIR) all	

install:
	-mkdir -p $(INSTALL_ROOT)/open/bin
	cp -a $(COMPO_DIR)/src/src/lzma/lzma $(INSTALL_ROOT)/open/bin

clean:
	make -C $(COMPO_DIR) clean

manual_gpl: 
	@-mv $(INSTALL_ROOT)/open/bin/lzma $(GPLSRC_PDIR)/open/bin/lzma

#This section is for RELEASE IN SDK COPY
manual_sdk: 
	@-mv $(INSTALL_ROOT)/open/bin/lzma $(SDKSRC_PDIR)/open/bin/lzma

copying:
