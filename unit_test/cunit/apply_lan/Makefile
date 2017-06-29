TLR_BASE ?= $(shell cd ../../..; pwd)
export TLR_BASE
include $(TLR_BASE)/build/makefile.globals

CODE_DIR=$(COMMANDS_BASE)/apply/lan
CONFIG_MGR_DIR =$(LIB_BASE)/config_mgr
DIGIUTILS_DIR = $(LIB_BASE)/libdigiutils
MSG_INC = $(MESSAGE_BASE)/include
LIBMESSAGE_DIR = $(LIB_BASE)/message


CC ?= gcc
CFLAGS = -g -Wall -Werror --std=gnu99 -D_CUNIT_ -D_GNU_SOURCE -fdata-sections -ffunction-sections
#CFLAGS += -fprofile-arcs -ftest-coverage


LIBPATH=-L$(LIB_BASE)/config_mgr
LDFLAGS = $(LIBPATH) -L/usr/local/lib -lcunit
#LDFLAGS = $(LIBPATH) -Wl,--gc-sections -lcunit
CODE_INC = -I. -I$(CODE_DIR) -I$(CONFIG_MGR_DIR) -I$(INSTALL_ROOT)/usr/include
CODE_INC += -I$(KERNEL_DIR)/include  -I$(UTILS_BASE)/include -I$(TLR_BASE)/include -I$(MESSAGE_BASE)/include


OBJS = apply_lan.o apply_lan_test.o
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
	echo codeinc $(CODE_INC)
	@$(CC) $(CODE_INC) $(CFLAGS) -c -o $@ $<
%.o: $(CODE_DIR)/%.c
	@$(CC) $(CODE_INC) $(CFLAGS) -c -o $@ $<

clean:
	@rm -f *.o $(EXE)

.PHONY: all clean run

