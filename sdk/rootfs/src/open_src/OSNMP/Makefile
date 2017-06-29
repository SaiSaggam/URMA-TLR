# RDS/00008: OASIS Makefile Conventions


TOPDIR := $(shell cd ../../../../; pwd)
SRC_TOP_DIR     = $(shell cd ../../; pwd)

KERNEL_DIR ?= ${TOPDIR}/linux/src/
LIBS_DIR = ${TOPDIR}/rootfs/src/mipsel-linux-uclibc-lib
INCLUDE_DIR = $(SRC_TOP_DIR)/include

AR 	= $(CROSS_COMPILE)ar
CC 	= $(CROSS_COMPILE)gcc
CXX	= $(CROSS_COMPILE)g++
RANLIB 	= $(CROSS_COMPILE)ranlib
STRIP 	= $(CROSS_COMPILE)strip

CFLAGS 	=
CFLAGS 	+= -g -Wall -I$(INCLUDE_DIR)

LDFLAGS =
LDFLAGS += -L${TOPDIR}/rootfs/src/mipsel-linux-uclibc-lib

CPPFLAGS += -I$(CFLAGS)

LIBS += -lcrypto -lssl

export LIBS


ifeq ($(ENABLE_ENDIANLESS_BIG), 1)
ENDIANLESS = big
else
ENDIANLESS = little
endif

#set snmp enterprise oid
ENTERPRISE_OID = $(SNMP_ENTERPRISE_OID)
ENTERPRISE_OID ?= 8072
#set snmp enterprise notification oid
ENTERPRISE_NOTIF_OID = $(SNMP_ENTERPRISE_NOTIF_OID)
ENTERPRISE_NOTIF_OID ?= .1.3.6.1.4.1.8072.4
#set snmp enterprise sysoid
ENTERPRISE_SYS_OID = ${SNMP_ENTERPRISE_SYS_OID}
ENTERPRISE_SYS_OID ?= .1.3.6.1.4.1.8072.3.2

#define which default mibs want to make
#you can using cflags for decided which mib module want to build-in
#global mib
SNMP_MIB_MODULES_INCLUDED = \
		host/hr_device \
		host/hr_disk \
		host/hr_network \
		host/hr_partition \
		host/hr_proc \
		host/hr_system \
		mibII \
		mibII/mta_sendmail \
		snmpv3mibs \
		ucd_snmp \
		tunnel \
		target \
		utilities \
		host \
		notification \
		util_funcs \
		utilities/execute \
		ip-mib \
		ip-mib/ipv4InterfaceTable \
		ip-mib/ipv6InterfaceTable \
		if-mib \
		tcp-mib \
		udp-mib \
		snmp-usm-dh-objects-mib \
		Rmon \
		tsm-mib \
		sctp-mib \
		bridge-mib/bridge-mib \
		ieee802dot11/ieee802dot11 \
		disman/event \
		disman/schedule \
		disman/traceroute \
		disman/ping \
		disman/nslookup \
		ipfwchains \
		agent_mibs \
		misc/ipfwacc \
		ip-forward-mib \
		ucd-snmp/diskio \
 
#set if want to install agentx
SNMP_MIB_MODULES_INCLUDED += \
		agentx \
#digi Sarian Monitor mib
#SNMP_MIB_MODULES_INCLUDED += \
#		digi-mib/Sarian-Monitor \

#not install
SNMP_MIB_MODULES_EXCLUDED = \
		host/host_filesys \
		hardware \
		notification-log-mib \
#defined transport
SNMP_TRANSPORTS_INCLUDED = Callback UDP UDPIPv6
#not install
SNMP_TRANSPORTS_EXCLUDED = TCPIPv6 Unix	

#SNMP v3 Security module
SNMP_SECURITY_MODULES = usm tsm

#Configure items 			
CONFIG_ARGS += \
	--host=$(TARGET)  \
	CC="$(CC)" \
	CXX="$(CXX)" \
	CFLAGS="$(CFLAGS)" \
	LDFLAGS="$(LDFLAGS)" \
	CPPFLAGS="$(CPPFLAGS)" \
	LIBS="$(LIBS)" \
	--with-cc=$(CC) \
	--with-linkcc=$(CC) \
	--with-ar=$(AR) \
	--with-endianness=$(ENDIANLESS) \
	--with-enterprise-oid=$(ENTERPRISE_OID) \
	--with-enterprise-notification-oid=$(ENTERPRISE_NOTIF_OID) \
	--with-enterprise-sysoid=$(ENTERPRISE_SYS_OID) \
	--prefix=/usr \
	--with-sys-contact=root@localhost \
	--with-sys-location=location \
	--with-persistent-directory=/var/snmp \
	--with-logfile=/var/log/snmpd.log \
	--with-cflags="$(CFLAGS)" \
	--with-ldflags="$(LDFLAGS)" \
	--with-libs="$(LIBS)" \
	--sysconfdir=/etc \
	--enable-shared \
	--enable-static \
	--disable-debugging \
	--disable-scripts \
	--disable-manuals \
	--disable-mib-loading \
	--disable-mibs \
	--enable-new-features \
	--enable-old-features \
	--disable-perl-cc-checks \
	--disable-embedded-perl \
	--enable-mfd-rewrites \
	--with-out-mib-modules="$(SNMP_MIB_MODULES_EXCLUDED)" \
	--with-mib-modules="$(SNMP_MIB_MODULES_INCLUDED)" \
	--with-out-transports="$(SNMP_TRANSPORTS_EXCLUDED)" \
	--with-transports="$(SNMP_TRANSPORTS_INCLUDED)" \
	--without-libwrap \
	--without-rpm \
	--without-perl-modules \
	--without-zlib \
	--without-elf \
	--with-security-modules="$(SNMP_SECURITY_MODULES)" \
	--enable-internal-md5 \
	--with-sysroot="$(LIBS_DIR)" \


CONFIG_ARGS += --with-default-snmp-version=3 \
		--with-openssl="$(LIBS_DIR)" \
		--with-rsaref="$(LIBS_DIR)" \

#endable support ipv6
CONFIG_ARGS += --enable-ipv6 \

#item for final, do not add any configure here
CONFIG_ARGS += --enable-mini-agent ; \

all: precheck
	
	@echo "Making for net-snmp-5.7.3.... "
	if [ ! -e net-snmp-5.7.3/Makefile ]; then \
		cd net-snmp-5.7.3; \
		./configure $(CONFIG_ARGS) \
	fi;
	make -C net-snmp-5.7.3 $(PREFIX)
	@echo "strip library *.so"
	$(CROSS_COMPILE)strip net-snmp-5.7.3/agent/.libs/*.so*
	$(CROSS_COMPILE)strip net-snmp-5.7.3/agent/.libs/*.a*
	$(CROSS_COMPILE)strip net-snmp-5.7.3/apps/.libs/*.so*
	$(CROSS_COMPILE)strip net-snmp-5.7.3/apps/.libs/*.a*
	$(CROSS_COMPILE)strip net-snmp-5.7.3/snmplib/.libs/*.so*
	$(CROSS_COMPILE)strip net-snmp-5.7.3/snmplib/.libs/*.a*
	$(CROSS_COMPILE)strip net-snmp-5.7.3/agent/helpers/.libs/*.so*
	$(CROSS_COMPILE)strip net-snmp-5.7.3/agent/helpers/.libs/*.a*

clean:
	if [ -e "net-snmp-5.7.3/Makefile" ]; then make -C net-snmp-5.7.3 clean; rm net-snmp-5.7.3/Makefile; fi

install: postcheck
	@echo "Install net-snmp tools..."
	@mkdir -p $(INSTALL_ROOT)/usr/bin
	@cp -a net-snmp-5.7.3/apps/.libs/snmpget              		$(INSTALL_ROOT)/usr/bin
	@cp -a net-snmp-5.7.3/apps/.libs/snmpset              		$(INSTALL_ROOT)/usr/bin
	@cp -a net-snmp-5.7.3/apps/.libs/snmpwalk             		$(INSTALL_ROOT)/usr/bin
	@cp -a net-snmp-5.7.3/apps/.libs/snmptrap             		$(INSTALL_ROOT)/usr/bin
	@echo "Install net-snmp library..."
	@mkdir -p $(INSTALL_ROOT)/usr/lib
	@cp -a net-snmp-5.7.3/agent/.libs/libnetsnmpagent.so*		$(INSTALL_ROOT)/usr/lib
	@cp -a net-snmp-5.7.3/agent/.libs/libnetsnmpmibs.so*		$(INSTALL_ROOT)/usr/lib
	@cp -a net-snmp-5.7.3/apps/.libs/libnetsnmptrapd.so*	        $(INSTALL_ROOT)/usr/lib
	@cp -a net-snmp-5.7.3/snmplib/.libs/libnetsnmp.so*	        $(INSTALL_ROOT)/usr/lib
	@cp -a net-snmp-5.7.3/agent/helpers/.libs/libnetsnmphelpers.so*	$(INSTALL_ROOT)/usr/lib
	@echo "Install net-snmp daemon..."
	@mkdir -p $(INSTALL_ROOT)/usr/sbin
	@cp -a net-snmp-5.7.3/agent/.libs/snmpd			        $(INSTALL_ROOT)/usr/sbin
	@cp -a net-snmp-5.7.3/apps/.libs/snmptrapd	          	$(INSTALL_ROOT)/usr/sbin

	@echo "Install net-snmp init script..."
	@mkdir -p $(INSTALL_ROOT)/usr/etc/init.d
	@cp -a files/init_snmp.sh					$(INSTALL_ROOT)/usr/etc/init.d
	
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
endif
