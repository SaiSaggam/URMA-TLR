#
#  Copyright (C) 2016 by Digi International Inc. All rights reserved.
#
TARGET ?= mips-linux-uclibc
KERNEL_DIR ?= $(SDK_BASE)/linux/src/

SRC_FILES = sub_agent.c \
			oid_to_parameter.c \
			snmp_utils.c \
			oid_to_element.c \
			oid_to_group.c \
			get_parameter_value.c \
			snmp_agent_debug.c \
			request_handlers.c \
			get_config_value.c \
			get_show_value.c \
			create_new_oid.c \
			index_element.c \
			find_element.c \
			read_show_element.c \
			set_parameter_value.c \
			apply_config.c

EXE=sub_agent

all: $(EXE)

CFLAGS = -I$(TLR_BASE)/sdk/rootfs/src/open_src/OSNMP/net-snmp-5.7.3/include 
CFLAGS += -I$(TLR_BASE)/sdk/rootfs/src/open_src/OSNMP/net-snmp-5.7.3/include/net-snmp
CFLAGS += -I$(TLR_BASE)/include -D_GNU_SOURCE

LFLAGS  = $(LIBPATH) -L$(TLR_BASE)/sdk/rootfs/root/lib -L$(TLR_BASE)/sdk/rootfs/root/usr/lib
LFLAGS += -lconfig_mgr -luci -lubox -ljson-c -lm -ldigiutils -lamitodm -lssl -lcrypto

$(EXE): $(SRC_FILES)
	echo $(LFLAGS)
	$(TLR_BASE)/sdk/rootfs/src/open_src/OSNMP/net-snmp-5.7.3/net-snmp-config --prefix=$(INSTALL_ROOT)/usr --compile-subagent $(EXE) --cflags '$(CFLAGS)' --ldflags '$(LFLAGS)' $(SRC_FILES)

clean:
	rm -f $(EXE)

install:
	-mkdir -p $(INSTALL_ROOT)/usr/bin
	install -m 0777 $(EXE) $(INSTALL_ROOT)/usr/bin

.PHONY: all clean install


