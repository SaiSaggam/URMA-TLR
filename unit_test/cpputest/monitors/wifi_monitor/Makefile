#define the location of the cpputest framework Makefile
CPPUTEST_TOPDIR ?= $(CURDIR)/../../../test_frameworks/cpputest
#define the directory where cpputest.mk can be found
UNIT_TEST_ROOT = $(CURDIR)/../..

include $(UNIT_TEST_ROOT)/cpputest.mk

# base location of production code
TLR_BASE ?= $(shell cd ../../../..; pwd)
include $(TLR_BASE)/build/makefile.globals

CODE_DIR=$(TOOLS_BASE)/wifi_monitor

CC=gcc
CXX=g++
RM=rm -f
CPPFLAGS+=-g -include CppUTest/MemoryLeakDetectorNewMacros.h
LIBPATH=
LIBS += -lgcov
LDFLAGS += -Wl,--gc-sections $(LIBPATH)
GCOV_ARTIFACTS=*.gcov *.gcda *.gcno
ALL_TEST_OBJECTS=*.to
ALL_PRODUCTION_OBJS=*.o


CODE_INC=-I$(TLR_BASE)/include -I$(CODE_DIR) -I$(LIB_BASE)/config_mgr -I$(UTILS_BASE)/include -I$(LIB_BASE)/message/include
#C flags for the code under test

CFLAGS=$(CODE_INC) \
	-g \
	-Wall -Werror --std=gnu99 \
	-fdata-sections -ffunction-sections \
	-D_GNU_SOURCE \
	-DUNIT_TEST \
	-fprofile-arcs -ftest-coverage

CPPFLAGS+= \
	-Wall -Werror \
	-I$(LIB_BASE)/config_mgr \
	-DUNIT_TEST

CONFIG_TESTSRCS=AllTests.cpp test_config.cpp
CONFIG_TESTOBJS:=$(subst .cpp,.to,$(CONFIG_TESTSRCS))
CONFIG_TESTOBJS:=$(subst .c,.o,$(CONFIG_TESTOBJS))
# define the code to test
CONFIG_SRCS=wifi_monitor_config.c
CONFIG_OBJS=$(subst .c,.o,$(CONFIG_SRCS))

# define the test executable name
CONFIG_EXE=test_config


# define the test code
TIMERS_TESTSRCS=AllTests.cpp test_timers.cpp
TIMERS_TESTOBJS:=$(subst .cpp,.to,$(TIMERS_TESTSRCS))
TIMERS_TESTOBJS:=$(subst .c,.o,$(TIMERS_TESTOBJS))
# define the code to test
TIMERS_SRCS=wifi_monitor_timers.c
TIMERS_OBJS=$(subst .c,.o,$(TIMERS_SRCS))

# define the test executable name
TIMERS_EXE=test_timers

# define the test code
MESSAGE_TESTSRCS=AllTests.cpp test_message.cpp
MESSAGE_TESTOBJS:=$(subst .cpp,.to,$(MESSAGE_TESTSRCS))
MESSAGE_TESTOBJS:=$(subst .c,.o,$(MESSAGE_TESTOBJS))
# define the code to test
MESSAGE_SRCS=wifi_monitor_message.c
MESSAGE_OBJS=$(subst .c,.o,$(MESSAGE_SRCS))

# define the test executable name
MESSAGE_EXE=test_message

# define the test code
IFCONFIG_TESTSRCS=AllTests.cpp test_ifconfig_commands.cpp
IFCONFIG_TESTOBJS:=$(subst .cpp,.to,$(IFCONFIG_TESTSRCS))
IFCONFIG_TESTOBJS:=$(subst .c,.o,$(IFCONFIG_TESTOBJS))
# define the code to test
IFCONFIG_SRCS=wifi_monitor_ifconfig_commands.c
IFCONFIG_OBJS=$(subst .c,.o,$(IFCONFIG_SRCS))

# define the test executable name
IFCONFIG_EXE=test_ifconfig_commands

# define the test code
EVENT_HANDLERS_TESTSRCS=AllTests.cpp test_event_handlers.cpp
EVENT_HANDLERS_TESTOBJS:=$(subst .cpp,.to,$(EVENT_HANDLERS_TESTSRCS))
EVENT_HANDLERS_TESTOBJS:=$(subst .c,.o,$(EVENT_HANDLERS_TESTOBJS))
# define the code to test
EVENT_HANDLERS_SRCS=wifi_monitor_event_handlers.c
EVENT_HANDLERS_OBJS=$(subst .c,.o,$(EVENT_HANDLERS_SRCS))

# define the test executable name
EVENT_HANDLERS_EXE=test_event_handlers

# define the test code
EVENT_ALLOC_TESTSRCS=AllTests.cpp test_event_alloc.cpp
EVENT_ALLOC_TESTOBJS:=$(subst .cpp,.to,$(EVENT_ALLOC_TESTSRCS))
EVENT_ALLOC_TESTOBJS:=$(subst .c,.o,$(EVENT_ALLOC_TESTOBJS))
# define the code to test
EVENT_ALLOC_SRCS=wifi_monitor_event_alloc.c
EVENT_ALLOC_OBJS=$(subst .c,.o,$(EVENT_ALLOC_SRCS))

# define the test executable name
EVENT_ALLOC_EXE=test_event_alloc

# define the test code
EVENT_TESTSRCS=AllTests.cpp test_event.cpp
EVENT_TESTOBJS:=$(subst .cpp,.to,$(EVENT_TESTSRCS))
EVENT_TESTOBJS:=$(subst .c,.o,$(EVENT_TESTOBJS))
# define the code to test
EVENT_SRCS=wifi_monitor_event.c
EVENT_OBJS=$(subst .c,.o,$(EVENT_SRCS))

# define the test executable name
EVENT_EXE=test_event

ALL_PRODUCTION_SRCS = $(TIMERS_SRCS) \
					  $(MESSAGE_SRCS) \
					  $(CONFIG_SRCS) \
					  $(IFCONFIG_SRCS) \
					  $(EVENT_HANDLERS_SRCS) \
					  $(EVENT_ALLOC_SRCS) \
					  $(EVENT_SRCS)

EXES=$(TIMERS_EXE) \
	 $(MESSAGE_EXE) \
	 $(CONFIG_EXE) \
	 $(IFCONFIG_EXE) \
	 $(EVENT_HANDLERS_EXE) \
	 $(EVENT_ALLOC_EXE) \
	 $(EVENT_EXE)

all: $(EXES) run coverage

$(EVENT_EXE): $(EVENT_OBJS) $(EVENT_TESTOBJS)
	@$(CXX) $(LDFLAGS) -o $@ $^ $(LIBS)

$(EVENT_ALLOC_EXE): $(EVENT_ALLOC_OBJS) $(EVENT_ALLOC_TESTOBJS)
	@$(CXX) $(LDFLAGS) -o $@ $^ $(LIBS)

$(EVENT_HANDLERS_EXE): $(EVENT_HANDLERS_OBJS) $(EVENT_HANDLERS_TESTOBJS)
	@$(CXX) $(LDFLAGS) -o $@ $(EVENT_HANDLERS_OBJS) $(EVENT_HANDLERS_TESTOBJS) $(LIBS)

$(CONFIG_EXE): $(CONFIG_OBJS) $(CONFIG_TESTOBJS)
	@$(CXX)  $(LDFLAGS) -o $@ $(CONFIG_OBJS) $(CONFIG_TESTOBJS) $(LIBS)

$(IFCONFIG_EXE): $(IFCONFIG_OBJS) $(IFCONFIG_TESTOBJS)
	@$(CXX) $(LDFLAGS) -o $@ $(IFCONFIG_OBJS) $(IFCONFIG_TESTOBJS) $(LIBS)

$(TIMERS_EXE): $(TIMERS_OBJS) $(TIMERS_TESTOBJS)
	@$(CXX) $(LDFLAGS) -o $@ $(TIMERS_OBJS) $(TIMERS_TESTOBJS) $(LIBS)

$(MESSAGE_EXE): $(MESSAGE_OBJS) $(MESSAGE_TESTOBJS)
	@$(CXX) $(LDFLAGS) -o $@ $(MESSAGE_OBJS) $(MESSAGE_TESTOBJS) $(LIBS)

run: $(EXES)
	@for exe in $^ ; do \
		./$$exe || exit 1 ;\
	done


coverage:
	@gcov $(ALL_PRODUCTION_SRCS)

clean:
	@$(RM) $(EXES) $(ALL_TEST_OBJECTS) $(ALL_PRODUCTION_OBJS) $(GCOV_ARTIFACTS)

dist-clean: clean

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.o: $(CODE_DIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.to: %.cpp
	$(CXX) -c $(CPPFLAGS) $(CODE_INC) $< -o $@

.PHONY: all clean coverage run dist-clean

