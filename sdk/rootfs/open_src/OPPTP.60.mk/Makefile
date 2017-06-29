#
# Makefile to compile OPPTP
#

include ../../makefile.envars

CHECKOUT_PREFIX ?= svn://svn.atc.amit.com.tw
COMPO_URL := $(CHECKOUT_PREFIX)/Sahara/OPPTP/trunk

COMPO_NAME := $(word 1,$(subst ., ,$(notdir $(CURDIR))))
ifeq ($(VERSION),0)
COMPO_REV := -r 151

ifeq ($(ENABLE_MULTI_WAN),1)
COMPO_REV_ := -r 64

ifeq (${COMMANDER_TRUNK2}, 1)
COMPO_REV_ :=
endif

COMPO_REV := ${COMPO_REV_}
endif

ifeq (${PROJECT_NAME}, ${filter ${PROJECT_NAME}, BDW571AM_002 BDW571AM_L02})
COMPO_REV := -r 113
endif

else
COMPO_REV := -r $(VERSION)
endif

COMPO_DIR := $(COMPO_BASE)/open_src/OPPTP

ifeq ($(ENABLE_MULTI_WAN),1)
COMPO_URL_ := $(CHECKOUT_PREFIX)/Sahara/OPPTP/branches/BAW483AM_20100209

ifeq (${COMMANDER_TRUNK2}, 1)
COMPO_URL_ := $(CHECKOUT_PREFIX)/Sahara/OPPTP/branches/CIH573AM_U11_20110816
endif

COMPO_URL := ${COMPO_URL_}
endif

ifeq (${PROJECT_NAME}, ${filter ${PROJECT_NAME}, BDW571AM_002 BDW571AM_L02})
COMPO_URL := $(CHECKOUT_PREFIX)/Sahara/OPPTP/branches/CIH573AM_U11_20110816
endif

# Use old version scripts.
ifeq (${PROJECT_NAME}, ${filter ${PROJECT_NAME}, CDE560ZY_003 CDG561ZY_0T1})
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
ifeq ($(TC3262),1)
PREFIX += TC3262=$(TC3262) 
endif

PREFIX += USE_SCRIPTS_V1=$(USE_SCRIPTS_V1)

#
ifeq ($(BIG_ENDIAN),1)
ENDIAN := -b
else
ENDIAN :=
endif

build:
ifeq ($(ENABLE_MULTI_WAN),1)

ifeq (${PROJECT_NAME}, ${filter ${PROJECT_NAME},CIH573AM_U11 CID561NC BDW571AM_L02 CDD531AM_U02 CID688NC})
	@make -C $(COMPO_DIR) all ${PREFIX}
else
	make $(OPTIONS) -C $(COMPO_DIR) ${PREFIX}
ifdef FULL_COPY
	${PRIV_TOOLS}/bin/mkuo ${COMPO_DIR}/options.pptp ${COMPO_DIR}/options.pptp.uo $(ENDIAN)
	${PRIV_TOOLS}/bin/mkuo ${COMPO_DIR}/pptp.conf ${COMPO_DIR}/pptp.conf.uo $(ENDIAN)
endif
endif

else
	@make -C $(COMPO_DIR) all ${PREFIX}
endif

install: 
	@make -C $(COMPO_DIR) install ${PREFIX}

uninstall: 
	@make -C $(COMPO_DIR) uninstall ${PREFIX}

clean:
	make -C $(COMPO_DIR) clean ${PREFIX}

copying:

