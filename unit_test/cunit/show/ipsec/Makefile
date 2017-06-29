TLR_BASE ?= $(shell cd ../../../..; pwd)
export TLR_BASE
include $(TLR_BASE)/build/makefile.globals

CODE_DIR = $(COMMANDS_BASE)/show/ipsec
CONFIG_MGR_DIR = $(LIB_BASE)/config_mgr

CC ?= gcc
CFLAGS = -g -Wall -Werror --std=gnu99 -D _CUNIT_ -fdata-sections -ffunction-sections

LDFLAGS=-Wl,--gc-sections -L/usr/local/lib -lcunit -ljson-c -lm

CODE_INC=-I$(CODE_DIR) -I$(CONFIG_MGR_DIR) 
CODE_INC+=-I$(OPENSRC_BASE)/json-c/src/ -I$(ROOTFS_BASE)/src/include -I$(TLR_BASE)/include
CODE_INC+=-I$(UTILS_BASE)/include

OBJS=ipsec_show.o parse_ipsec_status.o strlcpy.o show_ipsec_test.o
EXE=utest

all: run

run: $(EXE)
	@for exe in $^ ; do \
		./$$exe || exit 1 ; \
	done

$(EXE): $(OBJS)
	@$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	@$(CC) $(CODE_INC) $(CFLAGS) -c -o $@ $<

%.o: $(UTILS_BASE)/%.c
	@$(CC) $(CODE_INC) $(CFLAGS) -c -o $@ $<

%.o: $(CODE_DIR)/%.c
	@$(CC) $(CODE_INC) $(CFLAGS) -c -o $@ $<

clean:
	@rm -f *.o $(EXE)

.PHONY: all clean run
