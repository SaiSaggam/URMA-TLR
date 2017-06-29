# Generated automatically from Makefile.in by configure.
# $Id: Makefile.in,v 1.2 2002/09/30 19:45:00 dskoll Exp $
# Makefile for LNS handlers
# LIC: GPL

prefix=/usr/local
exec_prefix=${prefix}
mandir=${prefix}/man
docdir=/usr/local/doc/rp-pppoe-$(VERSION)
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


CC = $(CROSS_COMPILE)gcc
STRIP=$(CROSS_COMPILE)strip


HANDLERS=sync-pppd.so cmd.so

OBJS=pty.o sync-pppd.o dstring.o
SRCS=$(OBJS:.o=.c)
CFLAGS=-g -O2 -I.. -I../libevent -I../../libevent -Wall

ifeq ($(KERNEL_MODE), 1)
   CFLAGS += -DUSE_KERNEL -I../../../../../linux/src/include
endif

all: $(HANDLERS) l2tp-control

l2tp-control: cmd-control.o
	$(CC) -o l2tp-control cmd-control.o
	${STRIP} $@
	
cmd-control.o: cmd-control.c
	$(CC) -c -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -fPIC -c -o $@ $<

sync-pppd.so: pty.o sync-pppd.o
	$(CC) -shared -o $@ $^
	$(STRIP) $@

cmd.so: cmd.o dstring.o
	$(CC) -shared -o $@ $^
	$(STRIP) $@

clean:
	rm -f *.so *.o *~
	rm -f l2tp-control

install: all
	-mkdir -p $(RPM_INSTALL_ROOT)$(sbindir)
	-mkdir -p $(RPM_INSTALL_ROOT)$(prefix)/lib/l2tp/plugins
	$(install) -m 755 -s l2tp-control $(RPM_INSTALL_ROOT)$(sbindir)
	$(install) -m 755 $(HANDLERS) $(RPM_INSTALL_ROOT)$(prefix)/lib/l2tp/plugins
distclean: clean
