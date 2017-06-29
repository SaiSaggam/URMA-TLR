#
# Makefile to install startnet
#

include ../makefile.vars

COMPO_DIR := $(SCRIPTS_BASE)
USER_FS_DIR := $(TLR_BASE)/userfs
IMAGE_DIR := $(TLR_BASE)/sdk/image

build:

install:
	-mkdir -p $(INSTALL_ROOT)/usr/{include,etc,lib,bin,config}
	@cp $(COMPO_DIR)/usr/config/* $(INSTALL_ROOT)/usr/config/
	-rm -f $(IMAGE_DIR)/user.o
	/usr/sbin/mkfs.jffs2 -r $(USER_FS_DIR) -o $(IMAGE_DIR)/user.o -e 0x20000 -l --pad=0x500000 --no-cleanmarkers
	install -m 777 $(COMPO_DIR)/usr/bin/* $(INSTALL_ROOT)/usr/bin/
	install -m 777 $(COMPO_DIR)/usr/etc/inittab $(INSTALL_ROOT)/usr/etc/
	install -m 777 $(COMPO_DIR)/usr/etc/ipsec.updown $(INSTALL_ROOT)/usr/etc/
	-mkdir -p $(INSTALL_ROOT)/usr/etc/init.d
	for f in `find $(COMPO_DIR)/usr/etc/init.d/*`; do \
		# Don't chmod symlinks \
		if [ -h "$$f" ]; then \
			cp -a "$$f" "$(INSTALL_ROOT)/usr/etc/init.d/"; \
		else \
			install -m 777 "$$f" "$(INSTALL_ROOT)/usr/etc/init.d/"; \
		fi; \
	done
	-mkdir -p $(INSTALL_ROOT)/usr/etc/udhcpc
	install -m 777 $(COMPO_DIR)/usr/etc/udhcpc/* $(INSTALL_ROOT)/usr/etc/udhcpc/
	-mkdir -p $(INSTALL_ROOT)/usr/etc/ppp
	install -m 777 $(COMPO_DIR)/usr/etc/ppp/* $(INSTALL_ROOT)/usr/etc/ppp/
	-mkdir -p $(INSTALL_ROOT)/bin
	install -m 777 $(COMPO_DIR)/bin/* $(INSTALL_ROOT)/bin/
	-mkdir -p $(INSTALL_ROOT)/usr/share/udhcpc
	install -m 777 $(COMPO_DIR)/usr/share/udhcpc/* $(INSTALL_ROOT)/usr/share/udhcpc
	-mkdir -p $(INSTALL_ROOT)/sbin
	install -m 700 $(COMPO_DIR)/sbin/rsyslog_rotate $(INSTALL_ROOT)/sbin/
	-mkdir -p $(INSTALL_ROOT)/usr/mv2ram/etc
	install -m 644 $(COMPO_DIR)/usr/mv2ram/etc/rsyslog.conf $(INSTALL_ROOT)/usr/mv2ram/etc/

clean:

distclean:

copying:

