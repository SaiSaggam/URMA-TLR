#
# Makefile to compile OL2TP
#

include ../../makefile.envars

CHECKOUT_PREFIX ?= svn://svn.atc.amit.com.tw
COMPO_URL := $(CHECKOUT_PREFIX)/Sahara/OL2TP/trunk

COMPO_NAME := $(word 1,$(subst ., ,$(notdir $(CURDIR))))
ifeq ($(VERSION),0)

ifeq ($(ENABLE_MULTI_WAN),1)
COMPO_REV_ := -r 78

ifeq (${COMMANDER_TRUNK2}, 1)
COMPO_REV_ :=
endif

COMPO_REV := ${COMPO_REV_}

else
COMPO_REV := -r 180 #trunk
endif

ifeq (${PROJECT_NAME}, ${filter ${PROJECT_NAME}, BDW571AM_002 BDW571AM_L02})
COMPO_REV := -r 107
endif

ifeq (${PROJECT_NAME}, ${filter ${PROJECT_NAME}, BDE850IO_001})
COMPO_REV := -r 205
endif

else
COMPO_REV := -r $(VERSION)
endif

COMPO_DIR := $(COMPO_BASE)/open_src/OL2TP

ifeq ($(ENABLE_MULTI_WAN),1)
COMPO_URL_ := $(CHECKOUT_PREFIX)/Sahara/OL2TP/branches/BDW462AML_20100323

ifeq (${COMMANDER_TRUNK2}, 1)
COMPO_URL_ := $(CHECKOUT_PREFIX)/Sahara/OL2TP/branches/CIH573AM_U11_20110816
endif

COMPO_URL := ${COMPO_URL_}
endif

ifeq (${PROJECT_NAME}, ${filter ${PROJECT_NAME}, BDW571AM_002 BDW571AM_L02})
COMPO_URL := $(CHECKOUT_PREFIX)/Sahara/OL2TP/branches/CIH573AM_U11_20110816
endif

ifeq (${PROJECT_NAME}, ${filter ${PROJECT_NAME}, BDE850IO_001})
COMPO_URL := $(CHECKOUT_PREFIX)/Sahara/OL2TP/branches/OL2TP_BDE850IO_001
endif

# Use old version scripts
ifeq (${PROJECT_NAME}, ${filter ${PROJECT_NAME}, })
USE_SCRIPTS_V1=1
endif

include ../../makefile.compo
PRIV_TOOLS := $(TOPDIR)/tools/priv
OPEN_TOOLS := $(TOPDIR)/tools/open

PREFIX += PROJECT_NAME=${PROJECT_NAME} 
PREFIX += PROJECT_CUSTOMER=${PROJECT_CUSTOMER} 
PREFIX += PRIV_TOOLS=${PRIV_TOOLS} 
PREFIX += INSTALL_ROOT=$(INSTALL_ROOT) 
PREFIX += ENDIAN=$(ENDIAN)  
PREFIX += KERNEL_DIR=$(KERNEL_PATH) 
PREFIX += ARCH=mips

ifeq ($(HANDLE_OPEN_L2TP), 1)
# Must install and use plugin pppol2tp.so from OPPP
# For kernel version >= 2.6.23
PREFIX += KERNEL_MODE=1
else
PREFIX += KERNEL_MODE=
endif

PREFIX += USE_SCRIPTS_V1=$(USE_SCRIPTS_V1) 

ifeq ($(BIG_ENDIAN),1)
ENDIAN := -b
else
ENDIAN :=
endif

build:
	@make -C $(COMPO_DIR) all ${PREFIX}

#ifeq ($(ENABLE_MULTI_WAN),1)

#ifeq (${PROJECT_NAME}, ${filter ${PROJECT_NAME},CIH573AM_U11 CID561NC BDW571AM_L02 CDD531AM_U02 CID688NC})
#install:
#	@make -C $(COMPO_DIR) install ${PREFIX}
#else
#ifdef FULL_COPY
#	${PRIV_TOOLS}/bin/mkuo ${COMPO_DIR}/l2tp.conf ${COMPO_DIR}/l2tp.conf.uo $(ENDIAN)
#endif
#install: INSTALL.copy_OL2TP
#endif

#else 
install: 
	@make -C $(COMPO_DIR) install ${PREFIX}

#endif
uninstall: 
	@make -C $(COMPO_DIR) uninstall ${PREFIX}

clean:
	make -C $(COMPO_DIR) clean ${PREFIX}

copying:
