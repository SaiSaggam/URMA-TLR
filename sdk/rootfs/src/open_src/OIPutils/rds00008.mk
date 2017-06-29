# RDS/00008: OASIS Components Makefile Conventions

ifdef TARGET
    export TARGET_PREFIX = $(TARGET)-
endif

ifndef CROSS_COMPILE
    export CROSS_COMPILE = $(TARGET_PREFIX)
    export AS      = $(CROSS_COMPILE)as
    export LD      = $(CROSS_COMPILE)ld
    export CC      = $(CROSS_COMPILE)gcc
    export CPP     = $(CC) -E
    export AR      = $(CROSS_COMPILE)ar
    export NM      = $(CROSS_COMPILE)nm
    export STRIP   = $(CROSS_COMPILE)strip
    export OBJCOPY = $(CROSS_COMPILE)objcopy
    export OBJDUMP = $(CROSS_COMPILE)objdump
endif

