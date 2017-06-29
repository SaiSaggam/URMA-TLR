# Makefile for uClibc
#
# Copyright (C) 2002-2003 Erik Andersen <andersen@uclibc.org>
#
# This program is free software; you can redistribute it and/or modify it under
# the terms of the GNU Library General Public License as published by the Free
# Software Foundation; either version 2 of the License, or (at your option) any
# later version.
#
# This program is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE. See the GNU Library General Public License for more
# details.
#
# You should have received a copy of the GNU Library General Public License
# along with this program; if not, write to the Free Software Foundation, Inc.,
# 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA

TOPDIR := $(TOPDIR)/rootfs/src/mipsel-linux-uclibc-
#include $(TOPDIR)Rules.mak

CFLAGS += -DHAVE_CONFIG_H -I. 

#LIBFL=libfl.a
#LIBFL_SHARED=libfl.so
#LIBFL_SHARED_FULLNAME=libfl-$(MAJOR_VERSION).$(MINOR_VERSION).$(SUBLEVEL).so

INSTALL=echo
LN=ln
STRIPTOOL=$(STRIP)


CSRC = buf.c ecs.c misc.c parse.c scanflags.c  sym.c  tblcmp.c \
       ccl.c filter.c nfa.c regex.c scanopt.c tables.c  yylex.c \
       dfa.c gen.c main.c options.c scan.c skel.c tables_shared.c

OBJS=$(patsubst %.c,%.o, $(CSRC))

all: $(LIBFL)

$(LIBFL) ar-target: $(OBJS)
	$(AR) $(ARFLAGS) $(LIBFL) $(OBJS)
	$(RM) $(TOPDIR)lib/$(LIBFL)

$(OBJS): %.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@
	$(STRIPTOOL) -x -R .note -R .comment $*.o

$(OBJ): Makefile

shared: all
	$(CC) $(LDFLAGS) -shared -W1,-soname -o $(LIBFL_SHARED_FULLNAME) $(OBJS) -lm
	#$(LD) $(LDFLAGS) -soname=$(LIBFL_SHARED).$(MAJOR_VERSION) -o $(LIBFL_SHARED_FULLNAME) --whole-archive $(LIBFL) -lc -lm 
	ln -sf $(LIBFL_SHARED_FULLNAME) $(LIBFL_SHARED) 
	$(RM) $(TOPDIR)lib/$(LIBFL_SHARED_FULLNAME) $(TOPDIR)lib/$(LIBFL_SHARED).$(MAJOR_VERSION)

clean: 
	$(RM) *.[oa] *~ core $(LIBFL_SHARED)* $(LIBFL_SHARED_FULLNAME)* *.so
