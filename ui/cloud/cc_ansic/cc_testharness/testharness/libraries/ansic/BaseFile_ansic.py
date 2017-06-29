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
# Cloud Connector test framework - Base config class for ANSI C
#

#************** Import Common libraries
import logging      ### Functions and classes to implement a event logging system



##  Main Class for Config File
class BaseFile_ansic(object):
    "Based config class to work with Cloud Connector Test Framework"

    #************** Environment
    # Identifier for the script
    identifier = None
    # Source code base
    filePath = None
    # File Content
    fileContent = None


    #************** Logging
    ## Logger class to save system logging
    logger = None





    ## Class Builder
    def __init__(self, filePath):

        #************** Environment
        self.identifier = "BaseFile"
        self.filePath = filePath

        #************** Logging
        self.logger = logging.getLogger('cc.basefile')


    ## Read file
    def readFile(self):
        handlerFile = open(self.filePath, "r")
        self.fileContent = handlerFile.read()
        handlerFile.close()

    ## Write file
    def writeFile(self):
        handlerFile = open(self.filePath, "w")
        handlerFile.write(self.fileContent)
        handlerFile.close()



    def setupFile(self):
        # Write custom File
        self.writeFile()


    def replaceSentence(self,pattern,newSentence):
        if(pattern != None and newSentence != None):
            # Find the line with this pattern and then replace it with newSentence
            newFileContent = ""
            foundSentence = False
            indexLine = 0
            for line in self.fileContent.splitlines():
                if(foundSentence is not True):
                    # Check each Line with the pattern
                    if(pattern in line):
                        line = newSentence
                        foundSentence = True
                newFileContent += "%s\n" % line

            self.fileContent = newFileContent
