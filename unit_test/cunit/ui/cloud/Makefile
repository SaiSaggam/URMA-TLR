TLR_BASE ?= $(shell cd ../../../..; pwd)
export TLR_BASE
include $(TLR_BASE)/build/makefile.globals

CONFIG_MGR_DIR = $(LIB_BASE)/config_mgr
CODE_DIR = $(UI_BASE)/cloud
PUBLIC_HEADER_DIR=$(CODE_DIR)/cc_ansic/public/include

CC ?= gcc

CFLAGS = -g -Wall -Werror --std=gnu99 -D_GNU_SOURCE -D_CUNIT_ -DUNIT_TEST -fdata-sections -ffunction-sections

CFLAGS += -fprofile-arcs -ftest-coverage

LDFLAGS = -Wl,--gc-sections -lcunit -lm -ljson-c -lcrypto

CODE_INC = -I$(TLR_BASE)/include -I$(CODE_DIR) -I$(PUBLIC_HEADER_DIR) -I$(PUBLIC_HEADER_DIR)/custom -I$(UTILS_BASE)/include
CODE_INC += -I$(CONFIG_MGR_DIR) -I$(OPENSRC_BASE)/json-c/src/ -I$(LIB_BASE)/libifuptime -I$(LIB_BASE)/message/include

CONFIG_MGR_OBJS = config_table.o config_valid.o tlr_show_object.o
CC_UNITS_OBJS = cc_firmware.o cc_show.o cc_status.o
TEST_CC_OBJS = test_cc.o test_cc_firmware.o test_cc_show.o test_cc_status.o
CC_UNITS_SRCS = $(CC_UNITS_OBJS:.o=.c)

CC_OBJS=$(TEST_CC_OBJS) $(CC_UNITS_OBJS) $(CONFIG_MGR_OBJS)

CC_EXE = cloud_test

COVERAGE_SRCS = $(CC_UNITS_SRCS)

ALL_EXE =  $(CC_EXE)

all: run coverage

run: $(ALL_EXE)
	@for exe in $^ ; do \
		./$$exe || exit 1 ; \
	done

$(CC_EXE): $(CC_OBJS)
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
