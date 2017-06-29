#
# Makefile to compile OQuaggaRouging
#

include ../../makefile.envars

CHECKOUT_PREFIX ?= svn://svn.atc.amit.com.tw
COMPO_URL := $(CHECKOUT_PREFIX)/Sahara/Oopenssl_102d/trunk
COMPO_REV := -r 20
COMPO_DIR := $(COMPO_BASE)/open_src/Oopenssl_102d
SRC_TOP_DIR = $(shell cd ../../; pwd)

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

INSTALL_DIR = $(COMPO_DIR)/_install


# WITH_VTYSH = 1


build:
	@if test ! -e $(COMPO_DIR)/configed ; then ./configure $(COMPO_DIR) $(TARGET)  i386-redhat-linux ; fi
	@touch $(COMPO_DIR)/configed 
	@make -C $(COMPO_DIR) ${PREFIX}
#	@$(TARGET)-strip $(COMPO_DIR)/ripd/ripd
#	@$(TARGET)-strip $(COMPO_DIR)/zebra/zebra

install:
	@make -C $(COMPO_DIR) install_sw INSTALL_PREFIX=$(INSTALL_DIR)

	@mkdir -p $(INSTALL_ROOT)/usr/bin
	@cp -a ${INSTALL_DIR}/usr/bin/openssl		$(INSTALL_ROOT)/usr/bin

	@if [ ! -d $(SRC_TOP_DIR)/src/include/openssl ]; then \
	   mkdir $(SRC_TOP_DIR)/src/include/openssl;       \
	fi;
	@cp -af $(INSTALL_DIR)/usr/include/openssl/*	$(SRC_TOP_DIR)/src/include/openssl/

	# Fix library permissions
	@find $(INSTALL_DIR)/usr/lib -type f -name "lib*" ! -perm -u=w -print0 | xargs -0 -r chmod u+w;

	@cp -af $(INSTALL_DIR)/usr/lib/lib*		$(SRC_TOP_DIR)/src/$(TARGET)-lib/

	@mkdir -p $(INSTALL_ROOT)/usr/lib
	@cp -a $(INSTALL_DIR)/usr/lib/*.so*		$(INSTALL_ROOT)/usr/lib
	@cp -a $(INSTALL_DIR)/usr/lib/engines		$(INSTALL_ROOT)/usr/lib
	@cp -a $(INSTALL_DIR)/usr/lib/ssl		$(INSTALL_ROOT)/usr/lib
	@rm -rf $(INSTALL_ROOT)/usr/lib/ssl/man

	#@mkdir -p $(INSTALL_ROOT)/usr/bin/openssl_test
	#@cp -a ${COMPO_DIR}/test/*test					$(INSTALL_ROOT)/usr/bin/openssl_test
	#@chmod 755 $(INSTALL_ROOT)/usr/bin/openssl_test/*

clean:
	@rm -f $(COMPO_DIR)/configed
	@rm -rf $(INSTALL_DIR)
	#@-rm -f ${COMPO_DIR}/*.conf.uo
	if [ -e $(COMPO_DIR)/Makefile ]; then make -C $(COMPO_DIR) clean ${PREFIX}; fi;
#	@make -C $(COMPO_DIR) clean ${PREFIX}
#	@make -C $(COMPO_DIR)/Libs/Libncurses clean ${PREFIX}

copying:

