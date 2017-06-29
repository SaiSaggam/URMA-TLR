VERSION = 0.81

COPTS = -O2
CFLAGS = $(COPTS) -I. -I.. -I../.. -I../../.. -I$(KERNEL_DIR)/include  -fPIC 

PLUGINS := pptp.so

export CFLAGS

all: $(PLUGINS)

OBJS = dirutil.o pptp.o pptp_ctrl.o util.o \
       orckit_quirks.o pptp_callmgr.o pptp_quirks.o vector.o

%.so: $(OBJS)
	$(CC) $(CFLAGS) -o $@ -shared $^ $(LIBS) $(LDFLAG)

romfs: all
	$(ROMFSINST) /etc_ro/ppp/plugins/$(PLUGINS)

clean:
	rm -f *.o *.so

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<
