TLR_BASE ?= $(shell cd ../../..; pwd)
export TLR_BASE
include $(TLR_BASE)/build/makefile.globals

CODE_DIR=$(TOOLS_BASE)/cellular_monitor

CC ?= gcc

CFLAGS = -fdata-sections -ffunction-sections -Wall -Werror --std=gnu99 -D_GNU_SOURCE -D_CUNIT_ 
# CFLAGS = -g -Wall -Werror --std=gnu99 -D _CUNIT_
#standard CUNIT settings
USE_BASIC_STYLE=1
USE_AUTOMATED_STYLE=0
USE_CONSOLE_STYLE=0
CFLAGS+=-DUSE_BASIC_STYLE=$(USE_BASIC_STYLE) -DUSE_AUTOMATED_STYLE=$(USE_AUTOMATED_STYLE) -DUSE_CONSOLE_STYLE$=$(USE_CONSOLE_STYLE)

LDFLAGS = -L/usr/local/lib -lcunit

CODE_INC=-I$(CODE_DIR) -I$(ROOTFS_BASE)/src/include -I$(TLR_BASE)/include -I$(LIB_BASE)/config_mgr 
CODE_INC += -I$(LIB_BASE)/libcellular -I$(LIB_BASE)/message/include -I$(LIB_BASE)/libdigiutils/include
CODE_INC += -I$(UTILS_BASE)/include  -I$(INSTALL_ROOT)/usr/include

MONITOR_TEST=monitor_test
MONITOR_OBJS = cellular_monitor.o  cellular_monitor_test.o

STATE_TEST=state_test
STATE_OBJS = cellular_state.o cellular_state_test.o

MODEM_TEST=modem_test
MODEM_OBJS = cellular_modem.o cellular_modem_test.o

MESSAGE_TEST=message_test
MESSAGE_OBJS = cellular_message.o cellular_message_test.o

LED_TEST=led_test
LED_OBJS = cellular_led.o cellular_led_test.o

EXES=$(MONITOR_TEST) $(STATE_TEST) $(MODEM_TEST) $(MESSAGE_TEST) $(LED_TEST)
#EXES=$(MONITOR_TEST) $(LED_TEST)

all: $(EXES) run

run: $(EXES)
	@for exe in $^ ; do \
		./$$exe || exit 1 ; \
	done

$(MONITOR_TEST): $(MONITOR_OBJS)
	@$(CC) $(CODE_INC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(STATE_TEST): $(STATE_OBJS)
	@$(CC) $(CODE_INC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(MODEM_TEST): $(MODEM_OBJS)
	@$(CC) $(CODE_INC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(MESSAGE_TEST): $(MESSAGE_OBJS)
	@$(CC) $(CODE_INC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(LED_TEST): $(LED_OBJS)
	@$(CC) $(CODE_INC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	@$(CC) $(CODE_INC) $(CFLAGS) -c -o $@ $<
%.o: $(CODE_DIR)/%.c $(CODE_DIR)/%.h
	@$(CC) $(CODE_INC) $(CFLAGS) -c -o $@ $<
%.o: $(CODE_DIR)/%.c
	@$(CC) $(CODE_INC) $(CFLAGS) -c -o $@ $<

clean:
	@rm -f *.o $(EXES)

.PHONY: all clean run

