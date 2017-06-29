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
# Cloud Connector test framework - Device ANSI-C class
#

# Import Common libraries
import logging          ### Functions and classes to implement a event logging system
import os               ### Some useful functions on pathnames
import random           ### Provide pseudo-random number generators
import subprocess       ### Allows to spawn new processes, connect to their input/output/error pipes, and obtain their return codes
import time             ### Provides various time-related functions
#import fcntl            ### Provide file control and I/O control on file descriptors
import pexpect          ### Allows to spawn a child application and control it as if a human were typing commands.
import array            ### Defines an object type which can compactly represent an array of basic values. Used for reading an array from a file



# Import Device
libs = ["Device"]

for elem in libs:
    exec("import %s" % elem)
    exec("reload (%s)" % elem)


class Device_ansic(Device.Device):

    #*************** Environment
    # Source code inside sandbox path for the project
    sourceProjectPath = None

    # Handler to manage the Cloud Connector process
    cloudConnectorProcess = None



    def __init__(self):
        ## Call to __init__ from Device(): Initialize function
        super(Device_ansic, self).__init__()

        #*************** Environment
        self.identifier = "Device_ansic"

        #*************** Logging
        self.logger = logging.getLogger('cc.device.ansic')


        #*************** Local settings
        self.setDeviceFirmwareVersion("1.0.0.0")


    def getProvisionedDeviceID(self):
        # Get absolute path where the Device ID is stored
        deviceIdFilePath = os.path.join(self.sourceProjectPath,"device_id.cfg")

        self.loggerExecution.info("Trying to get the Device ID from file: '%s'" % deviceIdFilePath)

        # Implement a retries system
        fileHandler = None
        maxRetries = 10
        countRetries = 0

        while (fileHandler == None and countRetries<maxRetries):
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

        # Verify if we have a valid file handler
        if (fileHandler is None):
            self.loggerExecution.error("Could not read provisioned Device ID from: '%s'" % deviceIdFilePath)
            self.deviceID = None
            return

        # Get Device ID from file content
        hexArray = []
        for eachByte in fileContent:
            # Convert to hexadecimal of two digits each byte
            hexByte = "%02X" % ord(eachByte)
            hexArray.append(hexByte)

        # Compound the Device ID
        self.deviceID = "%s-%s-%s-%s" % ( "".join(hexArray[:4]),"".join(hexArray[4:8]),"".join(hexArray[8:12]),"".join(hexArray[12:]) )

        self.loggerExecution.info("Device ID successfully loaded from file: '%s'" % self.deviceID)



    def run(self):
        '''Run is the function that runs in the new thread and is called by
        start(), inherited from the Thread class
        '''
        ## Start logging
        self.startLoggingSystem()

        self.loggerExecution.info("Start Device for Cloud Connector for ANSI C from: '%s'" % self.sourceProjectPath)


        #**********************************#
        #**  METHOD A) Using subprocess  **#
        #**********************************#

        #process = subprocess.Popen(
            #['./connector'],
            #cwd=self.sourceProjectPath,
            #stdout=subprocess.PIPE,
            #stderr=subprocess.STDOUT)


        #fdh1 = process.stdout
        ## configure subprocess output as non blocking
        #fl = fcntl.fcntl(fdh1, fcntl.F_GETFL)
        #fcntl.fcntl(fdh1, fcntl.F_SETFL, fl | os.O_NONBLOCK)

        #while( self.testStopFlag!=True and process.poll()==None ):
            #try:
                ##print process.stdout.readline()
                #newLine = process.stdout.readline()
                ## Save in Execution timeline
                #self.loggerExecution.debug(repr(newLine))
                ## Save in console output
                #self.logger.info(newLine.rstrip('\n'))
            #except IOError:
                ## If datastream doesn't have more data to read, we wait a few seconds
                #time.sleep(0.01)

        ## Send SIGTERM signal
        #self.loggerExecution.info("Sending terminate sign to Cloud Connector executable")
        #try:
            #process.terminate()
            ##process.kill()
        #except OSError,e:
            #self.loggerExecution.exception(e)
            #self.testFinishedFlag = True

        ## Wait until finish process
        #while( self.testFinishedFlag is not True and process.poll()==None ):
            #try:
                ##print process.stdout.readline()
                ##newLine = process.stdout.readline()
                #newLine = process.stdout.read()
                ## Save in Execution timeline
                #self.loggerExecution.debug(repr(newLine))
                ## Save in console output
                #self.logger.info(newLine.rstrip('\n'))
            #except IOError:
                #pass
            #time.sleep(0.05)




        #*******************************#
        #**  METHOD B) Using pexpect  **#
        #*******************************#

        # Creates a new shell and execute the connector instance
        self.cloudConnectorProcess = pexpect.spawn(os.path.join(self.sourceProjectPath,"connector"))


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



    def sendSignalSIGINT(self):
        if ( self.cloudConnectorProcess is not None ):
            self.loggerExecution.info("Sending SIGINT (Ctrl+C) signal to the Cloud Connector executable")
            self.cloudConnectorProcess.sendintr()
            time.sleep(5)
