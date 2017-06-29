#
# Makefile to compile uci 
#
TOPDIR := $(shell cd ../../../; pwd)
#TARGET = mipsel-linux
#TARGET ?= mips-linux-uclibc
#TARGET ?= mips-linux-uclibc
KERNEL_DIR ?= ${TOPDIR}/linux/src/
COMPO_BASE := ${TOPDIR}/rootfs/src
include ../../makefile.envars
include ../../makefile.compo

ifndef TOOLPATH
    export TOOLPATH := $(shell cat $(TOPDIR)/TOOLPATH2)
	export PATH     := $(TOOLPATH):$(PATH)
endif

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
PREFIX += MAIL_ALERT=$(MAIL_ALERT)
PREFIX += KERNEL_DIR=${TOPDIR}/linux/src

COMPO_DIR := $(COMPO_BASE)/open_src/LibPAM

build:
	make -C $(COMPO_DIR) ${PREFIX}

install: 
	make -C $(COMPO_DIR) install ${PREFIX}
clean:
	-make -C $(COMPO_DIR) clean ${PREFIX}

uninstall: 
	-make -C $(COMPO_DIR) uninstall ${PREFIX}

copying:

distclean:
	-make -C $(COMPO_DIR) distclean ${PREFIX}