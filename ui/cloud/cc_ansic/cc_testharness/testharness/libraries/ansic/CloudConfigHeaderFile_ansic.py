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
# Cloud Connector test framework - Cloud Config Header class for ANSI C
#

#*************** Import Common libraries
import logging      ### Functions and classes to implement a event logging system


#*************** Import Classes path
libs = ["BaseFile_ansic"]

for elem in libs:
    exec("import %s" % elem)
    exec("reload (%s)" % elem)


##  Main Class for Cloud Config Header File
class CloudConfigHeaderFile_ansic(BaseFile_ansic.BaseFile_ansic):
    "app/connector_config.h class to work with Cloud Connector Test Framework"


    ## Class Builder
    def __init__(self, filePath):
        ## Call to __init__ from Host(): Initialize function
        super(CloudConfigHeaderFile_ansic, self).__init__(filePath)

        #*************** Environment
        self.identifier = "connector_config_h"

        #*************** Logging
        self.logger = logging.getLogger('cc.connector_config_h')


    def setupHeaderDefines(self,configType):
        # configType:   'nodebug', 'debug', 'library'
        if configType in ['nodebug',]:
            stringToFind = "#define CONNECTOR_DEBUG"
            newSentence = "#define CONNECTOR_NO_DEBUG"
            self.replaceSentence(stringToFind,newSentence)
        elif configType in ['debug']:
            stringToFind = "/* #define CONNECTOR_DEBUG */"
            newSentence = "#define CONNECTOR_DEBUG"
            self.replaceSentence(stringToFind,newSentence)
        elif configType in ['no_malloc']:
            stringToFind = "/* #define CONNECTOR_NO_MALLOC */"
            newSentence = "#define CONNECTOR_NO_MALLOC"
            self.replaceSentence(stringToFind,newSentence)
        elif configType in ['default']:
            # Do nothing
            pass
        else:
            self.logger.error("Configuration type not recognized: '%s'" % configType)



    def setupHeaderCompression(self, compressionEnabled):

        if ( compressionEnabled ):
            newSentence = "#define CONNECTOR_COMPRESSION"

            fileLines = self.fileContent

            # Find if the define is in the file then modify it
            newFileContent = ""
            for line in fileLines.splitlines():
                # Enable compile time data passing so these values are used.
                if('CONNECTOR_COMPRESSION' in line and "#define" in line):
                    newFileContent += "%s\n" % newSentence
                else:
                    newFileContent += "%s\n" % line

            # Save new file content
            self.fileContent = newFileContent



    def setupHeaderDataCompileTime(self):
        foundSentence = False
        fileLines = self.fileContent

        # Find if the define is in the file then modify it
        newFileContent = ""
        for line in fileLines.splitlines():
            # Enable compile time data passing so these values are used.
            if('ENABLE_COMPILE_TIME_DATA_PASSING' in line and "#define" in line):
                foundSentence = True
                newFileContent += "#define   ENABLE_COMPILE_TIME_DATA_PASSING   1\n"
            else:
                newFileContent += "%s\n" % line

        # If this sentence there is not in the file we add it
        if(foundSentence is not True):
            newFileContent = ""
            for line in fileLines.splitlines():
                # Add compile time data passing define.
                if('__CONNECTOR_CONFIG_H_' in line and "#define" in line):
                    newFileContent += "%s\n" % line
                    newFileContent += "\n"
                    newFileContent += "#define   ENABLE_COMPILE_TIME_DATA_PASSING   1\n"
                else:
                    newFileContent += "%s\n" % line

        # Save new file content
        self.fileContent = newFileContent



    def setupHeaderVendorID(self,vendorid):
        hex_string = '%08X' % int(vendorid)
        stringToFind = "#define CONNECTOR_VENDOR_ID"
        newSentence = "#define CONNECTOR_VENDOR_ID                            0x%s" % (hex_string)
        self.replaceSentence(stringToFind,newSentence)


    def setupHeaderCloudServer(self,serverURL):
        stringToFind = "#define CONNECTOR_CLOUD_URL"
        newSentence = "#define CONNECTOR_CLOUD_URL                            \"%s\"" % (serverURL)
        self.replaceSentence(stringToFind,newSentence)


    def setupHeaderDeviceType(self,deviceType):
        stringToFind = "#define CONNECTOR_DEVICE_TYPE"
        newSentence = "#define CONNECTOR_DEVICE_TYPE                          \"%s\"" % (deviceType)
        self.replaceSentence(stringToFind,newSentence)

    def setupHeaderTxKeepAlive(self,tx_keepalive):
        stringToFind = "#define CONNECTOR_TX_KEEPALIVE_IN_SECONDS"
        newSentence = "#define CONNECTOR_TX_KEEPALIVE_IN_SECONDS              %s" % (tx_keepalive)
        self.replaceSentence(stringToFind,newSentence)

    def setupHeaderRxKeepAlive(self,rx_keepalive):
        stringToFind = "#define CONNECTOR_RX_KEEPALIVE_IN_SECONDS"
        newSentence = "#define CONNECTOR_RX_KEEPALIVE_IN_SECONDS              %s" % (rx_keepalive)
        self.replaceSentence(stringToFind,newSentence)
