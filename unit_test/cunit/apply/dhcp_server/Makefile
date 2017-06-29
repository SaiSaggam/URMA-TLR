TLR_BASE ?= $(shell cd ../../../..; pwd)
export TLR_BASE
include $(TLR_BASE)/build/makefile.globals

CODE_DIR = $(COMMANDS_BASE)/apply/dhcp_server
CONFIG_MGR_DIR = $(LIB_BASE)/config_mgr

INC_DIR = -I. -I$(CODE_DIR) -I$(LIB_BASE)/config_mgr -I$(TLR_BASE)/include -I$(UTILS_BASE)/include

CC ?= gcc

CFLAGS = -fdata-sections -ffunction-sections -Wall -Werror --std=gnu99 -D_GNU_SOURCE -D_CUNIT_ $(INC_DIR)
LDFLAGS = -Wl,--gc-sections  -lcunit
#CFLAGS = -g -Wall -Werror -D _GNU_SOURCE --std=gnu99 -D _CUNIT_ -fdata-sections -ffunction-sections
#LDFLAGS = -lcunit

OBJS = apply_dhcp_server.o test_apply_dhcp_server.o
EXE=utest
all: $(EXE) run

run: $(EXE)
	@for exe in $^ ; do \
		./$$exe || exit 1 ; \
	done

$(EXE): $(OBJS)
	@$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	@$(CC) $(CFLAGS) -c -o $@ $<
%.o: $(CODE_DIR)/%.c $(CODE_DIR)/%.h
	@$(CC) $(CFLAGS) -c -o $@ $<
%.o: $(CODE_DIR)/%.c
	@$(CC) $(CFLAGS) -c -o $@ $<

clean:
	@rm -f *.o $(EXE)

.PHONY: all clean run
