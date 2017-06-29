#
# Makefile to compile Ralink RT2860AP
#

include ../../makefile.envars

CHECKOUT_PREFIX ?= svn://svn.atc.amit.com.tw
COMPO_URL := $(CHECKOUT_PREFIX)/Sahara/OMtdUtils/trunk

COMPO_NAME := $(word 1,$(subst ., ,$(notdir $(CURDIR))))
ifeq ($(VERSION),0)
COMPO_REV :=
else
COMPO_REV := -r $(VERSION)
endif

COMPO_DIR := $(COMPO_BASE)/open_src/OMtdUtils

include $(TOPDIR)/project_config/project.cflags

include ../../makefile.compo
PRIV_TOOLS := $(TOPDIR)/tools/priv
OPEN_TOOLS := $(TOPDIR)/tools/open 

PREFIX += PROJECT_NAME=${PROJECT_NAME} 
PREFIX += PROJECT_CUSTOMER=${PROJECT_CUSTOMER} 
PREFIX += PRIV_TOOLS=${PRIV_TOOLS} 
PREFIX += INSTALL_ROOT=$(INSTALL_ROOT) 
PREFIX += ENDIAN=$(ENDIAN)    
PREFIX += HANDLE_ETHER_PORT=BR0
PREFIX += HANDLE_CLI_MODE=${HANDLE_CLI_MODE}
PREFIX += KERNEL_SDK=${KERNEL_SDK}
PREFIX += TCSUPPORT_CPU_RT63365=${TCSUPPORT_CPU_RT63365}
PREFIX += COMMANDER_TRUNK2=${COMMANDER_TRUNK2}
PREFIX += HANDLE_FLASH_MAP_16M=${HANDLE_FLASH_MAP_16M}
PREFIX += PROJECT_UI_NAME=${PROJECT_UI_NAME}
PREFIX += WITHOUT_XATTR=1

MK_TARGET :=
ifeq ($(HANDLE_54FW), 1)
MK_TARGET := -f Makefile.54fw
endif

build:
	@make -C $(COMPO_DIR) ${MK_TARGET} ${PREFIX}

 
install: 
	@make -C $(COMPO_DIR) ${MK_TARGET} install ${PREFIX}


clean:
	@make -C $(COMPO_DIR) ${MK_TARGET} clean ${PREFIX}
	
copying:

