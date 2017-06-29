#
# Makefile to compile OSNMP
#

include ../../makefile.envars


COMPO_DIR := $(COMPO_BASE)/open_src/OSNMP

include ../../makefile.compo
PRIV_TOOLS := $(TOPDIR)/tools/priv
OPEN_TOOLS := $(TOPDIR)/tools/open

PREFIX += PROJECT_NAME=${PROJECT_NAME} 
PREFIX += PROJECT_CUSTOMER=${PROJECT_CUSTOMER} 
PREFIX += PRIV_TOOLS=${PRIV_TOOLS} 
PREFIX += INSTALL_ROOT=$(INSTALL_ROOT) 
PREFIX += ENDIAN=$(ENDIAN)
PREFIX += MAIL_ALERT=$(MAIL_ALERT)
PREFIX += KERNEL_DIR=${TOPDIR}/linux/src 

ifeq ($(BIG_ENDIAN),1)
ENDIAN := -b
else
ENDIAN :=
endif

#SNMP Enterprise Infomation
PREFIX += SNMP_ENTERPRISE_OID=${SNMP_ENTERPRISE_OID}
PREFIX += SNMP_ENTERPRISE_NOTIF_OID=${SNMP_ENTERPRISE_NOTIF_OID}
PREFIX += SNMP_ENTERPRISE_SYS_OID=${SNMP_ENTERPRISE_SYS_OID}

build:
	make -C $(COMPO_DIR) ${PREFIX}

include ../../makefile.install
install:
	@make -C $(COMPO_DIR) install ${PREFIX} 
clean:
	make -C $(COMPO_DIR) clean ${PREFIX}

copying:
