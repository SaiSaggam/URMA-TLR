#
# Makefile to compile libubox
#
include ../../makefile.envars
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
PREFIX += MAIL_ALERT=$(MAIL_ALERT)
PREFIX += KERNEL_DIR=${TOPDIR}/linux/src

COMPO_DIR := $(COMPO_BASE)/open_src/uci/libubox/build

build:
	echo "xyz:In libubox"
	echo $(COMPO_DIR)
	-mkdir -p $(COMPO_DIR)
	@cd $(COMPO_DIR); cmake ..
	@make -C $(COMPO_DIR) ubox ${PREFIX} 

install:
	-mkdir -p $(INSTALL_ROOT)/usr/{include,bin,config}
	@cp $(COMPO_DIR)/libubox.so $(INSTALL_ROOT)/usr/lib

clean:
	-rm -rf $(COMPO_DIR)

distclean:
	@make -C $(COMPO_DIR) distclean

copying: 

