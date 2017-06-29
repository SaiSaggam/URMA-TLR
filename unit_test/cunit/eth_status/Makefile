CODE_DIR = $(COMMANDS_BASE)/show/eth_status

CC ?= gcc
CFLAGS=-g -Wall -Werror --std=gnu99 -D _CUNIT_
LDFLAGS=-L/usr/local/lib -lcunit

CODE_INC=-I$(CODE_DIR)

OBJS = eth_status.o eth_status_test.o
EXE=utest

all: $(EXE) run

run: $(EXE)
	@for exe in $^ ; do \
		./$$exe || exit 1 ; \
	done

$(EXE): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CODE_INC) $(CFLAGS) -c -o $@ $<
%.o: $(CODE_DIR)/%.c $(CODE_DIR)/%.h
	$(CC) $(CODE_INC) $(CFLAGS) -c -o $@ $<
%.o: $(CODE_DIR)/%.c
	$(CC) $(CODE_INC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o $(EXE)

.PHONY: all clean run

