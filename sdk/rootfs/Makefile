#
# Makefile for root file system
#
# (Corner DE core file)

export TOPDIR := $(shell cd ..; pwd)
include makefile.envars

# get and sort all components
tmp_list_0 := $(wildcard */*.mk)
tmp_list_1 := $(foreach mk,$(tmp_list_0),$(word 2,$(subst ., ,$(mk))).$(word 1,$(subst ., ,$(mk))))
tmp_list_2 := $(sort $(tmp_list_1))
COMPO_LIST := $(foreach mk,$(tmp_list_2),$(word 2,$(subst ., ,$(mk))).$(word 1,$(subst ., ,$(mk))).mk)

ifndef NEW_PRJ_INSTALL
include makefile.config
endif

.PHONY: all rootfs main
all rootfs: main

ifdef FULL_COPY
include makefile.corner
else
chkrev:
endif

# function macros
GET_COMPO = $(word 2,$(subst ., ,$(subst /, ,$(1))))

TARGET_LIST := init_root init_src mk_compos glue
.PHONY: $(TARGET_LIST) 

main: $(TARGET_LIST) chkrev
	@echo "+------------------------------+"
	@echo "| \$$LOG_COLLECTOR=${LOG_COLLECTOR} \$$RD_LEVEL=${RD_LEVEL} |"
	@echo "+------------------------------+"

init_root: #INSTALL.init_root
	@rm -fr   $(INSTALL_ROOT)
ifdef FULL_COPY
	@mkdir -p $(INSTALL_ROOT)/bin
	#@mkdir -p $(INSTALL_ROOT)/etc
	@mkdir -p $(INSTALL_ROOT)/lib
	@mkdir -p $(INSTALL_ROOT)/proc
	@mkdir -p $(INSTALL_ROOT)/sys
	@mkdir -p $(INSTALL_ROOT)/sbin
	@mkdir -p $(INSTALL_ROOT)/ram
	@mkdir -p $(INSTALL_ROOT)/mtd
	@mkdir -p $(INSTALL_ROOT)/usr/bin
	@mkdir -p $(INSTALL_ROOT)/usr/sbin
	@mkdir -p $(INSTALL_ROOT)/usr/etc
	@mkdir -p $(INSTALL_ROOT)/usr/uo

ifeq ($(TC3262),1)
	#@-mkdir -p $(WWWDIR)
else
ifeq ($(MVUI2RAM),1)
	#@-mkdir -p $(WWWDIR)
else
	@-mkdir -p $(WWWDIR)
endif
endif

	@mkdir -p $(INSTALL_ROOT)/usr/lib
	@mkdir -p $(INSTALL_ROOT)/usr/mv2ram/tmp/csman
	@mkdir -p $(INSTALL_ROOT)/usr/mv2ram/etc/l2tp
	@mkdir -p $(INSTALL_ROOT)/usr/mv2ram/etc/iproute2
	@mkdir -p $(INSTALL_ROOT)/usr/mv2ram/etc/ppp/plugins
	@mkdir -p $(INSTALL_ROOT)/usr/mv2ram/etc/l7-protocols
	@mkdir -p $(INSTALL_ROOT)/usr/mv2ram/etc/xl2tpd
	@mkdir -p $(INSTALL_ROOT)/usr/mv2ram/var/log
	@mkdir -p $(INSTALL_ROOT)/usr/mv2ram/var/run
	@mkdir -p $(INSTALL_ROOT)/usr/mv2ram/var/run/xl2tpd
	@mkdir -p $(INSTALL_ROOT)/usr/mv2ram/var/xml
	@mkdir -p $(INSTALL_ROOT)/usr/mv2ram/var/dnrd

ifeq (${PROJECT_NAME}, ${filter ${PROJECT_NAME}, CIW563AM})
	@mkdir -p $(INSTALL_ROOT)/usr/d2
endif
ifeq ($(TC3262),1)
	@mkdir -p $(INSTALL_ROOT)/lib/modules
endif	
	@ln -fs ram/etc $(INSTALL_ROOT)/etc
	@ln -fs ram/var $(INSTALL_ROOT)/var
	@ln -fs ram/tmp $(INSTALL_ROOT)/tmp
ifeq ($(MYDLINK),y)
#	@mkdir -p $(INSTALL_ROOT)/usr/mv2ram/mydlink
	@mkdir -p $(INSTALL_ROOT)/usr/mydlink
#	@ln -fs ram/mydlink $(INSTALL_ROOT)/mydlink
	@ln -fs usr/mydlink $(INSTALL_ROOT)/mydlink

endif
	# let /dev/log be writable (for syslogd)
	@mkdir -p $(INSTALL_ROOT)/dev
	@ln -fs /var/log/socket $(INSTALL_ROOT)/dev/log
ifeq ($(TC3262),1)	
	@mkdir -p $(INSTALL_ROOT)/usr/mv2ram/tmp/www3
	@ln -fs /tmp/www3 $(INSTALL_ROOT)/usr/www
endif

ifeq ($(MVUI2RAM),1)	
	@mkdir -p $(INSTALL_ROOT)/usr/mv2ram/tmp/www3
	@ln -fs /tmp/www3 $(INSTALL_ROOT)/usr/www
endif

else
	@cp -a $(TOPDIR)/rootfs/root.released $(INSTALL_ROOT)
endif

# increase this number if RDS00047_LAYOUT is changed
RDS00047_LAYOUT_VER := 2

RDS00047_LAYOUT :=
RDS00047_LAYOUT += $(COMPO_BASE)/include
RDS00047_LAYOUT += $(COMPO_BASE)/$(TARGET)-lib
RDS00047_LAYOUT += $(COMPO_BASE)/amit_priv/include
RDS00047_LAYOUT += $(COMPO_BASE)/amit_priv/$(TARGET)-lib

init_src: $(COMPO_BASE)/.tag.prepared.$(RDS00047_LAYOUT_VER)

$(COMPO_BASE)/.tag.prepared.$(RDS00047_LAYOUT_VER):
ifdef FULL_COPY
	@mkdir -p $(RDS00047_LAYOUT)
	@rm -f $(COMPO_BASE)/.tag.prepared*
	@touch $@
endif


.PHONY: $(COMPO_LIST)
mk_compos: $(COMPO_LIST)

$(COMPO_LIST):
ifdef NEW_PRJ_INSTALL
	@tmp_comp=$@ ; \
	tmp_comp=$${tmp_comp##*/} ; \
	has_comp=$${tmp_comp%%.*.mk} ;\
	instcomp_list="$${TOPDIR}/project_config/project.instcomp" ; \
	[ -f "$${TOPDIR}/project_config/project.instcomp_user" ] && instcomp_list="$${instcomp_list} $${TOPDIR}/project_config/project.instcomp_user" ; \
	for tmp_comp in `grep -h "PRJ_INSTALL_[_0-9a-zA-Z\-]*=[1|y]" $${instcomp_list}`; \
	do  \
		tmp_comp=$${tmp_comp%%=1} ; \
		inst_comp=$${tmp_comp##PRJ_INSTALL_} ; \
		if test "$$has_comp" = "$$inst_comp" ; then \
			if test -e $@/Makefile; then \
				make -s -C $@ all-but-not-chkrev || exit $? ; \
			fi ; \
		fi ; \
	done
else
	@if test "$(EXCLUDE.$(call GET_COMPO,$@))" == "y"; then \
		echo "!!!---->>> $@ <<<--- is excluded"; \
 	elif test -f $@/Makefile; then \
		echo "*** Making $@..."; \
	   	make -s -C $@ all-but-not-chkrev; \
 	else \
 		echo "!!!---->>> $@ <<<--- may be removed"; \
 	fi
endif

glue:
	make -C $@ all
	@if test -x ~/bin/cde-hook/post-rootfs; then \
		~/bin/cde-hook/post-rootfs $(TOPDIR); \
	fi

CLEAN_LIST := $(patsubst %,clean-%,$(COMPO_LIST))

.PHONY: clean $(CLEAN_LIST)
clean: $(CLEAN_LIST)
ifdef FULL_COPY
	@rm -fr $(RDS00047_LAYOUT) # to make sure we have a clean start
	@rm -f $(COMPO_BASE)/.tag.prepared*
endif

$(CLEAN_LIST):
ifdef NEW_PRJ_INSTALL
	@tmp_comp=$@ ; \
	tmp_comp=$${tmp_comp##clean-*/} ; \
	has_comp=$${tmp_comp%%.*.mk} ;\
	instcomp_list="$${TOPDIR}/project_config/project.instcomp" ; \
	[ -f "$${TOPDIR}/project_config/project.instcomp_user" ] && instcomp_list="$${instcomp_list} $${TOPDIR}/project_config/project.instcomp_user" ; \
	for tmp_comp in `grep -h "PRJ_INSTALL_[_0-9a-zA-Z\-]*=[1|y]" $${instcomp_list}`; \
	do  \
		tmp_comp=$${tmp_comp%%=1} ; \
		inst_comp=$${tmp_comp##PRJ_INSTALL_} ; \
		if test "$$has_comp" = "$$inst_comp" ; then \
			if test -f $(patsubst clean-%,%,$@)/Makefile; then \
				echo "*** Cleaning $(patsubst clean-%,%,$@)..."; \
				make -s -C $(patsubst clean-%,%,$@) check-then-clean || exit $? ; \
			fi ; \
		fi ; \
	done 
else
	@if test "$(EXCLUDE.$(call GET_COMPO,$@))" == "y"; then \
		echo "!!!------>>> $(patsubst clean-%,%,$@) <<<--- is excluded"; \
	elif test -f $(patsubst clean-%,%,$@)/Makefile; then \
		echo "*** Cleaning $(patsubst clean-%,%,$@)..."; \
		make -s -C $(patsubst clean-%,%,$@) check-then-clean; \
	else \
		echo "!!!------>>> $(patsubst clean-%,%,$@) <<<--- may be removed"; \
	fi
endif

.PHONY: distclean
distclean:
	rm -fr $(INSTALL_ROOT)
	#rm -fr $(COMPO_BASE)
	rm -rf squashfs.o
	rm -rf root.tmp/

# must be located behind 'all' target. for safety, let it be at bottom
#include makefile.install


