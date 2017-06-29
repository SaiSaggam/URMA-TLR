# ***************************************************************************
# Copyright (c) 2011, 2012 Digi International Inc.,
# All rights not expressly granted are reserved.
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this file,
# You can obtain one at http://mozilla.org/MPL/2.0/.
#
# Digi International Inc. 11001 Bren Road East, Minnetonka, MN 55343
#
# ***************************************************************************
# Use GNU C Compiler
CPP = g++
CC = gcc

# Target Platform
CCAPI_SOURCE_DIR = ./source
CCAPI_INCLUDE = ./include
CONNECTOR_DIR = ..
CUSTOM_CONNECTOR_INCLUDE = $(CCAPI_SOURCE_DIR)/cc_ansic_custom_include
CONNECTOR_PUBLIC_INCLUDE = $(CONNECTOR_DIR)/public/include
CONNECTOR_PRIVATE_INCLUDE = $(CONNECTOR_DIR)/private
CONNECTOR_SOURCES = $(CONNECTOR_DIR)/private/connector_api.c $(CONNECTOR_DIR)/public/run/platforms/linux/debug.c $(CONNECTOR_DIR)/public/run/platforms/linux/os.c

TEST_DIR = ./

# CFLAG Definition
CFLAGS += $(DFLAGS)
# Enable Compiler Warnings
CFLAGS += -Winit-self -Wpointer-arith
CFLAGS += -Wformat-security
CFLAGS += -Wformat-y2k -Wcast-align -Wformat-nonliteral
CFLAGS += -Wpadded -Wredundant-decls -Wvariadic-macros
CFLAGS += -Wall -Werror -Wextra -pedantic
CFLAGS += -Wno-error=padded -Wno-error=format-nonliteral -Wno-unused-function -Wno-missing-field-initializers
CFLAGS += -Wno-error=long-long

# Include POSIX and GNU features.
CFLAGS += -D_POSIX_C_SOURCE=200112L -D_GNU_SOURCE
# Include Public Header Files.
CFLAGS += -I. -I$(CONNECTOR_PUBLIC_INCLUDE) -I$(CONNECTOR_PRIVATE_INCLUDE)
CFLAGS += -g -O0

CFLAGS += -DUNIT_TEST -DCONNECTOR_HAS_STDINT_HEADER

TESTS_SOURCES := $(shell find $(TEST_DIR) -name '*.cpp')

CSRCS = $(CONNECTOR_SOURCES) 

CPPSRCS = $(wildcard ./*.cpp) $(TESTS_SOURCES)

# Libraries to Link
LIBS = -lc -lCppUTest -lCppUTestExt -lpthread -lrt

CCFLAGS += $(CFLAGS) -std=c89

# Generated Sample Executable Name.
EXEC_NAME = test

# since each of the samples shares private and platform files, do a clean each time we make
.PHONY:all
all: $(EXEC_NAME)

# Linking Flags.
LDFLAGS += $(DFLAGS) -Wl,-Map,$(EXEC_NAME).map,--sort-common

COBJS = $(CSRCS:.c=.o)
CPPOBJS = $(CPPSRCS:.cpp=.o)

test: $(COBJS) $(CPPOBJS)
	$(CPP) $(CFLAGS) $(LDFLAGS) $^ $(LIBS) -o $@
	./$@
	
.cpp.o:
	$(CPP) $(CFLAGS) -c $< -o $@

.c.o:
	$(CC) -DUNIT_TEST $(CCFLAGS) -c $< -o $@

.PHONY: clean
clean:
	-rm -f $(COBJS) $(CPPOBJS)
