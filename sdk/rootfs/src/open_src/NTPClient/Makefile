
ifdef TARGET
    TARGET_PREFIX = $(TARGET)-
endif

ifndef CROSS_COMPILE
    CROSS_COMPILE = $(TARGET_PREFIX)
    CC = $(CROSS_COMPILE)gcc
    STRIP = $(CROSS_COMPILE)strip
endif

ifdef LOG_COLLECTOR
    CFLAGS += -DLOG_COLLECTOR=$(LOG_COLLECTOR)
endif

ifdef DEBUG_LEVEL
    CFLAGS += -DDEBUG_LEVEL=$(DEBUG_LEVEL)
endif

ifndef KERNEL_DIR
    KERNEL_DIR=/usr/src/linux
endif

EXEC = ntpclient
OBJS = ntpclient.o phaselock.o

ifdef DEBUG
CFLAGS += -g -DDEBUG
STRIP=true
else
CFLAGS += -O2
STRIP=$(CROSS_COMPILE)strip
endif
CFLAGS += -Wall

LFLAGS += $(LIBPATH) #-lcsman

all: precheck $(EXEC) 
	$(STRIP) --remove-section=.note --remove-section=.comment $(EXEC) 

$(EXEC): $(OBJS)
	$(CC) $(LFLAGS) -o ${EXEC} $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -c -o $@ $<

install: postcheck
	@cp -p ntpclient					$(INSTALL_ROOT)/usr/bin/

precheck:
	@echo ""
	@echo "Build $(shell basename `pwd`) for $(PROJECT_NAME) ($(PROJECT_CUSTOMER))"
ifeq ($(PROJECT_CUSTOMER), ) 
	@echo ""
	@echo "No specify PROJECT_CUSTOMER in Makefile. "
	@echo "Maybe you should update the components Makefile of the Project"
	@echo ""
	@exit 1
endif 

postcheck:                                   
	@echo ""
	@echo "Install $(shell basename `pwd`) to $(INSTALL_ROOT)" 
ifeq ($(INSTALL_ROOT), ) 
	@echo ""
	@echo "No specify INSTALL_ROOT in Makefile. "
	@echo "Maybe you should update the components Makefile of the Project"
	@echo ""
	@exit 1
endif  

clean:
	-rm -f $(EXEC)  *.o

