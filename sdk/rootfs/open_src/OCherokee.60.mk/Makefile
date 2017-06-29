#
# Makefile to compile OQuaggaRouging
#

include ../../makefile.envars

CHECKOUT_PREFIX ?= svn://svn.atc.amit.com.tw
COMPO_URL := $(CHECKOUT_PREFIX)/Sahara/OCherokee/trunk
COMPO_REV := 
COMPO_DIR := $(COMPO_BASE)/open_src/OCherokee

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
PREFIX += HANDLE_OSPF_BGP=$(HANDLE_OSPF_BGP)


# WITH_VTYSH = 1


build:
	@if test ! -e $(COMPO_DIR)/configed ; then ./configure $(COMPO_DIR) $(TARGET)  i386-redhat-linux ; fi
	@touch $(COMPO_DIR)/configed 
	@make -C $(COMPO_DIR) ${PREFIX} 
#	@$(TARGET)-strip $(COMPO_DIR)/ripd/ripd
#	@$(TARGET)-strip $(COMPO_DIR)/zebra/zebra

#install: install_sbin install_bin install_data install_etc install_include install_lib install_share install_var
install: install_sbin install_bin install_data install_etc install_share install_var

install_sbin:
	@mkdir -p $(INSTALL_ROOT)/usr/sbin
	@cp -a ${COMPO_DIR}/cherokee/cherokee				$(INSTALL_ROOT)/usr/sbin
	@cp -a ${COMPO_DIR}/cherokee/cherokee-admin			$(INSTALL_ROOT)/usr/sbin
	@cp -a ${COMPO_DIR}/cherokee/cherokee-worker			$(INSTALL_ROOT)/usr/sbin
	@chmod 755 $(INSTALL_ROOT)/usr/sbin/cherokee*

