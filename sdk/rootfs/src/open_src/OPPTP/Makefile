# $Id: Makefile,v 1.40 2006/02/13 03:06:25 quozl Exp $
VERSION=1.7.1
RELEASE=

#################################################################
# CHANGE THIS LINE to point to the location of your pppd binary.
PPPD = /usr/sbin/pppd
#################################################################

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


BINDIR=$(DESTDIR)/usr/sbin
MANDIR=$(DESTDIR)/usr/share/man/man8
PPPDIR=$(DESTDIR)/etc/ppp

ifeq (${USE_SCRIPTS_V1},1)
USE_SCRIPTS_V2=
else
USE_SCRIPTS_V2=1
endif

CC	= $(CROSS_COMPILE)gcc
STRIP = $(CROSS_COMPILE)strip

RM	= rm -f
OPTIMIZE= -O0
DEBUG	= -g
INCLUDE =
CFLAGS  = -Wall $(OPTIMIZE) $(DEBUG) $(INCLUDE)
LIBS	= -lutil
LDFLAGS	=

PPTP_BIN = pptp

PPTP_OBJS = pptp.o pptp_gre.o ppp_fcs.o \
            pptp_ctrl.o dirutil.o vector.o \
            inststr.o util.o version.o \
	    pptp_quirks.o orckit_quirks.o pqueue.o pptp_callmgr.o

PPTP_DEPS = pptp_callmgr.h pptp_gre.h ppp_fcs.h util.h \
	    pptp_quirks.h orckit_quirks.h config.h pqueue.h


all: precheck compile mkuyg 

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

mkuyg:
ifdef FULL_COPY
	${PRIV_TOOLS}/bin/mkuo options.pptp options.pptp.uo $(ENDIAN)
	${PRIV_TOOLS}/bin/mkuo pptp.conf pptp.conf.uo $(ENDIAN)
endif

compile: config.h pptp-ondemand get_csid_addr port-scan getHostIP $(PPTP_BIN) 
	@chmod +x scripts/pptp-action   
	@chmod +x scripts/pptp-start
	@chmod +x scripts/pptp-connect
	@chmod +x scripts/pptp-connect-nosync
	@chmod +x scripts/pptp-stop
	@chmod +x scripts/pptp-ip-up   
	@chmod +x scripts/pptpc-header  
	@chmod +x scripts/pptpc-action  
	@chmod +x scripts/pptpc-start  
	@chmod +x scripts/pptpc-ondemand  
	@chmod +x scripts/pptpc-stop
	@rm -f scripts/pptpc-pppopt
	@chmod +x scripts/pptpc-customer
	@chmod +x scripts-v2/pptp-action 
	@chmod +x scripts-v2/pptpc-customer 
	@chmod +x scripts-v2/pptpc-header 
	@chmod +x scripts-v2/pptpc-action 
	@chmod +x scripts-v2/pptpc-start
	@chmod +x scripts-v2/pptpc-stop
	@chmod +x scripts-v2/pptpc-ondemand

install: postcheck
ifdef FULL_COPY
	@cp -a options.pptp.uo			$(INSTALL_ROOT)/usr/uo
	@cp -a pptp.conf.uo				$(INSTALL_ROOT)/usr/uo
endif
	@cp -a pptp						$(INSTALL_ROOT)/usr/sbin
	@cp -a get_csid_addr			$(INSTALL_ROOT)/usr/bin
	@cp -a getHostIP				$(INSTALL_ROOT)/usr/bin
	@cp -a pptp-ondemand			$(INSTALL_ROOT)/usr/bin
	#@cp -a port-scan				$(INSTALL_ROOT)/usr/bin
	@cp -a scripts/pptp-start		$(INSTALL_ROOT)/usr/bin
	@cp -a scripts/pptp-stop		$(INSTALL_ROOT)/usr/bin
	@cp -a scripts/pptp-action		$(INSTALL_ROOT)/usr/bin
ifeq ($(TC3262),1)
	@cp scripts/pptp-connect-nosync		$(INSTALL_ROOT)/usr/bin/pptp-connect
else
	@cp -a scripts/pptp-connect		$(INSTALL_ROOT)/usr/bin
endif
	@cp -a scripts/pptp-ip-up		$(INSTALL_ROOT)/usr/bin
	@cp -a scripts/pptpc-header		$(INSTALL_ROOT)/usr/bin
	@cp -a scripts/pptpc-start		$(INSTALL_ROOT)/usr/bin
	@cp -a scripts/pptpc-stop		$(INSTALL_ROOT)/usr/bin
	@cp -a scripts/pptpc-action		$(INSTALL_ROOT)/usr/bin
	@cp -a scripts/pptpc-ondemand	$(INSTALL_ROOT)/usr/bin
	@cp -a scripts/pptpc-customer	$(INSTALL_ROOT)/usr/bin 	
ifeq (${USE_SCRIPTS_V2},1)
	@cp -a scripts-v2/pptp-action		$(INSTALL_ROOT)/usr/bin
	@cp -a scripts-v2/pptpc-header	$(INSTALL_ROOT)/usr/bin
	@cp -a scripts-v2/pptpc-customer	$(INSTALL_ROOT)/usr/bin
	@cp -a scripts-v2/pptpc-action	$(INSTALL_ROOT)/usr/bin
	@cp -a scripts-v2/pptpc-start	$(INSTALL_ROOT)/usr/bin
	@cp -a scripts-v2/pptpc-stop	$(INSTALL_ROOT)/usr/bin
	@cp -a scripts-v2/pptpc-ondemand	$(INSTALL_ROOT)/usr/bin
endif

uninstall: postcheck
	$(RM) $(BINDIR)/pptp $(MANDIR)/pptp.8
	@rm -f $(INSTALL_ROOT)/usr/sbin/pptp
	@rm -f $(INSTALL_ROOT)/usr/bin/get_csid_addr
	@rm -f $(INSTALL_ROOT)/usr/bin/getHostIP
	@rm -f $(INSTALL_ROOT)/usr/bin/pptp-ondemand
	@rm -f $(INSTALL_ROOT)/usr/bin/port-scan
	@rm -f $(INSTALL_ROOT)/usr/uo/options.pptp.uo
	@rm -f $(INSTALL_ROOT)/usr/uo/pptp.conf.uo	  
	@rm -f $(INSTALL_ROOT)/usr/bin/pptp-start	 
	@rm -f $(INSTALL_ROOT)/usr/bin/pptp-stop	 
	@rm -f $(INSTALL_ROOT)/usr/bin/pptp-action	 
	@rm -f $(INSTALL_ROOT)/usr/bin/pptp-connect	 
	@rm -f $(INSTALL_ROOT)/usr/bin/pptp-ip-up	 
	@rm -f $(INSTALL_ROOT)/usr/bin/pptpc-header	 
	@rm -f $(INSTALL_ROOT)/usr/bin/pptpc-start	 
	@rm -f $(INSTALL_ROOT)/usr/bin/pptpc-stop	 
	@rm -f $(INSTALL_ROOT)/usr/bin/pptpc-action	 
	@rm -f $(INSTALL_ROOT)/usr/bin/pptpc-ondemand
	@rm -f $(INSTALL_ROOT)/usr/bin/pptpc-customer   

$(PPTP_BIN): $(PPTP_OBJS) $(PPTP_DEPS)
	$(CC) -o $(PPTP_BIN) $(PPTP_OBJS) $(LDFLAGS) $(LIBS)
	$(STRIP) $@

config.h: 
	echo "/* text added by Makefile target config.h */" > config.h
	echo "#define PPTP_LINUX_VERSION \"$(VERSION)$(RELEASE)\"" >> config.h
	echo "#define PPPD_BINARY \"$(PPPD)\"" >> config.h

vector_test: vector_test.o vector.o
	$(CC) -o vector_test vector_test.o vector.o

pptp-ondemand: 
	$(CC) -o pptp-ondemand demand.c
	$(STRIP) pptp-ondemand

get_csid_addr: 
	$(CC) -o get_csid_addr get_csid_addr.c
	$(STRIP) get_csid_addr

port-scan: 
	$(CC) -o port-scan port-scan.c
	$(STRIP) port-scan

getHostIP: 
	$(CC) -o getHostIP getHostIP.c
	$(STRIP) getHostIP

clean:
	@-rm -f $(COMPO_DIR)/*.uo
	@$(RM) ${PPTP_BIN} *.o config.h pptp-ondemand get_csid_addr port-scan

clobber: clean
	$(RM) $(PPTP_BIN) vector_test

distclean: clobber

test: vector_test

old_install:
	mkdir -p $(BINDIR)
	install -o root -m 555 pptp $(BINDIR)
	mkdir -p $(MANDIR)
	install -m 644 pptp.8 $(MANDIR)
	mkdir -p $(PPPDIR)
	install -m 644 options.pptp $(PPPDIR)

dist: clobber
	$(RM) pptp-$(VERSION)$(RELEASE).tar.gz
	$(RM) -r pptp-$(VERSION)
	mkdir pptp-$(VERSION)
	cp --recursive ChangeLog Makefile *.c *.h options.pptp pptp.8 \
		Documentation Reference AUTHORS COPYING INSTALL NEWS \
		README DEVELOPERS TODO USING PROTOCOL-SECURITY \
		pptp-$(VERSION)/
	$(RM) -r pptp-$(VERSION)/CVS pptp-$(VERSION)/*/CVS
	tar czf pptp-$(VERSION)$(RELEASE).tar.gz pptp-$(VERSION)
	$(RM) -r pptp-$(VERSION)
	md5sum pptp-$(VERSION)$(RELEASE).tar.gz

deb:
	chmod +x debian/rules 
	fakeroot dpkg-buildpackage -us -uc
	mv ../pptp_$(VERSION)-0_i386.deb .

WEB=~/public_html/external/mine/pptp/pptpconfig
release:
	cp pptp_$(VERSION)-0_i386.deb $(WEB)
	cd $(WEB);make
