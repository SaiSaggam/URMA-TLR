# RDS/00008: OASIS Components Makefile Conventions

ifdef TARGET
    TARGET_PREFIX = $(TARGET)-
endif

ifndef CROSS
    CROSS = $(TARGET_PREFIX)
    AS      = $(CROSS)as
    LD      = $(CROSS)ld
    CC      = $(CROSS)gcc
    CPP     = $(CC) -E
    AR      = $(CROSS)ar
    NM      = $(CROSS)nm
    STRIP   = $(CROSS)strip
    OBJCOPY = $(CROSS)objcopy
    OBJDUMP = $(CROSS)objdump
endif

ifdef LOG_COLLECTOR
    CFLAGS += -DLOG_COLLECTOR=$(LOG_COLLECTOR)
endif

ifdef DEBUG_LEVEL
    CFLAGS += -DDEBUG_LEVEL=$(DEBUG_LEVEL)
endif

ifdef KERNEL_DIR
    CFLAGS += -I$(KERNEL_DIR)/include
endif
