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
# Cloud Connector test framework - Device class
#

################ Import Common libraries
import logging      ### Functions and classes to implement a event logging system
import threading    ### Constructs higher-level threading interfaces
import os           ### Some useful functions on pathnames
import time         ### Provides various time-related functions
import uuid         ### This module provides immutable UUID objects
import random       ### Provide pseudo-random number generators


# Import Common libraries
libs = ['ExceptionLibrary']  # Custom class for raising exception catched in Library
for elem in libs:
    exec("import %s" % elem)


##  Main Class for Device
#############################################################

class Device(threading.Thread):
    "Device class to work with Cloud Connector Test Framework"

    ################ Environment
    # Identifier for the script
    identifier = None
    # Output path
    testResultPath = None

    ################ Logging
    ## Logger class to save in connection logging
    logger = None
    ## Logger class to save in execution logging
    loggerExecution = None
    ## Log filename for System
    logFilename = None
    ## Log filename flag
    logExecutionEnable = None
    ## Log filename flag
    logFileEnable = None
    ## Log identifier name
    logIdentifier = None

    ################ Control and monitoring
    ## Test stop flag
    testStopFlag = None
    ## Test was finished flag
    testFinishedFlag = None

    ################ Settings
    # MAC Address
    deviceMAC = None
    # Device ID
    deviceID = None
    # Device ID Based on
    deviceIDMethod = None
    # Device Type
    deviceType = None
    # Device Firmware Version
    deviceFirmwareVersion = None
    # Device IMEI
    deviceIMEI = None
    # Device ESN
    deviceESN = None
    # Device MEID
    deviceMEID = None
    # Device TX Keepalive
    deviceTxKeepalive = None
    # Device RX Keepalive
    deviceRxKeepalive = None
    # Device generation method for MAC,IMEI,ESN,MEID
    deviceGenerationMethod = None
    # Bunch of fixed MACs
    listFixedMAC = None




    ## Class Builder
    def __init__(self):
        threading.Thread.__init__(self)

        #*************** Environment
        self.identifier = "Device"
        self.setName(self.identifier)

        #*************** Logging
        self.setLogIdentifier("cc")
        self.logFilename = "console_output.log"
        self.logFileEnable = True
        self.logExecutionEnable = True

        #*************** Settings
        self.listFixedMAC = []
        self.listFixedMAC.append("00409D:5514B1")
        self.listFixedMAC.append("0004F3:00E200")
        self.listFixedMAC.append("0004F3:0350B7")
        self.listFixedMAC.append("0004F3:0176D0")
        self.listFixedMAC.append("0004F3:019D65")
        self.listFixedMAC.append("0004F3:00D509")
        self.listFixedMAC.append("0004F3:00E193")
        self.listFixedMAC.append("0004F3:00CFB3")
        self.listFixedMAC.append("0004F3:01C1ED")
        self.listFixedMAC.append("0004F3:01DDC5")
        self.listFixedMAC.append("0004F3:01DC7F")
        self.listFixedMAC.append("0004F3:01DD78")
        self.listFixedMAC.append("00409D:25502B")
        self.listFixedMAC.append("0004F3:022AB2")
        self.listFixedMAC.append("00409D:5514B2")
        self.listFixedMAC.append("0004F3:01BC24")
        # Reserved for OLD dvts
        #self.listFixedMAC.append("00409D:37A836")
        #self.listFixedMAC.append("00409D:290AF3")
        #self.listFixedMAC.append("00409D:2902C6")
        #self.listFixedMAC.append("00409D:3C0B79")
        #self.listFixedMAC.append("00409D:29950B")
        #self.listFixedMAC.append("00409D:3D9B0B")
        #self.listFixedMAC.append("00409D:37F7E3")
        #self.listFixedMAC.append("0004F3:02290F")
        #self.listFixedMAC.append("00409D:37848E")
        #self.listFixedMAC.append("0004F3:03BB4C")
        #self.listFixedMAC.append("0004F3:029B03")
        #self.listFixedMAC.append("0004F3:02A444")




    ## Set identifier for logging system
    #   @param self reference for this object
    #   @param name string containing the log identifier
    def setLogIdentifier(self, name):
        if (name is None):
            raise ExceptionLibrary.Error('Device: Null log identifier')
        else:
            self.logIdentifier = name

    ## Set Identifier for this object
    #   @param self reference for this object
    #   @param name string containing the object identifier
    def setIdentifier(self, name):
        if (name is None):
            raise ExceptionLibrary.Error('Device: Null identifier')
        else:
            self.identifier = name
            self.setName(name)
            self.loggerExecution = logging.getLogger('%s.%s' % (self.logIdentifier,name) )



    def resetFlags(self):
        self.testStopFlag = False
        self.testFinishedFlag = False



    def generateDeviceMAC(self):

        # Remove Saved Device ID
        self.deviceID = None

        # Generate a new Device MAC
        if ( self.deviceGenerationMethod == "fixed" ):
            try:
                # Select a random item from the list
                index = random.randint(0, len(self.listFixedMAC)-1)
                # Get the element for the selected index
                fixedMAC = self.listFixedMAC[index]
                # Remove the element from the list
                self.listFixedMAC.pop(index)
                # Append the obtained element to the end
                self.listFixedMAC.insert(0,fixedMAC)

                # Save the selected MAC address
                self.deviceMAC = fixedMAC
            except IndexError, e:
                return (False, "We don't have more MACs available to provide: %s" % e)

        else:
            base_id = str.upper(str(uuid.uuid4())) ## ej: '9F3AD1F5-F0A5-4F8F-8F1A-BE8D14A8F107'

            MAC_ADDR_PROTOTYPE = '%s:%s' # AABBCC:DDEEFF
            self.deviceMAC = MAC_ADDR_PROTOTYPE % (base_id[:6], base_id[-6:])

        return (True,"Device MAC generated successfully")


    def generateDeviceIMEI(self):
        block_A = random.randint(100000,999999) ## ej: 123456
        block_B = random.randint(10,99) ## ej: 55
        block_C = random.randint(100000,999999) ## ej: 987654
        block_D = random.randint(0,9) ## ej: 3

        IMEI_NUMBER_PROTOTYPE = "%s-%s-%s-%s" # AAAAAA-BB-CCCCCC-D
        self.deviceIMEI = IMEI_NUMBER_PROTOTYPE % (block_A,block_B,block_C,block_D)


    def generateDeviceESN(self):
        base_id = str.upper(str(uuid.uuid4())) ## ej: '9F3AD1F5-F0A5-4F8F-8F1A-BE8D14A8F107'

        ESN_NUMBER_PROTOTYPE = "%s" # MMSSSSSS
        self.deviceESN = ESN_NUMBER_PROTOTYPE % (base_id[:8])


    def generateDeviceMEID(self):
        base_id = str.upper(str(uuid.uuid4())) ## ej: '9F3AD1F5-F0A5-4F8F-8F1A-BE8D14A8F107'
        block_R = base_id[:2]
        block_X = base_id[2:8]
        block_Z = base_id[-6:]

        MEID_NUMBER_PROTOTYPE = "%s%s%s" # RRXXXXXXZZZZZZ
        self.deviceMEID = MEID_NUMBER_PROTOTYPE % (block_R,block_X,block_Z)


    def generateDeviceType(self):
        self.deviceType = 'CC DVT %s' % str(uuid.uuid4().hex)[:25]

    def setDeviceID(self,newDeviceID):
        self.deviceID = newDeviceID

    def getDeviceID(self):
        if(self.deviceID != None):
            return self.deviceID
        else:
            if(self.getDeviceIDMethod() == "mac"):
                if(self.getDeviceMAC() != None):
                    # Generate Device ID
                    self.generateDeviceID()
                    return self.deviceID
            elif(self.getDeviceIDMethod() == "imei"):
                if(self.getDeviceIMEI() != None):
                    # Generate Device ID
                    self.generateDeviceID()
                    return self.deviceID
            elif(self.getDeviceIDMethod() == "esn"):
                if(self.getDeviceESN() != None):
                    # Generate Device ID
                    self.generateDeviceID()
                    return self.deviceID
            elif(self.getDeviceIDMethod() == "meid"):
                if(self.getDeviceMEID() != None):
                    # Generate Device ID
                    self.generateDeviceID()
                    return self.deviceID
            elif(self.getDeviceIDMethod() == "provision"):
                self.getProvisionedDeviceID()
                return self.deviceID
            else:
                return None

    def generateDeviceID(self):
        if(self.getDeviceIDMethod() == "mac"):
            # Device ID based on MAC:
            #--- Example MAC: 112233:445566
            #--- Device ID mapping: 00000000-00000000-112233FF-FF445566
            DEVICE_ID_BASED_ON_MAC_PROTOTYPE = '00000000-00000000-%sFF-FF%s'
            deviceMAC = self.getDeviceMAC() # AABBCC:DDEEFF
            self.deviceID = DEVICE_ID_BASED_ON_MAC_PROTOTYPE % (deviceMAC[:6], deviceMAC[-6:])
        elif(self.getDeviceIDMethod() == "imei"):
            # Device ID based on IMEI
            #--- Example IMEI: AAAAAA-BB-CCCCCC-D"
            #--- Device ID mapping: 00010000-00000000-0AAAAAAB-BCCCCCCD
            DEVICE_ID_BASED_ON_IMEI_PROTOTYPE = '00010000-00000000-0%s-%s'
            deviceIMEI = self.getDeviceIMEI() # AAAAAA-BB-CCCCCC-D
            self.deviceID = DEVICE_ID_BASED_ON_IMEI_PROTOTYPE % (str(deviceIMEI)[:6]+str(deviceIMEI)[7:8], str(deviceIMEI)[8:9]+str(deviceIMEI)[10:16]+str(deviceIMEI)[-1:])
        elif(self.getDeviceIDMethod() == "esn"):
            # Device ID based on ESN
            #--- Example ESN-Hex: MMSSSSSS
            #--- Device ID mapping: 00020000-00000000-00000000-MMSSSSSS
            DEVICE_ID_BASED_ON_ESN_PROTOTYPE = '00020000-00000000-00000000-%s'
            deviceESN = self.getDeviceESN() # MMSSSSSS
            self.deviceID = DEVICE_ID_BASED_ON_ESN_PROTOTYPE % (deviceESN)
        elif(self.getDeviceIDMethod() == "meid"):
            # Device ID based on MEID
            #--- Example MEID-Hex: RRXXXXXXZZZZZZ
            #--- Device ID mapping: 00040000-00000000-00RRXXXX-XXZZZZZZ
            DEVICE_ID_BASED_ON_MEID_PROTOTYPE = '00040000-00000000-00%s-%s'
            deviceMEID = self.getDeviceMEID() # RRXXXXXXZZZZZZ
            self.deviceID = DEVICE_ID_BASED_ON_MEID_PROTOTYPE % (str(deviceMEID)[:6],str(deviceMEID)[-8:])

    def getProvisionedDeviceID(self):
        self.deviceID = None

    def getDeviceIDMethod(self):
        return self.deviceIDMethod

    def setDeviceIDMethod(self,newMethod):
        self.deviceIDMethod = newMethod

    def getDeviceMAC(self):
        return self.deviceMAC

    def setDeviceMAC(self,newMAC):
        self.deviceMAC = newMAC

    def getDeviceType(self):
        return self.deviceType

    def setDeviceType(self,newType):
        self.deviceType = newType

    def getDeviceFirmwareVersion(self):
        return self.deviceFirmwareVersion

    def setDeviceFirmwareVersion(self,newFirmware):
        self.deviceFirmwareVersion = newFirmware



    def getDeviceIMEI(self):
        return self.deviceIMEI

    def setDeviceIMEI(self,newIMEI):
        self.deviceIMEI = newIMEI

    def getDeviceESN(self):
        return self.deviceESN

    def setDeviceESN(self,newESN):
        self.deviceESN = newESN

    def getDeviceMEID(self):
        return self.deviceMEID

    def setDeviceMEID(self,newMEID):
        self.deviceMEID = newMEID        

    def getDeviceTxKeepalive(self):
        return self.deviceTxKeepalive

    def setDeviceTxKeepalive(self,newKeepalive):
        self.deviceTxKeepalive = newKeepalive

    def getDeviceRxKeepalive(self):
        return self.deviceRxKeepalive

    def setDeviceRxKeepalive(self,newKeepalive):
        self.deviceRxKeepalive = newKeepalive


    def isFinished(self):
        return self.testFinishedFlag

    def stop(self):
        self.testStopFlag = True

    def run(self):
        ## Redefine for each Test
        raise Exception("NotImplementedException")


    def startLoggingSystem(self):

        ################ Handler Logging
        if(self.logFileEnable):
            # create logger
            self.logger = logging.getLogger(self.logFilename)
            self.logger.setLevel(logging.DEBUG)

            # create file handler which logs even debug messages
            fh = logging.FileHandler(os.path.join(self.testResultPath,self.logFilename))
            fh.setLevel(1)

            # create formatter
            formatter = logging.Formatter('%(message)s')

            # add formatter to file handler
            fh.setFormatter(formatter)

            # Clean previous handlers for the same logger name
            self.logger.handlers = []

            # add handlers to logger
            self.logger.addHandler(fh)



        #*************** Execution Logging
        if (self.logExecutionEnable):
            self.loggerExecution = logging.getLogger('%s.%s' % (self.logIdentifier,self.identifier) )

