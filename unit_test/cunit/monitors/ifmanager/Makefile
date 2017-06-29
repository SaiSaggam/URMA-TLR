TLR_BASE ?= $(shell cd ../../../..; pwd)
export TLR_BASE
include $(TLR_BASE)/build/makefile.globals

CODE_DIR = $(TOOLS_BASE)/ifmanager
CONFIG_MGR_DIR = $(LIB_BASE)/config_mgr
DIGIUTILS_DIR = $(LIB_BASE)/libdigiutils
MSG_INC = $(MESSAGE_BASE)/include
LIBMESSAGE_DIR = $(LIB_BASE)/message

CC ?= gcc
CFLAGS = -g -Wall -Werror --std=gnu99 -D_GNU_SOURCE -D_CUNIT_ -DUNIT_TEST -fdata-sections -ffunction-sections
CFLAGS += -fprofile-arcs -ftest-coverage

LIBPATH=-L$(LIB_BASE)/config_mgr

LDFLAGS = $(LIBPATH) -Wl,--gc-sections -lcunit

CODE_INC = -I$(TLR_BASE)/include -I$(CODE_DIR) -I$(CONFIG_MGR_DIR) -I$(UTILS_BASE)/include -I$(MSG_INC)

DIGIUTILS_OBJS = strlcpy.o
CONFIG_MGR_OBJS = config_table.o config_valid.o 
IFMANAGER_OBJS = ifmanager.o ifmessage.o ifmconfig.o failover.o failover_show.o ifhandler.o route.o add_route.o
TEST_OBJS = main.o test_failover.o test_route.o test_ifmessage.o test_ifmanager.o
IFMANAGER_SRCS = $(IFMANAGER_OBJS:.o=.c)

IFM_OBJS = $(DIGIUTILS_OBJS) $(CONFIG_MGR_OBJS) $(IFMANAGER_OBJS) $(TEST_OBJS)

IFM_EXE=ifm_test

PROBE_PRODUCTION_OBJS = probing.o
PROBE_TEST_OBJS = test_probing.o test_probing_main.o
PROBE_OBJS = $(PROBE_PRODUCTION_OBJS) $(PROBE_TEST_OBJS)
PROBE_TEST_EXE = probe_test
PROBE_SRCS = $(PROBE_PRODUCTION_OBJS:.o=.c)

IFHANDLER_PRODUCTION_OBJS = ifhandler.o
IFHANDLER_TEST_OBJS = test_ifhandler.o test_ifhandler_main.o
IFHANDLER_OBJS = $(IFHANDLER_PRODUCTION_OBJS) $(IFHANDLER_TEST_OBJS)
IFHANDLER_TEST_EXE = ifhandler_test
IFHANDLER_SRCS = $(IFHANDLER_PRODUCTION_OBJS:.o=.c)

DNS_HANDLE_PRODUCTION_OBJS = dns_handle.o
DNS_HANDLE_TEST_OBJS = test_dns_handle.o test_dns_handle_main.o
DNS_HANDLE_OBJS = $(DNS_HANDLE_PRODUCTION_OBJS) $(DNS_HANDLE_TEST_OBJS)
DNS_HANDLE_TEST_EXE = dns_handle_test
DNS_HANDLE_SRCS = $(DNS_HANDLE_PRODUCTION_OBJS:.o=.c)


COVERAGE_SRCS = $(IFMANAGER_SRCS) $(IFHANDLER_SRCS) $(PROBE_SRCS) $(DNS_HANDLE_SRCS)

ALL_EXE =  $(IFM_EXE) $(PROBE_TEST_EXE) $(IFHANDLER_TEST_EXE) $(DNS_HANDLE_TEST_EXE)

all: run coverage

run: $(ALL_EXE)
	@for exe in $^ ; do \
		./$$exe || exit 1 ; \
	done

$(DNS_HANDLE_TEST_EXE): $(DNS_HANDLE_OBJS)
	@$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(IFM_EXE): $(IFM_OBJS)
	@$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(PROBE_TEST_EXE): $(PROBE_OBJS)
	@$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(IFHANDLER_TEST_EXE): $(IFHANDLER_OBJS)
	@$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	@$(CC) $(CODE_INC) $(CFLAGS) -c -o $@ $<

%.o: $(CODE_DIR)/%.c
	@$(CC) $(CODE_INC) $(CFLAGS) -c -o $@ $<

%.o: $(CONFIG_MGR_DIR)/%.c
	@$(CC) $(CODE_INC) $(CFLAGS) -c -o $@ $<

%.o: $(DIGIUTILS_DIR)/%.c
	@$(CC) $(CODE_INC) $(CFLAGS) -c -o $@ $<

clean:
	@rm -f *.o *.gcno *.gcov *.gcda $(ALL_EXE)

coverage:
	@gcov $(COVERAGE_SRCS)

.PHONY: all clean run coverage
