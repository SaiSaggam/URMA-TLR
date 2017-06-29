HOST_PYTHON_PATH	?= $(COMPO_BASE)/open_src/python/_host_install
TARGET_PYTHON_PATH	?= $(COMPO_BASE)/open_src/python/_install

PYTHON_CFLAGS		= -I$(TARGET_PYTHON_PATH)/usr/include/python2.7
PYTHON_FOR_BUILD	= python2.7
PYTHON_COMPILEALL	= $(HOST_PYTHON_PATH)/usr/lib/python2.7/compileall.py

# Export host python path
export PATH		:= $(HOST_PYTHON_PATH)/usr/bin:$(PATH)
export PYTHONPATH	:= $(TARGET_PYTHON_PATH)/usr/lib/python2.7/sysconfigdata:$(TARGET_PYTHON_PATH)/usr/lib/python2.7/site-packages
export _python_sysroot	= $(TARGET_PYTHON_PATH)
export _python_prefix	= /usr
export _python_exec_prefix = /usr
