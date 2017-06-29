#
# Makefile to build busybox
#

include makefile.envars

CHECKOUT_PREFIX ?= svn://svn.atc.amit.com.tw
COMPO_URL := $(CHECKOUT_PREFIX)/Sahara/CornerBusybox/trunk
COMPO_REV := -r 82
COMPO_DIR := $(COMPO_BASE)

ifeq (${PROJECT_NAME}, BDE702DL_001)
COMPO_REV := -r 65
endif

ifeq (${PROJECT_NAME}, CDM532RP)
COMPO_URL := $(CHECKOUT_PREFIX)/Sahara/CornerBusybox/branches/CDM532RP/50150
COMPO_REV := -r 32
endif

#for MIPS BIG eadian
ifeq ($(TC3262),1)
export TARGET = mips-linux-uclibc
endif

PREFIX += PROJECT_NAME=${PROJECT_NAME} 
PREFIX += PROJECT_CUSTOMER=${PROJECT_CUSTOMER} 
PREFIX += PRIV_TOOLS=${PRIV_TOOLS} 
PREFIX += INSTALL_ROOT=$(INSTALL_ROOT) 
PREFIX += ENDIAN=$(ENDIAN) 
PREFIX += CROSS_COMPILE=$(TARGET)- 
PREFIX += ARCH=mips  
PREFIX += TOOLCHAIN432=${TOOLCHAIN432}
PREFIX += MIPS_32R2=$(MIPS_32R2)

ifeq ($(SUPPORT_QUALCOMM),1)
PREFIX += SUPPORT_QUALCOMM=1
endif

ifeq (${PROJECT_SHOW_HOSTLOGIN}, NOSHOW)
PREFIX += NO_HOSTNAME_LOGIN=1
endif

ifeq (${PROJECT_TIMESTAMP}, TIMESTAMP)
PREFIX += ENABLE_TIMESTAMP=1
endif

ifeq (${PROJECT_NAME}, BDE702DL_001)
PREFIX += ENABLE_PING_TIMEOUT_1SEC=1
endif
ifdef FULL_COPY
include makefile.compo
else
all: build install
endif

#

TARGET_PREFIX := $(TARGET)-

start_time := $(shell date)

#
ifeq ($(RD_LEVEL),1)
CONF_FILE = Config.RD_LEVEL
else
CONF_FILE = Config.mini
endif

.PHONY: prepared

build: prepared
	$(PREFIX) make CROSS_COMPILE=$(TARGET_PREFIX) ARCH=mips V=1 -C $(COMPO_DIR)

#prepared: $(COMPO_DIR)/.tag.patched $(COMPO_DIR)/.tag.config $(COMPO_DIR)/.tag.oldconfig
prepared: $(COMPO_DIR)/.tag.config $(COMPO_DIR)/.tag.oldconfig

#$(COMPO_DIR)/.tag.patched:
#	patch -d $(COMPO_DIR) -p1 -i ../patch_for_rootfs
#	touch $@

$(COMPO_DIR)/.tag.config:
	cp $(CONF_FILE) $(COMPO_DIR)/.config
	touch $@

$(COMPO_DIR)/.tag.oldconfig: 
	$(PREFIX) make ARCH=mips CROSS_COMPILE=$(TARGET_PREFIX) -C $(COMPO_DIR) oldconfig
	touch $@

install:
	@rm -fr $(INSTALL_ROOT)
	@mkdir -p $(INSTALL_ROOT)
	$(PREFIX) make CROSS_COMPILE=$(TARGET_PREFIX) PREFIX=$(INSTALL_ROOT) ARCH=mips V=1 -C $(COMPO_DIR) install
	@echo "*************"
	@echo "**  Done!  **"
	@echo "*************"

clean:
	@if test -f $(COMPO_DIR)/Makefile; then \
		make ARCH=mips CROSS_COMPILE=$(TARGET_PREFIX) -C $(COMPO_DIR) clean; \
		rm -f $(COMPO_DIR)/.tag.config $(COMPO_DIR)/.tag.oldconfig; \
	fi

#

distclean:
	rm -fr $(INSTALL_ROOT)
	#rm -fr $(COMPO_DIR)

menuconfig: prepared
	$(PREFIX) make CROSS_COMPILE=$(TARGET_PREFIX) -C $(COMPO_DIR) menuconfig


u = $(subst /, ,$(COMPO_URL))
t = $(word $(words $(u)), $(u))

FREV_TAG := $(shell pwd)/.tag.frev

MY_REV = $(strip $(word 2, $(COMPO_REV)))
F_REV = $(strip $(shell cat $(FREV_TAG)))

list: $(FREV_TAG)-clean $(FREV_TAG)
	@if [ $(F_REV) == $(MY_REV) ]; then \
        if [ $(t) == 'trunk' ]; then \
            echo -e "  " $(COMPO_URL) $(COMPO_REV) >> $(URL_FILE_LIST); \
        else \
            echo -e " B" $(COMPO_URL) $(COMPO_REV) >> $(URL_FILE_LIST); \
        fi; \
    elif [ ! $(F_REV) ]; then \
        if [ $(t) == 'trunk' ]; then \
            echo -e "X " $(COMPO_URL) $(COMPO_REV) >> $(URL_FILE_LIST); \
        else \
            echo -e "XB" $(COMPO_URL) $(COMPO_REV) >> $(URL_FILE_LIST); \
        fi; \
    else \
        if [ $(t) == 'trunk' ]; then \
            echo -e "U " $(COMPO_URL) $(COMPO_REV) "\050" $(F_REV) "\051" >> $(URL_FILE_LIST); \
        else \
            echo -e "UB" $(COMPO_URL) $(COMPO_REV) "\050" $(F_REV) "\051" >> $(URL_FILE_LIST); \
        fi; \
    fi

$(FREV_TAG):
	@echo "$(word 2,$(shell svn info $(COMPO_URL) | grep Revision))" > $@

$(FREV_TAG)-clean:
	@rm -f $(FREV_TAG)


