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

SRC_TOP_DIR = $(shell cd ../../; pwd)
INCLUDEDIR  = $(SRC_TOP_DIR)/include
CSIDDIR     = $(SRC_TOP_DIR)/include/csid
LIBDIR      = $(SRC_TOP_DIR)/$(TARGET)-lib

CFLAGS     += -I$(INCLUDEDIR)
CFLAGS     += -I$(CSIDDIR)                                                                       
#CFLAGS     += -I${TOPDIR}/rootfs/src/open_src/OEBTables/src/include
#CFLAGS     += -Isrc/include
#CFLAGS     += -I$(KERNEL_DIR)/include
#CFLAGS     += -I${TOPDIR}/rootfs/src/include/taglib -O2 
#CFLAGS     += -I${TOPDIR}/rootfs/src/open_src/OIPTables142/src/include/
#CFLAGS     += -O2 

AMIT_CFLAGS = ${CFLAGS}
CXXFLAGS    = ${CFLAGS}
CPPFLAGS    = ${CFLAGS}
LDFLAGS    += -L${TOPDIR}/rootfs/src/mipsel-linux-uclibc-lib
#KERNEL_INCLUDES = $(KERNEL_DIR)/include

#export AMIT_CFLAGS CFLAGS LIBDIR KERNEL_DIR KERNEL_INCLUDES TARGET CXXFLAGS CPPFLAGS LDFLAGS LIBS
export AMIT_CFLAGS CFLAGS LIBDIR TARGET CXXFLAGS CPPFLAGS LDFLAGS LIBS


all: precheck
	make -C src 
	@$(STRIP) src/ebtables
	@$(STRIP) src/libebtc.so
	@$(STRIP) src/extensions/libebt_802_3.so         
	@$(STRIP) src/extensions/libebtable_broute.so    
	@$(STRIP) src/extensions/libebtable_filter.so    
	@$(STRIP) src/extensions/libebtable_nat.so       
	@$(STRIP) src/extensions/libebt_among.so         
	@$(STRIP) src/extensions/libebt_arpreply.so      
	@$(STRIP) src/extensions/libebt_arp.so           
	@$(STRIP) src/extensions/libebt_ip6.so           
	@$(STRIP) src/extensions/libebt_ip.so            
	@$(STRIP) src/extensions/libebt_limit.so         
	@$(STRIP) src/extensions/libebt_log.so           
	@$(STRIP) src/extensions/libebt_mark_m.so        
	@$(STRIP) src/extensions/libebt_mark.so          
	@$(STRIP) src/extensions/libebt_nat.so           
	@$(STRIP) src/extensions/libebt_nflog.so         
	@$(STRIP) src/extensions/libebt_pkttype.so       
	@$(STRIP) src/extensions/libebt_redirect.so      
	@$(STRIP) src/extensions/libebt_standard.so      
	@$(STRIP) src/extensions/libebt_stp.so           
	@$(STRIP) src/extensions/libebt_ulog.so          
	@$(STRIP) src/extensions/libebt_vlan.so          
	
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
	@cp -af  src/ebtables 		  					$(INSTALL_ROOT)/usr/sbin
	@cp -af  src/ethertypes 		  				$(INSTALL_ROOT)/usr/mv2ram/etc
	@cp -af  src/libebtc.so 	  					$(INSTALL_ROOT)/lib
	@cp -af  src/extensions/libebt_802_3.so         $(INSTALL_ROOT)/lib
	@cp -af  src/extensions/libebtable_broute.so    $(INSTALL_ROOT)/lib
	@cp -af  src/extensions/libebtable_filter.so    $(INSTALL_ROOT)/lib 
	@cp -af  src/extensions/libebtable_nat.so       $(INSTALL_ROOT)/lib 
	@cp -af  src/extensions/libebt_among.so         $(INSTALL_ROOT)/lib  
	@cp -af  src/extensions/libebt_arpreply.so      $(INSTALL_ROOT)/lib  
	@cp -af  src/extensions/libebt_arp.so           $(INSTALL_ROOT)/lib  
	@cp -af  src/extensions/libebt_ip6.so           $(INSTALL_ROOT)/lib  
	@cp -af  src/extensions/libebt_ip.so            $(INSTALL_ROOT)/lib   
	@cp -af  src/extensions/libebt_limit.so         $(INSTALL_ROOT)/lib   
	@cp -af  src/extensions/libebt_log.so           $(INSTALL_ROOT)/lib   
	@cp -af  src/extensions/libebt_mark_m.so        $(INSTALL_ROOT)/lib   
	@cp -af  src/extensions/libebt_mark.so          $(INSTALL_ROOT)/lib   
	@cp -af  src/extensions/libebt_nat.so           $(INSTALL_ROOT)/lib   
	@cp -af  src/extensions/libebt_nflog.so         $(INSTALL_ROOT)/lib   
	@cp -af  src/extensions/libebt_pkttype.so       $(INSTALL_ROOT)/lib   
	@cp -af  src/extensions/libebt_redirect.so      $(INSTALL_ROOT)/lib   
	@cp -af  src/extensions/libebt_standard.so      $(INSTALL_ROOT)/lib   
	@cp -af  src/extensions/libebt_stp.so           $(INSTALL_ROOT)/lib    
	@cp -af  src/extensions/libebt_ulog.so          $(INSTALL_ROOT)/lib    
	@cp -af  src/extensions/libebt_vlan.so          $(INSTALL_ROOT)/lib    

uninstall: postcheck
	@rm -f  $(INSTALL_ROOT)/usr/sbin/ebtables
	@rm -f  $(INSTALL_ROOT)/lib/libebtc.so
	@rm -f  $(INSTALL_ROOT)/src/extensions/libebt_802_3.so
	@rm -f  $(INSTALL_ROOT)/src/extensions/libebtable_broute.so
	@rm -f  $(INSTALL_ROOT)/src/extensions/libebtable_filter.so
	@rm -f  $(INSTALL_ROOT)/src/extensions/libebtable_nat.so
	@rm -f  $(INSTALL_ROOT)/src/extensions/libebt_among.so
	@rm -f  $(INSTALL_ROOT)/src/extensions/libebt_arpreply.so
	@rm -f  $(INSTALL_ROOT)/src/extensions/libebt_arp.so
	@rm -f  $(INSTALL_ROOT)/src/extensions/libebt_ip6.so
	@rm -f  $(INSTALL_ROOT)/src/extensions/libebt_ip.so
	@rm -f  $(INSTALL_ROOT)/src/extensions/libebt_limit.so
	@rm -f  $(INSTALL_ROOT)/src/extensions/libebt_log.so
	@rm -f  $(INSTALL_ROOT)/src/extensions/libebt_mark_m.so
	@rm -f  $(INSTALL_ROOT)/src/extensions/libebt_mark.so
	@rm -f  $(INSTALL_ROOT)/src/extensions/libebt_nat.so
	@rm -f  $(INSTALL_ROOT)/src/extensions/libebt_nflog.so
	@rm -f  $(INSTALL_ROOT)/src/extensions/libebt_pkttype.so
	@rm -f  $(INSTALL_ROOT)/src/extensions/libebt_redirect.so
	@rm -f  $(INSTALL_ROOT)/src/extensions/libebt_standard.so
	@rm -f  $(INSTALL_ROOT)/src/extensions/libebt_stp.so
	@rm -f  $(INSTALL_ROOT)/src/extensions/libebt_ulog.so
	@rm -f  $(INSTALL_ROOT)/src/extensions/libebt_vlan.so

.PHONY: all clean

clean:
	cd src; rm -f `find . -name "*.o"` ; rm -f `find . -name *.so` ; cd - 
	-make clean -C src

distclean:

