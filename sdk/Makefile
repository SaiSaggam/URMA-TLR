#
# Makefile for full system
#

export TARGET   ?= mipsel-linux-uclibc
export TOPDIR   := $(shell pwd)
export TOOLPATH ?= $(shell cat TOOLPATH)
export PATH := $(TOOLPATH):$(PATH)

export LC_ALL := C

#

ifneq ($(wildcard ROM_ID),)
export ROM_ID := `cat $(TOPDIR)/ROM_ID`
endif

export model := `cat DOWNSIZE_PROJECT`

TOOLS_LIST  := tools
KERNEL_LIST := kernel_build kernel_install
ROOTFS_LIST := busybox rootfs
PART_LIST   := bootloader $(TOOLS_LIST) $(KERNEL_LIST) $(ROOTFS_LIST) show_o3g_version
CLEAN_LIST  := $(TOOLS_LIST) linux $(ROOTFS_LIST)
	
.PHONY: all toolchain $(PART_LIST) 

all:  check_project $(PART_LIST) fw_downsize

tools rootfs busybox:
	@make -s -C $@ all

bootloader:
	@make -s -C bootloader

kernel_install:
	@make -s -C linux install

kernel_build:
	@TARGET=mipsel-linux make -C linux all

fw_downsize:
	@if [ -f DOWNSIZE_PROJECT ]; then \
		echo	"---- $(model) Down Size ----"; \
		./fw-action fwds; \
	fi


#make a whole image
linuxall: 
	@make clean 
	@make -s $(PART_LIST)

#make a kernel.bin
linux: kernel_build kernel_install glue

#quickly make a filesystem
glue:
	@make -s -C rootfs glue

#make filesystem
rootfs: $(ROOTFS_LIST)

help:
	@echo "Make Command List"
	@echo "make: make all of the project without cleaning"
	@echo "make linuxall: make a whole new world"
	@echo "make linux: make kernel"
	@echo "make rootfs: make filesystem (include busybox, rootfs)"
	@echo "make busybox: make busybox and install to busybox/root"
	@echo "make glue: quickly make filesystem"
	@echo "make clean: clean linux ,filesystem"
	@echo "make svnst: check the all Modified/Add component in project "
	@echo "make svnlist: export the svn path/version of the used component  "
	@echo "make version: export the svn path of the used component for L3CRS_Compo_Table.txt "
	@echo "make list: show the svn path/version of the used component and the trunk*"
	@echo "make GPL: release GPL code of the current project"
	@echo "make GPL_tools: re-package tools code for GPL"
	@echo "make GPL_linux: re-package linux code for GPL"
	@echo "make GPL_rootfs: re-package rootfs code for GPL"
	@echo "make GPL_pack: package GPL code"
	@echo "make chkGPL: try to make GPL firmware, and compare with normal firmware"
	
show_o3g_version:
	@if [ -f rootfs/glue/get_o3g_version ]; then \
		echo "----------------------------"; \
		rootfs/glue/get_o3g_version; \
		echo "----------------------------"; \
	fi

check_project:
	@if [ ! -f PROJECT ] ; then \
		make -C project_config menuconfig; \
	else \
		make -C project_config check; \
	fi

menuconfig:
	@make -C project_config clean
	@make -C project_config menuconfig
	@. project_config/project.config ; \
		[ "$$CLEAN_PROJ" = "y" ] && \
			echo "Clean UP Project" && make -s clean || \
			echo "No Clean UP Project"

