#define the location of the cpputest framework Makefile
CPPUTEST_TOPDIR ?= $(CURDIR)/../test_frameworks/cpputest

export CPPUTEST_TOPDIR

TARGETS=\
	commands/show/system \
 	lib/config_mgr \
	lib/libdigiutils \
	monitors/vpn_monitor \
	monitors/bridge_monitor \
	monitors/dsl_monitor \
	tools/snmp/sub_agent \
	commands/show/ipsec \
	commands/show/manufacture \
	commands/wan/dhcp \
	commands/apply/wan \
	monitors/wifi_monitor

TARGETS_CLEAN = $(addsuffix .clean,$(TARGETS))

.PHONY: all
all: $(TARGETS)

.PHONY: $(TARGETS)
$(TARGETS):
	$(MAKE) -C $@ all

.PHONY: clean
clean: $(TARGETS_CLEAN)

.PHONY: $(TARGETS_CLEAN)
$(TARGETS_CLEAN):
	$(MAKE) -C $(basename $@) clean
