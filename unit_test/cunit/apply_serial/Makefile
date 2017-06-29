TLR_BASE ?= $(shell cd ../../..; pwd)
export TLR_BASE
include $(TLR_BASE)/build/makefile.globals

CODE_DIR=$(COMMANDS_BASE)/apply/serial
CONFIG_MGR_DIR =$(LIB_BASE)/config_mgr

CC ?= gcc
CFLAGS = -g -Wall -Werror --std=gnu99 -D _CUNIT_
LDFLAGS = -L/usr/local/lib -lcunit

CODE_INC =-I$(CODE_DIR) -I$(CONFIG_MGR_DIR) -I$(TLR_BASE)/include

OBJS = config_serial_wrk.o config_serial_test.o
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

