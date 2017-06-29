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

CC	=$(CROSS_COMPILE)gcc
COPTS	= -O2 
CFLAGS	= $(COPTS) -I../.. -I../../../include -I$(KERNEL_DIR)/include -fPIC
LDFLAGS	= -shared
INSTALL	= install

#***********************************************************************

DESTDIR = /usr/local
LIBDIR = $(DESTDIR)/lib/pppd/$(VERSION)

VERSION = $(shell awk -F '"' '/VERSION/ { print $$2; }' ../../patchlevel.h)

PLUGIN := pppoatm.so
PLUGIN_OBJS := pppoatm.o 

#*******
# Do we have the ATM libraries installed? Set HAVE_LIBATM to use them,
# or leave it unset to build the few routines we actually _use_ into
# the plugin directly.
#
#HAVE_LIBATM=yes

ifdef HAVE_LIBATM
LIBS := -latm
else
CFLAGS += -I.
PLUGIN_OBJS += text2qos.o text2atm.o misc.o ans.o
LIBS := -lresolv
endif

ifdef TC3262
    CFLAGS += -DTC3262
endif

#*********
all: $(PLUGIN)

$(PLUGIN): $(PLUGIN_OBJS)
	$(CC) $(CFLAGS) -o $@ -shared $^ $(LIBS)

install: all
	$(INSTALL) -d -m 755 $(LIBDIR)
	$(INSTALL) -c -m 4550 $(PLUGIN) $(LIBDIR)

clean:
	rm -f *.o *.so

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<
