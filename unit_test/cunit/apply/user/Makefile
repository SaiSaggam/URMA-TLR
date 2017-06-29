TLR_BASE ?= $(shell cd ../../../..; pwd)
export TLR_BASE
include $(TLR_BASE)/build/makefile.globals

CODE_DIR = $(COMMANDS_BASE)/apply/user
CONFIG_MGR_DIR = $(LIB_BASE)/config_mgr

CFLAGS = -g -Wall -Werror -D _GNU_SOURCE --std=gnu99 -D _CUNIT_ -fdata-sections -ffunction-sections
CFLAGS += -fprofile-arcs -ftest-coverage
LDFLAGS=-Wl,--gc-sections -lcunit

CODE_INC = -I$(CODE_DIR) -I$(CONFIG_MGR_DIR) -I$(UTILS_BASE)/include -I$(TLR_BASE)/include 

OBJS = apply_user_test.o apply_user.o config_last.o config_table.o config_valid.o
EXE=apply_user_test

all: $(EXE) run coverage

run: $(EXE)
	mkdir -p config/user 
	@for exe in $^ ; do \
		./$$exe || exit 1 ; \
	done

$(EXE): $(OBJS)
	@$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	@$(CC) $(CODE_INC) $(CFLAGS) -c -o $@ $<
%.o: $(CODE_DIR)/%.c
	@$(CC) $(CODE_INC) $(CFLAGS) -c -o $@ $<
%.o: $(CONFIG_MGR_DIR)/%.c
	@$(CC) $(CODE_INC) $(CFLAGS) -c -o $@ $<

clean:
	@rm -f *.o *.g* config/user/* $(EXE)

coverage:
	gcov config_last.c apply_user.c


.PHONY: all clean run coverage

