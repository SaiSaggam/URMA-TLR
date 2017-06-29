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
# Cloud Connector test framework - Device Connection Monitor class
#

################ Import Common libraries
import logging      ### Functions and classes to implement a event logging system
import time         ### Provides various time-related functions

################ Import Classes path
import DeviceCloud


##  Main Class for Device Cloud
class DeviceConnectionMonitor(object):
    "Device Connection Monitor class to work with Cloud Connector Test Framework"

    #*************** Environment
    ## Identifier for the script
    identifier = None


    #***************Logging
    ## Logger class to save in execution logging
    loggerExecution = None
    ## Log filename flag
    logExecutionEnable = None


    #*************** Connectivity Settings
    ## URL for device cloud account
    deviceCloudUrl = None
    ## Username for device cloud account
    deviceCloudUsername = None
    ## Password for device cloud account
    deviceCloudPassword = None


    #*************** Session Handler
    ## Handler to perform all request to the Cloud
    deviceCloudHandler = None

    #*************** Device Settings
    ## Device ID
    deviceID = None
    ## Last time that this device connects with the cloud
    deviceLastConnect = None
    ## Last time that this device disconnects from the cloud
    deviceLastDisconnect = None




    ## Class Builder
    def __init__(self, deviceID, username=None, password=None, hostname=None):
        #*************** Environment
        self.identifier = "DeviceConnectionMonitor"

        #*************** Logging
        self.loggerExecution = logging.getLogger('cc.device_monitor')
        self.logExecutionEnable = True

        #*************** Device Settings
        self.deviceID = deviceID

        if(username is not None):
            self.deviceCloudUsername = username

        if(password is not None):
            self.deviceCloudPassword = password

        if(hostname is not None):
            self.deviceCloudUrl = hostname



    #*************** Connectivity Settings
    def setDeviceCloudURL(self, new_value):
        if ( new_value!=None ):
            # Save new URL
            self.deviceCloudUrl = new_value

    def getDeviceCloudURL(self):
        return self.deviceCloudUrl


    def setDeviceCloudUsername(self, new_value):
        if ( new_value!=None ):
            self.deviceCloudUsername = new_value
        else:
            raise TypeError("Device Cloud Username is empty")

    def getDeviceCloudUsername(self):
        return self.deviceCloudUsername


    def setDeviceCloudPassword(self, new_value):
        if ( new_value!=None ):
            self.deviceCloudPassword = new_value
        else:
            raise TypeError("Device Cloud Password is empty")

    def getDeviceCloudPassword(self):
        return self.deviceCloudPassword






    def __initialize(self):
        # Create a session object to comunicate with the cloud
        self.deviceCloudHandler = DeviceCloud.DeviceCloud()

        # Set connection settings
        self.deviceCloudHandler.setDeviceCloudURL(self.deviceCloudUrl)
        self.deviceCloudHandler.setDeviceCloudUsername(self.deviceCloudUsername)
        self.deviceCloudHandler.setDeviceCloudPassword(self.deviceCloudPassword)

        # Open session
        if ( not self.deviceCloudHandler.connect() ):
            raise RuntimeError("Invalid Credentials on Cloud Account with server: '%s'" % self.deviceCloudUrl)



    def __getDeviceCore(self):

        # Get Device Core from the Cloud
        status,deviceCore,requestResponse = self.deviceCloudHandler.getDeviceCore(self.deviceID)

        if ( not status ):
            print(repr(deviceCore))
            print(repr(deviceCore.content))
            raise RuntimeError('Device %s not found.' % self.deviceID)

        # If device has never disconnected, dpLastDisconnectTime is not defined
        # so we set it to 0.
        if not deviceCore.has_key('dpLastDisconnectTime'):
            deviceCore['dpLastDisconnectTime'] = "0"

        # If device has never connected, dpLastConnectTime is not defined
        # so we set it to 0.
        if not deviceCore.has_key('dpLastConnectTime'):
            deviceCore['dpLastConnectTime'] = "0"

        return deviceCore



    def start(self):
        # Create the handler to comunicate with the Cloud
        self.__initialize()

        # Get device status
        deviceCore = self.__getDeviceCore()

        # Save the initial values
        self.deviceLastConnect = deviceCore['dpLastConnectTime']
        self.deviceLastDisconnect = deviceCore['dpLastDisconnectTime']



    def waitForConnect(self, timeout):
        # Get initial time
        startTime = time.time()
        duration = 0

        # waiting for connection
        while (duration < timeout):
            # Get status for the device
            deviceCore = self.__getDeviceCore()

            # Check if the connection time was modified
            if deviceCore['dpLastConnectTime'] != self.deviceLastConnect:
                # Device connected again
                self.deviceLastConnect = deviceCore['dpLastConnectTime']
                return deviceCore

            # Sleep a second to limit amount of polling
            time.sleep(1.0)
            duration = time.time() - startTime

        # Device never connected, raise exception.
        raise RuntimeError('Device %s did not connect within %d seconds.' % (self.deviceID, timeout))




    def waitForDisconnect(self, timeout):
        # Get initial time
        startTime = time.time()
        duration = 0

        # waiting for connection
        while (duration < timeout):
            # Get status for the device
            deviceCore = self.__getDeviceCore()

            # Check if the connection time was modified
            if deviceCore['dpLastDisconnectTime'] != self.deviceLastDisconnect:
                # Device disconnected again
                self.deviceLastDisconnect = deviceCore['dpLastDisconnectTime']
                return deviceCore

            # Sleep a second to limit amount of polling
            time.sleep(1.0)
            duration = time.time() - startTime

        # Device never connected, raise exception.
        raise RuntimeError('Device %s did not disconnect within %d seconds.' % (self.deviceID, timeout))



    def stayConnected(self, timeout):
        # Get initial time
        startTime = time.time()
        duration = 0
        deviceCore = None

        # waiting for connection
        while (duration < timeout):
            # Get status for the device
            deviceCore = self.__getDeviceCore()

            # Check if the device is connected or if the disconnect time was modified
            if (deviceCore['dpConnectionStatus'] != "1" or deviceCore['dpLastDisconnectTime'] != self.deviceLastDisconnect):
                # Device disconnects during the waiting
                self.deviceLastDisconnect = deviceCore['dpLastDisconnectTime']
                # Device should not disconnect, raise exception
                raise RuntimeError('Device %s did not stay connected during %d seconds.' % (self.deviceID, timeout))

            # Sleep a second to limit amount of polling
            time.sleep(1.0)
            duration = time.time() - startTime

        # Device remained connected all the time
        return deviceCore



    def stayDisconnected(self, timeout):
        # Get initial time
        startTime = time.time()
        duration = 0
        deviceCore = None

        # waiting for connection
        while (duration < timeout):
            # Get status for the device
            deviceCore = self.__getDeviceCore()

            # Check if the device is disconnected or if the connection time was modified
            if (deviceCore['dpConnectionStatus'] != "0" or deviceCore['dpLastConnectTime'] != self.deviceLastConnect):
                # Device connects during the waiting
                self.deviceLastConnect = deviceCore['dpLastConnectTime']
                # Device should not connect, raise exception
                raise RuntimeError('Device %s did not stay disconnected during %d seconds.' % (self.deviceID, timeout))

            # Sleep a second to limit amount of polling
            time.sleep(1.0)
            duration = time.time() - startTime

        # Device remained disconnected all the time
        return deviceCore



    def stop(self):
        if ( self.deviceCloudHandler is not None ):
            self.deviceCloudHandler.close()
