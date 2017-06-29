TLR_BASE ?= $(shell cd ../../..; pwd)
export TLR_BASE
include $(TLR_BASE)/build/makefile.globals

CODE_DIR = $(UI_BASE)/cli
CFG_DIR  = $(LIB_BASE)/config_mgr


CC ?= gcc
CFLAGS=-Wall -Werror -fdata-sections -ffunction-sections -g --std=gnu99 -D _CUNIT_
CFLAGS+=-D_GNU_SOURCE
LDFLAGS=-Wl,--gc-sections -L/usr/local/lib -lcunit

CODE_INC = -I$(CODE_DIR) -I$(CFG_DIR) -I$(UTILS_BASE)/include -I$(TLR_BASE)/include

OBJS = cli.o line_ed.o cli_test.o cli_autocomplete.o cli_help.o cli_tree.o cli_cmd.o parse.o strlcpy.o
PARSEOBJS = parse_input_test.o parse.o cli.o firewall_utils.o
TIMEOBJS = time_test.o cli_timeout.o
MAIN_TEST=utest
PARSE_TEST=parsetest
TIME_TEST=time_test

EXES=$(MAIN_TEST) $(TIME_TEST) $(PARSE_TEST)

all: $(EXES) run

run: $(EXES)
		@for exe in $^ ; do \
			./$$exe || exit 1 ; \
		done

$(MAIN_TEST): $(OBJS)
	@$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(PARSE_TEST): $(PARSEOBJS)
	@$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(TIME_TEST): $(TIMEOBJS)
	@$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	@$(CC) $(CODE_INC) $(CFLAGS) -c -o $@ $<
%.o: $(CODE_DIR)/%.c $(CODE_DIR)/%.h
	@$(CC) $(CODE_INC) $(CFLAGS) -c -o $@ $<
%.o: $(CODE_DIR)/%.c
	@$(CC) $(CODE_INC) $(CFLAGS) -c -o $@ $<
%.o: $(CFG_DIR)/%.c
	@$(CC) $(CODE_INC) $(CFLAGS) -c -o $@ $<
%.o: $(UTILS_BASE)/%.c
	@$(CC) $(CODE_INC) $(CFLAGS) -c -o $@ $<

clean:
	@rm -f *.o $(EXES)


.PHONY: all clean run
