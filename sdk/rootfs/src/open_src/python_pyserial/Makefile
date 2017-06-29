# RDS/00008: OASIS Makefile Conventions

#for test
TOPDIR := $(shell cd ../../../../; pwd)
TARGET ?= mipsel-linux-uclibc
KERNEL_DIR ?= ${TOPDIR}/linux/src/
INSTALL_DIR = $(shell pwd)/_install
UCLIBCDIR    = $(subst buildroot-gcc342/bin/,buildroot-gcc342/mipsel-linux-uclibc/lib,$(TOOLPATH))
UCLIBC_INCDIR = $(subst buildroot-gcc342/bin/,buildroot-gcc342/mipsel-linux-uclibc/sysroot/usr/include,$(TOOLPATH))

PREFIX += PROJECT_NAME=${PROJECT_NAME} 
PREFIX += PROJECT_CUSTOMER=${PROJECT_CUSTOMER} 
PREFIX += PRIV_TOOLS=${PRIV_TOOLS} 
PREFIX += INSTALL_ROOT=$(INSTALL_ROOT)
PREFIX += ENDIAN=$(ENDIAN)

SRC_TOP_DIR	=  $(shell cd ../../; pwd)
LIBDIR		=  $(SRC_TOP_DIR)/$(TARGET)-lib
CFLAGS		+= -I$(SRC_TOP_DIR)/include -I$(UCLIBCDIR)
LDFLAGS		+= -L$(SRC_TOP_DIR)/$(TARGET)-lib -L$(UCLIBC_INCDIR)

include ../python/python_extensions.mk

all:
	@cd src; \
	python setup.py build; \
	python setup.py install --prefix=$(INSTALL_DIR)/usr --root=/

	# Remove .py library files
	@find $(INSTALL_DIR)/usr/lib/python2.7/site-packages -type f -iname "*.py" -delete

install:
	@mkdir -p $(INSTALL_ROOT)/usr/lib/python2.7
	@cp -a $(INSTALL_DIR)/usr/lib/python2.7/site-packages $(INSTALL_ROOT)/usr/lib/python2.7/

clean:
	@rm -Rf $(INSTALL_DIR)

distclean:
	@rm -Rf $(INSTALL_DIR)

