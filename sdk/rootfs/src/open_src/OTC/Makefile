DESTDIR=
SBINDIR=/usr/sbin
CONFDIR=/etc/iproute2
DOCDIR=/usr/share/doc/iproute2
MANDIR=/usr/share/man

# Path to db_185.h include
DBM_INCLUDE:=/usr/include

DEFINES= -DRESOLVE_HOSTNAMES

#options if you have a bind>=4.9.4 libresolv (or, maybe, glibc)
LDLIBS=-lresolv
ADDLIB=

#options for decnet
ADDLIB+=dnet_ntop.o dnet_pton.o

#options for ipx
ADDLIB+=ipx_ntop.o ipx_pton.o

CC = $(TARGET)-gcc
STRIP = $(TARGET)-strip
HOSTCC = $(TARGET)-gcc
CCOPTS = -D_GNU_SOURCE -O2 -Wstrict-prototypes -Wall
CFLAGS = $(CCOPTS) -I../include $(DEFINES)

LDLIBS += -L../lib -lnetlink -lutil

SUBDIRS=lib ip tc #misc #netem

LIBNETLINK=../lib/libnetlink.a ../lib/libutil.a

all: precheck Config
	@for i in $(SUBDIRS); \
	do $(MAKE) $(MFLAGS) -C $$i; done
	$(STRIP) -s -R .note -R .comment tc/tc

Config:
	./configure $(KERNEL_INCLUDE)

tc-install: postcheck
	@cp -a tc/tc                  	$(INSTALL_ROOT)/usr/sbin
ifeq (${HANDLE_IPV6}, 1)
	@cp -a ip/ip			$(INSTALL_ROOT)/usr/sbin
endif
ifeq (${HANDLE_MULTI_WAN}, 1)
	@cp -a ip/ip			$(INSTALL_ROOT)/usr/sbin
	@cp -a etc/iproute2/rt_protos  	$(INSTALL_ROOT)/usr/mv2ram/etc/iproute2
	@cp -a etc/iproute2/rt_scopes  	$(INSTALL_ROOT)/usr/mv2ram/etc/iproute2
endif
ifeq (${HANDLE_IPSEC}, 1)
	@cp -a ip/ip			$(INSTALL_ROOT)/usr/sbin
endif
ifeq (${HANDLE_LOADSHARING}, 1)
	@cp -a ip/ip                  	$(INSTALL_ROOT)/usr/sbin
	@cp -a etc/iproute2/rt_protos  	$(INSTALL_ROOT)/usr/mv2ram/etc/iproute2
	@cp -a etc/iproute2/rt_scopes  	$(INSTALL_ROOT)/usr/mv2ram/etc/iproute2
	@cp -a etc/iproute2/rt_tables   $(INSTALL_ROOT)/usr/mv2ram/etc/iproute2
endif
ifeq (${HANDLE_INET_PPPOE}, 1)
	cp -a ip/ip					$(INSTALL_ROOT)/usr/sbin
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


install: all
	install -m 0755 -d $(DESTDIR)$(SBINDIR)
	install -m 0755 -d $(DESTDIR)$(CONFDIR)
	install -m 0755 -d $(DESTDIR)$(DOCDIR)/examples
	install -m 0755 -d $(DESTDIR)$(DOCDIR)/examples/diffserv
	install -m 0644 README.iproute2+tc $(shell find examples -maxdepth 1 -type f) \
		$(DESTDIR)$(DOCDIR)/examples
	install -m 0644 $(shell find examples/diffserv -maxdepth 1 -type f) \
		$(DESTDIR)$(DOCDIR)/examples/diffserv
	@for i in $(SUBDIRS) doc; do $(MAKE) -C $$i install; done
	install -m 0644 $(shell find etc/iproute2 -maxdepth 1 -type f) $(DESTDIR)$(CONFDIR)
	install -m 0755 -d $(DESTDIR)$(MANDIR)/man8
	install -m 0644 $(shell find man/man8 -maxdepth 1 -type f) $(DESTDIR)$(MANDIR)/man8
	ln -sf $(MANDIR)/man8/tc-pbfifo.8  $(DESTDIR)$(MANDIR)/man8/tc-bfifo.8
	ln -sf $(MANDIR)/man8/tc-pbfifo.8  $(DESTDIR)$(MANDIR)/man8/tc-pfifo.8
	install -m 0755 -d $(DESTDIR)$(MANDIR)/man3
	install -m 0644 $(shell find man/man3 -maxdepth 1 -type f) $(DESTDIR)$(MANDIR)/man3

clean:
	@for i in $(SUBDIRS) doc; \
	do $(MAKE) $(MFLAGS) -C $$i clean; done

clobber: clean
	rm -f Config

distclean: clean clobber

.EXPORT_ALL_VARIABLES:
