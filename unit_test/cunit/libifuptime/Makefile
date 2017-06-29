TLR_BASE ?= $(shell cd ../../..; pwd)
export TLR_BASE
include $(TLR_BASE)/build/makefile.globals

CODE_DIR = $(LIB_BASE)/libifuptime

CC ?= gcc
CFLAGS = -g -Wall -Werror --std=gnu99 -D_CUNIT_
LDFLAGS = -L/usr/local/lib -lcunit

CODE_INC=-I. -I$(CODE_DIR)

OBJS = ifuptime_lib.o libifuptime_test.o
EXE=utest

all: $(EXE) run

run: $(EXE)
	@for exe in $^ ; do \
		./$$exe || exit 1 ; \
	done

$(EXE): $(OBJS)
	@$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	@$(CC) $(CODE_INC) $(CFLAGS) -c -o $@ $<
%.o: $(CODE_DIR)/%.c $(CODE_DIR)/%.h
	@$(CC) $(CODE_INC) $(CFLAGS) -c -o $@ $<
%.o: $(CODE_DIR)/%.c
	@$(CC) $(CODE_INC) $(CFLAGS) -c -o $@ $<

clean:
	@rm -f *.o $(EXE)

.PHONY: all clean run

