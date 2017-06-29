##############################################################################
# Copyright (c) 2016 Digi International Inc., All Rights Reserved
#
# This software contains proprietary and confidential information of Digi
# International Inc.  By accepting transfer of this copy, Recipient agrees
# to retain this software in confidence, to prevent disclosure to others,
# and to make no use of this software other than that for which it was
# delivered.  This is an unpublished copyrighted work of Digi International
# Inc.  Except as permitted by federal law, 17 USC 117, copying is strictly
# prohibited.
#
# Restricted Rights Legend
#
# Use, duplication, or disclosure by the Government is subject to
# restrictions set forth in sub-paragraph (c)(1)(ii) of The Rights in
# Technical Data and Computer Software clause at DFARS 252.227-7031 or
# subparagraphs (c)(1) and (2) of the Commercial Computer Software -
# Restricted Rights at 48 CFR 52.227-19, as applicable.
#
# Digi International Inc. 11001 Bren Road East, Minnetonka, MN 55343
#
##############################################################################

LAST_TAG     = `git describe --tag --abbrev=0 --match TLR_build_* | sed "s/^TLR_build_//"`
LAST_COMMIT  = `git rev-parse --verify --short HEAD`
SW_VERSION_FILE = sdk/rootfs/root.released/usr/etc/tlr-version

SW_VERSION ?= $(LAST_TAG)-$(LAST_COMMIT)

REMOVE_FILES_FOR_RELEASE = \
	scripts/usr/etc/init.d/S11telnetd.sh \
	scripts/usr/etc/init.d/common_dev_board_cmds.sh

.PHONY: all
all: version_file
	-mkdir -p sdk/rootfs/digi
	@cp build/makefile.vars sdk/rootfs/digi/
	@cp -r build/*.mk sdk/rootfs/digi/
	@for exclude in $(EXCLUDE) ; do \
		rm -rf sdk/rootfs/digi/$$exclude.*.mk ; \
	done
	@cp build/config/project.instcomp_user sdk/project_config/
	@cp build/config/makefile.config_user sdk/rootfs/
	@make -C sdk/ all
	@rm -f sdk/image/lr54-manufacture-$(SW_VERSION).zip
	@zip -j sdk/image/lr54-manufacture-$(SW_VERSION).zip sdk/image/user.bin sdk/image/firmware.bin sdk/bootloader/bootloader.bin

.PHONY: version_file
version_file:
	echo "Software Version:" $(SW_VERSION) `date +'%m/%d/%y %H:%M:%S'` > $(SW_VERSION_FILE)

.PHONY: test
test:
	@echo "Running unit tests"
	@make -C unit_test/ all

.PHONY: release
release:
	$(eval SW_VERSION := $(LAST_TAG))
	@echo "Building release $(SW_VERSION)"
	@echo "Removing files for release"
	@rm -f $(REMOVE_FILES_FOR_RELEASE)
	$(MAKE) SW_VERSION=$(SW_VERSION)

.PHONY: install
install:
	@make -C sdk/ install

.PHONY: clean
clean:
	@make -C sdk/ clean
	@rm -rf sdk/rootfs/digi
	@rm -f sdk/project_config/project.instcomp_user
	@rm -f sdk/rootfs/makefile.config_user

.PHONY: distclean
distclean:
	# Hmm. there is no distclean target in sdk/Makefile
	#@make -C sdk/ distclean
	@make -C unit_test/ distclean

