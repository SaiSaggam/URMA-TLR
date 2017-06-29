# Add in your code directory (whatever you are testing)
#CODE_DIR=../../../whatever
CODE_DIR=.

CC ?= gcc
CFLAGS = -g -Wall --std=gnu99 -D _CUNIT_
LDFLAGS = -L/usr/local/lib -lcunit

CODE_INC=-I$(CODE_DIR)

OBJS = whatever_test.o

all: utest

utest: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	./utest

%.o: %.c
	$(CC) $(CODE_INC) $(CFLAGS) -c -o $@ $<
%.o: $(CODE_DIR)/%.c $(CODE_DIR)/%.h
	$(CC) $(CODE_INC) $(CFLAGS) -c -o $@ $<
%.o: $(CODE_DIR)/%.c
	$(CC) $(CODE_INC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o utest

.PHONY: all clean

