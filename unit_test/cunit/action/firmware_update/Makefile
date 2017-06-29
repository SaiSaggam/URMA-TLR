TLR_BASE ?= $(shell cd ../../../..; pwd)
export TLR_BASE
include $(TLR_BASE)/build/makefile.globals

CODE_DIR += $(COMMANDS_BASE)/action/update_firmware
CONFIG_MGR_DIR = $(LIB_BASE)/config_mgr

CC ?= gcc
CFLAGS=-g -Wall -Werror --std=gnu99 -D_GNU_SOURCE -D_CUNIT_ -DGET_FW_BEFORE_UPG -DENABLE_FWUPG_FILE_CHECKSUM

LDFLAGS = -L/usr/local/lib -lcunit -ljson-c -lm

CODE_INC = -I$(CODE_DIR) -I$(CONFIG_MGR_DIR)
CODE_INC += -I$(OPENSRC_BASE)/json-c/src/ -I$(ROOTFS_BASE)/src/include 
CODE_INC += -I$(UTILS_BASE)/include

#we redefine tlr_printf() to mocked_tlr_printf() so we don't have to see all the guff that the code under test prints out
CODE_UNDER_TEST_CFLAGS=-Dtlr_printf=mocked_tlr_printf

OBJS = update_firmware.o test_firmware_update.o strlcpy.o tlr_print.o system_command.o
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
	@$(CC) $(CODE_UNDER_TEST_CFLAGS) $(CODE_INC) $(CFLAGS) -c -o $@ $<
%.o: $(CODE_DIR)/%.c
	@$(CC) $(CODE_UNDER_TEST_CFLAGS) $(CODE_INC) $(CFLAGS) -c -o $@ $<
%.o: $(UTILS_BASE)/%.c $(CODE_DIR)/%.h
	@$(CC) $(CODE_INC) $(CFLAGS) -c -o $@ $<
%.o: $(UTILS_BASE)/%.c
	@$(CC) $(CODE_INC) $(CFLAGS) -c -o $@ $<

clean:
	@rm -f *.o $(EXE)

.PHONY: all clean run
