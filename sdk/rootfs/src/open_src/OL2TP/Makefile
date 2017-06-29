# Generated automatically from Makefile.in by configure.
# Generated automatically from Makefile.in by configure.
#***********************************************************************
#
# Makefile
#
# Makefile for L2TP code.
#
# Copyright (C) 2002 Roaring Penguin Software Inc.
#
# This software may be distributed under the terms of the GNU General
# Public License, Version 2, or (at your option) any later version.
#
# LIC: GPL
#
# $Id: Makefile.in,v 1.4 2004/05/16 04:12:48 dskoll Exp $
#***********************************************************************

VERSION=0.4

ifeq (${USE_SCRIPTS_V1},1)
USE_SCRIPTS_V2=
else
USE_SCRIPTS_V2=1
endif

DEFINES=
prefix=/usr/local
exec_prefix=${prefix}
mandir=${prefix}/man
docdir=/usr/local/doc/l2tp-$(VERSION)
install=/usr/bin/install -c
install_dir=/usr/bin/install -c -d
sbindir=${exec_prefix}/sbin

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

AR = $(CROSS_COMPILE)ar
CC = $(CROSS_COMPILE)gcc
RANLIB = $(CROSS_COMPILE)ranlib
STRIP=$(CROSS_COMPILE)strip


OBJS=auth.o debug.o dgram.o main.o md5.o network.o options.o peer.o session.o tunnel.o utils.o
EXTRA_OBJS=demand.o
EXTRA_TOOLS=get_csid_addr.o 
EXTRA_TOOLS_1=port-scan.o
EXTRA_TOOLS_2=getHostIP.o 
EXTRA_LIBS=libevent/*.o -ldl

SRCS=$(OBJS:.o=.c)
CFLAGS=-g -I.. -Ilibevent -Wall -Wstrict-prototypes -pedantic -D_GNU_SOURCE -DVERSION=\"$(VERSION)\" -DPREFIX=\"$(prefix)\"

ifeq ($(KERNEL_MODE), 1)
   CFLAGS += -DUSE_KERNEL -I../../../../../linux/src/include
endif

TARGETS=l2tpd libl2tp.a handlers l2tp-ondemand get_csid_addr port-scan getHostIP

all: precheck compile mkuyg

precheck:
ifeq ($(PROJECT_CUSTOMER), ) 
	@echo "Error:"
	@echo "You need specify PROJECT_CUSTOMER for Makefile"
	@exit 1
endif 
	@echo "Build OL2TP for $(PROJECT_NAME)"

postcheck:
ifeq ($(INSTALL_ROOT), ) 
	@echo "Error:"
	@echo "You need specify INSTALL_ROOT for Makefile"
	@exit 1
endif   

mkuyg:
ifdef FULL_COPY
	${PRIV_TOOLS}/bin/mkuo l2tp.conf l2tp.conf.uo $(ENDIAN)
endif

compile: $(TARGETS)
	@chmod +x scripts/l2tp-action
	@chmod +x scripts/l2tp-start
	@chmod +x scripts/l2tp-stop
	@chmod +x scripts/l2tpc-header
	@chmod +x scripts/l2tpc-action
	@chmod +x scripts/l2tpc-start
	@chmod +x scripts/l2tpc-stop
	@chmod +x scripts/l2tpc-ondemand
	@chmod +x scripts/l2tp.sh
	@chmod +x scripts-v2/l2tp-action
	@chmod +x scripts-v2/l2tpc-header
	@chmod +x scripts-v2/l2tpc-customer
	@chmod +x scripts-v2/l2tpc-action
	@chmod +x scripts-v2/l2tpc-start
	@chmod +x scripts-v2/l2tpc-stop
	@chmod +x scripts-v2/l2tpc-ondemand

install: postcheck
	@cp -af l2tpd						$(INSTALL_ROOT)/usr/sbin
	@cp -af l2tp-ondemand				$(INSTALL_ROOT)/usr/sbin
	@cp -af handlers/l2tp-control 		$(INSTALL_ROOT)/usr/sbin
	@cp -af handlers/sync-pppd.so		$(INSTALL_ROOT)/lib
	@cp -af handlers/cmd.so				$(INSTALL_ROOT)/lib
ifdef FULL_COPY
	@cp -af l2tp.conf.uo               $(INSTALL_ROOT)/usr/uo
endif
	@cp -af scripts/l2tp.sh	      		$(INSTALL_ROOT)/usr/bin
	@cp -af scripts/l2tp-start      	$(INSTALL_ROOT)/usr/bin
	@cp -af scripts/l2tp-stop       	$(INSTALL_ROOT)/usr/bin
	@cp -af scripts/l2tp-action     	$(INSTALL_ROOT)/usr/bin
	@cp -af scripts/l2tpc-action     	$(INSTALL_ROOT)/usr/bin 
	@cp -af scripts/l2tpc-start      	$(INSTALL_ROOT)/usr/bin 
	@cp -af scripts/l2tpc-stop       	$(INSTALL_ROOT)/usr/bin 
	@cp -af scripts/l2tpc-header      	$(INSTALL_ROOT)/usr/bin 
	@cp -af scripts/l2tpc-ondemand    	$(INSTALL_ROOT)/usr/bin 
	@cp -af scripts/l2tpc-customer    	$(INSTALL_ROOT)/usr/bin 
ifeq (${USE_SCRIPTS_V2},1)
	@cp -a scripts-v2/l2tp-action		  $(INSTALL_ROOT)/usr/bin
	@cp -a scripts-v2/l2tpc-header	  $(INSTALL_ROOT)/usr/bin
	@cp -a scripts-v2/l2tpc-customer	$(INSTALL_ROOT)/usr/bin
	@cp -a scripts-v2/l2tpc-action    $(INSTALL_ROOT)/usr/bin
	@cp -a scripts-v2/l2tpc-start     $(INSTALL_ROOT)/usr/bin
	@cp -a scripts-v2/l2tpc-stop      $(INSTALL_ROOT)/usr/bin
	@cp -a scripts-v2/l2tpc-ondemand  $(INSTALL_ROOT)/usr/bin
endif
	@cp -af get_csid_addr    			$(INSTALL_ROOT)/usr/bin 
	@cp -af getHostIP    				$(INSTALL_ROOT)/usr/bin 

uninstall: postcheck
	@rm -f  $(INSTALL_ROOT)/usr/sbin/l2tpd
	@rm -f  $(INSTALL_ROOT)/usr/sbin/l2tp-ondemand
	@rm -f  $(INSTALL_ROOT)/usr/sbin/l2tp-control
	@rm -f  $(INSTALL_ROOT)/lib/sync-pppd.so
	@rm -f  $(INSTALL_ROOT)/lib/cmd.so
	@rm -f  $(INSTALL_ROOT)/usr/bin/l2tp.sh	       
	@rm -f  $(INSTALL_ROOT)/usr/bin/l2tp-start     
	@rm -f  $(INSTALL_ROOT)/usr/bin/l2tp-stop      
	@rm -f  $(INSTALL_ROOT)/usr/bin/l2tp-action    
	@rm -f  $(INSTALL_ROOT)/usr/bin/l2tpc-action    
	@rm -f  $(INSTALL_ROOT)/usr/bin/l2tpc-start     
	@rm -f  $(INSTALL_ROOT)/usr/bin/l2tpc-stop      
	@rm -f  $(INSTALL_ROOT)/usr/bin/l2tpc-header    
	@rm -f  $(INSTALL_ROOT)/usr/bin/l2tpc-ondemand  
	@rm -f  $(INSTALL_ROOT)/usr/bin/l2tpc-customer  
	@rm -f  $(INSTALL_ROOT)/usr/bin/get_csid_addr 
	@rm -f  $(INSTALL_ROOT)/usr/bin/getHostIP

libl2tp.a: $(OBJS)
	rm -f $@
	$(AR) -rc $@ $^
	$(RANLIB) $@ || true

l2tpd: libl2tp.a libevent/libevent.a
	$(MAKE) -C handlers
	$(CC) -o l2tpd -rdynamic $(OBJS) $(EXTRA_LIBS)
	${STRIP} $@	

l2tp-ondemand: $(EXTRA_OBJS)
	$(CC) -o l2tp-ondemand $(EXTRA_OBJS)
	$(STRIP) $@

port-scan: $(EXTRA_TOOLS_1)
	$(CC) -o port-scan $(EXTRA_TOOLS_1)
	$(STRIP) $@   

getHostIP: $(EXTRA_TOOLS_2)
	$(CC) -o getHostIP $(EXTRA_TOOLS_2)
	$(STRIP) $@

libevent/libevent.a:
	test -d libevent || ln -s ../libevent .
	$(MAKE) -C libevent

get_csid_addr: $(EXTRA_TOOLS)
	$(CC) -o get_csid_addr $(EXTRA_TOOLS)
	$(STRIP) $@

%.o: %.c
	$(CC) -c -o $@ $(CFLAGS) $<

depend: .depend

.depend: $(SRCS)
	$(CC) -M $(CFLAGS) $^ > .depend

handlers:
	$(MAKE) -C handlers

# Release requires GNU tar!!
release: FORCE
	rm -f rp-l2tp-$(VERSION).tar.gz
	make -C libevent/Doc
	./make-release.sh $(VERSION)
	tar cvhf rp-l2tp-$(VERSION).tar rp-l2tp-$(VERSION)
	gzip -v -9 rp-l2tp-$(VERSION).tar

clean:
	rm -f *.o *.a *~ /*.uo
	rm -f l2tpd get_csid_addr l2tp-ondemand port-scan getHostIP                
	$(MAKE) -C handlers clean
	$(MAKE) -C libevent clean

distclean: clean
	rm -f Makefile config.cache
	$(MAKE) -C handlers distclean

-include .depend

old_install: all
	-mkdir -p $(RPM_INSTALL_ROOT)$(DESTDIR)$(sbindir)
	$(install) -m 755 -s l2tpd $(RPM_INSTALL_ROOT)$(DESTDIR)$(sbindir)
	-mkdir -p $(RPM_INSTALL_ROOT)$(DESTDIR)/etc/l2tp
	$(install) -m 644 l2tp.conf $(RPM_INSTALL_ROOT)$(DESTDIR)/etc/l2tp/l2tp.conf.example
	-mkdir -p $(RPM_INSTALL_ROOT)$(DESTDIR)$(mandir)/man8
	-mkdir -p $(RPM_INSTALL_ROOT)$(DESTDIR)$(mandir)/man5
	$(install) -m 644 man/l2tpd.8 $(RPM_INSTALL_ROOT)$(DESTDIR)$(mandir)/man8
	$(install) -m 644 man/l2tp.conf.5 $(RPM_INSTALL_ROOT)$(DESTDIR)$(mandir)/man5

	$(MAKE) -C handlers install

FORCE:

.PHONY: handlers

.PHONY: FORCE
