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
# Cloud Connector test framework - Platform Header class for ANSI C
#

#*************** Import Common libraries
import logging      ### Functions and classes to implement a event logging system


#*************** Import Classes path
libs = ["BaseFile_ansic"]

for elem in libs:
    exec("import %s" % elem)
    exec("reload (%s)" % elem)


##  Main Class for Cloud Config Header File
class platformHeaderFile_ansic(BaseFile_ansic.BaseFile_ansic):
    "public/run_step/platform/linux/platform.h class to work with Cloud Connector Test Framework"



    ## Class Builder
    def __init__(self, filePath):
        ## Call to __init__ from Host(): Initialize function
        super(platformHeaderFile_ansic, self).__init__(filePath)

        #*************** Environment
        self.identifier = "platform_h"

        #*************** Logging
        self.logger = logging.getLogger('cc.platform_h')



    def setupHeaderCertificate(self):
        stringToFind = "#define APP_SSL_CA_CERT"
        newSentence = "#define APP_SSL_CA_CERT \"../public/certificates/Digi_Int-ca-cert-public.crt\""
        self.replaceSentence(stringToFind,newSentence)

        # BUG: due to a wrong fix in the code we remove a comment with a wrong path
        stringToFind = "/* #define APP_SSL_CA_CERT"
        newSentence = ""
        self.replaceSentence(stringToFind,newSentence)
