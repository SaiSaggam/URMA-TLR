TLR_BASE ?= $(shell cd ../../../..; pwd)
export TLR_BASE
include $(TLR_BASE)/build/makefile.globals

CODE_DIR = $(COMMANDS_BASE)/show/route

CC ?= gcc
CFLAGS=-g -Wall -Werror --std=gnu99 -D _CUNIT_
LDFLAGS=-L/usr/local/lib -lcunit -ljson-c -lm

SRC_DIR = $(LIB_BASE)/libdigiutils
CODE_INC = -I$(CODE_DIR) -I$(OPENSRC_BASE)/json-c/src/ -I$(LIB_BASE)/libdigiutils/include
EXE = utest

OBJS = show_route_func.o show_route_test.o
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
