#
# Makefile to compile uci 
#

include ../../makefile.envars
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

COMPO_DIR := $(COMPO_BASE)/open_src/uci/build

build:
	echo $(COMPO_DIR)
	-mkdir -p $(COMPO_DIR)
	@cd $(COMPO_DIR); cmake ..
	@make -C $(COMPO_DIR) uci cli ${PREFIX} 

install:
	@cp $(COMPO_DIR)/libuci.so $(INSTALL_ROOT)/usr/lib/
	@cp $(COMPO_DIR)/uci $(INSTALL_ROOT)/sbin/uci_exe

	# Install UCI helper scripts
	@mkdir -p $(INSTALL_ROOT)/lib/config
	@install -m 0664 $(COMPO_DIR)/../files/uci.sh $(INSTALL_ROOT)/lib/config
	@install -m 0775 $(COMPO_DIR)/../files/functions.sh $(INSTALL_ROOT)/lib

clean:
	-rm -rf $(COMPO_DIR) 

distclean:
	@make -C $(COMPO_DIR) distclean

copying: 

