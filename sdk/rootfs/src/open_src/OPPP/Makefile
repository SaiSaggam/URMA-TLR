# PPP top-level Makefile for Linux.

DESTDIR = $(INSTROOT)/usr/local
BINDIR = $(DESTDIR)/sbin
INCDIR = $(DESTDIR)/include
MANDIR = $(DESTDIR)/share/man
ETCDIR = $(INSTROOT)/etc/ppp

# uid 0 = root
INSTALL= install

all: precheck
	cd chat; $(MAKE) $(MFLAGS) all
	cd pppd/plugins; $(MAKE) $(MFLAGS) all
	cd pppd; $(MAKE) $(MFLAGS) all
	cd pppstats; $(MAKE) $(MFLAGS) all
	cd pppdump; $(MAKE) $(MFLAGS) all

install: $(BINDIR) $(MANDIR)/man8 install-progs install-devel

install-progs:
	cd chat; $(MAKE) $(MFLAGS) install
	cd pppd/plugins; $(MAKE) $(MFLAGS) install
	cd pppd; $(MAKE) $(MFLAGS) install
	cd pppstats; $(MAKE) $(MFLAGS) install
	cd pppdump; $(MAKE) $(MFLAGS) install

install-etcppp: $(ETCDIR) $(ETCDIR)/options $(ETCDIR)/pap-secrets \
	$(ETCDIR)/chap-secrets

install-devel:
	cd pppd; $(MAKE) $(MFLAGS) install-devel

$(ETCDIR)/options:
	$(INSTALL) -c -m 644 etc.ppp/options $@
$(ETCDIR)/pap-secrets:
	$(INSTALL) -c -m 600 etc.ppp/pap-secrets $@
$(ETCDIR)/chap-secrets:
	$(INSTALL) -c -m 600 etc.ppp/chap-secrets $@

$(BINDIR):
	$(INSTALL) -d -m 755 $@
$(MANDIR)/man8:
	$(INSTALL) -d -m 755 $@
$(ETCDIR):
	$(INSTALL) -d -m 755 $@

clean:
	rm -f `find . -name '*.[oas]' -print`
	rm -f `find . -name 'core' -print`
	rm -f `find . -name '*~' -print`
	cd chat; $(MAKE) clean
	cd pppd/plugins; $(MAKE) clean
	cd pppd; $(MAKE) clean
	cd pppstats; $(MAKE) clean
	cd pppdump; $(MAKE) clean

dist-clean:	clean
	rm -f Makefile `find . -name Makefile -print`

#kernel:
#	cd linux; ./kinstall.sh

# no tests yet, one day...
installcheck:
	true

ppp-install: postcheck
	@cp -a pppd/pppd					$(INSTALL_ROOT)/usr/sbin
	@mkdir -p $(INSTALL_ROOT)/usr/mv2ram/etc/ppp
	@cp -a etc.ppp/options				$(INSTALL_ROOT)/usr/mv2ram/etc/ppp
	@cp -a scripts/ip-up				$(INSTALL_ROOT)/usr/mv2ram/etc/ppp
	@cp -a scripts/ip-down				$(INSTALL_ROOT)/usr/mv2ram/etc/ppp
ifeq (${TC3262}, 1)
	@mkdir -p $(INSTALL_ROOT)/usr/mv2ram/etc/ppp/plugins
	@cp -a pppd/plugins/pppoatm/pppoatm.so $(INSTALL_ROOT)/usr/mv2ram/etc/ppp/plugins
endif
	#@cp -a pppd/plugins/rp-pppoe/rp-pppoe.so $(INSTALL_ROOT)/usr/mv2ram/etc/ppp/plugins
	@cp -a scripts/ipv6-up				$(INSTALL_ROOT)/usr/mv2ram/etc/ppp
	@cp -a scripts/ipv6-down			$(INSTALL_ROOT)/usr/mv2ram/etc/ppp
	#@cp -a scripts/ipv6-up.check		$(INSTALL_ROOT)/usr/mv2ram/etc/ppp
ifdef FULL_COPY
	@cp -a etc.ppp/pap-secrets.uo		$(INSTALL_ROOT)/usr/uo
	@cp -a etc.ppp/chap-secrets.uo		$(INSTALL_ROOT)/usr/uo
endif
ifeq (${HANDLE_HALF_BRIDGE}, 1)
	@cp -a scripts/ppp-hb				$(INSTALL_ROOT)/usr/bin
endif
ifeq (${HANDLE_ACCEL_PPTP}, 1)
	@cp -a scripts/accel-pptp					$(INSTALL_ROOT)/usr/bin
	@cp -a pppd/plugins/pppopptp/pptp.so 		$(INSTALL_ROOT)/lib
endif
ifeq (${HANDLE_OPEN_L2TP}, 1)
	@cp -a pppd/plugins/pppol2tp/openl2tp.so 	$(INSTALL_ROOT)/lib
	@cp -a pppd/plugins/pppol2tp/pppol2tp.so 	$(INSTALL_ROOT)/lib
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
