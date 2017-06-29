##
# @file Makefile
#
# Makefile for Upgbuf
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

ifdef LOG_COLLECTOR
    CFLAGS += -DLOG_COLLECTOR=$(LOG_COLLECTOR)
endif
#ifdef DEBUG_LEVEL
    CFLAGS += -DDEBUG_LEVEL=9#$(DEBUG_LEVEL)
#endif

ifndef KERNEL_DIR
	KERNEL_DIR=/usr/src/linux
endif

ifdef KERNEL_VER
	 CFLAGS += -DKERNEL_VER=$(KERNEL_VER)
endif	 	


CC = $(CROSS_COMPILE)gcc
LD = $(CROSS_COMPILE)gcc
STRIP = $(CROSS_COMPILE)strip

# General definitions

ifdef DEBUG
	CFLAGS += -DDEBUG=${DEBUG}
endif

# Main rules
all: test.c
	$(CC) -o odmtest test.c -lamitodm -L$(INSTALL_ROOT)/lib -I../../include

# Object files dependencies

# Special targets

#.PHONY: install

install: 
	@cp -af odmtest $(INSTALL_ROOT)/bin

# Phony targets

.PHONY: clean distclean

clean:
	-rm -f *.o

distclean: clean

