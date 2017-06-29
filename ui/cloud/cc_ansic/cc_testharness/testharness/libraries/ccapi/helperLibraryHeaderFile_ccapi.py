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
# Cloud Connector test framework - Cloud Config Header class for CCAPI
#

#*************** Import Common libraries
import logging      ### Functions and classes to implement a event logging system


#*************** Import Classes path
libs = ["BaseFile_ccapi"]

for elem in libs:
    exec("import %s" % elem)
    exec("reload (%s)" % elem)


##  Main Class for Cloud Config Header File
class helperLibraryHeaderFile_ccapi(BaseFile_ccapi.BaseFile_ccapi):
    "app/helper/helper_library.h class to work with Cloud Connector Test Framework"


    ## Class Builder
    def __init__(self, filePath):
        ## Call to __init__ from Host(): Initialize function
        super(helperLibraryHeaderFile_ccapi, self).__init__(filePath)

        #*************** Environment
        self.identifier = "helper_library"

        #*************** Logging
        self.logger = logging.getLogger('cc.helper_library_h')


    ## Set the MAC address for this device
    def setupDeviceMAC(self,deviceMAC):

        newMac = '{' + '0x' + deviceMAC[0] + deviceMAC[1] + ', '
        newMac += '0x' + deviceMAC[2] + deviceMAC[3] + ', '
        newMac += '0x' + deviceMAC[4] + deviceMAC[5] + ', '
        newMac += '0x' + deviceMAC[7] + deviceMAC[8] + ', '
        newMac += '0x' + deviceMAC[9] + deviceMAC[10] + ', '
        newMac += '0x' + deviceMAC[11] + deviceMAC[12] + '}'

        stringToFind = "#define DEVICE_MAC_ADDRESS"
        newSentence = "#define DEVICE_MAC_ADDRESS        %s" % (newMac)
        self.replaceSentence(stringToFind,newSentence)

    ## Set the Device ID
    def setupDeviceID(self, deviceID):
        newDeviceID = '{'
        for blockIndex in range(0,35,9): # [0, 9, 18, 27]
            for digitIndex in range(0,8,2):
                newDeviceID += '0x' + deviceID[blockIndex+digitIndex] + deviceID[blockIndex+digitIndex+1]
                if ( (blockIndex+digitIndex+1)< len(deviceID)-1 ):
                    newDeviceID += ', '
        newDeviceID += '}'

        stringToFind = "#define DEVICE_ID"
        newSentence = "#define DEVICE_ID        %s" % (newDeviceID)
        self.replaceSentence(stringToFind,newSentence)


    def setupVendorID(self,vendorid):
        hex_string = '%08X' % int(vendorid)
        stringToFind = "#define DEVICE_CLOUD_VENDOR_ID"
        newSentence = "#define DEVICE_CLOUD_VENDOR_ID        0x%s" % (hex_string)
        self.replaceSentence(stringToFind,newSentence)


    def setupCloudServer(self,serverURL):
        stringToFind = "#define DEVICE_CLOUD_URL_STRING"
        newSentence = "#define DEVICE_CLOUD_URL_STRING        \"%s\"" % (serverURL)
        self.replaceSentence(stringToFind,newSentence)


    def setupDeviceType(self,deviceType):
        stringToFind = "#define DEVICE_TYPE_STRING"
        newSentence = "#define DEVICE_TYPE_STRING        \"%s\"" % (deviceType)
        self.replaceSentence(stringToFind,newSentence)


    def setupDeviceIP(self,deviceIP):

        if ( deviceIP is None ):
            # Use a default IP address
            deviceIP = "192.168.126.254"

        splittedIP = deviceIP.split(".") # [ 192, 168, 2, 168 ]
        newDeviceIP = '{'
        for eachBlock in splittedIP:
            newDeviceIP += '0x%X' % int(eachBlock)
            if ( eachBlock != splittedIP[-1] ):
                newDeviceIP += ', '

        newDeviceIP += '}'

        stringToFind = "#define DEVICE_IP"
        newSentence = "#define DEVICE_IP        %s" % (newDeviceIP)
        self.replaceSentence(stringToFind,newSentence)
