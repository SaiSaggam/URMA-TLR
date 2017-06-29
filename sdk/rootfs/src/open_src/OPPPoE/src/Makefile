# Makefile.  Generated from Makefile.in by configure.
#***********************************************************************
#
# Makefile
#
# Makefile for Roaring Penguin's Linux user-space PPPoE client.
#
# Copyright (C) 2000 Roaring Penguin Software Inc.
#
# This program may be distributed according to the terms of the GNU
# General Public License, version 2 or (at your option) any later version.
#
# LIC: GPL
#
# $Id: Makefile.in,v 1.90 2006/02/23 15:38:45 dfs Exp $
#***********************************************************************

ifdef TARGET
    TARGET_PREFIX = $(TARGET)-
endif

ifndef CROSS_COMPILE
    CROSS_COMPILE = $(TARGET_PREFIX)
endif

ifdef LOG_COLLECTOR
    CFLAGS += -DLOG_COLLECTOR=$(LOG_COLLECTOR)
endif

ifdef DEBUG_LEVEL
    CFLAGS += -DDEBUG_LEVEL=$(DEBUG_LEVEL)
endif

ifndef KERNEL_DIR
	KERNEL_DIR=/usr/src/linux
endif


CC = $(CROSS_COMPILE)gcc
AR = $(CROSS_COMPILE)ar
STRIP=$(CROSS_COMPILE)strip 

# Version is set ONLY IN THE MAKEFILE!  Don't delete this!
VERSION=3.8

DEFINES=
prefix=/usr
exec_prefix=${prefix}
mandir=${prefix}/man
docdir=/usr/share/doc/rp-pppoe-$(VERSION)
install=/usr/bin/install -c
install_dir=/usr/bin/install -c -d
sbindir=${exec_prefix}/sbin

# Plugin for pppd on Linux
LINUX_KERNELMODE_PLUGIN=rp-pppoe.so
PPPD_INCDIR=../../OPPP

# Licensed version
LIC_INCDIR=
LIC_LIBDIR=
LIC_LIB=
LIC_DEFINE=

# PPPoE relay -- currently only supported on Linux
PPPOE_RELAY=pppoe-relay

# Program paths
PPPOE_PATH=$(sbindir)/pppoe
PPPD_PATH=/usr/sbin/pppd

# Include ServPoET version if we're building for ServPoET


# Kernel-mode plugin gets installed here.
PLUGIN_DIR=/etc/ppp/plugins
PLUGIN_PATH=$(PLUGIN_DIR)/rp-pppoe.so

# Configuration file paths
PPPOESERVER_PPPD_OPTIONS=/etc/ppp/pppoe-server-options

PATHS='-DPPPOE_PATH="$(PPPOE_PATH)"' '-DPPPD_PATH="$(PPPD_PATH)"' \
	'-DPLUGIN_PATH="$(PLUGIN_PATH)"' \
	'-DPPPOE_SERVER_OPTIONS="$(PPPOESERVER_PPPD_OPTIONS)"'

CFLAGS= -O2 -Wall -Wstrict-prototypes -ansi $(LIC_INCDIR) $(DEFINES) $(LIC_DEFINE) $(PATHS) -Ilibevent
TARGETS=pppoe pppoe-server pppoe-sniff pppoe-relay rp-pppoe.so
PPPOE_SERVER_LIBS=$(LIC_LIBDIR) $(LIC_LIB)

all: precheck $(TARGETS)
	@echo ""
	@echo "Type 'make install' as root to install the software."

pppoe-sniff: pppoe-sniff.o if.o common.o debug.o
	$(CC) -o pppoe-sniff pppoe-sniff.o if.o common.o debug.o

pppoe-server: pppoe-server.o if.o debug.o common.o md5.o libevent/libevent.a 
	$(CC) -o pppoe-server  pppoe-server.o if.o debug.o common.o md5.o $(PPPOE_SERVER_LIBS) -Llibevent -levent

pppoe: pppoe.o if.o debug.o common.o ppp.o discovery.o
	$(CC) -o pppoe pppoe.o if.o debug.o common.o ppp.o discovery.o
	$(STRIP) $@

pppoe-relay: relay.o if.o debug.o common.o
	$(CC) -o pppoe-relay relay.o if.o debug.o common.o
	$(STRIP) -s -R .note -R .comment $@

pppoe.o: pppoe.c pppoe.h
	$(CC) $(CFLAGS) '-DVERSION="$(VERSION)"' -c -o pppoe.o pppoe.c

discovery.o: discovery.c pppoe.h
	$(CC) $(CFLAGS) '-DVERSION="$(VERSION)"' -c -o discovery.o discovery.c

ppp.o: ppp.c pppoe.h
	$(CC) $(CFLAGS) '-DVERSION="$(VERSION)"' -c -o ppp.o ppp.c

md5.o: md5.c md5.h
	$(CC) $(CFLAGS) '-DVERSION="$(VERSION)"' -c -o md5.o md5.c

pppoe-server.o: pppoe-server.c pppoe.h 
	$(CC) $(CFLAGS) '-DVERSION="$(VERSION)"' -c -o pppoe-server.o pppoe-server.c

pppoe-sniff.o: pppoe-sniff.c pppoe.h
	$(CC) $(CFLAGS) '-DVERSION="$(VERSION)"' -c -o pppoe-sniff.o pppoe-sniff.c

if.o: if.c pppoe.h
	$(CC) $(CFLAGS) '-DVERSION="$(VERSION)"' -c -o if.o if.c

libevent/libevent.a:
	cd libevent && $(MAKE)

common.o: common.c pppoe.h
	$(CC) $(CFLAGS) '-DVERSION="$(VERSION)"' -c -o common.o common.c

debug.o: debug.c pppoe.h
	$(CC) $(CFLAGS) '-DVERSION="$(VERSION)"' -c -o debug.o debug.c

relay.o: relay.c relay.h pppoe.h
	$(CC) $(CFLAGS) '-DVERSION="$(VERSION)"' -c -o relay.o relay.c

# Linux-specific plugin
rp-pppoe.so: plugin/libplugin.a plugin/plugin.o
	$(CC) -o rp-pppoe.so -shared plugin/plugin.o plugin/libplugin.a

plugin/plugin.o: plugin.c
	$(CC) '-DRP_VERSION="$(VERSION)"' $(CFLAGS) -I$(PPPD_INCDIR) -c -o plugin/plugin.o -fPIC plugin.c

plugin/libplugin.a: plugin/discovery.o plugin/if.o plugin/common.o plugin/debug.o
	$(AR) -rc $@ $^

plugin/discovery.o: discovery.c
	$(CC) $(CFLAGS) '-DVERSION="$(VERSION)"' -c -o plugin/discovery.o -fPIC discovery.c

plugin/if.o: if.c
	$(CC) $(CFLAGS) '-DVERSION="$(VERSION)"' -c -o plugin/if.o -fPIC if.c

plugin/debug.o: debug.c
	$(CC) $(CFLAGS) '-DVERSION="$(VERSION)"' -c -o plugin/debug.o -fPIC debug.c

plugin/common.o: common.c
	$(CC) $(CFLAGS) '-DVERSION="$(VERSION)"' -c -o plugin/common.o -fPIC common.c

install: postcheck
	#@cp -a ../src/pppoe					$(INSTALL_ROOT)/usr/sbin
	#@cp -a ../scripts/pppoe-start			$(INSTALL_ROOT)/usr/bin
	#@cp -a ../scripts/pppoe-connect			$(INSTALL_ROOT)/usr/bin
	#@cp -a ../scripts/pppoe-stop			$(INSTALL_ROOT)/usr/bin
	#@cp -a ../scripts/pppoe-action			$(INSTALL_ROOT)/usr/bin
	#@cp -a ../scripts/pppoe-status			$(INSTALL_ROOT)/usr/bin
