#	$Id: Makefile.linux,v 1.15 2006/06/04 05:07:46 paulus Exp $
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

CDEF1=	-DTERMIOS			# Use the termios structure
CDEF2=	-DSIGTYPE=void			# Standard definition
CDEF3=	-UNO_SLEEP			# Use the usleep function
CDEF4=	-DFNDELAY=O_NDELAY		# Old name value
CDEFS=	$(CDEF1) $(CDEF2) $(CDEF3) $(CDEF4)
CC=$(CROSS_COMPILE)gcc
STRIP=$(CROSS_COMPILE)strip
COPTS=	-O2 -pipe
CFLAGS=	$(COPTS) $(CDEFS)

INSTALL= install

all:	chat

chat:	chat.o
	$(CC) -o chat chat.o
	$(STRIP) -s -R .note -R .comment $@ 

chat.o:	chat.c
	$(CC) -c $(CFLAGS) -o chat.o chat.c

install: chat
	mkdir -p $(BINDIR) $(MANDIR)
	$(INSTALL) -s -c chat $(BINDIR)
	$(INSTALL) -c -m 644 chat.8 $(MANDIR)

clean:
	rm -f chat.o chat *~
