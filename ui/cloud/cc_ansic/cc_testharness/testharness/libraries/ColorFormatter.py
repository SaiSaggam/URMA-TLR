# ***************************************************************************
# Copyright (c) 2013 Digi International Inc.,
# All rights not expressly granted are reserved.
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this file,
# You can obtain one at http://mozilla.org/MPL/2.0/.
#
# Digi International Inc. 11001 Bren Road East, Minnetonka, MN 55343
#
# ***************************************************************************
#
# Cloud Connector test framework - Custom Formatter class to use with logging system
#

#************** Import Common libraries
import logging      ### Functions and classes to implement a event logging system



##  Formatter class to allow colors in logging system
class ColorFormatter(logging.Formatter):

    def color(self, level=None):
        codes = {\
            None:       (0,   0),
            'DEBUG':    (0,   2), # text gray
            'INFO':     (0,   0), # text standard
            'WARNING':  (1,  33), # text yellow
            'ERROR':    (1,  31), # text red
            'CRITICAL': (1, 101), # text black, background red
            }
        return (chr(27)+'[%d;%dm') % codes[level]


    def format(self, record):
        retval = logging.Formatter.format(self, record)
        return self.color(record.levelname) + retval + self.color()
