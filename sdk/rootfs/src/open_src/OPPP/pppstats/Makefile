#
# pppstats makefile
# $Id: Makefile.linux,v 1.9 2006/06/04 05:07:46 paulus Exp $
#

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


DESTDIR = $(INSTROOT)/usr/local
BINDIR = $(DESTDIR)/sbin
MANDIR = $(DESTDIR)/share/man/man8

PPPSTATSRCS = pppstats.c
PPPSTATOBJS = pppstats.o
CC=$(CROSS_COMPILE)gcc
#CC = gcc
COPTS = -O
COMPILE_FLAGS = -I../include
LIBS =

INSTALL= install

CFLAGS = $(COPTS) $(COMPILE_FLAGS)

all: pppstats

install: pppstats
	-mkdir -p $(MANDIR)
	$(INSTALL) -s -c pppstats $(BINDIR)
	$(INSTALL) -c -m 444 pppstats.8 $(MANDIR)

pppstats: $(PPPSTATSRCS)
	$(CC) $(CFLAGS) -o pppstats pppstats.c $(LIBS)

clean:
	rm -f pppstats *~ #* core

depend:
	cpp -M $(CFLAGS) $(PPPSTATSRCS) >.depend
#	makedepend $(CFLAGS) $(PPPSTATSRCS)
