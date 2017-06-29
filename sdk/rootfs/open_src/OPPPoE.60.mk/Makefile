#
# Makefile to compile OPPPoE
#

include ../../makefile.envars

CHECKOUT_PREFIX ?= svn://svn.atc.amit.com.tw
COMPO_URL := $(CHECKOUT_PREFIX)/Sahara/OPPPoE/trunk

COMPO_NAME := $(word 1,$(subst ., ,$(notdir $(CURDIR))))
ifeq ($(VERSION),0)
COMPO_REV :=
else
COMPO_REV := -r $(VERSION)
endif

COMPO_DIR := $(COMPO_BASE)/open_src/OPPPoE

include ../../makefile.compo
PRIV_TOOLS := $(TOPDIR)/tools/priv
OPEN_TOOLS := $(TOPDIR)/tools/open

PREFIX += PROJECT_NAME=${PROJECT_NAME} 
PREFIX += PROJECT_CUSTOMER=${PROJECT_CUSTOMER} 
PREFIX += PRIV_TOOLS=${PRIV_TOOLS} 
PREFIX += INSTALL_ROOT=$(INSTALL_ROOT) 
PREFIX += ENDIAN=$(ENDIAN) 
PREFIX += HANDLE_IPV6=${HANDLE_IPV6}
PREFIX += HANDLE_VOIPWAN=${HANDLE_VOIPWAN}
PREFIX += HANDLE_INET_PPPOE=${HANDLE_INET_PPPOE}
PREFIX += HANDLE_DNSQ_AUTOBACK=${HANDLE_DNSQ_AUTOBACK}
PREFIX += HANDLE_RUSSIAN=${HANDLE_RUSSIAN}
ifeq ($(BIG_ENDIAN),1)
ENDIAN := -b
else
ENDIAN :=
endif

build:
	make -C $(COMPO_DIR)/src ${PREFIX}

ifdef FULL_COPY
	${PRIV_TOOLS}/bin/mkuo ${COMPO_DIR}/configs/pppoe.conf ${COMPO_DIR}/configs/pppoe.conf.uo $(ENDIAN)

ifeq (${HANDLE_IPV6}, 1)
	${PRIV_TOOLS}/bin/mkuo ${COMPO_DIR}/configs/pppoev6.conf ${COMPO_DIR}/configs/pppoev6.conf.uo $(ENDIAN)
endif
ifeq (${HANDLE_VOIPWAN}, 1)
	${PRIV_TOOLS}/bin/mkuo ${COMPO_DIR}/configs/pppoe-voipwan.conf ${COMPO_DIR}/configs/pppoe-voipwan.conf.uo $(ENDIAN)
endif
endif

ifeq ($(shell if test $(word 2, ${COMPO_REV}) -lt 20 2>/dev/null; then echo 1; else echo 0; fi), 1)
include ../../makefile.install
install: INSTALL.copy_OPPPoE
 
else
install:
	@make -C $(COMPO_DIR)/src install ${PREFIX}

endif 


clean:
	make -C $(COMPO_DIR)/src clean ${PREFIX}
	-rm -f $(COMPO_DIR)/configs/*.uo

copying:
