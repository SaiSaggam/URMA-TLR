# Comply with RDS/00008: OASIS Makefile Conventions

ifdef TARGET
    TARGET_PREFIX = $(TARGET)-
endif

ifndef CROSS_COMPILE
    CROSS_COMPILE = $(TARGET_PREFIX)
    CC = $(CROSS_COMPILE)gcc
	CXX = $(CROSS_COMPILE)g++
	LD = $(CROSS_COMPILE)ld
	AR = $(CROSS_COMPILE)ar
	AS = $(CROSS_COMPILE)as
	RANLIB = $(CROSS_COMPILE)ranlib
    STRIP = $(CROSS_COMPILE)strip
endif

# Openswan used flags
CFLAGS+=-I$(COMPO_BASE)/include
LDFLAGS+=-L$(COMPO_BASE)/$(TARGET)-lib

#ifeq (${TARGET}, mipsel-linux-uclibc)
	STRIP_TAG=MIPS
#endif

ifeq ($(IPT_VER), )
SRC=src
else
SRC=src-$(IPT_VER)
endif

DESTDIR=$(COMPO_BASE)/open_src/OIPTables142/install
QOS_EXTRA=n

ifeq ($(QOS), y) 
QOS_EXTRA=n
endif

ifeq ($(CL_QOS), y) 
QOS_EXTRA=y
endif

ifeq ($(SMART_QOS), y)
QOS_EXTRA=y
endif

ifeq (${HANDLE_SMART_QOS}, 1)
QOS_EXTRA=y
endif

ifeq (${HANDLE_RULE_BASED_QOS}, 1)
QOS_EXTRA=y
endif

all: precheck compile 

compile:
	@if [ ! -e $(SRC)/Makefile ]; then \
		cd $(SRC);./configure \
		--prefix=/usr \
		--host=$(TARGET) \
		--with-kernel=$(KERNEL_DIR) \
		--sysconfdir=/etc \
		--with-xtlibdir=/usr/lib/iptables ; \
	fi;
	@rm -rf $(DESTDIR) ; mkdir -p $(DESTDIR)
	@make -C $(SRC) ; make install -C $(SRC) DESTDIR=$(DESTDIR)
	@$(STRIP) $(DESTDIR)/usr/sbin/*
	@$(STRIP) $(DESTDIR)/usr/lib/*.so*
	@$(STRIP) $(DESTDIR)/usr/lib/iptables/*.so*

install: postcheck
	@cp -af $(DESTDIR)/usr/sbin/*					$(INSTALL_ROOT)/usr/sbin/
	@cp -af $(DESTDIR)/usr/lib/libxtables.so*			$(INSTALL_ROOT)/usr/lib
	@-cp -af $(DESTDIR)/usr/lib/libip4tc*.so*			$(INSTALL_ROOT)/usr/lib > /dev/null 2>&1
	@rm -rf $(INSTALL_ROOT)/usr/lib/iptables
	@mkdir -p $(INSTALL_ROOT)/usr/lib/iptables
ifeq ($(IPT_VER), 1.4.21)
	@-cp -af $(DESTDIR)/usr/lib/libip6tc*.so*			$(INSTALL_ROOT)/usr/lib > /dev/null 2>&1
	@cp -af $(DESTDIR)/usr/lib/iptables/*.so			$(INSTALL_ROOT)/usr/lib/iptables
else
	@cp -af $(DESTDIR)/usr/lib/iptables/libipt_DNAT.so		$(INSTALL_ROOT)/usr/lib/iptables
	@cp -af $(DESTDIR)/usr/lib/iptables/libipt_MASQUERADE.so	$(INSTALL_ROOT)/usr/lib/iptables
	@cp -af $(DESTDIR)/usr/lib/iptables/libipt_icmp.so		$(INSTALL_ROOT)/usr/lib/iptables
	@cp -af $(DESTDIR)/usr/lib/iptables/libipt_SNAT.so		$(INSTALL_ROOT)/usr/lib/iptables
	@cp -af $(DESTDIR)/usr/lib/iptables/libipt_LOG.so		$(INSTALL_ROOT)/usr/lib/iptables
	@cp -af $(DESTDIR)/usr/lib/iptables/libxt_MARK.so		$(INSTALL_ROOT)/usr/lib/iptables
	@cp -af $(DESTDIR)/usr/lib/iptables/libxt_standard.so		$(INSTALL_ROOT)/usr/lib/iptables
	@cp -af $(DESTDIR)/usr/lib/iptables/libxt_udp.so		$(INSTALL_ROOT)/usr/lib/iptables
	@cp -af $(DESTDIR)/usr/lib/iptables/libxt_tcp.so		$(INSTALL_ROOT)/usr/lib/iptables
	@cp -af $(DESTDIR)/usr/lib/iptables/libxt_mac.so		$(INSTALL_ROOT)/usr/lib/iptables
	@cp -af $(DESTDIR)/usr/lib/iptables/libxt_state.so		$(INSTALL_ROOT)/usr/lib/iptables
	@cp -af $(DESTDIR)/usr/lib/iptables/libxt_iprange.so		$(INSTALL_ROOT)/usr/lib/iptables
	@cp -af $(DESTDIR)/usr/lib/iptables/libxt_pkttype.so		$(INSTALL_ROOT)/usr/lib/iptables
	@cp -af $(DESTDIR)/usr/lib/iptables/libxt_limit.so		$(INSTALL_ROOT)/usr/lib/iptables
#	@cp -a $(DESTDIR)/usr/lib/iptables/libxt_esp.so			$(INSTALL_ROOT)/usr/lib/iptables
	@cp -af $(DESTDIR)/usr/lib/iptables/libxt_TCPMSS.so		$(INSTALL_ROOT)/usr/lib/iptables
	@cp -af $(DESTDIR)/usr/lib/iptables/libxt_multiport.so		$(INSTALL_ROOT)/usr/lib/iptables
ifneq ($(IPT_VER), 1.4.21)
	@cp -af $(DESTDIR)/usr/lib/iptables/libipt_SPAP.so		$(INSTALL_ROOT)/usr/lib/iptables
endif
	@cp -af $(DESTDIR)/usr/lib/iptables/libipt_REJECT.so		$(INSTALL_ROOT)/usr/lib/iptables
ifneq ($(IPT_VER), 1.4.21)
	@cp -af $(DESTDIR)/usr/lib/iptables/libipt_webstr.so		$(INSTALL_ROOT)/usr/lib/iptables
	@cp -af $(DESTDIR)/usr/lib/iptables/libipt_domain.so		$(INSTALL_ROOT)/usr/lib/iptables
endif
	@cp -af $(DESTDIR)/usr/lib/iptables/libxt_time.so		$(INSTALL_ROOT)/usr/lib/iptables
	@cp -af $(DESTDIR)/usr/lib/iptables/libxt_length.so		$(INSTALL_ROOT)/usr/lib/iptables
	@cp -af $(DESTDIR)/usr/lib/iptables/libxt_hashlimit.so		$(INSTALL_ROOT)/usr/lib/iptables
	@cp -af $(DESTDIR)/usr/lib/iptables/libipt_REDIRECT.so		$(INSTALL_ROOT)/usr/lib/iptables
	@cp -af $(DESTDIR)/usr/lib/iptables/libxt_connlimit.so		$(INSTALL_ROOT)/usr/lib/iptables
	@cp -af $(DESTDIR)/usr/lib/iptables/libxt_mark.so		$(INSTALL_ROOT)/usr/lib/iptables
	@cp -af $(DESTDIR)/usr/lib/iptables/libxt_string.so		$(INSTALL_ROOT)/usr/lib/iptables
	@cp -af $(DESTDIR)/usr/lib/iptables/libxt_statistic.so		$(INSTALL_ROOT)/usr/lib/iptables
	@cp -af $(DESTDIR)/usr/lib/iptables/libxt_NOTRACK.so		$(INSTALL_ROOT)/usr/lib/iptables
ifeq ($(HANDLE_GUESTZONE), 1)
	@cp -af $(DESTDIR)/usr/lib/iptables/libxt_physdev.so		$(INSTALL_ROOT)/usr/lib/iptables
endif
ifeq ($(HANDLE_IPV6), 1) 
	@cp -af $(DESTDIR)/usr/lib/iptables/libip6t_icmp6.so		$(INSTALL_ROOT)/usr/lib/iptables
	@cp -af $(DESTDIR)/usr/lib/iptables/libip6t_LOG.so		$(INSTALL_ROOT)/usr/lib/iptables
endif

ifeq (${IPT_VER}, ${filter ${IPT_VER}, 1.4.10 1.4.21})
	@cp -af $(DESTDIR)/usr/lib/iptables/libxt_recent.so		$(INSTALL_ROOT)/usr/lib/iptables
else
	@cp -af $(DESTDIR)/usr/lib/iptables/libipt_recent.so		$(INSTALL_ROOT)/usr/lib/iptables
endif

ifeq ($(HANDLE_L7_BLOCK), 1) # For L7APP filter
ifneq ($(IPT_VER), 1.4.21)
	@cp -af $(DESTDIR)/usr/lib/iptables/libxt_layer7.so		$(INSTALL_ROOT)/usr/lib/iptables
endif
	@cp -af $(DESTDIR)/usr/lib/iptables/libxt_connlimit.so		$(INSTALL_ROOT)/usr/lib/iptables
ifeq (${IPT_VER}, ${filter ${IPT_VER}, 1.4.10 1.4.21})
	@cp -af $(DESTDIR)/usr/lib/iptables/libxt_recent.so		$(INSTALL_ROOT)/usr/lib/iptables
else
	@cp -af $(DESTDIR)/usr/lib/iptables/libipt_recent.so		$(INSTALL_ROOT)/usr/lib/iptables
endif
	@cp -a l7-protocols/protocols/*.pat     			$(INSTALL_ROOT)/usr/mv2ram/etc/l7-protocols
endif	

ifeq ($(HANDLE_TTL_ADJUST), 1)
	@cp -af $(DESTDIR)/usr/lib/iptables/libipt_ttl.so		$(INSTALL_ROOT)/usr/lib/iptables
	@cp -af $(DESTDIR)/usr/lib/iptables/libipt_TTL.so		$(INSTALL_ROOT)/usr/lib/iptables
endif

ifneq ($(IPT_VER), 1.4.21)
ifeq ($(HANDLE_PSD), 1)	
	@cp -af $(DESTDIR)/usr/lib/iptables/libxt_psd.so		$(INSTALL_ROOT)/usr/lib/iptables
endif
endif	

ifeq ($(HANDLE_IPV6), 1) 
	@echo "Install extra extension for IPv6"
	@-cp -af $(DESTDIR)/usr/lib/libip6tc*.so*			$(INSTALL_ROOT)/usr/lib > /dev/null 2>&1
endif

ifeq ($(HANDLE_LOADSHARING), 1) 
	@echo "Install extra extension for LoadSharing"
	@cp -a $(DESTDIR)/usr/lib/iptables/libxt_CONNMARK.so		$(INSTALL_ROOT)/usr/lib/iptables
	@cp -a $(DESTDIR)/usr/lib/iptables/libxt_conntrack.so		$(INSTALL_ROOT)/usr/lib/iptables
endif

ifeq ($(HANDLE_MULTIWAN), 1) 
	@echo "Install extra extension for MULTIWAN"
	@cp -a $(DESTDIR)/usr/lib/iptables/libxt_CONNMARK.so		$(INSTALL_ROOT)/usr/lib/iptables
	@cp -a $(DESTDIR)/usr/lib/iptables/libxt_conntrack.so		$(INSTALL_ROOT)/usr/lib/iptables
endif

ifeq (${QOS_EXTRA}, y)
	@echo "Install extra extension for CL-QoS or Smart-QoS"
ifneq ($(IPT_VER), 1.4.21)
	@cp -a $(DESTDIR)/usr/lib/iptables/libxt_layer7.so		$(INSTALL_ROOT)/usr/lib/iptables
endif
	@cp -a $(DESTDIR)/usr/lib/iptables/libxt_connlimit.so		$(INSTALL_ROOT)/usr/lib/iptables
ifeq ($(IPT_VER), 1.4.21)
	@cp -a $(DESTDIR)/usr/lib/iptables/libxt_TEE.so			$(INSTALL_ROOT)/usr/lib/iptables
else
	@cp -a $(DESTDIR)/usr/lib/iptables/libipt_ROUTE.so		$(INSTALL_ROOT)/usr/lib/iptables
endif
ifneq ($(IPT_VER), 1.4.21)
	@cp -a $(DESTDIR)/usr/lib/iptables/libipt_ALERT.so		$(INSTALL_ROOT)/usr/lib/iptables
endif
	@cp -a $(DESTDIR)/usr/lib/iptables/libxt_conntrack.so		$(INSTALL_ROOT)/usr/lib/iptables
	@cp -a $(DESTDIR)/usr/lib/iptables/libxt_dscp.so		$(INSTALL_ROOT)/usr/lib/iptables
	@cp -a $(DESTDIR)/usr/lib/iptables/libxt_DSCP.so		$(INSTALL_ROOT)/usr/lib/iptables
	@cp -a $(DESTDIR)/usr/lib/iptables/libxt_TOS.so			$(INSTALL_ROOT)/usr/lib/iptables
	@cp -a $(DESTDIR)/usr/lib/iptables/libxt_tos.so			$(INSTALL_ROOT)/usr/lib/iptables
	@cp -a l7-protocols/protocols/*.pat				$(INSTALL_ROOT)/usr/mv2ram/etc/l7-protocols
endif

ifeq ($(HANDLE_VPN), 1)
	@cp -a $(DESTDIR)/usr/lib/iptables/libxt_policy.so		$(INSTALL_ROOT)/usr/lib/iptables
endif
endif


precheck:
	@echo ""
	@echo "Build $(shell basename `pwd`) for $(PROJECT_NAME) ($(PROJECT_CUSTOMER))"
ifeq ($(PROJECT_CUSTOMER), ) 
	@echo ""
	@echo "No specify PROJECT_CUSTOMER in Makefile. "
	@echo "Maybe you should update the components Makefile of the Project"
	@echo ""
	@exit 1
endif 

postcheck:                                   
	@echo ""
	@echo "Install $(shell basename `pwd`) to $(INSTALL_ROOT)" 
ifeq ($(INSTALL_ROOT), ) 
	@echo ""
	@echo "No specify INSTALL_ROOT in Makefile. "
	@echo "Maybe you should update the components Makefile of the Project"
	@echo ""
	@exit 1
endif  

clean:
	rm -rf $(DESTDIR)
	if [ -e $(SRC)/Makefile ]; then make -C $(SRC) clean; fi;

distclean:
	rm -rf $(DESTDIR)
	if [ -e $(SRC)/Makefile ]; then make -C $(SRC) distclean; fi;


.PHONY: all clean

export CC CXX LD AR AS RANLIB STRIP CFLAGS LDFLAGS 


