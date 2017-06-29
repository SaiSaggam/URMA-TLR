# base location of production code
TLR_BASE ?= $(shell cd ../../..; pwd)
export TLR_BASE
include $(TLR_BASE)/build/makefile.globals

CODE_DIR = $(LIB_BASE)/config_mgr

CC ?= gcc
INC_DIR = -I. -I$(SRC_DIR) -I$(LIB_BASE)/config_mgr -I$(UTILS_BASE)/include 
INC_DIR += -I$(TLR_BASE)/include -I/usr/local/include/uci -I$(OPENSRC_BASE)/json-c/src/

INC_DIR += -I$(TLR_BASE)/sdk/rootfs/src/open_src/uci/

CFLAGS =  $(INC_DIR) -g -Wall -Werror --std=gnu99 -D_CUNIT_ -fdata-sections -ffunction-sections -D_GNU_SOURCE
CFLAGS += -fprofile-arcs -ftest-coverage

LDFLAGS=-Wl,--gc-sections -L/usr/local/lib -lcunit -lm -ljson-c -lcrypto

CODE_INC=-I$(CODE_DIR)

CONFIG_OBJS=config_mgr_test.o config_valid.o config_table.o config_cmd.o validate.o
CONFIG_EXE=utest_config

UTILS_OBJS=config_util_test.o config_valid.o config_table.o config_cmd.o config_util.o validate.o
UTILS_EXE=utest_utils
UTILS_SRC=config_util.c

STATUS_OBJS=status_test.o status_test_helper.o config_valid.o config_table.o status.o strlcpy.o
STATUS_EXE=utest_status
STATUS_SRC=status.c

PWD_GEN_OBJS=password_gen_test.o password_gen.o base32.o
PWD_GEN_EXE=utest_pwd_gen

TLR_BACKUP_OBJS=tlr_backup.o test_tlr_backup.o
TLR_BACKUP_EXE=utest_tlr_backup

COVERAGE_SRCS=$(UTILS_SRC) $(STATUS_SRC)

EXES=$(CONFIG_EXE) $(UTILS_EXE) $(STATUS_EXE) $(PWD_GEN_EXE) $(TLR_BACKUP_EXE)

all: $(EXES) run coverage

run: $(EXES)
	@for exe in $^ ; do \
		./$$exe || exit 1 ; \
	done

$(CONFIG_EXE): $(CONFIG_OBJS)
	@$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(UTILS_EXE): $(UTILS_OBJS)
	@$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(STATUS_EXE): $(STATUS_OBJS)
	@$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(PWD_GEN_EXE): $(PWD_GEN_OBJS)
	@$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(TLR_BACKUP_EXE): $(TLR_BACKUP_OBJS)
	@$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	@$(CC) $(CODE_INC) $(CFLAGS) -c -o $@ $<
%.o: $(CODE_DIR)/%.c
	@$(CC) $(CODE_INC) $(CFLAGS) -c -o $@ $<
%.o: $(UTILS_BASE)/%.c
	@$(CC) $(UTILS_BASE) $(CFLAGS) -c -o $@ $<

clean:
	@rm -f *.o *.gcno *.gcov *.gcda $(EXES)

coverage:
	@gcov $(COVERAGE_SRCS)

.PHONY: all clean run coverage



