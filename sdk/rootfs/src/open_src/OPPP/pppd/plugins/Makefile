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


#CC	= gcc
CC=$(CROSS_COMPILE)gcc
COPTS	= -O2 
CFLAGS	= $(COPTS) -I.. -I../.. -I../../include -fPIC
LDFLAGS	= -shared
INSTALL	= install

DESTDIR = $(INSTROOT)/usr/local
BINDIR = $(DESTDIR)/sbin
MANDIR = $(DESTDIR)/share/man/man8
LIBDIR = $(DESTDIR)/lib/pppd/$(VERSION)

SUBDIRS := rp-pppoe pppoatm  

ifdef HANDLE_ACCEL_PPTP 
SUBDIRS += pppopptp 
endif

ifdef HANDLE_OPEN_L2TP 
SUBDIRS += pppol2tp
endif

# Uncomment the next line to include the radius authentication plugin
SUBDIRS += radius
PLUGINS := minconn.so passprompt.so passwordfd.so winbind.so

# include dependencies if present
ifeq (.depend,$(wildcard .depend))
include .depend
endif

export CC LDFLAGS CFLAGS LIBDIR

all:	$(PLUGINS)
	for d in $(SUBDIRS); do $(MAKE) $(MFLAGS) -C $$d all; done

%.so: %.c
	$(CC) -o $@ $(LDFLAGS) $(CFLAGS) $^

VERSION = $(shell awk -F '"' '/VERSION/ { print $$2; }' ../patchlevel.h)

install: $(PLUGINS)
	$(INSTALL) -d $(LIBDIR)
	$(INSTALL) $? $(LIBDIR)
	for d in $(SUBDIRS); do $(MAKE) $(MFLAGS) -C $$d install; done

clean:
	rm -f *.o *.so *.a
	for d in $(SUBDIRS); do $(MAKE) $(MFLAGS) -C $$d clean; done

depend:
	$(CPP) -M $(CFLAGS) *.c >.depend
	for d in $(SUBDIRS); do $(MAKE) $(MFLAGS) -C $$d depend; done
