TLR_BASE ?= $(shell cd ../../../..; pwd)
export TLR_BASE
include $(TLR_BASE)/build/makefile.globals

CODE_DIR = $(COMMANDS_BASE)/show/lan

CC ?= gcc

SRC_DIR = $(LIB_BASE)/libdigiutils
CODE_INC = -I$(CODE_DIR) -I$(OPENSRC_BASE)/json-c/src/ -I$(LIB_BASE)/libdigiutils/include

CFLAGS = -g -Wall -Werror --std=gnu99 -D_GNU_SOURCE -DUNIT_TEST -D_CUNIT_ -fdata-sections -ffunction-sections
LDFLAGS=-Wl,--gc-sections $(LIBPATH) -L/usr/local/lib -lcunit -ljson-c -lm

CODE_INC = -I$(CODE_DIR) -I$(LIB_BASE)/config_mgr -I$(LIB_BASE)/libdigiutils/include
CODE_INC+=-I$(OPENSRC_BASE)/json-c/src/ -I$(ROOTFS_BASE)/src/include -I$(TLR_BASE)/include
CODE_INC+=-I$(UTILS_BASE)/include -I$(LIB_BASE)/message/include

EXE = utest

OBJS = show_lan_work.o show_lan_test.o
SRC_OBJS = show_parse_helper.o
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
