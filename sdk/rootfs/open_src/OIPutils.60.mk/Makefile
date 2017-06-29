#
# Makefile to compile popt lib
#

include ../../makefile.envars

CHECKOUT_PREFIX ?= svn://svn.atc.amit.com.tw
COMPO_URL := $(CHECKOUT_PREFIX)/Sahara/OIPutils/trunk
COMPO_REV :=
COMPO_DIR := $(COMPO_BASE)/open_src/OIPutils

include ../../makefile.compo

#
#ifeq ($(BIG_ENDIAN),1)
#ENDIAN := -b
#else
ENDIAN :=
#endif

PREFIX += PROJECT_NAME=${PROJECT_NAME} 
PREFIX += PROJECT_CUSTOMER=${PROJECT_CUSTOMER} 
PREFIX += INSTALL_ROOT=$(INSTALL_ROOT) 
#PREFIX += ENDIAN=$(ENDIAN)    

build:
	make -C $(COMPO_DIR) ${PREFIX}

install: 
	make -C $(COMPO_DIR) install ${PREFIX}
clean:
	-make -C $(COMPO_DIR) clean ${PREFIX}

