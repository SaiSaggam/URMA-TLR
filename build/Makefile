TARGET =
INSTALL_ROOT = /tmp
TLR_BASE := $(shell cd ..; pwd)
export TLR_BASE

include $(TLR_BASE)/build/makefile.globals

TARGETS = ../tools/command_definition ../lib/config_mgr ../lib/libdigiutils ../lib/message ../lib/libifuptime \
          ../tools/sendmsg ../tools/sendmsg_test ../tools/led_control ../ui/cli ../commands/action/date \
          ../commands/show/ip_stats ../commands/show/eth_stats ../commands/action/save_config ../commands/action/reboot \
          ../commands/action/clear ../commands/show/log ../commands/apply/system \
          ../commands/apply/serial ../commands/apply/lan ../commands/show/serial \
          ../commands/apply/eth ../commands/apply/ip \
          ../commands/apply/route ../commands/apply/ssh ../commands/show/route ../commands/show/dsl \
          ../ui/cloud ../commands/show/cloud \
          ../tools/resource_monitor ../tools/ifuptime ../tools/nlmon ../tools/dsl_monitor \
          ../tools/adsl_cmd ../tools/ifmanager ../tools/facres \
          ../tools/eth_monitor \
          ../tools/vpn_monitor \
          ../tools/snmp/sub_agent \
          ../commands/apply/cellular \
          ../commands/show/system ../commands/show/config \
          ../commands/action/firewall \
          ../commands/show/ipsec ../commands/show/firewall \
          ../commands/show/lan \
          ../commands/show/manufacture \
          ../unit_test \
	  ../misc


ifneq ($(TARGET), )
TARGETS += ../lib/libcellular ../tools/cellular_monitor ../commands/apply/defroute
else
CLEAN_TARGETS += ../lib/libcellular ../tools/cellular_monitor ../commands/apply/defroute
endif

TARGETS_CLEAN = $(addsuffix .clean,$(TARGETS) $(CLEAN_TARGETS))
TARGETS_INSTALL = $(addsuffix .install,$(TARGETS) )

export TARGET
export INSTALL_ROOT

.PHONY: all
all: $(TARGETS)

.PHONY: $(TARGETS)

../misc:
	@echo $(TARGET)
	$(MAKE) -C $@ all

../tools/command_definition:
	@echo $(TARGET)
	$(MAKE) -C $@ all

../tools/resource_monitor:
	@echo $(TARGET)
	$(MAKE) -C $@ all

../tools/snmp/sub_agent:
	@echo $(TARGET)
	$(MAKE) -C $@ all

../lib/message:
	$(MAKE) -C $@ all install

../tools/sendmsg:
../tools/sendmsg_test:
	$(MAKE) -C $@ all install

../tools/led_control:
	$(MAKE) -C $@ all install

../lib/libcellular:
	$(MAKE) -C $@ all install

../lib/libdigiutils:
	$(MAKE) -C $@ all install

../lib/libifuptime:
	$(MAKE) -C $@ all install

../tools/ifuptime:
	$(MAKE) -C $@ all install

../tools/nlmon:
	$(MAKE) -C $@ all install

../tools/ifmanager:
	$(MAKE) -C $@ all install

../tools/adsl_cmd:
	$(MAKE) -C $@ all install

../tools/facres:
	$(MAKE) -C $@ all install

../tools/dsl_monitor:
../tools/lan_monitor:
../tools/vpn_monitor:
../tools/eth_monitor:
	$(MAKE) -C $@ all install

../lib/config_mgr:
	$(MAKE) -C $@ all install

digi_cmds:
	$(MAKE) -C $@ all

../ui/cli:
	$(MAKE) -C $@ all install

../ui/cloud:
	$(MAKE) -C $@ all install

../unit_test:
	$(MAKE) -C $@ all

../commands/action/date:
	$(MAKE) -C $@ all install

../commands/show/ip_stats:
	$(MAKE) -C $@ all install

../commands/show/ipsec:
	$(MAKE) -C $@ all install

../commands/show/eth_stats:
	$(MAKE) -C $@ all install

../commands/show/log:
	$(MAKE) -C $@ all install

../commands/show/route:
	$(MAKE) -C $@ all install

../commands/show/dsl:
	$(MAKE) -C $@ all install

../commands/show/cloud:
	$(MAKE) -C $@ all install

../commands/action/save_config:
	$(MAKE) -C $@ all install

../commands/action/reboot:
	$(MAKE) -C $@ all install

../commands/action/clear:
	$(MAKE) -C $@ all install

../commands/action/update:
	$(MAKE) -C $@ all install

../commands/apply/system:
	$(MAKE) -C $@ all install

../commands/apply/eth:
	$(MAKE) -C $@ all install

../commands/apply/defroute:
	$(MAKE) -C $@ all install

../commands/apply/route:
	$(MAKE) -C $@ all install

../commands/apply/ip:
	$(MAKE) -C $@ all install

../commands/apply/ssh:
	$(MAKE) -C $@ all install

../commands/apply/lan:
	$(MAKE) -C $@ all install

../commands/apply/serial:
	$(MAKE) -C $@ all install

../commands/show/serial:
	$(MAKE) -C $@ all install

../commands/show/system:
	$(MAKE) -C $@ all install

../commands/show/manufacture:
	$(MAKE) -C $@ all install

../commands/show/config:
	$(MAKE) -C $@ all install

../tools/cellular_monitor:
	@echo $(TARGET)
	$(MAKE) -C $@ all install

../commands/action/firewall:
	@echo $(TARGET)
	$(MAKE) -C $@ all install

../unit_test:
	$(MAKE) -C $@ all install

.PHONY: clean
clean: $(TARGETS_CLEAN)

.PHONY: $(TARGETS_CLEAN)
$(TARGETS_CLEAN):
	$(MAKE) -C $(basename $@) clean

.PHONY: install
install: $(TARGETS_INSTALL)

.PHONY: $(TARGETS_INSTALL)
$(TARGETS_INSTALL):
	$(MAKE) -C $(basename $@) install

.PHONY: run
run:
	@LD_LIBRARY_PATH=$(DESTDIR)/usr/lib config_mgr/config_mgr &
	@LD_LIBRARY_PATH=$(DESTDIR)/usr/lib cli/cli
	@-pkill config_mgr
