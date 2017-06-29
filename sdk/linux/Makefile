#
# Makefile to build linux
#

include makefile.envars

CHECKOUT_PREFIX ?= svn://svn.atc.amit.com.tw
#COMPO_URL := $(CHECKOUT_PREFIX)/Sahara/KernelRalink2621/trunk
#COMPO_REV := -r 265
COMPO_URL := $(CHECKOUT_PREFIX)/Sahara/KernelRalink2621/branches/79027
COMPO_REV := -r 323

ifeq ($(WIRELESS_DRIVER),2503)
COMPO_URL := $(CHECKOUT_PREFIX)/Sahara/KernelRalink2621/branches/79027
COMPO_REV := -r 373  # after 324 use new WiFi driver 2.5.0.3
endif

ifeq (${PROJECT_NAME}, CDW531AM)
ifeq (${PROJECT_CUSTOMER}, LED)
#COMPO_URL := $(CHECKOUT_PREFIX)/Sahara/KernelRalink2621/branches/CDG531LT-FL/60097
#COMPO_URL := $(CHECKOUT_PREFIX)/Sahara/KernelRalink2621/trunk
#COMPO_REV := -r 293		#287
endif
endif

ifeq ($(KERNEL_SDK),40007620)
COMPO_URL := $(CHECKOUT_PREFIX)/Sahara/Ralink_SDK4.0.0.0/trunk/linux-2.6.36.x
COMPO_REV := -r 420
endif

ifeq (${PROJECT_NAME}, CDW57CAM_003)
#COMPO_URL := $(CHECKOUT_PREFIX)/Sahara/Ralink_SDK4.0.0.0/branches/linux-2.6.36.x_CDW57CAM_003
COMPO_URL := $(CHECKOUT_PREFIX)/Sahara/Ralink_SDK4.0.0.0/trunk/linux-2.6.36.x
#COMPO_REV := -r 362
COMPO_REV := -r 409
endif

ifeq ($(KERNEL_SDK),4000)
COMPO_URL := $(CHECKOUT_PREFIX)/Sahara/Ralink_SDK4.0.0.0/trunk/linux-2.6.36MT.x
COMPO_REV := -r 72
endif

ifeq ($(KERNEL_SDK),4010)
COMPO_URL := $(CHECKOUT_PREFIX)/Sahara/Ralink_SDK4.0.0.0/branches/SDK4010/linux-2.6.36MT.x
COMPO_REV := -r 424 #419
endif

ifeq ($(KERNEL_SDK),3500)
COMPO_URL := $(CHECKOUT_PREFIX)/Sahara/Ralink_SDK_3.5.0.0/trunk/kernel 
COMPO_REV := -r 457
endif

ifeq ($(KERNEL_SDK),3400)
COMPO_URL := $(CHECKOUT_PREFIX)/Sahara/Ralink_SDK3.4.0.0/trunk/KernelRalink
COMPO_REV := -r 177
endif

ifeq ($(KERNEL_SDK),3300)
COMPO_URL := $(CHECKOUT_PREFIX)/Sahara/KernelRalink2621_SDK3300/trunk
COMPO_REV := -r 85
endif


#for 7621 SDK4100
ifeq ($(KERNEL_SDK),41007621)

COMPO_URL := $(CHECKOUT_PREFIX)/Sahara/Ralink_SDK4.1.0.0/trunk/
#COMPO_REV := -r 83
COMPO_REV := -r 93

ifeq (${PROJECT_NAME}, ${filter ${PROJECT_NAME}, SDE852AM_00001 BDG87BAM_0T2 SDE852AM_B0002 SDE87BAM_00001})
COMPO_URL := $(CHECKOUT_PREFIX)/Sahara/Ralink_SDK4.1.0.0/trunk/
COMPO_REV := -r 78
endif

ifeq (${PROJECT_NAME}, BDE850IO_001)
COMPO_URL := $(CHECKOUT_PREFIX)/Sahara/Ralink_SDK4.1.0.0/branches/BDE850IO_001
COMPO_REV := -r 73
endif

COMMON_OPTION := ARCH=mips CROSS_COMPILE="/opt/buildroot-gcc463/usr/bin"/mipsel-linux-
COMMON_OPTION += CONFIG_CROSS_COMPILER_PATH="/opt/buildroot-gcc463/usr/bin"
endif

#for MIPS BIG eadian
ifeq ($(TC3262),1)
COMPO_URL := $(CHECKOUT_PREFIX)/Sahara/KernelTrendChip2622/trunk
COMPO_REV := -r 97
export TARGET := mips-linux-uclibc
endif


COMPO_DIR := $(COMPO_BASE)

ifdef FULL_COPY
USE_SVN_UP := yes
include makefile.compo
else
all: build
endif

#

TARGET_PREFIX := $(TARGET)-

ifeq (${PROJECT_NAME}, CTG532NC)
COMMON_OPTION += ENABLE_CTG532NC=1
endif

ifeq (${PROJECT_STAG}, STAG)
COMMON_OPTION += ENABLE_STAG=1
endif

ifeq ("$(ENABLE_MULTI_WAN)","1")
COMMON_OPTION += ENABLE_MULTI_WAN=1
endif

start_time := $(shell date)

#

IMAGE_DIR := ../image
PRIV_TOOLS := ../tools/priv

#

PREFIX :=
PREFIX += PROJECT_NAME=${PROJECT_NAME}

.PHONY: prepared

build: prepared
	CORNER_DE_PRJROOT=${TOPDIR} PATH=${TOPDIR}/linux/bin:${PATH}; \
	make ${COMMON_OPTION} -C $(COMPO_DIR) dep || echo $$? ;\
	make ${COMMON_OPTION} -C $(COMPO_DIR) ${PREFIX} || exit $$? ; \
	mkdir -p ../image; \
	rm -irf ${TOPDIR}/image/linux.bin;

prepared: $(COMPO_DIR)/.tag.config $(COMPO_DIR)/.tag.oldconfig

$(COMPO_DIR)/.tag.config:
	cp Config $(COMPO_DIR)/.config
	touch $@

$(COMPO_DIR)/.tag.oldconfig: 
	make ${COMMON_OPTION} -C $(COMPO_DIR) oldconfig
	touch $@

.PHONY: ins_modules ins_image

install: ins_modules comp_image ins_image
	@echo "*********************************************************************"
	@echo "* Done! $(start_time) ~ `date` *"
	@echo "*********************************************************************"

ins_modules:
	@echo "Install kernel modules"
	@rm -fr $(INSTALL_ROOT)
	@make -s ${COMMON_OPTION} INSTALL_MOD_PATH=$(INSTALL_ROOT) -C $(COMPO_DIR) modules_install
	@rm -f $(INSTALL_ROOT)/lib/modules/*/build $(INSTALL_ROOT)/lib/modules/*/source
	@ln -sf `ls $(INSTALL_ROOT)/lib/modules`/kernel $(INSTALL_ROOT)/lib/modules/kernel
	@find $(INSTALL_ROOT)/lib -name *.ko -print | xargs $(TARGET_PREFIX)strip -g -S -d

comp_image:
	@echo "**********************************************************************"
	@echo "Do Compress"
	@echo "**********************************************************************"
	@$(TARGET_PREFIX)objcopy -O binary -R .note -R comment -S $(COMPO_DIR)/vmlinux ./zImage
	@if [ -e ../tools/open/bin/lzma -a -x ../tools/open/bin/lzma ] ; then \
		../tools/open/bin/lzma -9 -f -S .lzma zImage ; \
	else \
		lzma -9 -f -S .lzma zImage ; \
	fi

ins_image:
	@mkdir -p ../image
	@cp ./zImage.lzma ../image

clean:
	@if test -f $(COMPO_DIR)/Makefile; then \
		make -C $(COMPO_DIR) clean; \
		rm -f $(COMPO_DIR)/.tag.config $(COMPO_DIR)/.tag.oldconfig; \
	fi


distclean:
	rm -fr $(INSTALL_ROOT)
	rm -fr $(COMPO_DIR)

menuconfig: prepared
	make ${COMMON_OPTION} -C $(COMPO_DIR) menuconfig


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




