#default cpputest version to use if not already defined
CPPUTEST_VERSION ?= 3.7.1

# Note - CPPUTEST_HOME path must match what the Makefile in CPPUTEST_TOPDIR
# uses.
INSTALL_ROOT = $(CPPUTEST_TOPDIR)/install
CPPUTEST_HOME = $(INSTALL_ROOT)/cpputest-$(CPPUTEST_VERSION)

# these variables need to be exported as they are required if building the cpputest libraries
export CPPUTEST_VERSION

CPPUTEST_LDFLAGS = -L$(CPPUTEST_HOME)/lib
CPPUTEST_CPPFLAGS = -I$(CPPUTEST_HOME)/include
CPPUTEST_BASE_LIBNAMES = CppUTest \
						 CppUTestExt

# create the list of library filenames
CPPUTEST_LIB_FILENAMES = $(addsuffix .a,$(addprefix $(CPPUTEST_HOME)/lib/lib,$(CPPUTEST_BASE_LIBNAMES)))

CPPFLAGS += $(CPPUTEST_CPPFLAGS)
LDFLAGS += $(CPPUTEST_LDFLAGS)

#create the list of libs to be added to the link command 
LIBS += $(addprefix -l,$(CPPUTEST_BASE_LIBNAMES))

all: $(CPPUTEST_LIB_FILENAMES)

$(CPPUTEST_LIB_FILENAMES):
	make -C $(CPPUTEST_TOPDIR)

