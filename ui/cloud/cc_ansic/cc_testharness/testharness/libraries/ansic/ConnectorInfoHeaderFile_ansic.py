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
# Cloud Connector test framework - Connector_info.h class for ANSI C
#

#*************** Import Common libraries
import logging      ### Functions and classes to implement a event logging system


#*************** Import Classes path
libs = ["BaseFile_ansic"]

for elem in libs:
    exec("import %s" % elem)
    exec("reload (%s)" % elem)


##  Main Class for Cloud Config Header File
class ConnectorInfoHeaderFile_ansic(BaseFile_ansic.BaseFile_ansic):
    "private/connector_info.h class to work with Cloud Connector Test Framework"


    ## Class Builder
    def __init__(self, filePath):
        ## Call to __init__ from Host(): Initialize function
        super(ConnectorInfoHeaderFile_ansic, self).__init__(filePath)

        #*************** Environment
        self.identifier = "connector_info_h"

        #*************** Logging
        self.logger = logging.getLogger('cc.connectorinfo_h')


    def setupHeaderDeviceFirmwareVersion(self,deviceFirmwareVersion):
        stringToFind = "#define CONNECTOR_SW_VERSION"
        newSentence = "#define CONNECTOR_SW_VERSION \"%s\"" % (deviceFirmwareVersion)
        self.replaceSentence(stringToFind,newSentence)


    def getDeviceFirmwareVersion(self):
        pattern = "#define CONNECTOR_SW_VERSION"
        # Find the line with this pattern
        for line in self.fileContent:
            # Check each Line with the pattern
            if(pattern in line):
                # #define CONNECTOR_SW_VERSION "2.0.0.0"

                # Split the line
                splitedLine = line.split()

                # Get the last
                firmwareVersion = splitedLine[-1].replace("\"","")

                return firmwareVersion
