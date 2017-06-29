# Generated automatically from Makefile.in by configure.
# $Id: Makefile.in,v 1.4 2004/08/11 14:21:40 dfs Exp $
#
# Makefile for event-handling library
#
# Copyright 2002 Roaring Penguin Software Inc.
#
# This software may be distributed according to the terms of the GNU
# General Public License, version 2 or (at your option) any later version.
# LIC: GPL


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
RANLIB = $(CROSS_COMPILE)ranlib

OBJS=event.o event_tcp.o hash.o event_sig.o
SRCS=$(OBJS:.o=.c)
HDRS=event.h event_tcp.h eventpriv.h hash.h
CFLAGS= -O2 -Wall -Wstrict-prototypes -ansi -I..


all: libevent.a

libevent.a: $(OBJS)
	rm -f libevent.a
	$(AR) -cq libevent.a $(OBJS)
	$(RANLIB) libevent.a

event.o: event.c $(HDRS)
	$(CC) $(CFLAGS) -c -o event.o event.c

hash.o: hash.c $(HDRS)
	$(CC) $(CFLAGS) -c -o hash.o hash.c

event_sig.o: event_sig.c $(HDRS)
	$(CC) $(CFLAGS) -c -o event_sig.o event_sig.c

event_tcp.o: event_tcp.c $(HDRS)
	$(CC) $(CFLAGS) -c -o event_tcp.o event_tcp.c

clean: FORCE
	rm -f *.a *.o *~

FORCE:

.phony: FORCE
