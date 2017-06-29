TLR_BASE ?= $(shell cd ../../../..; pwd)
export TLR_BASE
include $(TLR_BASE)/build/makefile.globals

CODE_DIR = $(UI_BASE)/web/urma/modules
CONFIG_MGR_DIR = $(LIB_BASE)/config_mgr

CC ?= gcc

#HOST_PYTHON_PATH = /
#TARGET_PYTHON_PATH = /
#include $(OPENSRC_BASE)/python/python_extensions.mk

CFLAGS = -g -Wall -Werror --std=gnu99 -D_GNU_SOURCE -D_CUNIT_ -DUNIT_TEST -fdata-sections -ffunction-sections

CFLAGS += -fprofile-arcs -ftest-coverage

LDFLAGS = -Wl,--gc-sections -lcunit -lxml2

CODE_INC = -I$(TLR_BASE)/include -I$(CODE_DIR) -I$(CONFIG_MGR_DIR) -I$(UTILS_BASE)/include -I/usr/include/libxml2/

CONFIG_MGR_OBJS = config_table.o config_valid.o
#DDPIF_OBJS=ddp_if.o ddp_tlr.o
#TEST_OBJS=main.o test_ddp_if.o test_ddp_tlr.o
DDPIF_OBJS = ddp_tlr.o ddp_firmware.o
TEST_OBJS = test_ddp.o test_ddp_stub.o test_ddp_tlr.o test_ddp_firmware.o
DDPIF_SRCS = $(DDPIF_OBJS:.o=.c)

DDP_OBJS=$(TEST_OBJS) $(DDPIF_OBJS) $(CONFIG_MGR_OBJS)

DDPIF_EXE = ddpif_test

COVERAGE_SRCS = $(DDPIF_SRCS)

ALL_EXE =  $(DDPIF_EXE)

all: run coverage

run: $(ALL_EXE)
	@for exe in $^ ; do \
		./$$exe || exit 1 ; \
	done

$(DDPIF_EXE): $(DDP_OBJS)
	@$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	@$(CC) $(CODE_INC) $(CFLAGS) -c -o $@ $<

%.o: $(CODE_DIR)/%.c
	@$(CC) $(CODE_INC) $(CFLAGS) -c -o $@ $<

%.o: $(CONFIG_MGR_DIR)/%.c
	@$(CC) $(CODE_INC) $(CFLAGS) -c -o $@ $<

clean:
	@rm -f *.o *.gcno *.gcov *.gcda $(ALL_EXE)

coverage:
	@gcov $(COVERAGE_SRCS)

.PHONY: all clean run coverage
