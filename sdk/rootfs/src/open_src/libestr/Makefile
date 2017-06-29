# RDS/00008: OASIS Makefile Conventions

#for test
TOPDIR := $(shell cd ../../../../; pwd)
TARGET ?= mipsel-linux-uclibc
KERNEL_DIR ?= ${TOPDIR}/linux/src/
INSTALL_DIR = $(shell pwd)/_install

PREFIX += PROJECT_NAME=${PROJECT_NAME} 
PREFIX += PROJECT_CUSTOMER=${PROJECT_CUSTOMER} 
PREFIX += PRIV_TOOLS=${PRIV_TOOLS} 
PREFIX += INSTALL_ROOT=$(INSTALL_ROOT)
PREFIX += ENDIAN=$(ENDIAN)

SRC_TOP_DIR	=	$(shell cd ../../; pwd)
INCLUDEDIR	=	$(SRC_TOP_DIR)/include
LIBDIR		=	$(SRC_TOP_DIR)/$(TARGET)-lib
LDLIB		=	-L$(SRC_TOP_DIR)/$(TARGET)-lib -L/opt/buildroot-gcc463/usr/mipsel-unknown-linux-uclibc/sysroot/usr/lib
CFLAGS		+=	-I$(INCLUDEDIR)

all:
	if [ ! -e src/Makefile ]; then \
		cd src; ./configure --host=$(TARGET) \
		CC=$(CROSS_COMPILE)gcc \
		CXX=$(CROSS_COMPILE)g++ \
		LD=$(CROSS_COMPILE)ld \
		STRIP=$(CROSS_COMPILE)strip \
		LDFLAGS="$(LDLIB)" \
		ac_cv_func_malloc_0_nonnull=yes \
		ac_cv_func_realloc_0_nonnull=yes \
		--prefix=/ \
		--sysconfdir=/etc; \
	fi;
	make -C src
	
install:
	@$(MAKE) -C src DESTDIR=$(INSTALL_DIR) install

	@mkdir -p $(INSTALL_ROOT)/lib
	@cp -a $(INSTALL_DIR)/lib/*.so*	$(INSTALL_ROOT)/lib/

clean:
	@rm -Rf $(INSTALL_DIR)
	if [ -e src/Makefile ]; then make -C ./src clean; rm src/Makefile; fi;

distclean:
	@rm -Rf $(INSTALL_DIR)
	if [ -e src/Makefile ]; then make -C ./src distclean; rm src/Makefile; fi;

