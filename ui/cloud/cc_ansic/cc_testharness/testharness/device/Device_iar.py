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
import uuid             ### This module provides immutable UUID objects
import random           ### Provide pseudo-random number generators
import subprocess       ### Allows to spawn new processes, connect to their input/output/error pipes, and obtain their return codes
import time             ### Provides various time-related functions


# Import Device
libs = ["Device"]

for elem in libs:
    exec("import %s" % elem)
    exec("reload (%s)" % elem)


class Device_iar(Device.Device):

    #*************** Environment


    def __init__(self):
        ## Call to __init__ from Device(): Initialize function
        super(Device_iar, self).__init__()

        #*************** Environment
        self.identifier = "Device_iar"

        #*************** Logging
        self.logger = logging.getLogger('cc.device.iar')
        self.logFileEnable = False

        #*************** Connection
        ## Connection Handler
        connectionHandler = None


    def run(self):
        '''Run is the function that runs in the new thread and is called by
        start(), inherited from the Thread class
        '''
        ## Start logging
        self.startLoggingSystem()

        self.loggerExecution.info("Start Device for Cloud Connector for IAR")



        self.connectionHandler.setOuputPath(self.testResultPath)
        self.connectionHandler.open()
        self.connectionHandler.start()


        while( self.testStopFlag!=True ):
            try:
                #newLine = self.connectionHandler.readLine()
                #if(newLine is not None):
                    ## Save in console output
                    #self.logger.info(newLine.rstrip('\n'))
                pass
            except IOError:
                # If datastream doesn't have more data to read, we wait a few seconds
                time.sleep(0.1)

        self.connectionHandler.stopExecution()


        self.testFinishedFlag = True
