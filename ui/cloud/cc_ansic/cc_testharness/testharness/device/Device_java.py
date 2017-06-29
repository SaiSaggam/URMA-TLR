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
# Cloud Connector test framework - Device IAR class
#

# Import Common libraries
import logging          ### Functions and classes to implement a event logging system
import os               ### Some useful functions on pathnames
import time             ### Provides various time-related functions
import sys              ### Access to variables or functions maintained by the interpreter

if ( "linux" in sys.platform ):
    import pexpect      ### Allows to spawn a child application and control it as if a human were typing commands.
else:
    import winpexpect as pexpect        ### Allows to spawn a child application and control it as if a human were typing commands.



# Import Device
libs = ["Device"]

for elem in libs:
    exec("import %s" % elem)
    exec("reload (%s)" % elem)


class Device_java(Device.Device):

    #*************** Environment
    # Source code inside sandbox path for the project
    sourceProjectPath = None

    # Handler to manage the Cloud Connector process
    cloudConnectorProcess = None


    def __init__(self):
        ## Call to __init__ from Device(): Initialize function
        super(Device_java, self).__init__()

        #*************** Environment
        self.identifier = "Device_java"

        #*************** Logging
        self.logger = logging.getLogger('cc.device.java')

        #*************** Connection
        ## Connection Handler
        connectionHandler = None


    def getProvisionedDeviceID(self):
        # Get absolute path where the Device ID is stored
        deviceIdFilePath = os.path.join(self.sourceProjectPath,"dist","connector_config.txt")

        self.loggerExecution.info("Trying to get the Device ID from file: '%s'" % deviceIdFilePath)

        # Implement a retries system
        fileHandler = None
        maxRetries = 10
        countRetries = 0
        self.deviceID = None

        while (self.deviceID == None and countRetries<maxRetries):
            countRetries+=1

            # Verify if file exists
            if( os.path.isfile(deviceIdFilePath) ):
                try:
                    fileHandler = open(deviceIdFilePath, "r")
                    fileContent = fileHandler.read()
                    fileHandler.close()
                except IOError,e:
                    self.loggerExecution.exception(e)

            if( fileHandler is None ):
                # Wait a second for the next loop
                time.sleep(1)
                continue
            else:
                # Get Device ID from file content
                for eachLine in fileContent.splitlines():
                    if("device_id=" in eachLine):
                        # Get the device ID
                        eachLine = eachLine.replace("device_id=","")
                        self.deviceID = eachLine.strip()
                        break

                if( self.deviceID is None or self.deviceID == "" ):
                    self.deviceID = None
                    # Wait a second for the next loop
                    time.sleep(1)

        # Verify if we have a valid file handler
        if (self.deviceID is None):
            self.loggerExecution.error("Could not read provisioned Device ID from: '%s'" % deviceIdFilePath)
        else:
            self.loggerExecution.info("Device ID successfully loaded from file: '%s'" % self.deviceID)


    def run(self):
        '''Run is the function that runs in the new thread and is called by
        start(), inherited from the Thread class
        '''
        ## Start logging
        self.startLoggingSystem()

        self.loggerExecution.info("Start Device for Cloud Connector for Java")


        # Creates a new shell and execute the connector instance
        command = "java -jar ConnectSample.jar"
        executionPath = os.path.join(self.sourceProjectPath,"dist")
        if ( "linux" in sys.platform ):
            self.cloudConnectorProcess = pexpect.spawn( command, cwd=executionPath )
        else:
            self.cloudConnectorProcess = pexpect.winspawn( command, cwd=executionPath )


        while( self.testStopFlag!=True ):

            patternIndex = self.cloudConnectorProcess.expect(["\n", pexpect.TIMEOUT, pexpect.EOF], 1)

            if patternIndex != 1:
                output = self.cloudConnectorProcess.before
                # Save in Execution timeline
                self.loggerExecution.debug(output)
                # Save in console output
                self.logger.info(output.rstrip('\n'))

            if patternIndex == 2:
                self.loggerExecution.warning("EOF signal received in Cloud Connector executable")
                break



        # Send SIGTERM signal
        self.loggerExecution.info("Sending terminate sign to Cloud Connector executable")
        try:

            self.cloudConnectorProcess.terminate()
            #self.cloudConnectorProcess.kill()
        except OSError,e:
            self.loggerExecution.exception(e)
            self.testFinishedFlag = True

        # Wait until finish the process
        while( self.testFinishedFlag is not True and self.cloudConnectorProcess.isalive() ):
            try:
                patternIndex = self.cloudConnectorProcess.expect(["\n", pexpect.TIMEOUT, pexpect.EOF], 1)

                if patternIndex != 1:
                    output = self.cloudConnectorProcess.before
                    # Save in Execution timeline
                    self.loggerExecution.debug(output)
                    # Save in console output
                    self.logger.info(output.rstrip('\n'))

                if patternIndex == 2:
                    break

            except IOError:
                pass
            time.sleep(0.05)


        self.testFinishedFlag = True
