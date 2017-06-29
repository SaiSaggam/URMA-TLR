TLR_BASE ?= $(shell cd ../../..; pwd)
export TLR_BASE
include $(TLR_BASE)/build/makefile.globals

CODE_DIR = $(TOOLS_BASE)/eth_monitor
CONFIG_MGR_DIR = $(LIB_BASE)/config_mgr
ETH_STATS_DIR = $(COMMANDS_BASE)/show/eth_stats

CC ?= gcc
CFLAGS = -g -Wall -Werror --std=gnu99 -D_CUNIT_ -D_GNU_SOURCE
LDFLAGS = -L/usr/local/lib -lcunit

CODE_INC=-I$(CODE_DIR) -I$(ROOTFS_BASE)/src/include -I$(CONFIG_MGR_DIR) -I$(LIB_BASE)/message/include
CODE_INC+=-I$(UTILS_BASE)/include \
		-I$(INSTALL_ROOT)/usr/include \
		-I$(ETH_STATS_DIR) \
		-I$(LIB_BASE)/libifuptime \
		-DETH_MONITOR_DEBUG=0

OBJS = poll_eth_func.o eth_monitor_test.o
OBJS2 = apply_eth_func.o eth_monitor_test2.o
TEST1_EXE=utest
TEST2_EXE=utest2

EXES=$(TEST1_EXE) $(TEST2_EXE)

all: $(EXES) run

run: $(EXES)
	@for exe in $^ ; do \
		./$$exe || exit 1 ; \
	done

$(TEST1_EXE): $(OBJS)
	@$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(TEST2_EXE): $(OBJS2)
	@$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	@$(CC) $(CODE_INC) $(CFLAGS) -c -o $@ $<
%.o: $(CODE_DIR)/%.c $(CODE_DIR)/%.h
	@$(CC) $(CODE_INC) $(CFLAGS) -c -o $@ $<
%.o: $(CODE_DIR)/%.c
	@$(CC) $(CODE_INC) $(CFLAGS) -c -o $@ $<

clean:
	@rm -f *.o $(EXES)

.PHONY: all clean run

