TLR_BASE ?= $(shell cd ../../..; pwd)
export TLR_BASE
include $(TLR_BASE)/build/makefile.globals
CODE_DIR = $(COMMANDS_BASE)/show/eth_stats

CC ?= gcc
CFLAGS=-g -Wall -Werror --std=gnu99  -D_GNU_SOURCE  -D_CUNIT_ -DUNIT_TEST -fdata-sections -ffunction-sections
LDFLAGS=-Wl,--gc-sections -L/usr/local/lib -lcunit

SRC_DIR = $(LIB_BASE)/libdigiutils
CODE_INC=-I$(CODE_DIR)
CODE_INC+= -I$(KERNEL_DIR)/include -I$(TLR_BASE)/include -I$(LIB_BASE)/config_mgr -I$(OPENSRC_BASE)/json-c/src/
CODE_INC+= -I$(LIB_BASE)/libifuptime -I$(LIB_BASE)/libdigiutils/include -I$(LIB_BASE)/message/include

OBJS = show_eth.o show_eth_test.o ethregs.o
SRC_OBJS = ip_helper.o strlcpy.o
EXE=utest

all: $(EXE) run

run: $(EXE)
	@for exe in $^ ; do \
		./$$exe || exit 1 ; \
	done

$(EXE): $(OBJS) $(SRC_OBJS)
	@$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	@$(CC) $(CODE_INC) $(CFLAGS) -c -o $@ $<
%.o: $(CODE_DIR)/%.c $(CODE_DIR)/%.h
	@$(CC) $(CODE_INC) $(CFLAGS) -c -o $@ $<
%.o: $(CODE_DIR)/%.c
	@$(CC) $(CODE_INC) $(CFLAGS) -c -o $@ $<
%.o: $(SRC_DIR)/%.c
	@$(CC) $(CODE_INC) $(CFLAGS) -c -o $@ $<

clean:
	@rm -f *.o $(EXE)

.PHONY: all clean run

