#
#


OBJS = helloworld.o

EXEC = helloworld

INCPATH= -I./ -I../../../include -I$(KERNEL_DIR)/include 

LIBPATH= -L../../../$(TARGET_PREFIX)lib

CFLAGS += $(INCPATH)

BINARIES = $(EXEC)

LFLAGS += $(LIBPATH)


all: $(BINARIES)
	$(foreach var,$(BINARIES),$(STRIP) --remove-section=.note --remove-section=.comment $(var);)

$(EXEC): $(OBJS)
	@$(CC) $(LFLAGS) -o ${EXEC} $(OBJS)


%.o: %.c
	@$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -c -o $@ $<

clean:
	-rm -f $(BINARIES) *.o

install:
	

