# This file is for RELEASE IN SDK COPY
#
# Makefile to compile OasisHeader
#

include ../../makefile.envars

COMPO_DIR := $(COMPO_BASE)/amit_open/OasisHeader

include ../../makefile.compo

#

CSIDBASE_DIR := $(COMPO_BASE)/var/csidbase
PSEC_DIR := $(COMPO_BASE)/../../p_section/
CSIDBASE_TAG := $(CSIDBASE_DIR)/.tag.rev$(word 2, $(COMPO_REV))

#
.PHONY: $(CSIDBASE_TAG)

build:
	tar c -C $(COMPO_DIR) --exclude .svn . | tar x -C $(COMPO_BASE)/include

install: $(CSIDBASE_TAG)

$(CSIDBASE_TAG): 
	rm -fr $(CSIDBASE_DIR)
	mkdir -p $(CSIDBASE_DIR)
	$(TOPDIR)/tools/priv/bin/csidbase -s $(COMPO_DIR)/csid -d $(CSIDBASE_DIR) -r
	cp -ar $(CSIDBASE_DIR) $(PSEC_DIR)
	touch $@

clean:
	rm -fr $(CSIDBASE_DIR)

copying:
