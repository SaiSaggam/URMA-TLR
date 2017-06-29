TLR_BASE ?= $(shell cd ../../../..; pwd)
export TLR_BASE
include $(TLR_BASE)/build/makefile.globals

CODE_DIR += $(COMMANDS_BASE)/show/cellular
CONFIG_MGR_DIR = $(LIB_BASE)/config_mgr
CELLULAR_STATUS_DIR = $(LIB_BASE)/libcellular

CC ?= gcc
CFLAGS = -g -Wall -Werror --std=gnu99 -D _CUNIT_ -fdata-sections -ffunction-sections
CFLAGS+=-DNO_RADIO_BAND_TEST

LIBPATH= -L$(TOP_DIR)/lib/libdigiutils
LDFLAGS=-Wl,--gc-sections $(LIBPATH) -L/usr/local/lib -lcunit -ljson-c -lm

CODE_INC = -I$(CODE_DIR) -I$(CONFIG_MGR_DIR) -I$(CELLULAR_STATUS_DIR)
CODE_INC += -I$(OPENSRC_BASE)/json-c/src/ -I$(ROOTFS_BASE)/src/include -I$(TLR_BASE)/include
CODE_INC += -I$(UTILS_BASE)/include -I$(LIB_BASE)/libdigiutils/include -I$(LIB_BASE)/message/include
CODE_INC += -I$(TLR_BASE)/sdk/rootfs/src/amit_open/OasisHeader

SRC_DIR = $(LIB_BASE)/libdigiutils
SRC_OBJS = show_parse_helper.o

OBJS=cellular_work.o cellular_show_json.o show_cellular_test.o strlcpy.o

EXE=utest
all: $(EXE) run

run: $(EXE)
	@for exe in $^ ; do \
		./$$exe || exit 1 ; \
	done

$(EXE): $(OBJS) $(SRC_OBJS)
	@$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	@$(CC) $(CODE_INC) $(CFLAGS) -c -o $@ $<
%.o: $(CODE_DIR)/%.c $(CODE_DIR)/%.h
	@$(CC) $(CODE_INC) $(CFLAGS) -c -o $@ $<
%.o: $(CODE_DIR)/%.c
	@$(CC) $(CODE_INC) $(CFLAGS) -c -o $@ $<
%.o: $(SRC_DIR)/%.c
	@$(CC) $(CODE_INC) $(CFLAGS) -c -o $@ $<

clean:
	@rm -f *.o $(EXE)

.PHONY: all clean run
