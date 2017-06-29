#define the location of the cpputest framework Makefile
CPPUTEST_TOPDIR ?= $(CURDIR)/../../../test_frameworks/cpputest
#define the directory where cpputest.mk can be found
UNIT_TEST_ROOT = $(CURDIR)/../..

include $(UNIT_TEST_ROOT)/cpputest.mk

# base location of production code
TLR_BASE ?= $(shell cd ../../../..; pwd)
include $(TLR_BASE)/build/makefile.globals

CODE_DIR=$(TOOLS_BASE)/bridge_monitor

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
	-DBRIDGE_MONITOR_DEBUG=0 \
	-fprofile-arcs -ftest-coverage

CPPFLAGS+= \
	-Wall -Werror \
	-I$(LIB_BASE)/config_mgr \
	-DUNIT_TEST

# define the test code
INTERFACES_TESTSRCS=AllTests.cpp test_interfaces.cpp test_config_commands.cpp test_interfaces_helper.c
INTERFACES_TESTOBJS:=$(subst .cpp,.to,$(INTERFACES_TESTSRCS))
INTERFACES_TESTOBJS:=$(subst .c,.o,$(INTERFACES_TESTOBJS))
# define the code to test
INTERFACES_SRCS=bridge_interfaces.c bridge_monitor_config_commands.c
INTERFACES_OBJS=$(subst .c,.o,$(INTERFACES_SRCS))

# define the test executable name
INTERFACES_EXE=test_interfaces

# define the test code
EVENT_TESTSRCS=AllTests.cpp test_events.cpp events_helper.c
EVENT_TESTOBJS:=$(subst .cpp,.to,$(EVENT_TESTSRCS))
EVENT_TESTOBJS:=$(subst .c,.o,$(EVENT_TESTOBJS))
# define the code to test
EVENT_SRCS=bridge_monitor_event.c
EVENT_OBJS=$(subst .c,.o,$(EVENT_SRCS))

# define the test executable name
EVENT_EXE=test_events

# define the test code
EVENT_ALLOC_TESTSRCS=AllTests.cpp test_event_alloc.cpp event_alloc_helper.c
EVENT_ALLOC_TESTOBJS:=$(subst .cpp,.to,$(EVENT_ALLOC_TESTSRCS))
EVENT_ALLOC_TESTOBJS:=$(subst .c,.o,$(EVENT_ALLOC_TESTOBJS))
# define the code to test
EVENT_ALLOC_SRCS=bridge_monitor_event_alloc.c
EVENT_ALLOC_OBJS=$(subst .c,.o,$(EVENT_ALLOC_SRCS))

# define the test executable name
EVENT_ALLOC_EXE=test_event_alloc

# define the test code
EVENT_HANDLERS_TESTSRCS=AllTests.cpp test_event_handlers.cpp
EVENT_HANDLERS_TESTOBJS:=$(subst .cpp,.to,$(EVENT_HANDLERS_TESTSRCS))
EVENT_HANDLERS_TESTOBJS:=$(subst .c,.o,$(EVENT_HANDLERS_TESTOBJS))
# define the code to test
EVENT_HANDLERS_SRCS=bridge_monitor_event_handlers.c
EVENT_HANDLERS_OBJS=$(subst .c,.o,$(EVENT_HANDLERS_SRCS))

# define the test executable name
EVENT_HANDLERS_EXE=test_event_handlers

# define the test code
MESSAGE_TESTSRCS=AllTests.cpp test_message.cpp
MESSAGE_TESTOBJS:=$(subst .cpp,.to,$(MESSAGE_TESTSRCS))
MESSAGE_TESTOBJS:=$(subst .c,.o,$(MESSAGE_TESTOBJS))
# define the code to test
MESSAGE_SRCS=bridge_monitor_message.c
MESSAGE_OBJS=$(subst .c,.o,$(MESSAGE_SRCS))

# define the test executable name
MESSAGE_EXE=test_message

# define the test code
CONFIG_TESTSRCS=AllTests.cpp test_config.cpp test_config_helper.c
CONFIG_TESTOBJS:=$(subst .cpp,.to,$(CONFIG_TESTSRCS))
CONFIG_TESTOBJS:=$(subst .c,.o,$(CONFIG_TESTOBJS))
# define the code to test
CONFIG_SRCS=bridge_monitor_config.c
CONFIG_OBJS=$(subst .c,.o,$(CONFIG_SRCS))

# define the test executable name
CONFIG_EXE=test_config

# define the test code
TIMERS_TESTSRCS=AllTests.cpp test_timers.cpp
TIMERS_TESTOBJS:=$(subst .cpp,.to,$(TIMERS_TESTSRCS))
TIMERS_TESTOBJS:=$(subst .c,.o,$(TIMERS_TESTOBJS))
# define the code to test
TIMERS_SRCS=bridge_monitor_timers.c
TIMERS_OBJS=$(subst .c,.o,$(TIMERS_SRCS))

# define the test executable name
TIMERS_EXE=test_timers

# define the test code
INTERFACES_UPDATE_TESTSRCS=AllTests.cpp test_interfaces_update.cpp test_config_helper.c
INTERFACES_UPDATE_TESTOBJS:=$(subst .cpp,.to,$(INTERFACES_UPDATE_TESTSRCS))
INTERFACES_UPDATE_TESTOBJS:=$(subst .c,.o,$(INTERFACES_UPDATE_TESTOBJS))
# define the code to test
INTERFACES_UPDATE_SRCS=bridge_interfaces_update.c
INTERFACES_UPDATE_OBJS=$(subst .c,.o,$(INTERFACES_UPDATE_SRCS))

# define the test executable name
INTERFACES_UPDATE_EXE=test_interfaces_update



ALL_PRODUCTION_SRCS=$(INTERFACES_SRCS) $(EVENT_SRCS) $(EVENT_ALLOC_SRCS) $(EVENT_HANDLERS_SRCS)
ALL_PRODUCTION_SRCS += $(MESSAGE_SRCS) $(CONFIG_SRCS) $(INTERFACES_UPDATE_SRCS) $(TIMERS_SRCS)

EXES =$(INTERFACES_EXE) $(EVENT_EXE) $(EVENT_ALLOC_EXE)  $(EVENT_HANDLERS_EXE) $(MESSAGE_EXE)
EXES += $(CONFIG_EXE) $(INTERFACES_UPDATE_EXE) $(TIMERS_EXE)

all: $(EXES) run coverage

$(EVENT_HANDLERS_EXE): $(EVENT_HANDLERS_OBJS) $(EVENT_HANDLERS_TESTOBJS)
	@$(CXX)  $(LDFLAGS) -o $@ $(EVENT_HANDLERS_TESTOBJS) $(EVENT_HANDLERS_OBJS) $(LIBS)

$(EVENT_ALLOC_EXE): $(EVENT_ALLOC_OBJS) $(EVENT_ALLOC_TESTOBJS)
	@$(CXX)  $(LDFLAGS) -o $@ $(EVENT_ALLOC_TESTOBJS) $(EVENT_ALLOC_OBJS) $(LIBS)

$(EVENT_EXE): $(EVENT_OBJS) $(EVENT_TESTOBJS)
	@$(CXX)  $(LDFLAGS) -o $@ $(EVENT_TESTOBJS) $(EVENT_OBJS) $(LIBS)

$(INTERFACES_EXE): $(INTERFACES_OBJS) $(INTERFACES_TESTOBJS)
	@$(CXX)  $(LDFLAGS) -o $@ $(INTERFACES_TESTOBJS) $(INTERFACES_OBJS) $(LIBS)

$(MESSAGE_EXE): $(MESSAGE_OBJS) $(MESSAGE_TESTOBJS)
	@$(CXX)  $(LDFLAGS) -o $@ $(MESSAGE_TESTOBJS) $(MESSAGE_OBJS) $(LIBS)

$(CONFIG_EXE): $(CONFIG_OBJS) $(CONFIG_TESTOBJS)
	@$(CXX)  $(LDFLAGS) -o $@ $(CONFIG_TESTOBJS) $(CONFIG_OBJS) $(LIBS)

$(TIMERS_EXE): $(TIMERS_OBJS) $(TIMERS_TESTOBJS)
	@$(CXX)  $(LDFLAGS) -o $@ $(TIMERS_TESTOBJS) $(TIMERS_OBJS) $(LIBS)

$(INTERFACES_UPDATE_EXE): $(INTERFACES_UPDATE_OBJS) $(INTERFACES_UPDATE_TESTOBJS)
	@$(CXX)  $(LDFLAGS) -o $@ $(INTERFACES_UPDATE_TESTOBJS) $(INTERFACES_UPDATE_OBJS) $(LIBS)

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
	@$(CC) $(CFLAGS) -c -o $@ $<

%.o: $(CODE_DIR)/%.c
	@$(CC) $(CFLAGS) -c -o $@ $<

%.to: %.cpp
	@$(CXX) -c $(CPPFLAGS) $(CODE_INC) $< -o $@

.PHONY: all clean coverage run dist-clean