ifeq (${HANDLE_RUSSIAN}, 1)	
	@cp -a ../scripts/pppoe_russian				$(INSTALL_ROOT)/usr/bin
endif
	@mkdir -p $(INSTALL_ROOT)/usr/mv2ram/etc/ppp/plugins
	@cp -a ../src/rp-pppoe.so				$(INSTALL_ROOT)/usr/mv2ram/etc/ppp/plugins
ifdef FULL_COPY
	#@cp -a ../configs/pppoe.conf.uo			$(INSTALL_ROOT)/usr/uo
endif
ifeq (${HANDLE_DNSQ_AUTOBACK}, 1)
	@cp -a ../scripts/pppoe-dnsq-action		$(INSTALL_ROOT)/usr/bin/pppoe-action
	#@rm -f $(INSTALL_ROOT)/usr/bin/pppoe-start
	#@rm -f $(INSTALL_ROOT)/usr/bin/pppoe-connect
	#@rm -f $(INSTALL_ROOT)/usr/bin/pppoe-stop
	#@rm -f $(INSTALL_ROOT)/usr/bin/pppoe-status
	#@rm -f $(INSTALL_ROOT)/usr/bin/pppoe.conf.uo
endif
ifeq (${HANDLE_VOIPWAN}, 1)
	@cp -a ../scripts/pppoe-voipwan-start	$(INSTALL_ROOT)/usr/bin
	@cp -a ../scripts/pppoe-voipwan-connect	$(INSTALL_ROOT)/usr/bin
	@cp -a ../scripts/pppoe-voipwan-stop	$(INSTALL_ROOT)/usr/bin
	@cp -a ../scripts/pppoe-voipwan-action	$(INSTALL_ROOT)/usr/bin
	@cp -a ../scripts/pppoe-voipwan-status	$(INSTALL_ROOT)/usr/bin
ifdef FULL_COPY
	@cp -a ../configs/pppoe-voipwan.conf.uo	$(INSTALL_ROOT)/usr/uo
endif
endif
ifeq (${HANDLE_INET_PPPOE}, 1)
	@cp -a ../scripts/lcs-pppoe             $(INSTALL_ROOT)/usr/bin 
	#@cp -a ../scripts/lcs-pppoe-monitor     $(INSTALL_ROOT)/usr/bin 
	@cp -a ../scripts/lcs-pppoe-header      $(INSTALL_ROOT)/usr/bin 
	@cp -a ../scripts/ip-up-lcs             $(INSTALL_ROOT)/usr/mv2ram/etc/ppp/
	@cp -a ../scripts/ip-down-lcs           $(INSTALL_ROOT)/usr/mv2ram/etc/ppp/
endif
ifeq (${HANDLE_IPV6}, 1)
ifdef FULL_COPY
	@cp -a ../configs/pppoev6.conf.uo		$(INSTALL_ROOT)/usr/uo
endif
	@cp -a ../scripts/pppoev6-start			$(INSTALL_ROOT)/usr/bin
	@cp -a ../scripts/pppoev6-connect		$(INSTALL_ROOT)/usr/bin
	@cp -a ../scripts/pppoev6-stop			$(INSTALL_ROOT)/usr/bin
	@cp -a ../scripts/pppoev6-action		$(INSTALL_ROOT)/usr/bin
endif

precheck:
	@echo ""
	@echo "Build $(shell basename `pwd`) for $(PROJECT_NAME) ($(PROJECT_CUSTOMER))"
ifeq ($(PROJECT_CUSTOMER), ) 
	@echo ""
	@echo "No specify PROJECT_CUSTOMER in Makefile. "
	@echo "Maybe you should update the components Makefile of the Project"
	@echo ""
	@exit 1
endif 

postcheck:                                   
	@echo ""
	@echo "Install $(shell basename `pwd`) to $(INSTALL_ROOT)" 
