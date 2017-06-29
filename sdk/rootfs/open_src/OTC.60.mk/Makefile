
include ../../makefile.envars

CHECKOUT_PREFIX ?= svn://svn.atc.amit.com.tw
COMPO_URL := $(CHECKOUT_PREFIX)/Sahara/OTC/trunk

COMPO_NAME := $(word 1,$(subst ., ,$(notdir $(CURDIR))))
ifeq ($(VERSION),0)
COMPO_REV :=
else
COMPO_REV := -r $(VERSION)
endif

COMPO_DIR := $(COMPO_BASE)/open_src/OTC

include ../../makefile.compo
PRIV_TOOLS := $(TOPDIR)/tools/priv
OPEN_TOOLS := $(TOPDIR)/tools/open

PREFIX += PROJECT_NAME=${PROJECT_NAME} 
PREFIX += PROJECT_CUSTOMER=${PROJECT_CUSTOMER} 
PREFIX += PRIV_TOOLS=${PRIV_TOOLS} 
PREFIX += INSTALL_ROOT=$(INSTALL_ROOT) 
PREFIX += ENDIAN=$(ENDIAN) 
PREFIX += KERNEL_DIR=${TOPDIR}/linux 
PREFIX += IPTABLES_DIR=${TOPDIR}/rootfs/src/open_src/OIPTables
PREFIX += HANDLE_LOADSHARING=${HANDLE_LOADSHARING}
PREFIX += HANDLE_IPV6=${HANDLE_IPV6}
PREFIX += HANDLE_IPSEC=${HANDLE_IPSEC}
PREFIX += HANDLE_MULTI_WAN=${ENABLE_MULTI_WAN}
PREFIX += HANDLE_INET_PPPOE=${HANDLE_INET_PPPOE}

build:
	make -C $(COMPO_DIR) ${PREFIX}

ifeq ($(shell if test $(word 2, ${COMPO_REV}) -lt 8 2>/dev/null; then echo 1; else echo 0; fi), 1)
include ../../makefile.install
install: INSTALL.copy_OTC
 
else
install:
	@make -C $(COMPO_DIR) tc-install ${PREFIX}

endif   

clean:
	make -C $(COMPO_DIR) clean ${PREFIX} 

copying:
