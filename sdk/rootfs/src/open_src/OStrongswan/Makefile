# RDS/00008: OASIS Makefile Conventions

TOPDIR := $(shell cd ../../../../; pwd)
TARGET ?= mips-linux-uclibc
KERNEL_DIR ?= ${TOPDIR}/linux/src/

ifeq ($(TARGET), mips-linux-uclibc)
	ARCH = mips
endif

ifndef TOOLPATH
    export TOOLPATH := $(shell cat $(TOPDIR)/TOOLPATH2)
	export PATH     := $(TOOLPATH):$(PATH)
endif

include rds00008.mk

INSTALL_DIR = $(shell pwd)/install
SRC_TOP_DIR = $(shell cd ../../; pwd)
INCLUDEDIR  = $(SRC_TOP_DIR)/include
CSIDDIR     = $(SRC_TOP_DIR)/include/csid
LIBDIR      = $(SRC_TOP_DIR)/$(TARGET)-lib
UCLIBCDIR 	= $(subst buildroot-gcc342/bin/,buildroot-gcc342/mipsel-linux-uclibc/lib,$(TOOLPATH))
BUSY_DIR = $(TOPDIR)/busybox/root/bin
CFLAGS     += -I$(INCLUDEDIR)
CFLAGS     += -I$(CSIDDIR)                                                                       
#CFLAGS     += -I$(KERNEL_DIR)/include
CFLAGS     += -I$(KERNEL_DIR)/usr/include

CFLAGS	   += -DTIOCSRS485

AMIT_CFLAGS = ${CFLAGS}
CXXFLAGS    = ${CFLAGS}
CPPFLAGS    = ${CFLAGS}
LDFLAGS    += -L${TOPDIR}/rootfs/src/mipsel-linux-uclibc-lib
LDFLAGS    += -L$(INSTALL_ROOT)/lib 
#LDFLAGS    += -lcsman
INCLUDEKERNEL = -I$(KERNEL_DIR)/include
#PIDDIR = -I$(INSTALL_ROOT)/var/run
#IPSECLIBDIR = -I$(INSTALL_ROOT)/usr/lib
#IPSECDIR = -I$(INSTALL_ROOT)/usr/libexec/ipsec
#IPSECPLUGINDIR = -I$(INSTALL_ROOT)/var/run/ipsecplugin
#@SED@         = /bin
#export @SED@
PKG_MOD_AVAILABLE:= \
	        addrblock \
	        aes \
	        agent \
	        attr \
	        attr-sql \
	        blowfish \
	        ccm \
	        cmac \
	        constraints \
	        coupling \
	        ctr \
	        des \
	        dhcp \
	        dnskey \
	        duplicheck \
	        eap-identity \
	        eap-md5 \
	        farp \
	        fips-prf \
	        gcm \
	        gmp \
	        ha \
	        hmac \
	        kernel-libipsec \
	        kernel-netlink \
	        kernel-pfkey \
	        md4 \
	        md5 \
	        nonce \
	        openssl \
	        pem \
	        pgp \
	        pkcs1 \
	        pkcs8 \
	        pkcs11 \
	        pubkey \
	        random \
	        resolve \
	        revocation \
	        sha1 \
	        sha2 \
	        socket-default \
	        socket-dynamic \
	        stroke \
	        test-vectors \
	        updown \
	        vici \
	        whitelist \
	        x509 \
	        xauth-eap \
	        xauth-generic \
	        xcbc
	
PKG_CONFIG_DEPENDS:= \
	        CONFIG_STRONGSWAN_DEVICE_RANDOM \
	        CONFIG_STRONGSWAN_DEVICE_URANDOM \
	        CONFIG_STRONGSWAN_ROUTING_TABLE \
	        CONFIG_STRONGSWAN_ROUTING_TABLE_PRIO \
	        $(patsubst %,CONFIG_PACKAGE_strongswan-mod-%,$(PKG_MOD_AVAILABLE));
CONFIGURE_ARGS+= \
		--host=$(TARGET) \
		CC=$(CROSS_COMPILE)gcc \
		CXX=$(CROSS_COMPILE)g++ \
		STRIP=$(CROSS_COMPILE)strip \
	        --prefix= \
	        --exec-prefix=/usr \
	        --with-ipsec-script=ipsec \
	        --disable-scripts \
	        --disable-static \
	        --disable-load-warning  \
	        --enable-libipsec \
	        --enable-cmd \
	        --with-piddir=/var/run \
		--with-ipseclibdir=/usr/lib/strongswan \
	        --with-ipsecdir=/usr/libexec/strongswan \
	        --with-strongswan-conf=/usr/etc/strongswan.conf \
	        --with-plugindir=/usr/lib/strongswan/plugins \
                $(foreach m,$(PKG_MOD_AVAILABLE), \
	          $(if $(CONFIG_PACKAGE_strongswan-mod-$(m)),--disable-$(m),--enable-$(m)) \
		)

export AMIT_CFLAGS CFLAGS LIBDIR KERNEL_DIR TARGET CXXFLAGS CPPFLAGS LDFLAGS 

all: precheck
	@echo "Making for Strongswan.... "
	if [ ! -e src/Makefile ]; then \
		cd src; ./configure $(CONFIGURE_ARGS); \
        fi; 
	@make -C src
	@echo "Mstrip library .so" 
	@LIBDIRS=`find ./src/src/ -type d -name ".libs"`; \
		for DIR in $$LIBDIRS; do \
			$(foreach m,$(PKG_MOD_AVAILABLE), \
				if [ -f $$DIR/libstrongswan-$(m).so* ]; then \
					$(CROSS_COMPILE)strip $$DIR/libstrongswan-$(m).so*; \
				fi; \
			) \
		done
	$(CROSS_COMPILE)strip src/src/libstrongswan/.libs/libstrongswan.so*;
	$(CROSS_COMPILE)strip src/src/libcharon/.libs/libcharon.so*;
	$(CROSS_COMPILE)strip src/src/libhydra/.libs/libhydra.so*;
	$(CROSS_COMPILE)strip src/src/libipsec/.libs/libipsec.so*;
	$(CROSS_COMPILE)strip src/src/libcharon/plugins/vici/.libs/libvici.so*;
	$(CROSS_COMPILE)strip src/src/libstrongswan/plugins/rc2/.libs/libstrongswan-rc2.so*;
	$(CROSS_COMPILE)strip src/src/libstrongswan/plugins/pkcs7/.libs/libstrongswan-pkcs7.so*;
	$(CROSS_COMPILE)strip src/src/libstrongswan/plugins/pkcs12/.libs/libstrongswan-pkcs12.so*;
	$(CROSS_COMPILE)strip src/src/libstrongswan/plugins/sshkey/.libs/libstrongswan-sshkey.so*;

precheck:
	@echo "Build $(shell basename `pwd`) for $(PROJECT_NAME)"
ifeq ($(PROJECT_CUSTOMER), ) 
	@$(error You need specify PROJECT_CUSTOMER for Makefile)
endif 

postcheck:
	@echo "Install $(shell basename `pwd`) to $(INSTALL_ROOT)" 
ifeq ($(INSTALL_ROOT), ) 
	@$(error You need specify INSTALL_ROOT for Makefile)
endif 

install: postcheck
	@cp -a src/src/ipsec/_ipsec $(INSTALL_ROOT)/usr/sbin/ipsec
	#@mkdir -p $(INSTALL_ROOT)/usr/libexec
	#@chmod 777 $(INSTALL_ROOT)/usr/libexec
	@mkdir -p $(INSTALL_ROOT)/usr/libexec/strongswan
	@chmod 777 $(INSTALL_ROOT)/usr/libexec/strongswan
	@cp -a src/src/starter/.libs/starter $(INSTALL_ROOT)/usr/libexec/strongswan/
	@cp -a src/src/stroke/.libs/stroke $(INSTALL_ROOT)/usr/libexec/strongswan/
	@cp -a src/src/charon/.libs/charon $(INSTALL_ROOT)/usr/libexec/strongswan/
	@cp -a src/src/scepclient/.libs/scepclient $(INSTALL_ROOT)/usr/libexec/strongswan/
	@cp -a src/src/_updown/_updown $(INSTALL_ROOT)/usr/libexec/strongswan/
	@cp -a src/src/pki/.libs/pki $(INSTALL_ROOT)/usr/bin/
	@cp -a src/src/charon-cmd/.libs/charon-cmd $(INSTALL_ROOT)/usr/libexec/strongswan/
	@mkdir -p $(INSTALL_ROOT)/usr/lib/strongswan/plugins
	#@chmod 755 $(INSTALL_ROOT)/usr/lib/strongswan
	@cp -a src/src/libstrongswan/.libs/libstrongswan.so*	$(INSTALL_ROOT)/usr/lib/strongswan
	@cp -a src/src/libcharon/.libs/libcharon.so*		$(INSTALL_ROOT)/usr/lib/strongswan
	@cp -a src/src/libhydra/.libs/libhydra.so* $(INSTALL_ROOT)/usr/lib/strongswan
	@cp -a src/src/libipsec/.libs/libipsec.so* $(INSTALL_ROOT)/usr/lib/strongswan
	@cp -a src/src/libcharon/plugins/vici/.libs/libvici.so* $(INSTALL_ROOT)/usr/lib/strongswan

	@# Copy plugin libraries
	@LIBDIRS=`find ./src/src/ -type d -name ".libs"`; \
		for DIR in $$LIBDIRS; do \
			$(foreach m,$(PKG_MOD_AVAILABLE), \
				if [ -f $$DIR/libstrongswan-$(m).so* ]; then \
					cp -a $$DIR/libstrongswan-$(m).so* $(INSTALL_ROOT)/usr/lib/strongswan/plugins/ ; \
				fi; \
			) \
		done
	@cp -a src/src/libstrongswan/plugins/rc2/.libs/libstrongswan-rc2.so* $(INSTALL_ROOT)/usr/lib/strongswan/plugins
	@cp -a src/src/libstrongswan/plugins/pkcs7/.libs/libstrongswan-pkcs7.so* $(INSTALL_ROOT)/usr/lib/strongswan/plugins
	@cp -a src/src/libstrongswan/plugins/pkcs12/.libs/libstrongswan-pkcs12.so* $(INSTALL_ROOT)/usr/lib/strongswan/plugins
	@cp -a src/src/libstrongswan/plugins/sshkey/.libs/libstrongswan-sshkey.so* $(INSTALL_ROOT)/usr/lib/strongswan/plugins

	@# Copy configuration files
	@mkdir -p $(INSTALL_ROOT)/usr/etc
	@cp -a src/conf/strongswan.conf			$(INSTALL_ROOT)/usr/etc/
	@mkdir -p $(INSTALL_ROOT)/usr/etc/strongswan.d
	@cp -a src/conf/options/charon-logging.conf	$(INSTALL_ROOT)/usr/etc/strongswan.d/
	@cp -a src/conf/options/charon.conf		$(INSTALL_ROOT)/usr/etc/strongswan.d/
	@cp -a src/conf/options/pki.conf		$(INSTALL_ROOT)/usr/etc/strongswan.d/
	@cp -a src/conf/options/pool.conf		$(INSTALL_ROOT)/usr/etc/strongswan.d/
	@cp -a src/conf/options/scepclient.conf		$(INSTALL_ROOT)/usr/etc/strongswan.d/
	@cp -a src/conf/options/starter.conf		$(INSTALL_ROOT)/usr/etc/strongswan.d/

	@# Copy plugin configuration files...
	@mkdir -p $(INSTALL_ROOT)/usr/etc/strongswan.d/charon
	@$(foreach m,$(PKG_MOD_AVAILABLE), \
		cp -a src/conf/plugins/$(m).conf	$(INSTALL_ROOT)/usr/etc/strongswan.d/charon/; \
	)
	@# ... and copy some additional configuration files
	@cp -a src/conf/plugins/pkcs7.conf		$(INSTALL_ROOT)/usr/etc/strongswan.d/charon/
	@cp -a src/conf/plugins/pkcs12.conf		$(INSTALL_ROOT)/usr/etc/strongswan.d/charon/
	@cp -a src/conf/plugins/rc2.conf		$(INSTALL_ROOT)/usr/etc/strongswan.d/charon/
	@cp -a src/conf/plugins/sshkey.conf		$(INSTALL_ROOT)/usr/etc/strongswan.d/charon/

	@# StrongSwan will need the following directories under /etc/ (created by src/starter/Makefile)
	@mkdir -p $(INSTALL_ROOT)/usr/mv2ram/etc/ipsec.d/{aacerts,acerts,cacerts,certs,crls,ocspcerts,private,reqs}

	@# Copy init script
	@mkdir -p $(INSTALL_ROOT)/usr/etc/init.d
	@cp -a files/init_strongswan.sh $(INSTALL_ROOT)/usr/etc/init.d/

uninstall: 
#	@rm -f $(INSTALL_ROOT)/sbin/ipsec
#	@rm -f $(INSTALL_ROOT)/lib/libstrongswan.so*
#	@rm -f $(INSTALL_ROOT)/lib/libcharon.so*
#	@rm -f $(INSTALL_ROOT)/lib/libstrongswan-updown.so*
#	@rm -f $(INSTALL_ROOT)/lib/libstrongswan-stroke.so*

.PHONY: all clean

clean:
	if [ -e src/Makefile ]; then make -C ./src clean; fi;
	@rm -f src/Makefile

distclean:
	if [ -e src/Makefile ]; then make -C ./src distclean; fi;
	@rm -f src/Makefile
