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
# Cloud Connector test framework - Network SMS Proxy class for ANSI C
#

#*************** Import Common libraries
import logging      ### Functions and classes to implement a event logging system
import re           ### This module provides regular expression matching operations


#*************** Import Classes path
libs = ["BaseFile_ansic"]

for elem in libs:
    exec("import %s" % elem)
    exec("reload (%s)" % elem)


##  Main Class for Cloud Config Header File
class NetworkSmsProxyFile_ansic(BaseFile_ansic.BaseFile_ansic):
    "public/run_step/platform/linux/network_sms_proxy.c_aux class to work with Cloud Connector Test Framework"



    ## Class Builder
    def __init__(self, filePath):
        ## Call to __init__ from BaseFile_ansic(): Initialize function
        super(NetworkSmsProxyFile_ansic, self).__init__(filePath)

        #*************** Environment
        self.identifier = "network_sms_proxy_c_aux"

        #*************** Logging
        self.logger = logging.getLogger('cc.network_sms_proxy_c_aux')



    def setupGatewayIP(self, newIP):

        # Initialize flags
        newFileContent = ""
        foundSentence = False

        # Split the file content in lines
        lines = self.fileContent.splitlines()

        # Check all lines in the file to find all testcases
        for eachLine in lines:
            # Original sentence:   #define GW_ADDR   "10.101.1.121"
            pattern = "^ *#define +GW_ADDR"
            # Find the line with the test cases header
            if ( re.search(pattern, eachLine, flags=re.IGNORECASE) ):
                eachLine = '#define GW_ADDR        "%s"' % newIP
                foundSentence = True

            # Add the readed line
            newFileContent += "%s\n" % eachLine

        # If this sentence there is not in the file we add it
        if( not foundSentence ):
            self.logger.error("Error adding the gateway IP in network_sms_proxy_c_aux")

        # Save new file content
        self.fileContent = newFileContent
