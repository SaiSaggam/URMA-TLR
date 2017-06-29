##
# @file Makefile
#
# Makefile for usbreset
#

# Which target to build
#	make			=> Linux PC version
#	TARGET=${TARGET} make	=> Target device version
#	DEBUG=1 make		=> Enable debugging code


ifdef TARGET
    TARGET_PREFIX = $(TARGET)-
endif

ifndef CROSS_COMPILE
    CROSS_COMPILE = $(TARGET_PREFIX)
endif

ifndef KERNEL_DIR
	KERNEL_DIR=/usr/src/linux
endif

ifdef KERNEL_VER
	 CFLAGS += -DKERNEL_VER=$(KERNEL_VER)
endif

ifndef PREFIX
PREFIX=/usr/
endif

ifndef SBINDIR
SBINDIR=$(PREFIX)/sbin
endif

PROGS = usbreset

COPTS = -D_GNU_SOURCE -O2 -Wall -Werror -g
ifeq ($(origin LOPTS), undefined)
LOPTS =
endif

CFLAGS	= $(COPTS) -I. -I../../include
LDFLAGS	= $(LOPTS)

CC = $(CROSS_COMPILE)gcc
LD = $(CROSS_COMPILE)gcc
STRIP = $(CROSS_COMPILE)strip

# General definitions

ifdef DEBUG
	CFLAGS += -DDEBUG=${DEBUG}
endif

# Main rules
%.o: %.c %<
	$(CC) $(CFLAGS) -c $<

all: $(PROGS)

# Object files dependencies

# Special targets

#.PHONY: install

install: all
	@$(STRIP) --strip-unneeded $(PROGS)
	@install -m 0755 -d $(INSTALL_ROOT)$(SBINDIR)
	@install -m 0755 $(PROGS) $(INSTALL_ROOT)$(SBINDIR)

# Phony targets

.PHONY: clean distclean

clean:
	-rm -f *.o
	-rm -f $(PROGS)

distclean: clean

usbreset: usbreset.o
	$(CC) $(LDFLAGS) -o usbreset usbreset.o