install_bin:
	@mkdir -p $(INSTALL_ROOT)/usr/bin
	@cp -a ${COMPO_DIR}/cget/cget					$(INSTALL_ROOT)/usr/bin
	@cp -a ${COMPO_DIR}/cherokee/cherokee-admin-launcher		$(INSTALL_ROOT)/usr/bin
	@cp -a ${COMPO_DIR}/cherokee-config				$(INSTALL_ROOT)/usr/bin
	@cp -a ${COMPO_DIR}/cherokee/cherokee-macos-askpass		$(INSTALL_ROOT)/usr/bin
	@cp -a ${COMPO_DIR}/cherokee/cherokee-panic			$(INSTALL_ROOT)/usr/bin
	@cp -a ${COMPO_DIR}/cherokee/cherokee-tweak			$(INSTALL_ROOT)/usr/bin
	@cp -a ${COMPO_DIR}/admin/CTK/CTK-run				$(INSTALL_ROOT)/usr/bin
	@chmod 755 $(INSTALL_ROOT)/usr/bin/*

install_data:
	@#mkdir -p $(INSTALL_ROOT)/usr/share/aclocal
	@#cp -a ${COMPO_DIR}/cherokee.m4					$(INSTALL_ROOT)/usr/share/aclocal/
	@#mkdir -p $(INSTALL_ROOT)/usr/share/cherokee
	@#cp -ar ${COMPO_DIR}/admin					$(INSTALL_ROOT)/usr/share/cherokee/
	@#find $(INSTALL_ROOT)/usr/share/cherokee/admin -name Makefile -type f -print | xargs /bin/rm -f
	@#find $(INSTALL_ROOT)/usr/share/cherokee/admin -name Makefile.am -type f -print | xargs /bin/rm -f
	@#find $(INSTALL_ROOT)/usr/share/cherokee/admin -name Makefile.in -type f -print | xargs /bin/rm -f
	@mkdir -p $(INSTALL_ROOT)/usr/share/cherokee/deps
	@cp -a ${COMPO_DIR}/cherokee/*.deps				$(INSTALL_ROOT)/usr/share/cherokee/deps/

install_etc:
	@mkdir -p $(INSTALL_ROOT)/usr/mv2ram/etc/cherokee
	@cp -a ${COMPO_DIR}/cherokee.conf.sample			$(INSTALL_ROOT)/usr/mv2ram/etc/cherokee/cherokee.conf
	@#cp -a ${COMPO_DIR}/performance.conf.sample			$(INSTALL_ROOT)/usr/mv2ram/etc/cherokee/cherokee.conf.perf_sample

install_include:
	@mkdir -p $(INSTALL_ROOT)/usr/include/cherokee
	@cp -a ${COMPO_DIR}/cherokee/*.h				$(INSTALL_ROOT)/usr/include/cherokee/

install_lib:
	@mkdir -p $(INSTALL_ROOT)/usr/lib
	@cp -a ${COMPO_DIR}/cherokee/.libs/*-base.*			$(INSTALL_ROOT)/usr/lib/
	@cp -a ${COMPO_DIR}/cherokee/.libs/*-client.*			$(INSTALL_ROOT)/usr/lib/
	@cp -a ${COMPO_DIR}/cherokee/.libs/*-server.*			$(INSTALL_ROOT)/usr/lib/
	@#mkdir -p $(INSTALL_ROOT)/usr/lib/cherokee
	@#cp -a ${COMPO_DIR}/cherokee/.libs/libplugin_*			$(INSTALL_ROOT)/usr/lib/cherokee/
	@find $(INSTALL_ROOT)/usr/lib -name *.o -type f -print | xargs /bin/rm -f
	@find $(INSTALL_ROOT)/usr/lib -name *.lai -type f -print | xargs /bin/rm -f
	@mkdir -p $(INSTALL_ROOT)/usr/lib/pkgconfig
	@cp -a ${COMPO_DIR}/cherokee.pc					$(INSTALL_ROOT)/usr/lib/pkgconfig/

install_share:
	@mkdir -p $(INSTALL_ROOT)/usr/share/cherokee
	@cp -ar ${COMPO_DIR}/icons					$(INSTALL_ROOT)/usr/share/cherokee/icons
	@cp -ar ${COMPO_DIR}/themes					$(INSTALL_ROOT)/usr/share/cherokee/themes
	@#mkdir -p $(INSTALL_ROOT)/usr/share/doc/cherokee
	@#cp -a ${COMPO_DIR}/doc/*.html					$(INSTALL_ROOT)/usr/share/doc/cherokee/
	@#mkdir -p $(INSTALL_ROOT)/usr/share/doc/cherokee/media
	@#cp -ar ${COMPO_DIR}/doc/media/css				$(INSTALL_ROOT)/usr/share/doc/cherokee/media/css
	@#cp -ar ${COMPO_DIR}/doc/media/images				$(INSTALL_ROOT)/usr/share/doc/cherokee/media/images
	@find $(INSTALL_ROOT)/usr/share/cherokee -name Makefile -type f -print | xargs /bin/rm -f
	@find $(INSTALL_ROOT)/usr/share/cherokee -name Makefile.am -type f -print | xargs /bin/rm -f
	@find $(INSTALL_ROOT)/usr/share/cherokee -name Makefile.in -type f -print | xargs /bin/rm -f

install_var:
	@mkdir -p $(INSTALL_ROOT)/usr/mv2ram/var/www/images
	@cp -a ${COMPO_DIR}/www/*.png					$(INSTALL_ROOT)/usr/mv2ram/var/www/images
	@cp -a ${COMPO_DIR}/www/*.ico					$(INSTALL_ROOT)/usr/mv2ram/var/www/images
	@cp -a ${COMPO_DIR}/www/*.html					$(INSTALL_ROOT)/usr/mv2ram/var/www
	@#mkdir -p $(INSTALL_ROOT)/usr/mv2ram/var/run
	@#mkdir -p $(INSTALL_ROOT)/usr/mv2ram/var/log
	@#mkdir -p $(INSTALL_ROOT)/usr/mv2ram/var/lib
	@#mkdir -p $(INSTALL_ROOT)/usr/mv2ram/var/lib/cherokee
	@#mkdir -p $(INSTALL_ROOT)/usr/mv2ram/var/lib/cherokee/graphs
	@#mkdir -p $(INSTALL_ROOT)/usr/mv2ram/var/lib/cherokee/graphs/images

clean:
	@rm -f $(COMPO_DIR)/configed
	@-rm -f ${COMPO_DIR}/*.conf.uo
	if [ -e $(COMPO_DIR)/Makefile ]; then make -C $(COMPO_DIR) clean ${PREFIX}; fi;
#	@make -C $(COMPO_DIR) clean ${PREFIX}
#	@make -C $(COMPO_DIR)/Libs/Libncurses clean ${PREFIX}

copying:

