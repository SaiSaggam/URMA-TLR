#
# Makefile for extra tools
#
# (Corner DE core file)

export TOPDIR := $(shell cd ..; pwd)

export TARGET       :=
export COMPO_BASE   := $(TOPDIR)/tools/src
export INSTALL_ROOT := $(TOPDIR)/tools

#

.PHONY: all main

all: main

#

ifneq ($(wildcard makefile.corner),)
include makefile.corner
endif

include $(TOPDIR)/project_config/project.config
include compo/makefile.config

# function macros

GET_COMPO = $(word 2,$(subst ., ,$(subst /, ,$(1))))

#

.PHONY: bin_subdir src_subdir $(COMPO_LIST) tools_ver

main: bin_subdir src_subdir $(COMPO_LIST) tools_ver
	@echo "*********"
	@echo "* Done! *"
	@echo "*********"

bin_subdir: $(INSTALL_ROOT)/.tag.dir-ready

src_subdir: $(COMPO_BASE)/.tag.dir-ready

$(INSTALL_ROOT)/.tag.dir-ready:
ifdef FULL_COPY
	mkdir -p $(INSTALL_ROOT)/priv/bin
	mkdir -p $(INSTALL_ROOT)/open/bin
	touch $@
endif

$(COMPO_BASE)/.tag.dir-ready:
ifdef FULL_COPY
	mkdir -p $(COMPO_BASE)/include
	mkdir -p $(COMPO_BASE)/lib
	touch $@
endif

$(COMPO_LIST):
	@if test "$(EXCLUDE.$(call GET_COMPO,$@))" != "y"; then \
		if test -f $@/Makefile; then \
			echo "*** Making $@..."; \
			make -s -C $@; \
		else \
			echo "!!!---->>> $@ <<<--- may be removed"; \
		fi \
	fi

tools_ver:
ifdef FULL_COPY
	@TOOL_VER=`svnversion -n $(TOPDIR)/tools`; \
		echo $${TOOL_VER%M} > $(INSTALL_ROOT)/.tag.tools_ver
endif

#

.PHONY: clean

clean: $(patsubst %,clean-%,$(COMPO_LIST))
ifdef FULL_COPY
	@rm -fr $(COMPO_BASE)/include
	@mkdir -p $(COMPO_BASE)/include
	@rm -fr $(COMPO_BASE)/lib
	@mkdir -p $(COMPO_BASE)/lib
endif

$(patsubst %,clean-%,$(COMPO_LIST)):
	@if test "$(EXCLUDE.$(call GET_COMPO,$@))" != "y"; then \
		if test -f $(patsubst clean-%,%,$@)/Makefile; then \
			echo "*** Cleaning $(patsubst clean-%,%,$@)..."; \
			make -s -C $(patsubst clean-%,%,$@) check-then-clean; \
		else \
			echo "!!!------>>> $(patsubst clean-%,%,$@) <<<--- may be removed"; \
		fi \
	fi

#

.PHONY: distclean

distclean:
	rm -fr open priv tmp
	rm -f $(INSTALL_ROOT)/.tag.*
	#rm -fr $(COMPO_BASE)

	