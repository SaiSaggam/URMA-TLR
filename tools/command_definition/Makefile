CONFIG_FILE = ./config_files/commands.conf
PLATFORM_FILE = ./config_files/platform.txt
CONFIGMGR_DIR = $(LIB_BASE)/config_mgr
CONFIG_GENERATOR_DIR = $(UI_BASE)/cloud/cc_ansic/tools/config
JSON_GENERATOR_DIR = json-simple-1.1.1
CONNECTOR_DIR = $(UI_BASE)/cloud
REFERENCE_DOCS_DIR = $(LIB_BASE)/config_mgr/reference_docs
CONFIG_ARTIFACTS = $(addprefix $(CONFIGMGR_DIR)/,config_def.h config_table.c config_table.h config_valid.c)

.PHONY: all
all: $(CONFIG_ARTIFACTS)

ifndef RCI_OPTIONS
RCI_OPTIONS ="-rci-noUpload"
endif

$(CONFIG_ARTIFACTS): json-simple-1.1.1.jar ConfigGenerator.jar command_tool.jar $(CONFIG_FILE) $(PLATFORM_FILE)
	echo $(CONFIG_ARTIFACTS)
	java -jar command_tool.jar -config_mgr -uci -mib -json -doc -rci $(RCI_OPTIONS) -rcioutpath=$(CONNECTOR_DIR) -p=$(PLATFORM_FILE) -o=$(CONFIGMGR_DIR) $(CONFIG_FILE)
	@zip -j $(REFERENCE_DOCS_DIR)/reference_docs.zip $(REFERENCE_DOCS_DIR)/*.htm

json-simple-1.1.1.jar:
	ant -f $(JSON_GENERATOR_DIR) -Ddist.dir=$(CURDIR)

ConfigGenerator.jar:
	ant -f $(CONFIG_GENERATOR_DIR) -Ddist.dir=$(CURDIR)

command_tool.jar: $(CONFIG_FILE) $(PLATFORM_FILE)
	ant -f ./build.xml

.PHONY: clean
clean:
	ant clean -f $(JSON_GENERATOR_DIR) -Ddist.dir=$(CURDIR)
	ant clean -f $(CONFIG_GENERATOR_DIR) -Ddist.dir=$(CURDIR)
	ant clean
	-rm -f $(CONFIG_ARTIFACTS)

.PHONY: install
install:
	-mkdir -p $(INSTALL_ROOT)/usr/web/descriptors
	install -m 0444 ./descriptor*.xml $(INSTALL_ROOT)/usr/web/descriptors
