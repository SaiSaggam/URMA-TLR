#
# Makefile to compile OIPTables
#

include ../../makefile.envars

CHECKOUT_PREFIX ?= svn://svn.atc.amit.com.tw
COMPO_URL := $(CHECKOUT_PREFIX)/Sahara/OIPTables142/trunk

COMPO_NAME := $(word 1,$(subst ., ,$(notdir $(CURDIR))))
ifeq ($(VERSION),0)
COMPO_REV :=
else
COMPO_REV := -r $(VERSION)
endif

COMPO_DIR := $(COMPO_BASE)/open_src/OIPTables142

include ../../makefile.compo
PRIV_TOOLS := $(TOPDIR)/tools/priv
OPEN_TOOLS := $(TOPDIR)/tools/open

#
# Specify the version of iptable (The default is 1.4.10)
# SDK 3200/3300/3400/3500 use 1.4.2
#
ifeq (${KERNEL_SDK}, ${filter ${KERNEL_SDK}, 3200 3300 3400 3500})
IPT_VER :=
else
#IPT_VER := 1.4.10
IPT_VER := 1.4.21
endif

#
# 74/54 FW 
#
MK_TARGET :=
ifeq ($(HANDLE_54FW), 1)
MK_TARGET := -f Makefile.54fw
endif

PREFIX += PROJECT_NAME=${PROJECT_NAME} 
PREFIX += PROJECT_CUSTOMER=${PROJECT_CUSTOMER} 
PREFIX += PRIV_TOOLS=${PRIV_TOOLS} 
PREFIX += INSTALL_ROOT=$(INSTALL_ROOT) 
PREFIX += ENDIAN=$(ENDIAN) 
PREFIX += KERNEL_DIR=${TOPDIR}/linux/src
PREFIX += IPTABLES_SRC=$(IPT_SRC) 
PREFIX += ARCH=mips 
PREFIX += HANDLE_LOADSHARING=$(HANDLE_LOADSHARING) 
PREFIX += HANDLE_MULTIWAN=$(ENABLE_MULTI_WAN) 
PREFIX += HANDLE_IPV6=$(HANDLE_IPV6) 
PREFIX += QOS=${QOS}
PREFIX += CL_QOS=${CL_QOS}
PREFIX += SMART_QOS=${SMART_QOS}
ifeq ($(TC3262),1)
PREFIX += TC3262=${TC3262}
endif
PREFIX += IPT_VER=${IPT_VER}
PREFIX += HANDLE_PSD=${HANDLE_PSD}
PREFIX += HANDLE_L7_BLOCK=${HANDLE_L7_BLOCK}
PREFIX += HANDLE_SMART_QOS=${HANDLE_SMART_QOS}
PREFIX += HANDLE_RULE_BASED_QOS=${HANDLE_RULE_BASED_QOS}
PREFIX += HANDLE_VPN=${HANDLE_VPN}

build:
	@make -C $(COMPO_DIR) ${MK_TARGET} ${PREFIX}
	
install: 
	@make -C $(COMPO_DIR) ${MK_TARGET} install ${PREFIX}

clean:
	@make -C $(COMPO_DIR) ${MK_TARGET} distclean ${PREFIX}

copying:

