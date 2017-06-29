TARGET_PREFIX=
TLR_BASE := $(shell cd ..; pwd)
export TLR_BASE 

include $(TLR_BASE)/build/makefile.globals

TARGETS = cunit cpputest
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

install:

distclean:
	$(MAKE) -C test_frameworks distclean

