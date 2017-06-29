CC=gcc
CXX=g++
RM=rm -f
TESTCFLAGS=-g -include CppUTest/MemoryLeakDetectorNewMacros.h
TESTCPPFLAGS=-g -include CppUTest/MemoryLeakDetectorNewMacros.h
CPPFLAGS=-g 
LDFLAGS=-g $(shell root-config --ldflags)
TESTLDLIBS=-lCppUTest -lCppUTestExt

TESTSRCS=AllTests.cpp eth_status.cpp test.cpp
TESTOBJS=$(subst .cpp,.to,$(TESTSRCS))
SRCS=production.cpp eth_status_helper.cpp eth_status.cpp
OBJS=$(subst .cpp,.o,$(SRCS))

all: production test

production: $(OBJS)
	$(CXX) $(LDFLAGS) -o production $(OBJS) 

test: $(TESTOBJS)
	$(CXX) $(TESTLDFLAGS) -o test $(TESTOBJS) $(TESTLDLIBS) 
	./test

depend: .depend

.depend: $(SRCS)
	rm -f ./.depend
	$(CXX) $(CPPFLAGS) -MM $^>>./.depend;

clean:
	$(RM) $(TESTOBJS) $(OBJS) test production

dist-clean: clean
	$(RM) *~ .depend

include .depend

%.to: %.cpp
	echo $(CXX) -c $(TESTCFLAGS) $(TESTCPPFLAGS) $< -o $@
	$(CXX) -c $(TESTCFLAGS) $(TESTCPPFLAGS) $< -o $@
