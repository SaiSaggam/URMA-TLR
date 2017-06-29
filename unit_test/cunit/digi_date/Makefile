# Add in your code directory
TLR_BASE ?= $(shell cd ../../..; pwd)
export TLR_BASE
include $(TLR_BASE)/build/makefile.globals

CODE_DIR = $(COMMANDS_BASE)/action/date

CC ?= gcc
CFLAGS = -g -Wall -Werror --std=gnu99 -D _CUNIT_
LDFLAGS = -L/usr/local/lib -lcunit

CODE_INC=-I$(CODE_DIR) -I$(LIB_BASE)/libdigiutils/include

OBJS = digi_date_wrk.o digi_date_test.o date_stubs.o
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