ifeq ($(INSTALL_ROOT), ) 
	@echo ""
	@echo "No specify INSTALL_ROOT in Makefile. "
	@echo "Maybe you should update the components Makefile of the Project"
	@echo ""
	@exit 1
endif    


install_: all
	-mkdir -p $(RPM_INSTALL_ROOT)$(sbindir)
	$(install) -m 755 pppoe $(RPM_INSTALL_ROOT)$(sbindir)
	$(install) -m 755 pppoe-server $(RPM_INSTALL_ROOT)$(sbindir)
	if test -x licensed-only/pppoe-server-control ; then $(install) -m 755 licensed-only/pppoe-server-control $(RPM_INSTALL_ROOT)$(sbindir); fi
	if test -x pppoe-relay ; then $(install) -m 755 pppoe-relay $(RPM_INSTALL_ROOT)$(sbindir); fi
	if test -x pppoe-sniff; then $(install) -m 755 pppoe-sniff $(RPM_INSTALL_ROOT)$(sbindir); fi
	$(install) -m 755 ../scripts/pppoe-connect $(RPM_INSTALL_ROOT)$(sbindir)
	$(install) -m 755 ../scripts/pppoe-start $(RPM_INSTALL_ROOT)$(sbindir)
	$(install) -m 755 ../scripts/pppoe-status $(RPM_INSTALL_ROOT)$(sbindir)
	$(install) -m 755 ../scripts/pppoe-stop $(RPM_INSTALL_ROOT)$(sbindir)
	$(install) -m 755 ../scripts/pppoe-setup $(RPM_INSTALL_ROOT)$(sbindir)
	-mkdir -p $(RPM_INSTALL_ROOT)$(docdir)
	$(install) -m 644 ../doc/CHANGES $(RPM_INSTALL_ROOT)$(docdir)
	$(install) -m 644 ../doc/KERNEL-MODE-PPPOE $(RPM_INSTALL_ROOT)$(docdir)
	$(install) -m 644 ../doc/HOW-TO-CONNECT $(RPM_INSTALL_ROOT)$(docdir)
	$(install) -m 644 ../doc/LICENSE $(RPM_INSTALL_ROOT)$(docdir)
	$(install) -m 644 ../README $(RPM_INSTALL_ROOT)$(docdir)
	$(install) -m 644 ../SERVPOET $(RPM_INSTALL_ROOT)$(docdir)
	$(install) -m 644 ../configs/pap-secrets $(RPM_INSTALL_ROOT)$(docdir)
	-mkdir -p $(RPM_INSTALL_ROOT)$(mandir)/man8
	for i in $(TARGETS) ; do \
		if test -f ../man/$$i.8 ; then \
			$(install) -m 644 ../man/$$i.8 $(RPM_INSTALL_ROOT)$(mandir)/man8 || exit 1; \
		fi; \
	done
	$(install) -m 644 ../man/pppoe-start.8 $(RPM_INSTALL_ROOT)$(mandir)/man8
	$(install) -m 644 ../man/pppoe-stop.8 $(RPM_INSTALL_ROOT)$(mandir)/man8
	$(install) -m 644 ../man/pppoe-status.8 $(RPM_INSTALL_ROOT)$(mandir)/man8
	$(install) -m 644 ../man/pppoe-connect.8 $(RPM_INSTALL_ROOT)$(mandir)/man8
	$(install) -m 644 ../man/pppoe-setup.8 $(RPM_INSTALL_ROOT)$(mandir)/man8
	-mkdir -p $(RPM_INSTALL_ROOT)$(mandir)/man5
	$(install) -m 644 ../man/pppoe.conf.5 $(RPM_INSTALL_ROOT)$(mandir)/man5
	-mkdir -p $(RPM_INSTALL_ROOT)/etc/ppp
	-mkdir -p $(RPM_INSTALL_ROOT)$(PLUGIN_DIR)
	-echo "# Directory created by rp-pppoe for kernel-mode plugin" > $(RPM_INSTALL_ROOT)$(PLUGIN_DIR)/README
	@if test -r rp-pppoe.so; then $(install) -m 755 rp-pppoe.so $(RPM_INSTALL_ROOT)$(PLUGIN_DIR); fi
	@for i in pppoe.conf firewall-standalone firewall-masq ; do \
		if [ ! -f $(RPM_INSTALL_ROOT)/etc/ppp/$$i ] ; then \
			$(install) -m 644 ../configs/$$i $(RPM_INSTALL_ROOT)/etc/ppp ; \
		else \
			echo "NOT overwriting existing $(RPM_INSTALL_ROOT)/etc/ppp/$$i" ;\
			$(install) -m 644 ../configs/$$i $(RPM_INSTALL_ROOT)/etc/ppp/$$i-$(VERSION) ;\
		fi ;\
	done
	@if [ ! -f $(RPM_INSTALL_ROOT)$(PPPOESERVER_PPPD_OPTIONS) ] ; then \
		$(install) -m 644 ../configs/pppoe-server-options $(RPM_INSTALL_ROOT)$(PPPOESERVER_PPPD_OPTIONS) ; \
	else \
		echo "NOT overwriting existing $(RPM_INSTALL_ROOT)$(PPPOESERVER_PPPD_OPTIONS)"; \
		$(install) -m 644 ../configs/pppoe-server-options $(RPM_INSTALL_ROOT)$(PPPOESERVER_PPPD_OPTIONS)-example ; \
	fi
	@if [ -f /etc/redhat-release ] ; then \
		echo "Looks like a Red Hat system; installing $(RPM_INSTALL_ROOT)/etc/rc.d/init.d/pppoe" ; \
		mkdir -p $(RPM_INSTALL_ROOT)/etc/rc.d/init.d ;\
		$(install) -m 755 ../scripts/pppoe-init $(RPM_INSTALL_ROOT)/etc/rc.d/init.d/pppoe ; \
	fi
	@if [ -f /etc/turbolinux-release ] ; then \
		echo "Looks like a TurboLinux system; installing $(RPM_INSTALL_ROOT)/etc/rc.d/init.d/pppoe" ; \
		mkdir -p $(RPM_INSTALL_ROOT)/etc/rc.d/init.d ;\
		$(install) -m 755 ../scripts/pppoe-init-turbolinux $(RPM_INSTALL_ROOT)/etc/rc.d/init.d/pppoe ; \
	fi
	@if [ -f /etc/SuSE-release ] ; then \
		echo "Looks like a SuSE Linux system; installing $(RPM_INSTALL_ROOT)/etc/rc.d/init.d/pppoe" ; \
		mkdir -p $(RPM_INSTALL_ROOT)/etc/rc.d/init.d ;\
		$(install) -m 755 ../scripts/pppoe-init-suse $(RPM_INSTALL_ROOT)/etc/rc.d/init.d/pppoe ; \
	fi

	# L2TP
	@if [ -f l2tp/handlers/sync-pppd.so ] ; then \
		mkdir -p $(RPM_INSTALL_ROOT)/usr/lib/l2tp/plugins ; \
		$(install) -m 755 l2tp/handlers/sync-pppd.so $(RPM_INSTALL_ROOT)/usr/lib/l2tp/plugins ; \
		mkdir -p $(RPM_INSTALL_ROOT)/etc/l2tp ; \
		$(install) -m 600 l2tp/l2tp.conf $(RPM_INSTALL_ROOT)/etc/l2tp/l2tp.conf.example ; \
	fi
	@echo ""
	@echo "Type 'pppoe-setup' to configure the software."

servpoet-tgz: distro-servpoet
	cd .. && tar cvf servpoet-$(VERSION)$(BETA).tar servpoet-$(VERSION)$(BETA)
	gzip -f -v -9 ../servpoet-$(VERSION)$(BETA).tar
	rm -rf ../servpoet-$(VERSION)$(BETA)

tgz: distro
	cd .. && tar cvf rp-pppoe-$(VERSION)$(BETA).tar rp-pppoe-$(VERSION)$(BETA)
	gzip -f -v -9 ../rp-pppoe-$(VERSION)$(BETA).tar
	rm -rf ../rp-pppoe-$(VERSION)$(BETA)

distro-servpoet: distro
	cp ../servpoet.spec ../rp-pppoe-$(VERSION)$(BETA)
	$(MAKE) -C licensed-only distro VERSION=$(VERSION) BETA=$(BETA)
	mv ../rp-pppoe-$(VERSION)$(BETA) ../servpoet-$(VERSION)$(BETA)

distro:
	rm -rf ../rp-pppoe-$(VERSION)$(BETA)
	mkdir ../rp-pppoe-$(VERSION)$(BETA)
	for i in README SERVPOET go go-gui rp-pppoe.spec ; do \
		cp ../$$i ../rp-pppoe-$(VERSION)$(BETA) || exit 1; \
	done
	mkdir ../rp-pppoe-$(VERSION)$(BETA)/gui
	for i in en.msg ja.msg Makefile.in tkpppoe.in wrapper.c tkpppoe.1 pppoe-wrapper.1 ; do \
		cp ../gui/$$i ../rp-pppoe-$(VERSION)$(BETA)/gui || exit 1; \
	done
	mkdir ../rp-pppoe-$(VERSION)$(BETA)/gui/html
	for i in mainwin-busy.png mainwin-nonroot.png mainwin.png props-advanced.png props-basic.png props-nic.png props-options.png tkpppoe.html ; do \
		cp ../gui/html/$$i ../rp-pppoe-$(VERSION)$(BETA)/gui/html || exit 1; \
	done
	mkdir ../rp-pppoe-$(VERSION)$(BETA)/configs
	for i in firewall-masq firewall-standalone pap-secrets pppoe-server-options pppoe.conf ; do \
		cp ../configs/$$i ../rp-pppoe-$(VERSION)$(BETA)/configs || exit 1; \
	done
	mkdir ../rp-pppoe-$(VERSION)$(BETA)/doc
	for i in CHANGES KERNEL-MODE-PPPOE HOW-TO-CONNECT LICENSE PROBLEMS ; do \
		cp ../doc/$$i ../rp-pppoe-$(VERSION)$(BETA)/doc || exit 1; \
	done
	mkdir ../rp-pppoe-$(VERSION)$(BETA)/man
	for i in pppoe-connect.8 pppoe-setup.8 pppoe-start.8 pppoe-status.8 pppoe-stop.8 pppoe-server.8 pppoe-sniff.8 pppoe.8 pppoe-relay.8 pppoe.conf.5 ; do \
		cp ../man/$$i ../rp-pppoe-$(VERSION)$(BETA)/man || exit 1; \
	done
	mkdir ../rp-pppoe-$(VERSION)$(BETA)/scripts
	for i in pppoe-connect.in pppoe-init-suse.in pppoe-init-turbolinux.in pppoe-init.in pppoe-setup.in pppoe-start.in pppoe-stop.in pppoe-status ; do \
		cp ../scripts/$$i ../rp-pppoe-$(VERSION)$(BETA)/scripts || exit 1; \
	done
	mkdir ../rp-pppoe-$(VERSION)$(BETA)/src
	for i in Makefile.in install-sh common.c config.h.in configure configure.in debug.c discovery.c if.c md5.c md5.h ppp.c pppoe-server.c pppoe-sniff.c pppoe.c pppoe.h pppoe-server.h plugin.c relay.c relay.h ; do \
		cp ../src/$$i ../rp-pppoe-$(VERSION)$(BETA)/src || exit 1; \
	done
	mkdir ../rp-pppoe-$(VERSION)$(BETA)/src/libevent
	for i in Makefile.in event.c event.h event_tcp.c event_tcp.h eventpriv.h hash.c hash.h event_sig.c ; do \
		cp ../src/libevent/$$i ../rp-pppoe-$(VERSION)$(BETA)/src/libevent || exit 1; \
	done
	mkdir ../rp-pppoe-$(VERSION)$(BETA)/src/plugin

distro-beta: beta-check
	$(MAKE) distro BETA=-BETA-$(BETA)

beta-check:
	@if test "$(BETA)" = "" ; then \
		echo "Usage: make distro-beta BETA=<x>"; \
		exit 1; \
	fi

rpms: tgz
	cp ../rp-pppoe-$(VERSION).tar.gz /usr/src/redhat/SOURCES
	gpg --detach-sign /usr/src/redhat/SOURCES/rp-pppoe-$(VERSION).tar.gz
	cd ..; \
	rpm -ba rp-pppoe.spec
	rpm --addsign /usr/src/redhat/SRPMS/rp-pppoe-$(VERSION)-1.src.rpm \
		/usr/src/redhat/RPMS/i386/rp-pppoe-$(VERSION)-1.i386.rpm \
		/usr/src/redhat/RPMS/i386/rp-pppoe-gui-$(VERSION)-1.i386.rpm

servpoet-rpms: servpoet-tgz
	cp ../servpoet-$(VERSION).tar.gz /usr/src/redhat/SOURCES
	cd .. && rpm -ba servpoet.spec

clean:
	rm -f *.o pppoe pppoe-sniff pppoe-server core rp-pppoe.so plugin/*.o plugin/libplugin.a *~
	test -f licensed-only/Makefile && $(MAKE) -C licensed-only clean || true
	test -f libevent/Makefile && $(MAKE) -C libevent clean || true
	test -f l2tp/Makefile && $(MAKE) -C l2tp clean || true

distclean: clean
	rm -f Makefile config.h config.cache config.log config.status
	rm -f libevent/Makefile
	rm -f ../scripts/pppoe-connect ../scripts/pppoe-start ../scripts/pppoe-stop ../scripts/pppoe-init ../scripts/pppoe-setup ../scripts/pppoe-init-suse ../scripts/pppoe-init-turbolinux

update-version:
	sed -e 's/^Version: .*$$/Version: $(VERSION)/' ../rp-pppoe.spec > ../rp-pppoe.spec.new && mv ../rp-pppoe.spec.new ../rp-pppoe.spec
	sed -e 's+^Source: .*$$+Source: http://www.roaringpenguin.com/pppoe/rp-pppoe-$(VERSION).tar.gz+' ../rp-pppoe.spec > ../rp-pppoe.spec.new && mv ../rp-pppoe.spec.new ../rp-pppoe.spec
	test -f ../servpoet.spec && sed -e 's/^Version: .*$$/Version: $(VERSION)/' ../servpoet.spec > ../servpoet.spec.new && mv ../servpoet.spec.new ../servpoet.spec || true
	test -f ../servpoet.spec && sed -e 's+^Source: .*$$+Source: http://www.roaringpenguin.com/pppoe/servpoet-$(VERSION).tar.gz+' ../servpoet.spec > ../servpoet.spec.new && mv ../servpoet.spec.new ../servpoet.spec || true

# Convenience target for David!  Don't try to use this one.
km:
	./configure --enable-plugin=/home/dfs/Archive/PPP/ppp-2.4.0.pppoe4-patched-dfs

licensed-only:
	$(MAKE) -C licensed-only all VERSION=$(VERSION) SERVPOET_VERSION=$(SERVPOET_VERSION)

l2tp: libevent/libevent.a
	$(MAKE) -C l2tp all

.PHONY: update-version

.PHONY: clean

.PHONY: distclean

.PHONY: rpms

.PHONY: licensed-only

.PHONY: distro

.PHONY: l2tp
