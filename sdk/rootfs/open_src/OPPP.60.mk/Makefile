#
# Makefile to compile OPPP
#

include ../../makefile.envars

CHECKOUT_PREFIX ?= svn://svn.atc.amit.com.tw
COMPO_URL  := $(CHECKOUT_PREFIX)/Sahara/OPPP244/trunk

COMPO_NAME := $(word 1,$(subst ., ,$(notdir $(CURDIR))))
ifeq ($(VERSION),0)
COMPO_REV :=

ifeq ($(ENABLE_MULTI_WAN),1)
COMPO_REV_ := -r 145

ifeq (${COMMANDER_TRUNK2}, 1)
COMPO_REV_ = ${COMPO_REV}
endif

ifeq (${HANDLE_IPV6}, 1)
COMPO_REV_ = ${COMPO_REV}
endif

COMPO_REV := ${COMPO_REV_}
endif

ifeq (${PROJECT_NAME}, ${filter ${PROJECT_NAME}, BDW571AM_002 BDW571AM_L02})
COMPO_REV := -r 202
endif

else
COMPO_REV := -r $(VERSION)
endif

COMPO_DIR  := $(COMPO_BASE)/open_src/OPPP

include ../../makefile.compo
PRIV_TOOLS := $(TOPDIR)/tools/priv
OPEN_TOOLS := $(TOPDIR)/tools/open

PREFIX += PROJECT_NAME=${PROJECT_NAME} 
PREFIX += PROJECT_CUSTOMER=${PROJECT_CUSTOMER} 
PREFIX += PRIV_TOOLS=${PRIV_TOOLS} 
PREFIX += INSTALL_ROOT=$(INSTALL_ROOT) 
PREFIX += KERNEL_DIR=${TOPDIR}/linux/src
PREFIX += ENDIAN=$(ENDIAN) 
PREFIX += HANDLE_HALF_BRIDGE=$(HANDLE_HALF_BRIDGE) 
PREFIX += HANDLE_OPEN_L2TP=$(HANDLE_OPEN_L2TP) 
PREFIX += HANDLE_ACCEL_PPTP=$(HANDLE_ACCEL_PPTP) 
PREFIX += ENABLE_MULTI_WAN=$(ENABLE_MULTI_WAN) 
PREFIX += HANDLE_IPV6=$(HANDLE_IPV6) 
PREFIX += HANDLE_LCP_CHECK_CSQ=$(HANDLE_LCP_CHECK_CSQ) 
ifeq ($(TC3262),1)
PREFIX += TC3262=$(TC3262) 
endif

ifeq ($(BIG_ENDIAN),1)
ENDIAN := -b
else
ENDIAN :=
endif

build:
	make -C $(COMPO_DIR) ${PREFIX}

ifdef FULL_COPY
	${PRIV_TOOLS}/bin/mkuo ${COMPO_DIR}/etc.ppp/pap-secrets ${COMPO_DIR}/etc.ppp/pap-secrets.uo $(ENDIAN)
	${PRIV_TOOLS}/bin/mkuo ${COMPO_DIR}/etc.ppp/chap-secrets ${COMPO_DIR}/etc.ppp/chap-secrets.uo $(ENDIAN)
endif

ifeq ($(shell if test $(word 2, ${COMPO_REV}) -lt 149 2>/dev/null; then echo 1; else echo 0; fi), 1)
include ../../makefile.install
install: INSTALL.copy_OPPP
 
else
install:
	@make -C $(COMPO_DIR) ppp-install ${PREFIX}

endif 

clean:
	make -C $(COMPO_DIR) clean ${PREFIX}
	-rm -f $(COMPO_DIR)/etc.ppp/*.uo

copying:
