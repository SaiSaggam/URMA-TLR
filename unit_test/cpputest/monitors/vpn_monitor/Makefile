#define the location of the cpputest framework Makefile
CPPUTEST_TOPDIR ?= $(CURDIR)/../../../test_frameworks/cpputest
#define the directory where cpputest.mk can be found
UNIT_TEST_ROOT = $(CURDIR)/../..

include $(UNIT_TEST_ROOT)/cpputest.mk

# base location of production code
TLR_BASE ?= $(shell cd ../../../..; pwd)
include $(TLR_BASE)/build/makefile.globals

CODE_DIR=$(TOOLS_BASE)/vpn_monitor

CC=gcc
CXX=g++
RM=rm -f
CPPFLAGS+=-g -include CppUTest/MemoryLeakDetectorNewMacros.h
LIBPATH=
LIBS += -lgcov
LDFLAGS += -Wl,--gc-sections $(LIBPATH)

CODE_INC=-I$(TLR_BASE)/include -I$(CODE_DIR) -I$(LIB_BASE)/config_mgr -I$(UTILS_BASE)/include -I$(LIB_BASE)/message/include
#C flags for the code under test

CFLAGS=$(CODE_INC) \
	-g \
	-Wall -Werror --std=gnu99 \
	-fdata-sections -ffunction-sections \
	-D_GNU_SOURCE \
	-DUNIT_TEST \
	-DVPN_MONITOR_DEBUG=0 \
	-fprofile-arcs -ftest-coverage 

# define the test code
IPSEC_TESTSRCS=AllTests.cpp test_ipsec.cpp
IPSEC_TESTOBJS=$(subst .cpp,.to,$(IPSEC_TESTSRCS))
# define the code to test
IPSEC_SRCS=ipsec.c
IPSEC_OBJS=$(subst .c,.o,$(IPSEC_SRCS))

# define the test code
CONFIG_TESTSRCS=AllTests.cpp test_config.cpp
CONFIG_TESTOBJS=$(subst .cpp,.to,$(CONFIG_TESTSRCS))
# define the code to test
CONFIG_SRCS=vpn_config.c
CONFIG_OBJS=$(subst .c,.o,$(CONFIG_SRCS))

# define the test code
COUNT_SAS_TESTSRCS=AllTests.cpp test_count_sas.cpp
COUNT_SAS_TESTOBJS=$(subst .cpp,.to,$(COUNT_SAS_TESTSRCS))
# define the code to test
COUNT_SAS_SRCS=count_sas.c
COUNT_SAS_OBJS=$(subst .c,.o,$(COUNT_SAS_SRCS))



# define the test executable name
IPSEC_EXE=test_ipsec
CONFIG_EXE=test_config
COUNT_SAS_EXE=test_count_sas

EXES=$(IPSEC_EXE) $(CONFIG_EXE) $(COUNT_SAS_EXE)

all: $(EXES) run coverage

$(IPSEC_EXE): $(IPSEC_OBJS) $(IPSEC_TESTOBJS)
	@$(CXX)  $(LDFLAGS) -o $@ $(IPSEC_TESTOBJS) $(IPSEC_OBJS) $(LIBS)

$(CONFIG_EXE): $(CONFIG_OBJS) $(CONFIG_TESTOBJS)
	@$(CXX) $(LDFLAGS) -o $@ $(CONFIG_TESTOBJS) $(CONFIG_OBJS) $(LIBS)

$(COUNT_SAS_EXE): $(COUNT_SAS_OBJS) $(COUNT_SAS_TESTOBJS)
	@$(CXX) $(LDFLAGS) -o $@ $(COUNT_SAS_TESTOBJS) $(COUNT_SAS_OBJS) $(LIBS)

run: $(EXES)
	@for exe in $^ ; do \
		./$$exe || exit 1 ; \
	done

coverage:
	@gcov $(IPSEC_SRCS) $(CONFIG_SRCS) $(COUNT_SAS_SRCS)

clean:
	@$(RM) $(IPSEC_TESTOBJS) $(CONFIG_TESTOBJS) $(IPSEC_OBJS) $(CONFIG_OBJS) $(COUNT_SAS_TESTOBJS) $(COUNT_SAS_OBJS) $(EXES) *.gcov *.gcda *.gcno

dist-clean: clean

%.o: %.c
	@$(CC) $(CFLAGS) -c -o $@ $<

%.o: $(CODE_DIR)/%.c
	@$(CC) $(CFLAGS) -c -o $@ $<

%.to: %.cpp
	@$(CXX) -c $(CPPFLAGS) $(CODE_INC) $< -o $@

.PHONY: all clean coverage run dist-clean

