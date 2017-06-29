TLR_BASE ?= $(shell cd ../../../..; pwd)
export TLR_BASE
include $(TLR_BASE)/build/makefile.globals

CODE_DIR = $(COMMANDS_BASE)/show/cloud
CLOUD_DIR = $(UI_BASE)/cloud
PUBLIC_HEADER_DIR = $(CLOUD_DIR)/cc_ansic/public/include
CONFIG_MGR_DIR = $(LIB_BASE)/config_mgr

CC ?= gcc

CFLAGS = -g -Wall -Werror --std=gnu99 -D_GNU_SOURCE -D_CUNIT_ -DUNIT_TEST -fdata-sections -ffunction-sections

CFLAGS += -fprofile-arcs -ftest-coverage

LDFLAGS = -Wl,--gc-sections -lcunit

CODE_INC = -I$(TLR_BASE)/include -I$(CODE_DIR) -I$(UTILS_BASE)/include -I$(LIB_BASE)/message/include -I$(CLOUD_DIR) -I$(CONFIG_MGR_DIR)

SC_UNITS_OBJS = show_cloud.o
TEST_SC_OBJS = test_main.o test_show_cloud.o
SC_UNITS_SRCS = $(SC_UNITS_OBJS:.o=.c)

SC_OBJS=$(TEST_SC_OBJS) $(SC_UNITS_OBJS) 

SC_EXE = show_cloud_test

COVERAGE_SRCS = $(SC_UNITS_SRCS)

ALL_EXE =  $(SC_EXE)

all: run coverage

run: $(ALL_EXE)
	@for exe in $^ ; do \
		./$$exe || exit 1 ; \
	done

$(SC_EXE): $(SC_OBJS)
	@$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	@$(CC) $(CODE_INC) $(CFLAGS) -c -o $@ $<

%.o: $(CODE_DIR)/%.c
	@$(CC) $(CODE_INC) $(CFLAGS) -c -o $@ $<

clean:
	@rm -f *.o *.gcno *.gcov *.gcda $(ALL_EXE)

coverage:
	@gcov $(COVERAGE_SRCS)

.PHONY: all clean run coverage
