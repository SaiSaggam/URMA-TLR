# the cpputest version to build if not already defined
CPPUTEST_VERSION ?= 3.7.1

CPPUTEST_TOPDIR = $(CURDIR)

# the directory that all tar files will be downloaded into
TAR_ROOT = $(CPPUTEST_TOPDIR)/tar

# the directory that all cpputest source code will be extracted into
SRC_ROOT = $(CPPUTEST_TOPDIR)/src

# the directory that all cpputest code will be built in
BUILD_ROOT = $(CPPUTEST_TOPDIR)/build

# the directory that all cpputest output files (lib/include) will be installed into
INSTALL_ROOT = $(CPPUTEST_TOPDIR)/install

# the install directory for a specific cpputest version
CPPUTEST_HOME = $(INSTALL_ROOT)/cpputest-$(CPPUTEST_VERSION)

# the source directory for a specific cpputest version
SRC_DIR = $(SRC_ROOT)/cpputest-$(CPPUTEST_VERSION)

SRC_EXTRACTED = $(SRC_DIR)/._cpputest-$(CPPUTEST_VERSION)

# the build directory for a specifc cpputest version
BUILD_DIR = $(BUILD_ROOT)/cpputest-$(CPPUTEST_VERSION)

# the base tar filename
BASE_TAR_FILENAME = cpputest-$(CPPUTEST_VERSION).tar.gz

# the location for the downloaded tar file
TAR_FILE = $(TAR_ROOT)/$(BASE_TAR_FILENAME)

# the Makfile created as part of the configure process
MAKEFILE = $(BUILD_DIR)/Makefile

CPPUTEST_BASE_LIBNAMES = CppUTest \
						 CppUTestExt

# create the list of library filenames from the base names
CPPUTEST_LIBS = $(addsuffix .a,$(addprefix $(CPPUTEST_HOME)/lib/lib,$(CPPUTEST_BASE_LIBNAMES)))

all: $(CPPUTEST_LIBS)

$(TAR_FILE):
	@echo "retrieving cpputest tarball $(TAR_FILE)"
	@mkdir -p $(TAR_ROOT)
	@wget https://github.com/cpputest/cpputest.github.io/blob/master/releases/$(BASE_TAR_FILENAME)?raw=true -O $@

$(SRC_EXTRACTED): $(TAR_FILE)
	@echo "extracting to $(SRC_DIR)"
	@mkdir -p $(SRC_DIR)
	@tar -xvf $^ -C $(SRC_ROOT)
	@#ensure that $(SRC_EXTRACTED) is present to avoid unnecessary tar operations
	touch $(SRC_EXTRACTED)

$(MAKEFILE): $(SRC_EXTRACTED)
	@#running configure will create $(MAKEFILE) in $(BUILD_DIR)
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && \
	$(SRC_DIR)/configure \
		--prefix=$(CPPUTEST_HOME)

$(CPPUTEST_LIBS): $(MAKEFILE)
	@echo "making $(CPPUTEST_LIBS)"
	@cd $(BUILD_DIR) && \
	make install

clean:

distclean: clean
	@rm -rf $(TAR_ROOT)
	@rm -rf $(SRC_ROOT)
	@rm -rf $(BUILD_ROOT)
	@rm -rf $(INSTALL_ROOT)

.PHONY: distclean clean all

