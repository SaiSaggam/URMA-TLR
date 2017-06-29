TARGETS=\
	action/firmware_update \
	action/update_dsl \
	action/save_config \
	action/firewall \
	digi_date \
	apply/ip \
	apply/user \
	show/route \
	show_eth \
	show/cellular \
	show/ipsec \
	apply_serial \
	apply_lan \
	show/firewall \
	cli \
	config_mgr \
	resource_monitor \
	libifuptime \
	lib/libdigiutils \
	lib/message \
	eth_monitor \
	cellular_monitor \
	dsl_monitor \
	monitors/ifmanager \
	show/lan \
	show/failover \
	show/manufacture \
	ui/web \
	ui/cloud \
	show/cloud \
	facres \
	apply/dhcp_server

#A (hopefully) temporary hack to allow certain tests to be excluded from "make test".
TARGETS := $(filter-out $(EXCLUDE),$(TARGETS))

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
