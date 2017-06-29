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
# Cloud Connector test framework - Host class
#

################ Import Common libraries
import logging      ### Functions and classes to implement a event logging system
import sys          ### Access to variables or functions maintained by the interpreter
import os           ### Some useful functions on pathnames
import subprocess   ### Allows to spawn new processes, connect to their input/output/error pipes, and obtain their return codes
import time         ### Provides various time-related functions
import shutil       ### Functions to do high-level operations on files and collections of files

################ Import Classes path


##  Main Class for Host
#############################################################

class Host(object):
    "Host class to work with Cloud Connector Test Framework"

    ################ Environment
    # Identifier for the script
    identifier = None
    # Test cases result path
    testResultPath = None
    # source code path for Cloud Connector Test cases
    sourceTestCasesPath = None

    ################ Command line arguments
    # Array List for input arguments
    inputArguments = None

    ################ Logging
    ## Logger class to save system logging
    logger = None


    ################ Execution handlers
    ## Handler Device containing the device script object.
    handlerDevice = None
    ## Handler to connect with the Device Cloud object.
    handlerDeviceCloud = None


    #*************** Network
    ## Default Network interface
    defaultNetworkInterface = None





    ## Class Builder
    ########################################################
    def __init__(self):

        ################ Environment
        self.identifier = "Host"

        ################ Logging
        self.logger = logging.getLogger('cc.host')

        #*************** Network settings
        self.defaultNetworkInterface = "eth0"



    def createEnvironment(self):
        ## Redefine for each Host
        raise Exception("NotImplementedException")

    def createSourceProject(self, sourceGroupPath,testType,testGroup):
        ## Redefine for each Host
        raise Exception("NotImplementedException")

    def buildSourceProject(self):
        ## Redefine for each Host
        raise Exception("NotImplementedException")

    def flashSourceProject(self):
        ## Redefine for each Host
        raise Exception("NotImplementedException")

    def launchCloudConnectorOnDevice(self):
        ## Redefine for each Host
        raise Exception("NotImplementedException")

    def stopCloudConnectorOnDevice(self):
        ## Redefine for each Host
        raise Exception("NotImplementedException")

    def saveSourceProject(self,testType,testGroup):
        ## Redefine for each Host
        raise Exception("NotImplementedException")

    def removeSourceProject(self):
        ## Redefine for each Host
        raise Exception("NotImplementedException")


    def setTestResultPath(self, path):
        # Save the result path for Host
        self.testResultPath = path
        # Save the result path for Device
        self.handlerDevice.testResultPath = path

    def getIP(self, interface=None):
        ## Redefine for each Host
        raise Exception("NotImplementedException")


    ## Device Functions
    ########################################################
    def getHandlerDevice(self):
        # Create a main handler
        handler = None

        # Get device type from the command line
        platform = self.inputArguments.platform

        # Generic Library Device_platform, ej. Device_ansic
        library = "Device_%s" % (platform)

        # Try to import this class
        try:
            exec("import %s" % library )
            chain = "handler = %s.%s()" % (library,library)
            exec(chain)
            self.logger.info("'%s' library for device was successfully loaded" % library)

            # Set identifier for logging system
            handler.setLogIdentifier("cc")
            handler.setIdentifier("Connector")

            # Set the Device ID generation method
            handler.deviceGenerationMethod = self.inputArguments.deviceid_generation_method

            return (True,handler)
        except ImportError:
            _msg = "'%s' library for device was not found." % library
            self.logger.error(_msg)
            return (False,_msg)

    def resetDevice(self):
        ## Redefine for each Host
        raise Exception("NotImplementedException")

    def getDeviceID(self):
        return self.handlerDevice.getDeviceID()

    def getDeviceMAC(self):
        # Setup Device MAC
        if(self.inputArguments.devicemac != None):
            # Set Device MAC
            self.handlerDevice.setDeviceMAC(self.inputArguments.devicemac)
        return self.handlerDevice.getDeviceMAC()

    def setDeviceMAC(self, newDeviceMAC):
        return self.handlerDevice.setDeviceMAC(newDeviceMAC)


    def getDeviceFirmwareVersion(self):
        # Setup Device Firmware Level
        if(self.inputArguments.devicefirmware_version != None):
            # Set Device Firmware Level
            self.handlerDevice.setDeviceFirmwareVersion(self.inputArguments.devicefirmware_version)
        return self.handlerDevice.getDeviceFirmwareVersion()

    def setDeviceFirmwareVersion(self, newDeviceFirmwareVersion):
        return self.handlerDevice.setDeviceFirmwareVersion(newDeviceFirmwareVersion)


    def getDeviceType(self):
        # Setup Device Type
        if(self.inputArguments.devicetype != None):
            # Set Device Type
            self.handlerDevice.setDeviceType(self.inputArguments.devicetype)

        return self.handlerDevice.getDeviceType()
