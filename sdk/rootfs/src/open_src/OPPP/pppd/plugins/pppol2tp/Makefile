COPTS = -O2
VERSION = 0.16
PLUGINS := pppol2tp.so openl2tp.so

CFLAGS = $(COPTS) -I. -I.. -I../.. -I../../.. -I$(KERNEL_DIR)/include  -fPIC 
export CFLAGS

all: $(PLUGINS)

%.so: %.o
	$(CC) $(CFLAGS) -o $@ -shared $^ $(LIBS)

romfs: all
	$(ROMFSINST) /etc_ro/ppp/plugins/pppol2tp.so
	$(ROMFSINST) /etc_ro/ppp/plugins/openl2tp.so

clean:
	rm -f *.o *.so

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<
