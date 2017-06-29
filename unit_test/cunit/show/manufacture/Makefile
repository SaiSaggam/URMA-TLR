TLR_BASE ?= $(shell cd ../../../..; pwd)
export TLR_BASE
include $(TLR_BASE)/build/makefile.globals

CODE_UNDER_TEST_DIR = $(COMMANDS_BASE)/show/manufacture
DIGIUTILS_DIR = $(LIB_BASE)/libdigiutils

CC?=gcc
CFLAGS=-g -Wall -Werror --std=gnu99 -D_CUNIT_ -fdata-sections -ffunction-sections
LDFLAGS=-Wl,--gc-sections -L/usr/local/lib -lcunit

CODE_INC=-I. -I$(CODE_UNDER_TEST_DIR) \
		-I$(ROOTFS_BASE)/usr/include \
		-I$(UTILS_BASE)/include \
		-I$(TLR_BASE)/include \
		-I$(ROOTFS_BASE)/src/include \
		-I$(LIB_BASE)/config_mgr \
		-I$(LIB_BASE)/libcellular

CFLAGS+=-D_GNU_SOURCE
CFLAGS+=$(CODE_INC)

MAC_ADDRESS_TEST_OBJS=mac_address_test.o mac_address.o
MAC_ADDRESS_TEST_EXE=mac_address_test
FIRMWARE_CHECKSUM_TEST_OBJS=firmware_checksum_test.o firmware_checksum.o
FIRMWARE_CHECKSUM_TEST_EXE=firmware_checksum_test
CELLULAR_TEST_OBJS=cellular_test.o strlcpy.o cellular.o
CELLULAR_TEST_EXE=cellular_test
USB_VERSION_TEST_OBJS=usb_version_test.o strlcpy.o usb_version.o strip_whitespace.o get_field.o
USB_VERSION_TEST_EXE=usb_version_test
DSL_VERSION_TEST_OBJS=dsl_version_test.o strlcpy.o dsl_version.o strip_whitespace.o get_field.o
DSL_VERSION_TEST_EXE=dsl_version_test

EXES=$(MAC_ADDRESS_TEST_EXE) $(FIRMWARE_CHECKSUM_TEST_EXE) $(CELLULAR_TEST_EXE) $(USB_VERSION_TEST_EXE) $(DSL_VERSION_TEST_EXE)

all: $(EXES) run

run: $(EXES)
	@for exe in $^ ; do \
		./$$exe || exit 1 ; \
	done

$(CELLULAR_TEST_EXE): $(CELLULAR_TEST_OBJS)
	@$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(MAC_ADDRESS_TEST_EXE): $(MAC_ADDRESS_TEST_OBJS)
	@$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(FIRMWARE_CHECKSUM_TEST_EXE): $(FIRMWARE_CHECKSUM_TEST_OBJS)
	@$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(USB_VERSION_TEST_EXE): $(USB_VERSION_TEST_OBJS)
	@$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(DSL_VERSION_TEST_EXE): $(DSL_VERSION_TEST_OBJS)
	@$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	@$(CC) $(CFLAGS) -c -o $@ $<
%.o: $(CODE_UNDER_TEST_DIR)/%.c $(CODE_UNDER_TEST_DIR)/%.h
	@$(CC) $(CFLAGS) -c -o $@ $<
%.o: $(CODE_UNDER_TEST_DIR)/%.c
	@$(CC) $(CFLAGS) -c -o $@ $<
%.o: $(DIGIUTILS_DIR)/%.c $(DIGIUTILS_DIR)/%.h
	@$(CC) $(CFLAGS) -c -o $@ $<
%.o: $(DIGIUTILS_DIR)/%.c
	@$(CC) $(CFLAGS) -c -o $@ $<

clean:
	@rm -f *.o $(EXES)

.PHONY: all clean run

