TLR_BASE ?= $(shell cd ../../../..; pwd)
export TLR_BASE 
include $(TLR_BASE)/build/makefile.globals

CODE_UNDER_TEST_DIR = $(LIB_BASE)/message
DIGIUTILS_DIR = $(LIB_BASE)/libdigiutils

CC?=gcc
CFLAGS=-g -Wall -Werror --std=gnu99 -D_CUNIT_ -fdata-sections -ffunction-sections
LDFLAGS=-Wl,--gc-sections -L/usr/local/lib -lcunit

CODE_INC=-I. -I$(CODE_UNDER_TEST_DIR) -I$(CODE_UNDER_TEST_DIR)/include -I$(ROOTFS_BASE)/usr/include -I$(UTILS_BASE)/include -I$(TLR_BASE)/include
CODE_INC+=-I$(LIB_BASE)/config_mgr

CFLAGS+=-D_GNU_SOURCE
CFLAGS+=$(CODE_INC)

TIMER_TEST_OBJS=timer_test.o timer_timval.o timespec_helpers.o 
TIMER_TEST_EXE=timer_test
QUEUE_TEST_OBJS=queue_test.o queue.o
QUEUE_TEST_EXE=queue_test
INTERFACES_TEST_OBJS=interfaces_test.o interfaces.o
INTERFACES_TEST_EXE=interfaces_test
PARSING_TEST_OBJS=parsing_test.o parsing.o update_message.o
PARSING_TEST_EXE=parsing_test
ARGS_TEST_OBJS=args_test.o args.o
ARGS_TEST_EXE=args_test
MESSAGES_TEST_OBJS=messages_test.o messages.o parsing_get_wan_message_info.o args.o
MESSAGES_TEST_EXE=messages_test

EXES=$(TIMER_TEST_EXE) $(QUEUE_TEST_EXE) $(INTERFACES_TEST_EXE) $(PARSING_TEST_EXE) $(ARGS_TEST_EXE) $(MESSAGES_TEST_EXE)

all: $(EXES) run

run: $(EXES)
	@for exe in $^ ; do \
		./$$exe || exit 1 ; \
	done

$(MESSAGES_TEST_EXE): $(MESSAGES_TEST_OBJS)
	@$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(INTERFACES_TEST_EXE): $(INTERFACES_TEST_OBJS)
	@$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(QUEUE_TEST_EXE): $(QUEUE_TEST_OBJS)
	@$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(TIMER_TEST_EXE): $(TIMER_TEST_OBJS)
	@$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(PARSING_TEST_EXE): $(PARSING_TEST_OBJS)
	@$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(ARGS_TEST_EXE): $(ARGS_TEST_OBJS)
	@$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	@$(CC) $(CFLAGS) -c -o $@ $<
%.o: $(CODE_UNDER_TEST_DIR)/%.c $(CODE_UNDER_TEST_DIR)/%.h
	@$(CC) $(CFLAGS) -c -o $@ $<
%.o: $(CODE_UNDER_TEST_DIR)/%.c
	@$(CC) $(CFLAGS) -c -o $@ $<
%.o: $(DIGIUTILS_DIR)/%.c $(DIGIUTILS_DIR)/%.h
	@$(CC) $(CFLAGS) -c -o $@ $<
%.o: $(DIGIUTILS_DIR)/%.c
	@$(CC) $(CFLAGS) -c -o $@ $<

clean:
	@rm -f *.o $(EXES)

.PHONY: all clean run

