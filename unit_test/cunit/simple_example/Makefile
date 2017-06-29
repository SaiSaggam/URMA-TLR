CC ?= gcc
CFLAGS = -g -Wall --std=gnu99
LDFLAGS = -L/usr/local/lib -lcunit
#INC = -I/usr/include

OBJS = max.o max_test.o

all: utest

utest: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c %.h
	$(CC) $(CFLAGS)  -c -o $@ $<
%.o: %.c
	$(CC) $(CFLAGS)  -c -o $@ $<

clean:
	rm -f *.o utest

.PHONY: all clean
