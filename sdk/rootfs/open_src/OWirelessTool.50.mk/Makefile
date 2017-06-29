#
# Makefile to compile OWirelessTool
#

include ../../makefile.envars

CHECKOUT_PREFIX ?= svn://svn.atc.amit.com.tw
COMPO_URL := $(CHECKOUT_PREFIX)/Sahara/OWirelessTool/trunk

COMPO_NAME := $(word 1,$(subst ., ,$(notdir $(CURDIR))))
ifeq ($(VERSION),0)
COMPO_REV :=

ifeq (${KERNEL_SDK}, ${filter ${KERNEL_SDK}, 3200 3300 3400})
COMPO_REV := -r 4
endif

else
COMPO_REV := -r $(VERSION)
endif

COMPO_DIR := $(COMPO_BASE)/open_src/OWirelessTool


include ../../makefile.compo
PRIV_TOOLS := $(TOPDIR)/tools/priv
OPEN_TOOLS := $(TOPDIR)/tools/open

PREFIX += PROJECT_NAME=${PROJECT_NAME} 
PREFIX += PROJECT_CUSTOMER=${PROJECT_CUSTOMER} 
PREFIX += PRIV_TOOLS=${PRIV_TOOLS} 
PREFIX += INSTALL_ROOT=$(INSTALL_ROOT) 
PREFIX += ENDIAN=$(ENDIAN)
PREFIX += KERNEL_SDK=$(KERNEL_SDK)

build:
	@make -C $(COMPO_DIR) all ${PREFIX}

install:
ifeq (${KERNEL_SDK}, ${filter ${KERNEL_SDK}, 3200 3300 3400})
	@cp -a  $(COMPO_DIR)/iwconfig		$(INSTALL_ROOT)/usr/sbin
	@cp -a  $(COMPO_DIR)/iwpriv 		$(INSTALL_ROOT)/usr/sbin
	@#cp -a $(COMPO_DIR)/iwspy 			$(INSTALL_ROOT)/usr/sbin
	@#cp -a $(COMPO_DIR)/iwevent 		$(INSTALL_ROOT)/usr/sbin
	@#cp -a $(COMPO_DIR)/ifrename 		$(INSTALL_ROOT)/usr/sbin
	@if [ -e $(COMPO_DIR)/libiw.so.29 ]; then cp -a $(COMPO_DIR)/libiw.so.29 $(INSTALL_ROOT)/lib/ ; fi 
else
	@make -C $(COMPO_DIR) wt-install ${PREFIX}
endif

clean:
	@make -C $(COMPO_DIR) realclean ${PREFIX}

copying:
