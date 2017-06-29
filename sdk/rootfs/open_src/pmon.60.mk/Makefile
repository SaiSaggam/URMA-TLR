#
# Makefile to compile pmon
#
#

include ../../makefile.envars

COMPO_DIR := $(COMPO_BASE)/open_src/pmon

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

build: 
	@make -C $(COMPO_DIR) ${PREFIX}

install:
	@make -C $(COMPO_DIR) install ${PREFIX}


clean:
	@make -C $(COMPO_DIR) clean ${PREFIX}

copying:
