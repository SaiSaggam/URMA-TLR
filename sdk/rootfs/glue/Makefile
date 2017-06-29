#This is for RELEASE IN SDK COPY
#
# Makefile for glue logic
#

export TOPDIR ?= $(shell cd ../..; pwd)
include ../makefile.envars

#for MIPS BIG eadian
ifeq ($(TC3262),1)
UCLIBCDIR = $(subst trendchip/mips-linux-uclibc/bin,trendchip/mips-linux-uclibc/lib,$(TOOLPATH))
else
UCLIBCDIR = $(subst buildroot-gcc342/bin/,buildroot-gcc342/mipsel-linux-uclibc/lib,$(TOOLPATH))
endif



STRIP=$(TARGET)-strip
#STRIP=$(CROSS_COMPILE)strip
#

.PHONY: all main

all: main

ifneq ($(wildcard makefile.corner),)
include makefile.corner
endif

#

IMAGE_DIR  	:= $(TOPDIR)/image
FINAL_ROOT 	:= $(TOPDIR)/image/root
ifeq (${PROJECT_NAME}, ${filter ${PROJECT_NAME},CDW57CAM_003 BDG761AM_0P1 BDG761DL_0T1 BDG761AM_0T1 BDE761AM_001 \
BDE761AM_002 BDE77AAM_001 BDG77AAM_0P1 BDG771AM_LP1 IDG761AM_0PGP1 IDG760AM_0P001 IDG761AM_0P001 CIG771DL_UT1 \
BDP77AAM_001 \
BDP77BAM_001 CDW561DL_003 IDE701AM_00001 IDE701AM_GP001 IDE701AM_DA001 BIH77AAM_LT3 CTG760DL_0T1 CTG771DL_UT1 \
CMW56CAM_U03 CDW561AM_U03 APC772AM_P01 APO771AM_P01 APW771AM_P01 CTG561DL_0T3 BDE702AM_001 BID77AAM_003 \
CDH561AM_0P3 BDG762DL_0P1 CTG561DL_UP3 BDE87BAM_001 SDE852AM_00001 BTG771AM_LT1 BTG771AM_0T1 CDM561DL_003 \
APC77BAM_P01 APW77BAM_P01 BID77BAM_003 CDD77BAM_U03 CDG561AM_0T3 CTG561AM_0T3 CIG771AM_0T1 BIG771AM_0T1 \
CID57AAM_U03 CTG560DL_0T3 CTG760DL_0T2 APP87BAM_P01 BDG762DL_OP2 CTG561DL_0P1 IOG87BDG_0T001})
UI_DIR	   	:= $(TOPDIR)/rootfs/src/amit_priv/CWebUI/www
UI_SRC_DIR  := $(TOPDIR)/rootfs/src/amit_priv/CWebUI

ifeq (${PROJECT_NAME}, ${filter ${PROJECT_NAME},BDP77AAM_001})
ifeq (${PROJECT_CUSTOMER}, ${filter ${PROJECT_CUSTOMER},DDC_WAP_6150})
UI_DIR	   	:= $(TOPDIR)/rootfs/src/amit_priv/_WebUI/www
UI_SRC_DIR  := $(TOPDIR)/rootfs/src/amit_priv/_WebUI
endif
endif

ifeq (${PROJECT_NAME}, ${filter ${PROJECT_NAME},BDP77BAM_001})
ifeq (${PROJECT_CUSTOMER}, ${filter ${PROJECT_CUSTOMER},DDC_WAP_6150})
UI_DIR	   	:= $(TOPDIR)/rootfs/src/amit_priv/_WebUI/www
UI_SRC_DIR  := $(TOPDIR)/rootfs/src/amit_priv/_WebUI
endif
endif

ifeq (${PROJECT_NAME}, ${filter ${PROJECT_NAME},APC772AM_P01})
ifeq (${PROJECT_CUSTOMER}, ${filter ${PROJECT_CUSTOMER},DDC_WAP_6150})
UI_DIR	   	:= $(TOPDIR)/rootfs/src/amit_priv/_WebUI/www
UI_SRC_DIR  := $(TOPDIR)/rootfs/src/amit_priv/_WebUI
endif

ifeq (${PROJECT_CUSTOMER}, ${filter ${PROJECT_CUSTOMER},SML_DG_WM2005SI})
UI_DIR	   	:= $(TOPDIR)/rootfs/src/amit_priv/_WebUI/www
UI_SRC_DIR  := $(TOPDIR)/rootfs/src/amit_priv/_WebUI
endif
endif

ifeq (${PROJECT_NAME}, ${filter ${PROJECT_NAME},APW771AM_P01})
ifeq (${PROJECT_CUSTOMER}, ${filter ${PROJECT_CUSTOMER},DDC_WAP_6150})
UI_DIR	   	:= $(TOPDIR)/rootfs/src/amit_priv/_WebUI/www
UI_SRC_DIR  := $(TOPDIR)/rootfs/src/amit_priv/_WebUI
endif
endif

else
UI_DIR	   	:= $(TOPDIR)/rootfs/src/amit_priv/_WebUI/www
UI_SRC_DIR  := $(TOPDIR)/rootfs/src/amit_priv/_WebUI
endif

ifeq (${PROJECT_NAME}, ${filter ${PROJECT_NAME},BDE771AM_001})
UI_DIR	   	:= $(TOPDIR)/rootfs/src/amit_priv/CWebUI/www
UI_SRC_DIR  := $(TOPDIR)/rootfs/src/amit_priv/CWebUI
endif

ifeq (${PROJECT_NAME}, ${filter ${PROJECT_NAME},BDW762AM_001})
UI_DIR          := $(TOPDIR)/rootfs/src/amit_priv/CWebUI/www
UI_SRC_DIR  := $(TOPDIR)/rootfs/src/amit_priv/CWebUI
endif

ifeq (${PROJECT_NAME}, ${filter ${PROJECT_NAME},BDE850IO_001})
UI_DIR	   	:= $(TOPDIR)/rootfs/src/amit_priv/_WebUI/www
UI_SRC_DIR  := $(TOPDIR)/rootfs/src/amit_priv/_WebUI
endif

ifeq (${PROJECT_NAME}, ${filter ${PROJECT_NAME},ODG761AM_2T1})
UI_DIR          := $(TOPDIR)/rootfs/src/amit_priv/CWebUI/www
UI_SRC_DIR  := $(TOPDIR)/rootfs/src/amit_priv/CWebUI
endif

ifeq (${PROJECT_NAME}, ${filter ${PROJECT_NAME},IDG700AM_0T001 IDG700AM_BTGT1})
UI_DIR          := $(TOPDIR)/rootfs/src/amit_priv/CWebUI/www
UI_SRC_DIR  := $(TOPDIR)/rootfs/src/amit_priv/CWebUI
endif

ifeq (${PROJECT_NAME}, ${filter ${PROJECT_NAME},IOG761AM_0P001 IOG700AM_0PCR1 IOG700AM_0TCR1 IOG761AM_0TDA1 IOG761AM_0T001})
UI_DIR          := $(TOPDIR)/rootfs/src/amit_priv/CWebUI/www
UI_SRC_DIR  := $(TOPDIR)/rootfs/src/amit_priv/CWebUI
endif

ifeq (${PROJECT_NAME}, ${filter ${PROJECT_NAME},IOG761MU_0T001})
UI_DIR	   	:= $(TOPDIR)/rootfs/src/amit_priv/_WebUI/www
UI_SRC_DIR  := $(TOPDIR)/rootfs/src/amit_priv/_WebUI
endif

ifeq (${PROJECT_CUSTOMER}, ${filter ${PROJECT_CUSTOMER},SML_DGLB1054UV})
UI_DIR	   	:= $(TOPDIR)/rootfs/src/amit_priv/_WebUI/www
UI_SRC_DIR  := $(TOPDIR)/rootfs/src/amit_priv/_WebUI
endif

PRIV_TOOLS 	:= $(TOPDIR)/tools/priv
OPEN_TOOLS 	:= $(TOPDIR)/tools/open

#for MIPS BIG eadian
ifeq ($(TC3262),1)
#MKSQUASHFS := mksquashfs_lzma-3.2
MKSQUASHFS := $(TOPDIR)/tools/mksquashfs-lzma
else
ifeq ($(KERNEL_SDK), 41007621)
MKSQUASHFS5 := mksquashfs_4.2
else
MKSQUASHFS 	:= mksquashfs_lzma-3.2
MKSQUASHFS4 := mksquashfs_4.0
endif
endif
SQUASHFS   	:= $(TOPDIR)/image/squashfs.o
MYDLINK_DIR	:= $(TOPDIR)/rootfs/src/open_src/OMyDlink/MyDlink
CUST_DEFAULT_DIR	:= $(TOPDIR)/rootfs/glue/Default

ifeq (${PROJECT_NAME}, ${filter ${PROJECT_NAME},BDG561AM CIW563AM})
WIFI_BIN_PATH := $(FINAL_ROOT)/usr/mv2ram/etc/Wireless/RT2860
else
WIFI_BIN_PATH := $(FINAL_ROOT)/usr/mv2ram/etc/Wireless/RT2860AP
endif

.PHONY: show_msg cp_glue mk_root cp_wifi_bin strip_proj remove_files mk_image_squashfs

ifdef FULL_COPY
main: show_msg cp_glue mk_root cp_wifi_bin strip_proj remove_files mk_image_squashfs mk_mydlink_squashfs show_image image_add_time
else
main: show_msg cp_glue mk_root cp_wifi_bin strip_proj remove_files mk_image_squashfs mk_mydlink_squashfs show_image_gpl
endif

show_msg:
	@echo "*** Making glue logic ***"

cp_glue: mk_pre4 inst_glue #INSTALL.copy_glue

inst_glue:
ifdef FULL_COPY
	@cp -a etc.inittab				$(INSTALL_ROOT)/usr/etc/inittab
	@cp -a etc.rcS					$(INSTALL_ROOT)/usr/etc/rcS
	@cp -a etc.install_mysql_radius.sh              $(INSTALL_ROOT)/usr/etc/install_mysql_radius.sh
ifeq (${PROJECT_NAME}, ${filter ${PROJECT_NAME},CTG760DL_0T1 BDG762DL_0P1 CTG760DL_0T2 BDG762DL_OP2})
	@cp -a etc.fstab1				$(INSTALL_ROOT)/usr/etc/fstab
else
	@cp -a etc.fstab				$(INSTALL_ROOT)/usr/etc/fstab
endif
	@cp -a etc.profile				$(INSTALL_ROOT)/usr/etc/profile
ifeq ($(TC3262),1)	
	@cp -a led.conf					$(INSTALL_ROOT)/usr/mv2ram/etc/led.conf
endif	
	@cp -a etc.services				$(INSTALL_ROOT)/usr/mv2ram/etc/services
	@cp -a etc.passwd				$(INSTALL_ROOT)/usr/mv2ram/etc/passwd
	@cp -a etc.group	 	   		$(INSTALL_ROOT)/usr/mv2ram/etc/group
	@cp -a etc.issue.net			$(INSTALL_ROOT)/usr/mv2ram/etc/issue.net
	@cp -a usr.bin.telnetd-action	$(INSTALL_ROOT)/usr/bin/telnetd-action
	@chmod 777						$(INSTALL_ROOT)/usr/etc/rcS
	@mkdir -p						$(INSTALL_ROOT)/root
	@MASCOT=`svn info | grep "Repository Root"`; MASCOT=$${MASCOT##*/}; \
			echo "$$MASCOT" > $(INSTALL_ROOT)/usr/mv2ram/etc/hostname
	@cp -a etc.install_mysql_radius.sh          $(INSTALL_ROOT)/usr/etc/install_mysql_radius.sh
ifeq ($(HANDLE_CSMANX), 1)
	@mkdir -p						$(INSTALL_ROOT)/usr/mv2ram/tmp/csmanx
	@cp pre4.dat					$(INSTALL_ROOT)/usr/mv2ram/tmp/csmanx
	@ln -fs /usr/www/pre7.dat		$(INSTALL_ROOT)/usr/mv2ram/tmp/csmanx/pre7.dat
	@ln -fs /usr/www/reset.dat		$(INSTALL_ROOT)/usr/mv2ram/tmp/csmanx/reset.dat
endif
	@mkdir -p						$(INSTALL_ROOT)/usr/mv2ram/tmp/csman
	@mv pre4.dat					$(INSTALL_ROOT)/usr/mv2ram/tmp/csman
	@ln -fs /usr/www/pre7.dat		$(INSTALL_ROOT)/usr/mv2ram/tmp/csman/pre7.dat
	@ln -fs /usr/www/reset.dat		$(INSTALL_ROOT)/usr/mv2ram/tmp/csman/reset.dat
ifeq ($(HANDLE_CUST_DEFAULT),1)
	@ln -fs /tmp/custom/reset.dat	$(INSTALL_ROOT)/usr/mv2ram/tmp/csman/custom_reset.dat
endif
	@if [ -e $(TOPDIR)/image/svn.list ]; then cp -a $(TOPDIR)/image/svn.list $(INSTALL_ROOT)/usr/etc/svn.list ; fi
ifeq ($(ENABLE_MULTI_WAN),1)
	@cp -a etc.rt_tables			$(INSTALL_ROOT)/usr/mv2ram/etc/iproute2/rt_tables
endif
ifeq ($(FLASHOFDM_FOR_FINLAND),y)
	@echo "Copy FlashOFDM image for Finland "
	@mkdir -p						$(INSTALL_ROOT)/lib/modules
	@cp -a finland_ft3000.img		$(INSTALL_ROOT)/lib/modules/ft3000.img
endif
ifeq ($(FLASHOFDM_FOR_GERMANY),y)
	@echo "Copy FlashOFDM image for Germany "
	@mkdir -p						$(INSTALL_ROOT)/lib/modules
	@cp -a germany_ft3000.img		$(INSTALL_ROOT)/lib/modules/ft3000.img
endif
ifeq (${PROJECT_NAME}, CDG531LT_FL)
	@echo "Copy FlashOFDM image for LeadTek "
	@mkdir -p                       $(INSTALL_ROOT)/lib/modules
	@cp -a ft3000.img450h           $(INSTALL_ROOT)/lib/modules/ft3000.img
endif
	
endif
        
mk_pre4:
ifdef FULL_COPY
	@export CORNER_DE_PRJROOT=$(TOPDIR); FW_REV=`$(TOPDIR)/rootfs/glue/getfwrev`; \
	FW_VER=${shell cat $(TOPDIR)/VERSION}; \
	MASCOT=`svn info | grep "Repository Root"`; MASCOT=$${MASCOT##*/}; \
	echo ""; \
	echo "-------------------------------------------------"; \
	echo "$$MASCOT,$$FW_VER,$$FW_REV"; \
	echo "-------------------------------------------------"; \
	echo ""
ifeq ($(TC3262),1)
	@export CORNER_DE_PRJROOT=$(TOPDIR); FW_REV=`$(TOPDIR)/rootfs/glue/getfwrev` \
	FW_VER=${shell cat $(TOPDIR)/VERSION} \
	$(PRIV_TOOLS)/bin/mkcsxf -d $(TOPDIR)/rootfs/src/var/csidbase -o pre4.dat -cb pre4.txt -X c > /dev/null
else
	@export CORNER_DE_PRJROOT=$(TOPDIR); FW_REV=`$(TOPDIR)/rootfs/glue/getfwrev` \
	FW_VER=${shell cat $(TOPDIR)/VERSION} \
	$(PRIV_TOOLS)/bin/mkcsxf -d $(TOPDIR)/rootfs/src/var/csidbase -o pre4.dat -zc pre4.txt -X c > /dev/null
endif
	
endif

mk_root:
	@rm -fr $(FINAL_ROOT)
	@mkdir -p $(FINAL_ROOT)
	@if test -d $(TOPDIR)/linux/root; then \
		cp -ar $(TOPDIR)/linux/root/*   $(FINAL_ROOT); \
	fi
	@cp -ar $(TOPDIR)/busybox/root/* $(FINAL_ROOT)
	@cp -ar $(TOPDIR)/rootfs/root/*  $(FINAL_ROOT)
	@if test -d $(FINAL_ROOT)/modules; then \
		tar c -C $(FINAL_ROOT)/modules . | tar x -k -C $(FINAL_ROOT)/lib/modules; \
		rm -fr $(FINAL_ROOT)/modules; \
	fi
#ifeq (${PROJECT_NAME}, ${filter ${PROJECT_NAME},BDE761AM_001 BDE702DL_001 BDE77ADL_001 CIG771AM_UT1 CDW761ZY_U01 CDW78AZY_U01 IDG561AM_16M BDG761AM_0P1 BDP77AAM_001 BDP77BAM_001 IDE701AM_GP001 BDE761AM_002 CTG771DL_UT1 CTG760DL_0T1 BDG762DL_0P1 BDG762DL_OP2})
ifdef FULL_COPY
	@chmod +x check_image 
	@chmod +x etc.inittab
	@chmod +x etc.rcS
	@chmod +x grablibs
endif
#endif	
	@./grablibs $(FINAL_ROOT) $(UCLIBCDIR) $(PROJECT_NAME)

cp_wifi_bin:
	@echo "*** Copy Wifi Bin ***"
ifeq (${CP_MT7621_BIN}, 1)
ifeq (${MT76X2_MULTIPLE_CARD_SUPPORT}, 1)
	@cp -f $(TOPDIR)/rootfs/glue/MT7602E_AP_2T2R.bin $(WIFI_BIN_PATH)
	@cp -f $(TOPDIR)/rootfs/glue/MT7612E_AP_2T2R.bin $(WIFI_BIN_PATH)
else
ifeq (${AP_MT7602E}, 1)
	@cp -f $(TOPDIR)/rootfs/glue/MT7602E_AP_2T2R.bin $(WIFI_BIN_PATH)
endif
ifeq (${AP_MT7612E}, 1)
	@cp -f $(TOPDIR)/rootfs/glue/MT7612E_AP_2T2R.bin $(WIFI_BIN_PATH)
endif
endif
endif

ifeq (${AP_MT7612E}, 1)
	@cp -f $(TOPDIR)/rootfs/glue/MT7612E_AP_2T2R.bin $(WIFI_BIN_PATH)
endif

ifeq (${AP_MT7603E}, 1)
	@cp -f $(TOPDIR)/rootfs/glue/MT7603E_AP_2T2R.bin $(WIFI_BIN_PATH)
endif

ifeq (${CP_MT7620_BIN}, 1)
	@cp -f $(TOPDIR)/rootfs/glue/MT7620_AP_2T2R.bin $(WIFI_BIN_PATH)
endif

ifeq (${CP_MT7610_BIN}, 1)
	@cp -f $(TOPDIR)/rootfs/glue/MT7610E-V10-20130128-FEM-1ANT.bin $(WIFI_BIN_PATH)
endif

ifeq (${ATS_MT7610e}, 1)
	@cp -f $(TOPDIR)/rootfs/glue/MT7610E-V10-20130128-FEM-1ANT.bin $(WIFI_BIN_PATH)
endif

ifeq (${WIRELESS_FUNCTION}, 1)
ifeq (${PROJECT_NAME}, ${filter ${PROJECT_NAME},BDP77AAM_001 BDP77BAM_001 APC772AM_P01 APO771AM_P01 APW771AM_P01 APC77BAM_P01 APW77BAM_P01 ODG761AM_2T1})
	@cp -f $(TOPDIR)/rootfs/glue/RT5592_PCIe_2T2R_V1_7.bin $(WIFI_BIN_PATH)
	@cp -f $(TOPDIR)/rootfs/glue/RT5592EP_PCIe_2T2R_2.4G_V1_4.bin $(WIFI_BIN_PATH)
else
	@cp -f $(TOPDIR)/rootfs/glue/RT5392_PCIe_2T2R_ALC_V1_3.bin $(WIFI_BIN_PATH)
	@cp -f $(TOPDIR)/rootfs/glue/RT3092_PCIe_LNA_2T2R_ALC_V1_2.bin $(WIFI_BIN_PATH)
endif
	@cp -f $(TOPDIR)/rootfs/glue/RT5592EP_PCIe_2T2R_5G_V1_5.bin $(WIFI_BIN_PATH)

endif

ifeq ($(RT_CHIP), 3593)
	@cp -f $(TOPDIR)/rootfs/glue/RT3593_PCIe_3T3R_V1_3.bin $(WIFI_BIN_PATH)
endif

ifeq ($(RT_CHIP), 5592)
	@cp -f $(TOPDIR)/rootfs/glue/RT5592EP_PCIe_2T2R_5G_V1_5.bin $(WIFI_BIN_PATH)
endif

ifeq (${CP_RT5350_BIN}, 1)
	@cp -f $(TOPDIR)/rootfs/glue/RT5350_AP_1T1R_V1_0.bin $(WIFI_BIN_PATH)
endif
ifeq (${CP_RT3352_BIN}, 1)
	@cp -f $(TOPDIR)/rootfs/glue/RT3352_AP_2T2R_V11.bin $(WIFI_BIN_PATH)
endif
ifeq (${CP_RT3883_BIN}, 1)
ifeq (${PROJECT_NAME}, CDE665NC_001)
	@cp -f $(TOPDIR)/rootfs/glue/RT3662_AP_2T3R_V0_0.bin $(WIFI_BIN_PATH)
else
	@cp -f $(TOPDIR)/rootfs/glue/RT3883_AP_3T3R_V0_1.bin $(WIFI_BIN_PATH)
endif
	@cp -f $(TOPDIR)/rootfs/glue/RT5392_PCIe_2T2R_ALC_V1_3.bin $(WIFI_BIN_PATH)
endif

ifeq (${CP_RT3052_BIN}, 1)
	@cp -f $(TOPDIR)/rootfs/glue/RT3050_AP_1T1R_V1_0.bin  $(WIFI_BIN_PATH)
endif
ifeq (${CP_RT3050_BIN}, 1)
	@cp -f $(TOPDIR)/rootfs/glue/RT3050_AP_1T1R_V1_0.bin  $(WIFI_BIN_PATH)
endif

remove_files:
	@echo "*** Removing files specified in remove_list and remove_list.user ***"
	# Lines, started with:
	# - 'f': removes files recursively
	# - 'd': removes directories recursively
	# - 'a': removes files + directories recursively
	# - 'p;: removes the specified path (file or directory)
	@-for list in "remove_list" "remove_list.user"; do \
		[ -f "$$list" ] && (cat "$$list"; echo) | while read modifier file; do \
			case "$$modifier" in \
				f) find "$(FINAL_ROOT)" -type f -name "$$file" -exec rm -f {} \; ;; \
				d) find "$(FINAL_ROOT)" -type d -name "$$file" -prune -exec rm -rf {} \; ;; \
				a) find "$(FINAL_ROOT)" -name "$$file" -prune -exec rm -rf {} \; ;; \
				p) rm -rf "$(FINAL_ROOT)/$$file" ;; \
				*) ;; \
			esac;	\
		done; \
	done
	@echo "Done"

strip_proj:
ifeq (${STRIP_FINAL_PROJ}, y)
	@echo "*** STRIP ALL Binary and Library ***"
	@-for t in `find ${FINAL_ROOT} -type f` ; do \
		if dd if=$$t bs=1 skip=1 count=3 2>/dev/null | grep -qs 'ELF'; then \
			${STRIP} --strip-unneeded --remove-section=.comment --remove-section=.note $$t; \
		fi; \
	done
	@echo ""
endif

mk_image_squashfs:
	@rm -f $(SQUASHFS)
	@echo "*** Compressing ROOTFS ***"
ifeq ($(KERNEL_SDK), 262263368)
	@$(MKSQUASHFS) $(FINAL_ROOT) $(SQUASHFS) -be -all-root > /dev/null
else	
ifeq ($(KERNEL_SDK), 4000)
	@$(MKSQUASHFS4) $(FINAL_ROOT) $(SQUASHFS) -all-root > /dev/null
else
ifeq ($(KERNEL_SDK), 4010)
	@$(MKSQUASHFS4) $(FINAL_ROOT) $(SQUASHFS) -all-root > /dev/null
else
ifeq ($(KERNEL_SDK), 40007620)
	@$(MKSQUASHFS4) $(FINAL_ROOT) $(SQUASHFS) -all-root > /dev/null
else
ifeq ($(KERNEL_SDK), 41007621)
	#$(MKSQUASHFS5) $(FINAL_ROOT) $(SQUASHFS) -all-root > /dev/null
	$(MKSQUASHFS5) $(FINAL_ROOT) $(SQUASHFS) -all-root 
else
	@$(MKSQUASHFS) $(FINAL_ROOT) $(SQUASHFS) -le -all-root > /dev/null
endif
endif
endif
endif
endif
ifdef FULL_COPY
ifeq ($(PROJECT_VOICE), DISABLE_VOICE)
	@$(PRIV_TOOLS)/bin/binman --s d_cdg.bms --d $(IMAGE_DIR)
	#@ls -l $(IMAGE_DIR)/*.bin
else
	@$(PRIV_TOOLS)/bin/binman --s d.bms --d $(IMAGE_DIR)
	#@ls -l $(IMAGE_DIR)/*.bin
endif
else
	@$(PRIV_TOOLS)/bin/binboy-static > /dev/null 2>&1 ;\
		if [ "$$?" = "0" ]; then \
			cd ../../image; \
			$(PRIV_TOOLS)/bin/binboy-static @linux ; \
			$(PRIV_TOOLS)/bin/binboy-static @rootfs ; \
			$(PRIV_TOOLS)/bin/binboy-static @user ; \
			$(PRIV_TOOLS)/bin/binboy-static @all ; \
			cd - ;\
		    exit 0; \
		else \
			$(PRIV_TOOLS)/bin/binboy > /dev/null 2>&1 ;\
    		if [ "$$?" = "0" ]; then \
				cd ../../image; \
				$(PRIV_TOOLS)/bin/binboy @linux ; \
				$(PRIV_TOOLS)/bin/binboy @rootfs ; \
				$(PRIV_TOOLS)/bin/binboy @user ; \
				$(PRIV_TOOLS)/bin/binboy @all ; \
				cd - ;\
		        exit 0; \
		    else \
		        echo "Error !! Can't apply header on firmware, please check your compile environment !"; \
		        echo ""; \
		        exit 1; \
		    fi; \
		fi
endif

show_image:
ifeq ($(MYDLINK),y)
	@$(PRIV_TOOLS)/bin/check_image BOOT.bin kernel.bin rootfs.bin ui.bin mydlink.bin
else
	@$(PRIV_TOOLS)/bin/check_image BOOT.bin kernel.bin rootfs.bin ui.bin
endif

show_image_gpl:
	@echo ""
	@echo "======= Show image size ======"
	@echo "kernel.bin  : `ls -l ../../image/kernel.bin   |awk '{print $$5}'` (`du -h ../../image/kernel.bin  | awk '{print $$1}'`)"
	@echo "rootfs.bin  : `ls -l ../../image/rootfs.bin   |awk '{print $$5}'` (`du -h ../../image/rootfs.bin  | awk '{print $$1}'`)"
ifeq ($(MYDLINK),y)
	@echo "mydlink.bin : `ls -l ../../image/mydlink.bin  |awk '{print $$5}'` (`du -h ../../image/mydlink.bin  | awk '{print $$1}'`)"
endif
	@echo "firmware.bin: `ls -l ../../image/firmware.bin |awk '{print $$5}'` (`du -h ../../image/firmware.bin| awk '{print $$1}'`)"
	@echo ""
	@echo "======= Show ROM/FM ID info.  ======"
	@if [ -e ../../../tools/src/BinMan/binman -a -e ../../../tools/src/BinMan/binman.bms ]; then \
		cp ../../../tools/src/BinMan/binman .  ;\
		cp ../../../tools/src/BinMan/binman.bms . ;\
		./binman ../../image/firmware.bin  | grep "rom_id\|family" ;\
		rm -f binman binman.bms ;\
	fi
	@echo ""
	@echo ""

show_image_old:
	@ROOTSIZE=$(shell du -s $(FINAL_ROOT) | cut -f1); \
	IMGSIZE=$(shell du -s $(SQUASHFS) |  cut -f1); \
	echo "=============================================="; \
	echo "Summary:"; \
	echo "==>Squashfs disk size  = $$ROOTSIZE KBytes"; \
	echo "==>Squashfs image size = $$IMGSIZE KBytes"; \
	echo "=============================================="

mk_ui_squashfs:
	@rm -f $(IMAGE_DIR)/UI.sqfs $(IMAGE_DIR)/LANG.sqfs
ifeq ($(KERNEL_SDK), 262263368)
	@$(MKSQUASHFS) $(UI_DIR) $(IMAGE_DIR)/UI.sqfs -be -all-root > /dev/null
else	
ifeq ($(KERNEL_SDK), 4000)
	@$(MKSQUASHFS4) $(UI_DIR) $(IMAGE_DIR)/UI.sqfs -all-root -no-progress > /dev/null
else
ifeq ($(KERNEL_SDK), 4010)
	@$(MKSQUASHFS4) $(UI_DIR) $(IMAGE_DIR)/UI.sqfs -all-root -no-progress > /dev/null
else
ifeq ($(KERNEL_SDK), 40007620)
	@$(MKSQUASHFS4) $(UI_DIR) $(IMAGE_DIR)/UI.sqfs -all-root -no-progress > /dev/null
else
ifeq ($(KERNEL_SDK), 41007621)
	@$(MKSQUASHFS5) $(UI_DIR) $(IMAGE_DIR)/UI.sqfs -all-root -no-progress > /dev/null
else
	@$(MKSQUASHFS) $(UI_DIR) $(IMAGE_DIR)/UI.sqfs -le -all-root -no-progress > /dev/null
endif
endif
endif
endif
endif

ifdef FULL_COPY
ifeq ($(HANDLE_UI_LANGUAGE), 1)
	@mkdir -p $(UI_DIR)/lang
ifeq ($(KERNEL_SDK), 4000)
	@$(MKSQUASHFS4) $(UI_DIR)/lang $(IMAGE_DIR)/LANG.sqfs -all-root -no-progress > /dev/null
else
ifeq ($(KERNEL_SDK), 4010)
	@$(MKSQUASHFS4) $(UI_SRC_DIR)/lang $(IMAGE_DIR)/LANG.sqfs -all-root -no-progress > /dev/null
else
ifeq ($(KERNEL_SDK), 40007620)
	@$(MKSQUASHFS4) $(UI_DIR)/lang $(IMAGE_DIR)/LANG.sqfs -all-root -no-progress > /dev/null
else
	@$(MKSQUASHFS) $(UI_DIR)/lang $(IMAGE_DIR)/LANG.sqfs -le -all-root -no-progress > /dev/null
endif
endif       
	@$(PRIV_TOOLS)/bin/binman --s ui_lang.bms --d $(IMAGE_DIR) > /dev/null
	@$(PRIV_TOOLS)/bin/binman --s lang.bms --d $(IMAGE_DIR) > /dev/null
endif       
else
	@$(PRIV_TOOLS)/bin/binman --s ui.bms --d $(IMAGE_DIR) > /dev/null
endif
else
	@$(PRIV_TOOLS)/bin/binman --s ui.bms --d $(IMAGE_DIR) > /dev/null
endif

ifeq ($(HANDLE_CUST_DEFAULT), 1)
	@mkdir -p $(CUST_DEFAULT_DIR)
	@rm -f $(IMAGE_DIR)/r_default.sqfs
	@export CORNER_DE_PRJROOT=$(TOPDIR); FW_REV=`$(TOPDIR)/rootfs/glue/getfwrev` \
	FW_VER=${shell cat $(TOPDIR)/VERSION} \
	$(PRIV_TOOLS)/bin/mkcsxf -d $(TOPDIR)/rootfs/src/var/csidbase -o $(CUST_DEFAULT_DIR)/reset.dat -zc custom_reset.txt -X s > /dev/null

ifeq ($(KERNEL_SDK), 4000)
	@$(MKSQUASHFS4) $(CUST_DEFAULT_DIR) $(IMAGE_DIR)/r_default.sqfs -all-root -no-progress > /dev/null
else
ifeq ($(KERNEL_SDK), 4010)
	@$(MKSQUASHFS4) $(CUST_DEFAULT_DIR) $(IMAGE_DIR)/r_default.sqfs -all-root -no-progress > /dev/null
else
ifeq ($(KERNEL_SDK), 40007620)
	@$(MKSQUASHFS4) $(CUST_DEFAULT_DIR) $(IMAGE_DIR)/r_default.sqfs -all-root -no-progress > /dev/null

else
	@$(MKSQUASHFS) $(CUST_DEFAULT_DIR) $(IMAGE_DIR)/r_default.sqfs -le -all-root -no-progress > /dev/null
endif
endif       
endif       
	@$(PRIV_TOOLS)/bin/binman --s r_default.bms --d $(IMAGE_DIR) > /dev/null
endif


mk_mydlink_squashfs:
	@rm -f $(IMAGE_DIR)/mydlink.sqfs
	@rm -r -f $(MYDLINK_DIR)/.svn	
ifeq ($(MYDLINK),y)
ifeq ($(KERNEL_SDK), 262263368)
	@$(MKSQUASHFS) $(MYDLINK_DIR) $(IMAGE_DIR)/mydlink.sqfs -be -all-root -no-progress > /dev/null
else
ifeq ($(KERNEL_SDK), 4000)
	@$(MKSQUASHFS4) $(MYDLINK_DIR) $(IMAGE_DIR)/mydlink.sqfs -all-root -no-progress > /dev/null
else
ifeq ($(KERNEL_SDK), 4010)
	@$(MKSQUASHFS4) $(MYDLINK_DIR) $(IMAGE_DIR)/mydlink.sqfs -all-root -no-progress > /dev/null
else
ifeq ($(KERNEL_SDK), 40007620)
	@$(MKSQUASHFS4) $(MYDLINK_DIR) $(IMAGE_DIR)/mydlink.sqfs -all-root -no-progress > /dev/null
else
	@$(MKSQUASHFS) $(MYDLINK_DIR) $(IMAGE_DIR)/mydlink.sqfs -le -all-root -no-progress > /dev/null
endif
endif
endif
endif

ifdef FULL_COPY
	@$(PRIV_TOOLS)/bin/binman --s mydlink.bms --d $(IMAGE_DIR) > /dev/null
else
	@$(PRIV_TOOLS)/bin/binboy-static > /dev/null 2>&1 ;\
		if [ "$$?" = "0" ]; then \
			cd ../../image; \
			$(PRIV_TOOLS)/bin/binboy-static @mydlink ; \
			cd - ;\
		    exit 0; \
		else \
			$(PRIV_TOOLS)/bin/binboy > /dev/null 2>&1 ;\
    		if [ "$$?" = "0" ]; then \
				cd ../../image; \
				$(PRIV_TOOLS)/bin/binboy @mydlink ; \
				cd - ;\
		        exit 0; \
		    else \
		        echo "Error !! Can't apply header on firmware, please check your compile environment !"; \
		        echo ""; \
		        exit 1; \
		    fi; \
		fi    
endif
endif

image_add_time:
	@cd $(IMAGE_DIR) ; \
	find -name "*.*-*.bin" -delete ; \
	for f in * ; do \
		[ "$$f" = "$${f%%.bin}" ] && continue ; \
		f_="$${f%%.bin}" ; \
		t="`stat -c %y \"$$f\"`" ; \
		t1="`expr substr \"$$t\" 6 5`" ; \
		t1="`echo $$t1 | sed 's/-//g'`" ; \
		t2="`expr substr \"$$t\" 12 5`" ; \
		t2="`echo $$t2 | sed 's/://g'`" ; \
		mv "$$f" "$$f_.$$t1-$$t2.bin" ; \
	done ; \
	cd -

.PHONY: rootfs
rootfs:
	make -C .. all

#include ../makefile.install

