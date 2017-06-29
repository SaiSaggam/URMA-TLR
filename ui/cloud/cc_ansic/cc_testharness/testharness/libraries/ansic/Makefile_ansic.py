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
# Cloud Connector test framework - Makefile Generator class for ANSI C
#

################ Import Common libraries
import sys          ### Access to variables or functions maintained by the interpreter
import os           ### Some useful functions on pathnames
import re           ### Provides regular expression matching operations similar to those found in Perl
import logging      ### Functions and classes to implement a event logging system



##  Main Class for Test Case
#############################################################

class Makefile_ansic(object):
    "Makefile class to work with Cloud Connector Test Framework"

    ################ Environment
    # Identifier for the script
    identifier = None
    # Template filename
    makefileTemplate = None
    # Template file path
    makefileTemplatePath = None
    # Template file content
    makefileTemplateContent = None
    # Destination filename
    makefileDestination = None
    # Destination file path
    makefileDestinationPath = None
    # Destination file content
    makefileDestinationContent = None


    ################ Logging
    ## Logger class to save system logging
    logger = None


    ################ Script
    # Library type
    sampleLibraryType = None # ej. 'run' or 'step'
    # Test type
    sampleType = None # ej. 'dvt','run_sample'...
    # Test Group
    sampleGroup = None # ej. 'connect_to_device_cloud'...
    # Substitutions dictionary
    substitutionsList = None
    # Coverage support
    enableCoverage = None
    # TCP over SSL support
    enableTCPSSL = None
    # Compiler
    defaultCompiler = None
    # Platform
    compilationPlatform = None





    ## Class Builder
    ########################################################
    def __init__(self, mkTemplatePath=None, libType=None, sType=None, sGroup=None):

        ################ Environment
        self.identifier = "Makefile"
        self.makefileTemplate = "Makefile_ansic.template"
        self.makefileDestination = "Makefile"
        self.enableCoverage = False
        self.enableTCPSSL = False
        if(mkTemplatePath is None):
            self.makefileTemplatePath = os.path.abspath(os.path.join(os.path.dirname(__file__), self.makefileTemplate))
        else:
            self.makefileTemplatePath = os.path.abspath(os.path.join(mkTemplatePath, self.makefileTemplate))


        ################ Logging
        self.logger = logging.getLogger('cc.makefile')

        ################ Script
        self.sampleLibraryType = libType
        self.sampleType = sType
        self.sampleGroup = sGroup
        self.substitutionsList = {}
        self.substitutionsList["${CC_VAR_COMPILER}"] = ""
        self.substitutionsList["${CC_VAR_TARGET_PLATFORM}"] = ""
        self.substitutionsList["${CC_VAR_CONNECTOR_DIR}"] = ""
        self.substitutionsList["${CC_VAR_PUBLIC_HEADER_DIR}"] = ""
        self.substitutionsList["${CC_VAR_PLATFORM_DIR}"] = ""
        self.substitutionsList["${CC_VAR_PLATFORM_VPATH}"] = ""
        self.substitutionsList["${CC_VAR_PLATFORM_HEADER_INCLUDE}"] = ""
        self.substitutionsList["${CC_VAR_GAMMU_INCLUDE}"] = ""
        self.substitutionsList["${CC_VAR_APP_SRCS}"] = ""
        self.substitutionsList["${CC_VAR_PLATFORM_SRCS}"] = ""
        self.substitutionsList["${CC_VAR_SRCS}"] = ""
        self.substitutionsList["${CC_VAR_LIBS}"] = ""
        self.substitutionsList["${CC_VAR_POST_SAMPLE}"] = ""


    ## Read file
    def readTemplateFile(self):
        self.logger.info("Read Makefile teamplate from '%s'" % self.makefileTemplatePath)
        handlerFile = open(self.makefileTemplatePath, "r")
        self.makefileTemplateContent = handlerFile.read()
        handlerFile.close()

    ## Write file
    def writeDestinationFile(self):
        self.logger.info("Write Makefile on '%s'" % self.makefileDestinationPath)
        handlerFile = open(self.makefileDestinationPath, "w")
        handlerFile.write(self.makefileDestinationContent)
        handlerFile.close()


    ## Set coverage support
    def setCoverageSupport(self, status):
        self.enableCoverage = status

    ## Set TCP over SSL support
    def setSSLSupport(self, status):
        self.enableTCPSSL = status

    ## Set compiler
    def setCompiler(self, architecture):
        if(architecture == "ARM"):
            self.defaultCompiler = "arm-linux-gcc"
        else:
            self.defaultCompiler = "gcc"

    ## Set Platform
    def setPlatform(self, platform):
        self.compilationPlatform = platform

    ## Generate Makefile for sample
    def generateMakefile(self, outputPath):
        # Save output path
        self.makefileDestinationPath = os.path.abspath(os.path.join(outputPath, self.makefileDestination))
        # Get content from template
        self.makefileDestinationContent = self.makefileTemplateContent

        ################ Setup initial vars for each project
        # List of *.c files in the app directory.
        sourceFileList = [ f for f in os.listdir(outputPath) if f.endswith('.c') ]


        # Samples that only link and don't require platform code.
        linkSamplesList = ['compileandlinkformetrics', 'compile_and_link']
        # Samples that uses SSL connection.
        sslSamplesList = ['connect_on_ssl']
        # Samples that uses Gammu SMS library.
        gammuSamplesList = ['sm_sms_gammu', 'sm_sms_multipart_gammu']



        ################ Script
        # Compiler
        self.substitutionsList["${CC_VAR_COMPILER}"] = self.defaultCompiler

        # Platform
        if(self.sampleGroup in linkSamplesList):
            self.substitutionsList["${CC_VAR_TARGET_PLATFORM}"] = ""
        else:
            # Platform is "linux" or "template"
            self.substitutionsList["${CC_VAR_TARGET_PLATFORM}"] = "# Target Platform\nPLATFORM = %s" % self.compilationPlatform


        # Common vars for all samples: CC_VAR_CONNECTOR_DIR and CC_VAR_PUBLIC_HEADER_DIR
        if(self.sampleType == "samples"):
            # For SAMPLES we have the following structure
            #-- tmp:
            #    |---- private
            #    |---- public
            #           |---- run
            #                  |---- platforms
            #                  |---- samples
            #                          |---- connect_to_device_cloud
            #                          |---- connect_on_ssl
            #           |---- step
            #
            self.substitutionsList["${CC_VAR_CONNECTOR_DIR}"] = "../../../../private"
            self.substitutionsList["${CC_VAR_PUBLIC_HEADER_DIR}"] = "../../../include"
        else:
            # For DVT we have the following structure
            #-- tmp:
            #    |---- app
            #    |---- private
            #    |---- public
            self.substitutionsList["${CC_VAR_CONNECTOR_DIR}"] = "../private"
            self.substitutionsList["${CC_VAR_PUBLIC_HEADER_DIR}"] = "../public/include"



        # Platform directory
        if(self.sampleGroup in linkSamplesList):
            self.substitutionsList["${CC_VAR_PLATFORM_DIR}"] = ""
        else:

            if(self.sampleType == "samples"):
                newContent = "# Location of Platform Src Code.\nPLATFORM_DIR=../../platforms/$(PLATFORM)"
            else:
                newContent = "# Location of Platform Src Code.\nPLATFORM_DIR=../public/%s/platforms/$(PLATFORM)" % self.sampleLibraryType

            # Platform path variable
            self.substitutionsList["${CC_VAR_PLATFORM_DIR}"] = newContent



        # Platform Vpath directory
        if(self.sampleGroup in linkSamplesList):
            self.substitutionsList["${CC_VAR_PLATFORM_VPATH}"] = ""
        else:
            self.substitutionsList["${CC_VAR_PLATFORM_VPATH}"] = "vpath $(PLATFORM_DIR)/%.c"

        # Platform Header Directory
        if(self.sampleGroup in linkSamplesList):
            self.substitutionsList["${CC_VAR_PLATFORM_HEADER_INCLUDE}"] = ""
        else:
            self.substitutionsList["${CC_VAR_PLATFORM_HEADER_INCLUDE}"] = \
                "# Include Platform Header Files.\nCFLAGS += -iquote$(PLATFORM_DIR)"


        # Includes for GAMMU support on SMS samples
        if(self.sampleGroup in gammuSamplesList):
            self.substitutionsList["${CC_VAR_GAMMU_INCLUDE}"] = \
                "# Include Gammu libraries for SMS support.\nCFLAGS += -I/usr/include/gammu"
        else:
            self.substitutionsList["${CC_VAR_GAMMU_INCLUDE}"] = ""



        # Platform source files
        if(self.sampleGroup in linkSamplesList):
            self.substitutionsList["${CC_VAR_PLATFORM_SRCS}"] = ""
        else:
            # Assume this is the base set of Platform files
            # Only include files who are also not in local directory.
            self.substitutionsList["${CC_VAR_PLATFORM_SRCS}"] = "PLATFORM_SRCS = "
            for f in ['os.c', 'config.c', 'debug.c', 'main.c']:
                if f not in sourceFileList:
                    self.substitutionsList["${CC_VAR_PLATFORM_SRCS}"] += '$(PLATFORM_DIR)/%s ' % f


            # Assume this is the base set of network files
            # Only include files who are also not in local directory.
            networkSourceFiles = []
            #** All projects required the network_dns.c file
            networkSourceFiles.append('network_dns.c')
            #** Include support to SSL only for the projects in the list
            if(self.sampleGroup in sslSamplesList or self.enableTCPSSL):
                networkSourceFiles.append('network_tcp_ssl.c')
            else:
                networkSourceFiles.append('network_tcp.c')
            #** Include support to UDP and SMS only for DVT projects
            if(self.sampleType != "samples"):
                # Add support to UDP
                networkSourceFiles.append('network_udp.c')
                # Add support to SMS
                networkSourceFiles.append('network_sms.c')

            for f in networkSourceFiles:
                if f not in sourceFileList:
                    self.substitutionsList["${CC_VAR_PLATFORM_SRCS}"] += '$(PLATFORM_DIR)/%s ' % f



        # APP source files: Add all *.c files in the directory.
        self.substitutionsList['${CC_VAR_APP_SRCS}'] = 'APP_SRCS = ' + ' '.join([ re.sub(r'\.c$', '.c', f) for f in sourceFileList ])

        # if there is an *.rci file, assume rci is enabled and that ConfigGenerator
        # will generate a remote_config.c we want to compile
        # List of *.rci files in the app directory.
        sourceRCIFileList = [ f for f in os.listdir(outputPath) if f.endswith('.rci') ]

        if ( len(sourceRCIFileList) > 0 and "remote_config.c" not in sourceFileList ):
            self.substitutionsList['${CC_VAR_APP_SRCS}'] += " remote_config.c"

        # Source list
        if(self.sampleGroup in linkSamplesList):
            self.substitutionsList["${CC_VAR_SRCS}"] = "SRCS = $(APP_SRCS) $(PRIVATE_SRCS)"
        else:
            self.substitutionsList["${CC_VAR_SRCS}"] = "SRCS = $(APP_SRCS) $(PLATFORM_SRCS) $(PRIVATE_SRCS)"


        # Add linked libraries needed for all projects
        self.substitutionsList['${CC_VAR_LIBS}'] = "LIBS = -lc -lz"
        if(self.sampleGroup not in linkSamplesList):
            if(self.sampleLibraryType == "run"):
                self.substitutionsList['${CC_VAR_LIBS}'] += ' -lpthread -lrt'

            if(self.sampleGroup in sslSamplesList or self.enableTCPSSL):
                self.substitutionsList['${CC_VAR_LIBS}'] += ' -lssl'
                self.substitutionsList['${CC_VAR_LIBS}'] += ' -lcrypto'

            if(self.sampleGroup in gammuSamplesList):
                self.substitutionsList['${CC_VAR_LIBS}'] += ' -lGammu'

            if(self.enableCoverage):
                self.substitutionsList['${CC_VAR_LIBS}'] += ' -lgcov'



        ################ Specific sample rules
        if(self.sampleGroup == 'file_system'):
            if('file_system.c' not in sourceFileList):
                # Add file_system.c to PLATFORM_SRCS.
                self.substitutionsList["${CC_VAR_PLATFORM_SRCS}"] += " $(PLATFORM_DIR)/file_system.c"

            if(self.sampleType == "dvt"):
                self.substitutionsList['${CC_VAR_LIBS}'] += "\nAPP_ENABLE_MD5=true";
                self.substitutionsList['${CC_VAR_LIBS}'] += "\n\nifeq ($(APP_ENABLE_MD5),true)\n"
                self.substitutionsList['${CC_VAR_LIBS}'] += "LIBS += -lcrypto\n"
                self.substitutionsList['${CC_VAR_LIBS}'] += "CFLAGS+= -DAPP_ENABLE_MD5=true\n"
                self.substitutionsList['${CC_VAR_LIBS}'] += "endif\n"
        elif(self.sampleGroup == 'ic_timing'):
            self.substitutionsList['${CC_VAR_LIBS}'] += ' -lrt'

        ################ Specific DVT rules
        if(self.sampleType == 'functional_filesystem'):
            if('file_system.c' not in sourceFileList):
                # Add file_system.c to PLATFORM_SRCS.
                self.substitutionsList["${CC_VAR_PLATFORM_SRCS}"] += " $(PLATFORM_DIR)/file_system.c"

            self.substitutionsList['${CC_VAR_LIBS}'] += "\nAPP_ENABLE_MD5=true";
            self.substitutionsList['${CC_VAR_LIBS}'] += "\n\nifeq ($(APP_ENABLE_MD5),true)\n"
            self.substitutionsList['${CC_VAR_LIBS}'] += "LIBS += -lcrypto\n"
            self.substitutionsList['${CC_VAR_LIBS}'] += "CFLAGS+= -DAPP_ENABLE_MD5=true\n"
            self.substitutionsList['${CC_VAR_LIBS}'] += "endif\n"

        ################ Modify all rules
        for eachLine in self.substitutionsList:
            self.makefileDestinationContent = self.makefileDestinationContent.replace(eachLine, self.substitutionsList[eachLine])


    def setupFile(self):
        # Write custom File
        self.writeDestinationFile()