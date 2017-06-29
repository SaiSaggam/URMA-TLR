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
# Cloud Connector test framework - Host IAR class
#

################ Import Common libraries
import tempfile     ### This module generates temporary files and directories
import logging      ### Functions and classes to implement a event logging system
import sys          ### Access to variables or functions maintained by the interpreter
import os           ### Some useful functions on pathnames
import subprocess   ### Allows to spawn new processes, connect to their input/output/error pipes, and obtain their return codes
import time         ### Provides various time-related functions
import shutil       ### Functions to do high-level operations on files and collections of files

################ Import Cloud Connector libraries for ANSI C
libs = ["Host_win","SerialHandler"]

for elem in libs:
    exec("import %s" % elem)
    exec("reload (%s)" % elem)


class Host_iar(Host_win.Host_win):
    ################ Environment


    def __init__(self):
        ## Call to __init__ from Host(): Initialize function
        super(Host_iar, self).__init__()

        ################ Environment
        self.identifier = "Host_iar"

        ################ Logging
        self.logger = logging.getLogger('cc.host.iar')




    def createEnvironment(self):
        self.logger.info("Creating environment for Host IAR")

        return (True,"Environment for Host was successfully created")




    def resetDevice(self):
        self.logger.info("Reseting device for Host IAR")

        ################ Create handlerDevice
        result,handler = self.getHandlerDevice()
        if(result != True):
            return (False,"An error was occurred getting the handler Device: %s" % handler)
        else:
            self.handlerDevice = handler

            ################ Setup handlerDevice
            status = self.setupDevice()
            if(status[0] != True):
                return (False,"An error was occurred setting the handler Device: %s" % status[1])
            else:
                return (True,"Environment for Host was successfully created")


    def setupDevice(self):
        self.handlerDevice.deviceID = "00000000-00000000-00049DFF-FF010204"


        # Get the serial port path and the baud rate
        handler = SerialHandler.SerialHandler(self.inputArguments.deviceSerialConsolePort)

        # Set Serial Port baudrate
        handler.connectionBaudrate = self.inputArguments.deviceSerialConsoleBaudrate

        # Set connection method
        handler.connectionMethod = self.inputArguments.deviceConnectionMethod

        handler.setLogIdentifier("cc")
        handler.setIdentifier("console_output")

        handler.disableLogExecution()

        self.handlerDevice.connectionHandler = handler

        return(True,"Device was set successfully")




    def createSourceProject(self, sourceGroupPath,testType,testGroup):
        self.logger.info("Create Source project for Host IAR")

        return (True,"Source was successfully created")




    def buildSourceProject(self):
        self.logger.info("Build Source project for Host IAR")

        return (True,"Cloud Connector Project was successfully compiled")




    def flashSourceProject(self):
        self.logger.info("Skip flash process to Cloud Connector for ANSI C")

        return (True,"Cloud Connector Project was successfully updloaded")




    def launchCloudConnectorOnDevice(self):
        self.logger.info("Start Cloud Connector for Host IAR")

        # Set Output folder
        self.handlerDevice.testResultPath = self.testResultPath
        # Launch thread monitoring Device
        self.handlerDevice.start()

        return True




    def stopCloudConnectorOnDevice(self):
        self.logger.info("Stop Cloud Connector for ANSI C on the PC")

        # Launch thread monitoring Device
        self.handlerDevice.stop()

        # Wait until device is stopped
        timeout = 60
        while( timeout>0 and self.handlerDevice.isFinished()!=True):
            timeout-=1
            time.sleep(2)

        if(timeout>0):
            self.logger.info("Cloud Connector for IAR was stopped successfully")
            return True
        else:
            self.logger.error("Cloud Connector for IAR was not stopped due to a timeout")
            return False


        return True




    def saveSourceProject(self,testType,testGroup):
        self.logger.info("Save Source project for Host IAR")

        # Get IP of this server
        serverIP= self.getIP()
        self.logger.info("Host has the following IP: %s" % serverIP)

        return True




    def removeSourceProject(self):
        self.logger.info("Remove Source project for Host IAR")

        return True