cp_reset:
	@if [ -f project_config/reset/reset.txt ] ; then \
		mv project_config/reset/reset.txt project_config/reset/reset.txt.old ; \
	fi ; \
	echo "#################" > ${TOPDIR}/project_config/reset/reset.txt.tmp ; \
	echo "##  reset.txt  ##" >> ${TOPDIR}/project_config/reset/reset.txt.tmp ; \
	echo "#################" >> ${TOPDIR}/project_config/reset/reset.txt.tmp ; \
	echo "" >> ${TOPDIR}/project_config/reset/reset.txt.tmp ; \
	
	@if [ -f ${TOPDIR}/project_config/reset/SI_reset.txt ] ; then \
		echo "" >> ${TOPDIR}/project_config/reset/reset.txt.tmp ; \
		echo "###############  SI_reset.txt  ###############" >> ${TOPDIR}/project_config/reset/reset.txt.tmp ; \
		echo "" >> ${TOPDIR}/project_config/reset/reset.txt.tmp ; \
		cat ${TOPDIR}/project_config/reset/SI_reset.txt | grep -v "^#" | tr -d '\b\r' >> ${TOPDIR}/project_config/reset/reset.txt.tmp ; \
	fi ; \
	
	@for reset_list in `ls ${TOPDIR}/project_config/reset | grep _reset.txt | grep -v SI_reset.txt`; \
	do\
		echo "" >> ${TOPDIR}/project_config/reset/reset.txt.tmp ; \
		echo "###############  $${reset_list}  ###############" >> ${TOPDIR}/project_config/reset/reset.txt.tmp ; \
		echo "" >> ${TOPDIR}/project_config/reset/reset.txt.tmp ; \
		if [ "$${reset_list}" == "wifi_reset.txt"  ] ; then \
			wifi="`cat ${TOPDIR}/project_config/project.config | grep 551 | grep -v '^#'`" ; \
			if [ "$$wifi" == "" ] ; then \
				continue; \
			fi ; \
		fi ; \
		if [ "$${reset_list}" == "SSLVPN_reset.txt"  ] ; then \
			sslvpn="`cat ${TOPDIR}/project_config/project.config | grep 75d | grep -v '^#'`" ; \
			if [ "$$sslvpn" == "" ] ; then \
				continue; \
			fi ; \
		fi ; \
		cat ${TOPDIR}/project_config/reset/$${reset_list} | grep -v "^#" | tr -d '\b\r' >> ${TOPDIR}/project_config/reset/reset.txt.tmp ; \
	done
	@cat -s ${TOPDIR}/project_config/reset/reset.txt.tmp > ${TOPDIR}/project_config/reset/reset.txt
	@rm ${TOPDIR}/project_config/reset/reset.txt.tmp
	@echo ${TOPDIR}/rootfs/glue/ ${TOPDIR}/rootfs/amit_priv/CWebUI.90.mk/tmp/ | xargs -n 1 cp ${TOPDIR}/project_config/reset/reset.txt

svnlist:
	@rm -f image/svn.list
	@url="`svn info | grep URL: | cut -d " " -f 2`" ;\
	ver="`svn info | grep Rev: | cut -d " " -f 4`" ;\
	echo "$$url -r $$ver" >> image/svn.list ; 
	@url="`svn info bootloader/src | grep URL: | cut -d " " -f 2`" ;\
	ver="`svn info bootloader/src | grep Rev: | cut -d " " -f 4`" ;\
	echo "$$url -r $$ver" >> image/svn.list ;
	@url="`svn info linux/src | grep URL: | cut -d " " -f 2`" ;\
	ver="`svn info linux/src | grep Rev: | cut -d " " -f 4`" ;\
	echo "$$url -r $$ver" >> image/svn.list ;
	@instcomp_list="$${TOPDIR}/project_config/project.instcomp" ; \
	[ -f "$${TOPDIR}/project_config/project.instcomp_user" ] && instcomp_list="$${instcomp_list} $${TOPDIR}/project_config/project.instcomp_user" ; \
	for tmp_comp in `grep -h "PRJ_INSTALL_[_0-9a-zA-Z\-]*=[1|y]" $${instcomp_list}`; \
	do\
		tmp_comp=$${tmp_comp%%=1} ; \
		inst_comp=$${tmp_comp##PRJ_INSTALL_} ; \
		if [ -d rootfs/src/amit_priv/$${inst_comp} ]; then \
			url="`svn info rootfs/src/amit_priv/$${inst_comp} | grep URL: | cut -d " " -f 2`" ;\
			ver="`svn info rootfs/src/amit_priv/$${inst_comp} | grep Rev: | cut -d " " -f 4`" ;\
			echo "$$url -r $$ver" >> image/svn.list ; \
		elif [ -d rootfs/src/open_src/$${inst_comp} ]; then \
			url="`svn info rootfs/src/open_src/$${inst_comp} | grep URL: | cut -d " " -f 2`" ;\
			ver="`svn info rootfs/src/open_src/$${inst_comp} | grep Rev: | cut -d " " -f 4`" ;\
			echo "$$url -r $$ver" >> image/svn.list ; \
		elif [ -d rootfs/src/amit_open/$${inst_comp} ]; then \
			url="`svn info rootfs/src/amit_open/$${inst_comp} | grep URL: | cut -d " " -f 2`" ;\
			ver="`svn info rootfs/src/amit_open/$${inst_comp} | grep Rev: | cut -d " " -f 4`" ;\
			echo "$$url -r $$ver" >> image/svn.list ; \
		fi;\
	done
#	cd rootfs/src/amit_priv/CWebUI/ ;\
#  	./ui_info_new.sh ;\
#   	cd ../../../../;
   
version:
	@instcomp_list="$${TOPDIR}/project_config/project.instcomp" ; \
	[ -f "$${TOPDIR}/project_config/project.instcomp_user" ] && instcomp_list="$${instcomp_list} $${TOPDIR}/project_config/project.instcomp_user" ; \
	for tmp_comp in `grep -h "PRJ_INSTALL_[_0-9a-zA-Z\-]*=[1|y]" $${instcomp_list}`; \
	do\
		tmp_comp=$${tmp_comp%%=1} ; \
		inst_comp=$${tmp_comp##PRJ_INSTALL_} ; \
		if [ -d rootfs/src/amit_priv/$${inst_comp} ]; then \
			url_src="`svn info rootfs/src/amit_priv/$${inst_comp} | grep URL: | cut -c32-`" ;\
			prj_ver="`cat ${TOPDIR}/project_config/project.version | grep -v "/" | grep $${inst_comp} | sed 's#.*=##'`" ; \
			sed -i "s#$$inst_comp=v#$$url_src=v#g" ${TOPDIR}/project_config/L3CRS_Compo_Table.txt ;\
			if [ "$$prj_ver" == "v0_20990000" ] ; then \
				ver_src="`svn info rootfs/src/amit_priv/$${inst_comp} | grep Rev: | cut -d " " -f 4`" ;\
				url_mod="`svn info rootfs/src/amit_priv/$${inst_comp} | grep URL: | cut -c32- | sed 's#/#\\\/#g'`" ; \
				sed -i "/$$url_mod=v.*/s/v0_20990000.*/v0_20990000($$ver_src)/g" ${TOPDIR}/project_config/L3CRS_Compo_Table.txt ;\
			fi;\
		elif [ -d rootfs/src/open_src/$${inst_comp} ]; then \
			url_src="`svn info rootfs/src/open_src/$${inst_comp} | grep URL: | cut -c32-`" ;\
			sed -i "s#$$inst_comp=v#$$url_src=v#g" ${TOPDIR}/project_config/L3CRS_Compo_Table.txt ;\
			prj_ver="`cat ${TOPDIR}/project_config/project.version | grep -v "/" | grep -w $${inst_comp} | sed 's#.*=##'`" ; \
			if [ "$$prj_ver" == "v0_20990000" ] ; then \
				url_mod="`svn info rootfs/src/open_src/$${inst_comp} | grep URL: | cut -c32- | sed 's#/#\\\/#g'`" ; \
				ver_src="`svn info rootfs/src/open_src/$${inst_comp} | grep Rev: | cut -d " " -f 4`" ;\
				sed -i "/$$url_mod=v.*/s/v0_20990000.*/v0_20990000($$ver_src)/g" ${TOPDIR}/project_config/L3CRS_Compo_Table.txt ;\
			fi;\
		elif [ -d rootfs/src/amit_open/$${inst_comp} ]; then \
			url_src="`svn info rootfs/src/amit_open/$${inst_comp} | grep URL: | cut -c32-`" ;\
			sed -i "s#$$inst_comp=v#$$url_src=v#g" ${TOPDIR}/project_config/L3CRS_Compo_Table.txt ;\
			prj_ver="`cat ${TOPDIR}/project_config/project.version | grep -v "/" | grep -w $${inst_comp} | sed 's#.*=##'`" ; \
			if [ "$$prj_ver" == "v0_20990000" ] ; then \
				url_mod="`svn info rootfs/src/amit_open/$${inst_comp} | grep URL: | cut -c32- | sed 's#/#\\\/#g'`" ; \
				ver_src="`svn info rootfs/src/amit_open/$${inst_comp} | grep Rev: | cut -d " " -f 4`" ;\
				sed -i "/$$url_mod=v.*/s/v0_20990000.*/v0_20990000($$ver_src)/g" ${TOPDIR}/project_config/L3CRS_Compo_Table.txt ;\
			fi;\
		fi;\
	done	
	
	@for ui_comp in `grep "/" ${TOPDIR}/project_config/project.version `; \
	do\
		ui_name=$${ui_comp%%=*} ; \
		ui_ver=$${ui_comp##$$ui_name=} ; \
		if [ "$$ui_ver" == "v0_20990000" ] ; then \
			svnlist_line="`grep $${ui_name} ${TOPDIR}/image/svn.list`"; \
			if [ -n "$${svnlist_line}" ] ; then \
				ver="`echo $${svnlist_line} | cut -d " " -f 3`" ; \
				ui_name="`echo $${ui_name} | sed 's#\/#\\\/#g'`" ; \
				sed -i "/$$ui_name=v.*/s/v0_20990000.*/v0_20990000($$ver)/g" ${TOPDIR}/project_config/L3CRS_Compo_Table.txt ;\
			fi; \
		fi; \
	done
  
CLEAN_LIST := $(patsubst %,clean-%,$(CLEAN_LIST))

.PHONY: clean $(CLEAN_LIST)

clean: $(CLEAN_LIST)
	rm -f image/*.bin
	rm -f image/zImage.lzma
	rm -f image/squashfs.o

$(CLEAN_LIST):
	@echo "*** Cleaning $(patsubst clean-%,%,$@)..."
	@make -s -C $(patsubst clean-%,%,$@) clean

#

ifneq ($(wildcard makefile.corner),)
include makefile.corner
endif
