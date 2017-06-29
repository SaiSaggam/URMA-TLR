#
#  Python hello world sample module
#
#  Copyright (C) 2016 by Digi International Inc.
#  All rights reserved.
#

LIBRARY  = _helloworld.so
DISTDIR = usr/lib/python2.7/site-packages

include ../python/python_extensions.mk

#DEFINE  += -g -DDEBUG
DEFINE += -DLINUX_SPECIFIC
INCLUDE = -I./

CFLAGS += $(DEFINE) $(INCLUDE)
LDLIBS +=

# Python headers
CFLAGS += -fno-strict-aliasing -g -O2 -DNDEBUG -fwrapv -O3 -W -Wall -Werror -Wstrict-prototypes -fPIC
CFLAGS += $(PYTHON_CFLAGS)

ODIR    = build/
OBJS    = $(patsubst %.c,$(ODIR)%.o,$(wildcard *.c))
OBJ     = $(patsubst %.so,$(ODIR)%.o,$@)
PYOBJS  = helloworld.pyc


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
	-cp $(addprefix $(ODIR),$(PYOBJS)) dist/$(DISTDIR)/
	-cp *.egg-info dist/$(DISTDIR)/

install: $(LIBRARY) $(PYOBJS)
	-mkdir -p  $(INSTALL_ROOT)/$(DISTDIR)
	install -m 0755 $(addprefix $(ODIR),$^) $(INSTALL_ROOT)/$(DISTDIR)
	install -m 0644 *.egg-info $(INSTALL_ROOT)/$(DISTDIR)

distutils:
	python2.7 setup.py clean -a build

distutils_install:
	python2.7 setup.py install
