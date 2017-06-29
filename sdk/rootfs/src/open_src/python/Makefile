# RDS/00008: OASIS Makefile Conventions

#for test
TOPDIR := $(shell cd ../../../../; pwd)
TARGET ?= mipsel-linux-uclibc
KERNEL_DIR ?= ${TOPDIR}/linux/src/
INSTALL_DIR = $(shell pwd)/_install
HOST_INSTALL = $(shell pwd)/_host_install
UCLIBCDIR    = $(subst buildroot-gcc342/bin/,buildroot-gcc342/mipsel-linux-uclibc/lib,$(TOOLPATH))
UCLIBC_INCDIR = $(subst buildroot-gcc342/bin/,buildroot-gcc342/mipsel-linux-uclibc/sysroot/usr/include,$(TOOLPATH))

PREFIX += PROJECT_NAME=${PROJECT_NAME} 
PREFIX += PROJECT_CUSTOMER=${PROJECT_CUSTOMER} 
PREFIX += PRIV_TOOLS=${PRIV_TOOLS} 
PREFIX += INSTALL_ROOT=$(INSTALL_ROOT)
PREFIX += ENDIAN=$(ENDIAN)

SRC_TOP_DIR	=  $(shell cd ../../; pwd)
LIBDIR		=  $(SRC_TOP_DIR)/$(TARGET)-lib
CFLAGS		+= -I$(SRC_TOP_DIR)/include -I$(UCLIBCDIR) -I$(SRC_TOP_DIR)/open_src/Oopenssl_102d/_install/usr/include
LDFLAGS		+= -L$(SRC_TOP_DIR)/$(TARGET)-lib -L$(UCLIBC_INCDIR) -L$(SRC_TOP_DIR)/open_src/Oopenssl_102d/_install/usr/lib

export PATH	:= $(HOST_INSTALL)/usr/bin:$(PATH)

all: for_host .stamp_target_built

.stamp_target_built:
	echo "Building for target"
	
	if [ ! -e src/Makefile ]; then \
		cd src; \
		ac_cv_have_long_long_format=yes \
		ac_cv_file__dev_ptmx=yes \
		ac_cv_file__dev_ptc=yes \
		ac_cv_working_tzset=yes \
		./configure \
			--target=$(TARGET) \
			--host=$(TARGET) \
			--build=i686-pc-linux-gnu \
			CC=$(CROSS_COMPILE)gcc \
			CXX=$(CROSS_COMPILE)g++ \
			LD=$(CROSS_COMPILE)gcc \
			STRIP=$(CROSS_COMPILE)strip \
			LDFLAGS="$(LDFLAGS)" \
			CFLAGS="$(CFLAGS)" \
			--prefix=/usr \
			--exec_prefix=/usr \
			--sysconfdir=/etc \
			--enable-shared \
			--disable-curses \
			--with-expat=builtin \
			--disable-bsddb \
			--disable-sqlite3 \
			--disable-codecs-cjk \
			--disable-bz2 \
			--disable-ossaudiodev \
			--disable-pydoc \
			--disable-test-modules \
			--disable-lib2to3 \
			--disable-gdbm \
			--disable-tk \
			--disable-nis \
			--disable-dbm \
			--disable-pyo-build \
			--without-cxx-main \
			--without-doc-strings ; \
	fi;
	make -C src

	-mkdir -p $(INSTALL_DIR)
	$(MAKE) -C src install DESTDIR=$(INSTALL_DIR)

	@ln -sf python2 $(INSTALL_DIR)/usr/bin/python

	# Stripping needs write permission to library
	chmod u+w $(INSTALL_DIR)/usr/lib/libpython2.7.so.1.0

	# Remove useless files
	rm -f $(INSTALL_DIR)/usr/bin/python*-config
	rm -f $(INSTALL_DIR)/usr/bin/smtpd.py
	find $(INSTALL_DIR)/usr/lib/python2.7/config/ -type f -not \
		-name pyconfig.h -a -not -name Makefile -delete

	find $(INSTALL_DIR)/usr/lib/python2.7 -type f -iname "*.exe" -delete

	# We need only the compiled .pyc files on the target
	find $(INSTALL_DIR)/usr/lib/python2.7 -type f -iname "*.py" -delete

	touch .stamp_target_built

for_host:
	if [ ! -f $(HOST_INSTALL)/usr/bin/python -o ! -f $(HOST_INSTALL)/usr/bin/pgen ]; then \
		rm -f .stamp_target_built; \
		echo "Building for host"; \
		cd src; \
		./configure \
			CC=gcc \
			CXX=g++ \
			LD=ld \
			STIP=strip \
			--prefix=$(HOST_INSTALL)/usr \
			--exec_prefix=$(HOST_INSTALL)/usr \
			--sysconfdir=$(HOST_INSTALL)/etc \
			--without-cxx-main \
			--disable-sqlite3 \
			--disable-tk \
			--with-expat=builtin \
			--disable-curses \
			--disable-codecs-cjk \
			--disable-nis \
			--enable-unicodedata \
			--disable-dbm \
			--disable-gdbm \
			--disable-bsddb \
			--disable-test-modules \
			--disable-bz2 \
			--disable-ossaudiodev \
			--disable-pydoc \
			--disable-lib2to3 \
			--disable-pyo-build && \
		make python && \
		make Parser/pgen && \
		mkdir -p $(HOST_INSTALL) && \
		make install && \
		ln -sf python2 $(HOST_INSTALL)/usr/bin/python && \
		ln -sf python2-config $(HOST_INSTALL)/usr/bin/python-config && \
		install -m 0755 Parser/pgen $(HOST_INSTALL)/usr/bin && \
		make distclean; \
	fi

install:
	@mkdir -p $(INSTALL_ROOT)/usr/{bin,lib}
	@cp -af $(INSTALL_DIR)/usr/bin/*			$(INSTALL_ROOT)/usr/bin/
	@cp -af $(INSTALL_DIR)/usr/lib/libpython2.7.so.*	$(INSTALL_ROOT)/usr/lib/
	@cp -af $(INSTALL_DIR)/usr/lib/python2.7		$(INSTALL_ROOT)/usr/lib/

	@ # Install python sandbox binary as a copy of the python binary
	@cp -af $(INSTALL_DIR)/usr/bin/python2.7		$(INSTALL_ROOT)/usr/bin/python2.7_sb
	@ln -sf python2.7_sb					$(INSTALL_ROOT)/usr/bin/python2_sb
	@ln -sf python2.7_sb					$(INSTALL_ROOT)/usr/bin/python_sb

clean:
	@rm -f .stamp_target_built
	@rm -Rf $(INSTALL_DIR)
	@rm -Rf $(HOST_INSTALL)
	if [ -e src/Makefile ]; then make -C ./src clean; rm src/Makefile; fi;

distclean:
	@rm -f .stamp_target_built
	@rm -Rf $(INSTALL_DIR)
	@rm -Rf $(HOST_INSTALL)
	if [ -e src/Makefile ]; then make -C ./src distclean; rm src/Makefile; fi;

