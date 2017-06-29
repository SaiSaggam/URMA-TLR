#
#  Python DDP module
#
#  Copyright (C) 2016 by Digi International Inc.
#  All rights reserved.
#
TARGET ?= mips-linux-uclibc
KERNEL_DIR ?= $(SDK_BASE)/linux/src/

include $(TLR_BASE)/build/rds00008.mk

URMA_BASE = $(UI_BASE)/web/urma
LIBRARY  = ddp_if.so
DISTDIR = usr/lib/python2.7/site-packages

include $(OPENSRC_BASE)/python/python_extensions.mk
LIBPATH = -L$(INSTALL_ROOT)/usr/lib -L$(INSTALL_ROOT)/lib -Wl,-rpath-link=$(INSTALL_ROOT)/lib

#DEFINE  += -g -DDEBUG
DEFINE += -DLINUX_SPECIFIC
INCLUDE = -I./ -I$(LIB_BASE)/config_mgr/ -I$(LIB_BASE)/libdigiutils/include/

CFLAGS += $(DEFINE) $(INCLUDE)
LDLIBS += $(LIBPATH) -lconfig_mgr -ldigiutils

# Python headers
CFLAGS += -fno-strict-aliasing -g -O2 -DNDEBUG -fwrapv -W -Wall -Werror -Wstrict-prototypes -fPIC
CFLAGS += $(PYTHON_CFLAGS)

ODIR    = build/
OBJS    = $(patsubst %.c,$(ODIR)%.o,$(wildcard *.c))
OBJ     = $(patsubst %.so,$(ODIR)%.o,$@)
PYOBJS  =

.PHONY: clean dist install distutils_install

all: $(LIBRARY) dist

$(LIBRARY): $(OBJS) $(PYOBJS)
	$(CC) -shared $(LDFLAGS) -o $(ODIR)$@ $(OBJS) $(LDLIBS)

$(OBJS): $(ODIR)%.o: %.c
	-mkdir -p $(@D)
	$(COMPILE.c) -o $@ $<

$(PYOBJS): %.pyc: %.py
	$(PYTHON_FOR_BUILD) -Wi -tt $(PYTHON_COMPILEALL) -d dist/$(DISTDIR) $^
	-cp $@ $(ODIR)$@

clean:
	-rm -f $(PYOBJS)
	-rm -Rf dist
	-rm -Rf build

dist:
	-mkdir -p dist/$(DISTDIR)
	-cp $(addprefix $(ODIR),$(LIBRARY)) dist/$(DISTDIR)/
	#-cp $(addprefix $(ODIR),$(PYOBJS)) dist/$(DISTDIR)/
	-cp *.egg-info dist/$(DISTDIR)/

install: $(LIBRARY) $(PYOBJS)
	-mkdir -p $(INSTALL_ROOT)/$(DISTDIR)
	-mkdir -p $(INSTALL_ROOT)/usr/{bin,web}
	install -m 0755 $(addprefix $(ODIR),$^) $(INSTALL_ROOT)/$(DISTDIR)
	install -m 0644 *.egg-info $(INSTALL_ROOT)/$(DISTDIR)
	@tar -zxf $(URMA_BASE)/device_dist.tar.gz -C $(INSTALL_ROOT)/usr/web/
	install -m 0777 $(URMA_BASE)/fw_progress.sh $(INSTALL_ROOT)/usr/web
	install -m 0777 $(URMA_BASE)/webserver $(INSTALL_ROOT)/usr/bin

distutils:
	python2.7 setup.py clean -a build

distutils_install:
	python2.7 setup.py install
