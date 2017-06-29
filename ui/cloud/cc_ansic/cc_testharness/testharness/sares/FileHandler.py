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
# S.A.R.E.S Framework - Connection handler class based on a File

# Import to get compatibility with SARES
#import ExpandSysPaths
import sys

libs = ['ConnectionHandler','FileReader']

for elem in libs:
    exec("import %s" % elem)
    exec("reload (%s)" % elem)


## Connection handler class based on a File
class FileHandler(ConnectionHandler.ConnectionHandler):
    "Main class to manage a serial port"
    ## System path to the file
    connectionFilePath = None


    ## Class Builder
    def __init__(self, path=None):
        super(FileHandler, self).__init__()
        self.setIdentifier("FileHandler")
        ## Initialize file path ej. /tmp/text.log
        self.connectionFilePath = path


    ## (private) Creates(__init__) the Command Library object
    #   @param self reference for this object
    def _initializeConnectionHandler(self):
        # Create handler, port and baudrate are mandatory
        self.connectionHandler = FileReader.FileReader(self.connectionFilePath)


    ## Clean environment before exit
    #   @param self reference for this object
    #   @return true if all was ok, otherwise false
    def free(self):
        ConnectionHandler.free(self)
        del self.connectionFilePath


    def stopExecution(self):
        self.stop = True
        self.connectionHandler.close()

