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
# Cloud Connector test framework - Device Cloud class
#

################ Import Common libraries
import logging                  ### Functions and classes to implement a event logging system
import requests                 ### Makes your integration with web services seamless (HTTP)
import xmltodict                ### Makes working with XML feel like you are working with JSON
import xml.parsers.expat        ### Provide a interface to fast XML parsing using Expat
import base64                   ### Provides data encoding and decoding as specified in RFC 3548 (Base16, Base32, and Base64 algorithms)


##  Main Class for Device Cloud
class DeviceCloud(object):
    "Device Cloud class to work with Cloud Connector Test Framework"

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
    ## VendorID for device cloud account
    deviceCloudVendorID = None
    ## Connection type with Device Cloud (SSL)
    deviceCloudSecure = False


    #*************** Session Handler
    ## Handler to perform all request to the Cloud
    deviceCloudSession = None

    #*************** Web Services Settings
    ## Base URL to unsecure requests to the web services
    webServiceURL = None
    ## Base URL to secure requests to the web services
    webServiceSecureURL = None




    ## Class Builder
    def __init__(self):
        #*************** Environment
        self.identifier = "DeviceCloud"

        #*************** Logging
        self.loggerExecution = logging.getLogger('cc.device_cloud')
        self.logExecutionEnable = True

        #*************** Session Handler
        self.deviceCloudSession = requests.sessions.Session()



    #*************** Connectivity Settings
    def setDeviceCloudURL(self, new_value):
        if ( new_value!=None ):
            # Save new URL
            self.deviceCloudUrl = new_value
            # Compound web services url
            self.webServiceURL = 'http://%s/ws/%%s' % new_value
            self.webServiceSecureURL = 'https://%s/ws/%%s' % new_value

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


    def setDeviceCloudVendorID(self, new_value):
        if ( new_value!=None ):
            self.deviceCloudVendorID = new_value
        else:
            raise TypeError("Device Cloud Vendor ID is empty")

    def getDeviceCloudVendorID(self):
        return self.deviceCloudVendorID


    def enableConnectionSecure(self):
        self.deviceCloudSecure = True

    def disableConnectionSecure(self):
        self.deviceCloudSecure = False

    def isConnectionSecure(self):
        return self.deviceCloudSecure


    def close(self):
        if(self.deviceCloudSession is not None):
            self.deviceCloudSession.close()


    def connect(self):
        if(self.getDeviceCloudURL() is None):
            raise ValueError("Device Cloud URL is empty")
        if(self.deviceCloudUsername is None):
            raise ValueError("Device Cloud Username is empty")
        if(self.deviceCloudPassword is None):
            raise ValueError("Device Cloud Password is empty")


        #********************************#
        #*** Method A) using requests ***#
        #********************************#
        #requestResponse = requests.get(   self.webServiceURL % 'DeviceVendor', # URL to connect
                            #auth=(self.deviceCloudUsername, self.deviceCloudPassword) # Authentication
                        #)

        #********************************#
        #*** Method B) using sessions ***#
        #********************************#
        # Set Authentication settings (Username/Password) for this session
        self.deviceCloudSession.auth = (self.deviceCloudUsername, self.deviceCloudPassword)

        # Use a different URL for a secure connection
        if ( self.isConnectionSecure() ):
            requestURL = self.webServiceSecureURL
        else:
            requestURL = self.webServiceURL

        requestResponse = self.deviceCloudSession.get( requestURL % 'DeviceVendor', timeout=60 )


        #print "Content:\n%s\n\n" % requestResponse.content
        #print "headers:\n%s\n\n" %  requestResponse.headers
        #print "status_code:\n%s\n\n" % requestResponse.status_code
        #print "text:\n%s\n\n" % requestResponse.text
        #print "url:\n%s\n\n" % requestResponse.url

        # Check request status
        if( requestResponse.status_code == 200 ):

            # If VendorID is empty we get it from the request
            if ( self.getDeviceCloudVendorID() is None ):

                # Convert xml into a dict
                document = xmltodict.parse(requestResponse.text)

                # Check request
                if( document.has_key("result") ):
                    resultTotalRows = document["result"]["resultTotalRows"]
                    requestedStartRow = document["result"]["resultTotalRows"]
                    resultSize = document["result"]["resultSize"]
                    requestedSize = document["result"]["requestedSize"]
                    remainingSize = document["result"]["remainingSize"]

                    # Check if we can extract the vendorID from this account
                    if( document["result"].has_key("DeviceVendor") ):

                        if( resultSize == "1" ):
                            deviceVendorElement = document["result"]["DeviceVendor"]
                        else:
                            deviceVendorElement = document["result"]["DeviceVendor"][0]

                        self.deviceCloudVendorID = deviceVendorElement["dvVendorId"]

            # Correct response
            return True
        else:
            # Error response
            return False



    def getRequest(self, resourceName, **kwargs):

        # Use a different URL for a secure connection
        if ( self.isConnectionSecure() ):
            requestURL = self.webServiceSecureURL
        else:
            requestURL = self.webServiceURL

        # Create a GET request against the resourceName
        requestResponse = self.deviceCloudSession.get( requestURL % resourceName, **kwargs)


        # Handle case for async calls.  If async the 'return_response'
        # flag will be True, which means request hasn't been completely
        # sent yet.  The call will be deferred.
        if kwargs.has_key('return_response'):
            return requestResponse


        if (requestResponse.status_code == 200):
            try:
                # Create a new attribute in the response object to insert the dict
                requestResponse.resource = xmltodict.parse(requestResponse.content)
            except xml.parsers.expat.ExpatError,e:
                requestResponse.resource = None

        return requestResponse




    def postRequest(self, resourceName, **kwargs):

        # Use a different URL for a secure connection
        if ( self.isConnectionSecure() ):
            requestURL = self.webServiceSecureURL
        else:
            requestURL = self.webServiceURL

        # Create a POST request against the resourceName
        requestResponse = self.deviceCloudSession.post( requestURL % resourceName, **kwargs)

        # Some requests returns 200 or 201, we try to parse it
        try:
            # Create a new attribute in the response object to insert the dict
            requestResponse.resource = xmltodict.parse(requestResponse.content)
        except xml.parsers.expat.ExpatError,e:
            requestResponse.resource = None

        return requestResponse



    def putRequest(self, resourceName, **kwargs):

        # Use a different URL for a secure connection
        if ( self.isConnectionSecure() ):
            requestURL = self.webServiceSecureURL
        else:
            requestURL = self.webServiceURL

        # Create a PUT request against the resourceName
        requestResponse = self.deviceCloudSession.put( requestURL % resourceName, **kwargs)

        if (requestResponse.status_code == 200):
            # Create a new attribute in the response object to insert the dict
            requestResponse.resource = xmltodict.parse(requestResponse.content)

        return requestResponse




    def deleteRequest(self, resourceName, **kwargs):
        # Use a different URL for a secure connection
        if ( self.isConnectionSecure() ):
            requestURL = self.webServiceSecureURL
        else:
            requestURL = self.webServiceURL

        # Create a POST request against the resourceName
        requestResponse = self.deviceCloudSession.request('delete', requestURL % resourceName, **kwargs)

        if (requestResponse.status_code == 200):
            # Create a new attribute in the response object to insert the dict
            requestResponse.resource = xmltodict.parse(requestResponse.content)

        return requestResponse




    def createDevice(self, deviceID, **options):

        # Create a basic query based on a dict
        # basically this is the same than:
        # <DeviceCore>
        #       <devConnectwareId>00000000-00000000-00049DFF-FFAABBCC</devConnectwareId>
        # </DeviceCore>
        query = {
                    'DeviceCore': {
                                    'devConnectwareId': deviceID,
                                  }
                }


        # Now if the user pass more arguments to the query we add it
        # for example if the parameter 'dpUdpSmEnabled' was passed with the value 'true'
        # this will be the final query
        # <DeviceCore>
        #       <devConnectwareId>00000000-00000000-00049DFF-FFAABBCC</devConnectwareId>
        #       <dpUdpSmEnabled>true</dpUdpSmEnabled>
        # </DeviceCore>
        for eachKey in options:
            query["DeviceCore"][eachKey] = options[eachKey]


        # Convert a dict into a xml
        data = xmltodict.unparse(query)

        # Send the request
        requestResponse = self.postRequest('DeviceCore',data=data, timeout=60)

        # Check query was successfully
        if (requestResponse.status_code == 201):
            return(True,requestResponse)
        else:
            return(False,requestResponse)



    def getDevID(self, deviceID):
        # Get DeviceCore
        result,deviceCore,requestResponse = self.getDeviceCore(deviceID)

        if(result):
            # Check if DeviceCore has entry "id"
            if( deviceCore.has_key("id") ):
                # Check if DeviceCore has entry "devId"
                if( deviceCore["id"].has_key("devId") ):
                    return (True,deviceCore["id"]["devId"])

        return (False,deviceCore)



    def getDeviceType(self, deviceID):
        # Get DeviceCore
        result,deviceCore,requestResponse = self.getDeviceCore(deviceID)

        if(result):
            if( deviceCore.has_key("dpDeviceType") ):
                return (True,deviceCore["dpDeviceType"])

        return (False,deviceCore)



    def getDeviceVendorID(self, deviceID):
        # Get DeviceCore
        result,deviceCore,requestResponse = self.getDeviceCore(deviceID)

        if(result):
            if( deviceCore.has_key("dvVendorId") ):
                return (True,deviceCore["dvVendorId"])

        return (False,deviceCore)



    def getDeviceFirmwareLevel(self, deviceID):
        # Get DeviceCore
        result,deviceCore,requestResponse = self.getDeviceCore(deviceID)

        if(result):
            if( deviceCore.has_key("dpFirmwareLevelDesc") ):
                return (True,deviceCore["dpFirmwareLevelDesc"])

        return (False,deviceCore)



    def getDeviceMAC(self, deviceID):
        # Get DeviceCore
        result,deviceCore,requestResponse = self.getDeviceCore(deviceID)

        if(result):
            if( deviceCore.has_key("devMac") ):
                return (True,deviceCore["devMac"])

        return (False,deviceCore)



    def getDeviceCore(self, deviceID):

        # We get all DeviceCore entries
        #resourceName = "DeviceCore/?condition=devConnectwareId='%s'" % deviceID
        # We use 'devVersion=0' condition to get the most recent version
        resourceName = "DeviceCore/?condition=devConnectwareId='%s' and devVersion=0" % deviceID

        # Send the delete request
        requestResponse = self.getRequest( resourceName, timeout=60 )

        # Check query was successfully
        if (requestResponse.status_code == 200
            and requestResponse.resource["result"]["resultSize"] != "0"):

            # We have only one DeviceCore
            if( requestResponse.resource["result"]["resultSize"] == "1" ):
                deviceCore = requestResponse.resource["result"]["DeviceCore"]
            else:
                # We have several DeviceCore
                numDeviceCores = len(requestResponse.resource["result"]["DeviceCore"])
                # We get the last one
                deviceCore = requestResponse.resource["result"]["DeviceCore"][numDeviceCores-1]

            # Extract Device Core from response
            return(True,deviceCore,requestResponse)
        else:
            return(False,requestResponse,requestResponse)



    def removeDevice(self, deviceID):

        resourceName = "DeviceCore/?condition=devConnectwareId='%s'" % deviceID

        # Send the delete request
        requestResponse = self.deleteRequest( resourceName, timeout=60 )

        # Check query was successfully
        if (requestResponse.status_code == 200):
            return(True,requestResponse)
        else:
            return(False,requestResponse)


    def cleanDeviceCache(self, deviceType):

        resourceName = "DeviceMetaData/?condition=dmDeviceType='%s'" % deviceType

        # Send the delete request
        requestResponse = self.deleteRequest( resourceName, timeout=60 )

        # Check query was successfully
        if (requestResponse.status_code == 200):
            return(True,requestResponse)
        else:
            return(False,requestResponse)



    def updateBatteryOperatedSupport(self, deviceID, value):

        # Get DevId for this device
        query = {
                    'DeviceCore': {
                                    'devConnectwareId': deviceID,
                                    'dpSmBatteryOperated': value,
                                  }
                }


        # Convert a dict into a xml
        data = xmltodict.unparse(query)

        # Send the request
        requestResponse = self.putRequest('DeviceCore',data=data, timeout=60)

        # Check query was successfully
        if (requestResponse.status_code == 200):
            return(True,requestResponse)
        else:
            return(False,requestResponse)



    def addDevicePhoneNumber(self, deviceID, phoneNumber):

        # Get DevId for this device
        result,devId = self.getDevID( deviceID )

        if( not result ):
            return (False,devId)

        # Create request to associate a phone number with the device
        phoneRequest = '''<?xml version="1.0" ?>
            <NetworkInterface>
                <devId>%s</devId>
                <niInterfaceType>0</niInterfaceType>
                <niTerminated>false</niTerminated>
                <niPhone>%s</niPhone>
                <niActivePhone>true</niActivePhone>
            </NetworkInterface>'''

        query = phoneRequest % (devId, phoneNumber)

        # Send the request
        requestResponse = self.postRequest("NetworkInterface", data=query, timeout=60)

        # Check the result
        if requestResponse.status_code == 201:
            return (True,requestResponse)
        else:
            return (False,requestResponse)



    def associateDevicePhoneNumber(self, deviceID, phoneNumber):

        #** Get niId for this device
        resourceName = "DeviceInterface/?condition=devConnectwareId='%s'" % deviceID

        # Send the request to obtain it
        requestResponse = self.getRequest(resourceName, timeout=60)

        # Check query was successfully
        if (requestResponse.status_code != 200):
            return (False,requestResponse)

        # Get xml response in dict format
        dataResponse = requestResponse.resource

        # Check if this device has a result
        if ( dataResponse.has_key("result") ):
            resultSize = dataResponse["result"]["resultSize"]

            # Check if this device has not entries of 'DeviceInterface'
            if( resultSize == "1" ):

                # Check if we can extract the 'DeviceInterface'
                if( dataResponse["result"].has_key("DeviceInterface") != True ):
                    return (False,requestResponse)

                # Check if we can extract the 'id'
                if( dataResponse["result"]["DeviceInterface"].has_key("id") != True ):
                    return (False,requestResponse)

                # Check if we can extract the 'niId'
                if( dataResponse["result"]["DeviceInterface"]["id"].has_key("niId") != True ):
                    return (False,requestResponse)

                # If the niId is 0, this device has not associate any phone number
                if( dataResponse["result"]["DeviceInterface"]["id"]["niId"] == "0" ):
                    # We need to add a new entry for this device
                    return self.addDevicePhoneNumber(deviceID, phoneNumber)


            # else, We have some NetworkInterfaces (1..N) associated to this Device (niId different than 0)
            niId = None
            numDeviceInterfaces = int(resultSize)
            indexItem = 0
            # Find if any of these NetworkInterfaces has the entry 'niPhone'
            while( niId is None and indexItem<numDeviceInterfaces ):
                # Get a DeviceInterface
                eachDeviceInterface = dataResponse["result"]["DeviceInterface"][indexItem]

                # Check if exist the entry "niPhone" in this DeviceInterface
                if( eachDeviceInterface.has_key("niPhone") ):
                    # This is the entry that we need to update

                    # Check if we can extract the 'id'
                    if( eachDeviceInterface.has_key("id") != True ):
                        return (False,requestResponse)

                    # Check if we can extract the 'niId'
                    if( eachDeviceInterface["id"].has_key("niId") != True ):
                        return (False,requestResponse)

                    # Get the niId of this entry
                    niId = eachDeviceInterface["id"]["niId"]

                indexItem+=1


            # If we don't found any item with the entry 'niPhone' we add it
            if( niId is None ):
                # We need to add a new entry for this device
                return self.addDevicePhoneNumber(deviceID, phoneNumber)
            else:
                # We update the entry with the new phone number
                resourceName = "NetworkInterface/%s/0" % niId # (replace with your device's niId, 0 means most recent version)

                niPhoneRequest = '''<?xml version="1.0" ?>
                                    <NetworkInterface>
                                        <niTerminated>false</niTerminated>
                                        <niPhone>%s</niPhone>
                                        <niActivePhone>true</niActivePhone>
                                    </NetworkInterface>'''

                query = niPhoneRequest % (phoneNumber)

                # Send the request
                requestResponse = self.putRequest(resourceName, data=query, timeout=60)

                # Check the result
                if requestResponse.status_code == 200:
                    return (True,requestResponse)
                else:
                    return (False,requestResponse)

        else:
            return (False,requestResponse)



    def isDeviceConnected(self, deviceID):

        # Get DeviceCore
        result,deviceCore,requestResponse = self.getDeviceCore(deviceID)

        # Check the request was successfully
        if( not result ):
            raise RuntimeError("Unknown error getting the DeviceCore request: '%s'" % (requestResponse.content))

        try:
            # Get connection status value
            dpConnectionStatus = deviceCore["dpConnectionStatus"]

            if(dpConnectionStatus == "1"):
                return True
            elif(dpConnectionStatus == "0"):
                return False
            else:
                raise KeyError("Unknown status of key dpConnectionStatus: %s" % dpConnectionStatus)
        except KeyError,e:
            if(self.logExecutionEnable):
                self.loggerExecution.error("Response from Cloud:\n%s" % requestResponse.content )

            raise e



    def isDeviceRegistered(self, deviceID):

        # Get DeviceCore
        result,deviceCore,requestResponse = self.getDeviceCore(deviceID)

        # Check the request was successfully
        if ( not result ):
            # Verify that the request was executed without results
            if (requestResponse.status_code == 200
                and requestResponse.resource["result"]["resultSize"] == "0"):
                return False
            else:
                raise RuntimeError("Unknown error getting the DeviceCore request: '%s'" % (requestResponse.content))
        else:
            # Request was successfully with results (Device exists)
            try:
                # Get connection status value
                devTerminated = deviceCore["devTerminated"]

                if(devTerminated == "true"):
                    return False
                elif(devTerminated == "false"):
                    return True
                else:
                    raise KeyError("Unknown status of key devTerminated: %s" % devTerminated)
            except KeyError,e:
                if(self.logExecutionEnable):
                    self.loggerExecution.error("Response from Cloud:\n%s" % requestResponse.content )

                raise e



    def sendRebootRequest(self, deviceID, replyRequired=False):

        if ( replyRequired ):
            # Request and Response with reply
            rebootRequest = \
            """<sci_request version="1.0">
            <reboot>
                <targets>
                <device id="%s"/>
                </targets>
            </reboot>
            </sci_request>"""

            rebootResponse = """<sci_reply version="1.0"><reboot><device id="%s"><submitted/></device></reboot></sci_reply>"""
        else:
            # Request and Response without reply
            rebootRequest = \
            """<sci_request version="1.0">
            <reboot reply="none">
                <targets>
                <device id="%s"/>
                </targets>
            </reboot>
            </sci_request>"""

            rebootResponse = """<sci_reply version="1.0"><reboot></reboot></sci_reply>"""


        query = rebootRequest % deviceID
        if ( replyRequired ):
            expectedResponse = rebootResponse % deviceID
        else:
            expectedResponse = rebootResponse

        # Send SCI request to reboot the Device
        requestResponse = self.postRequest("sci", data=query, timeout=60)

        # Check query was successfully
        if (requestResponse.status_code == 200):

            # Check if the response is the expected
            if ( requestResponse.content == expectedResponse ):
                return(True,requestResponse)
            else:
                return(False,requestResponse)
        else:
            return(False,requestResponse)




    def sendDisconnectRequest(self, deviceID, replyRequired=False):

        if ( replyRequired ):
            # Request and Response with reply
            disconnectRequest = \
            """<sci_request version="1.0">
                <disconnect>
                    <targets>
                        <device id="%s"/>
                    </targets>
                </disconnect>
            </sci_request>"""

            disconnectResponse = """<sci_reply version="1.0"><disconnect><device id="%s"><disconnected/></device></disconnect></sci_reply>"""
        else:
            # Request and Response without reply
            disconnectRequest = \
            """<sci_request version="1.0">
                <disconnect reply="none">
                    <targets>
                        <device id="%s"/>
                    </targets>
                </disconnect>
            </sci_request>"""

            disconnectResponse = """<sci_reply version="1.0"><disconnect></disconnect></sci_reply>"""


        query = disconnectRequest % deviceID
        if ( replyRequired ):
            expectedResponse = disconnectResponse % deviceID
        else:
            expectedResponse = disconnectResponse

        # Send SCI request to reboot the Device
        requestResponse = self.postRequest("sci", data=query, timeout=60)

        # Check query was successfully
        if (requestResponse.status_code == 200):

            # Check if the response is the expected
            if ( requestResponse.content == expectedResponse ):
                return(True,requestResponse)
            else:
                return(False,requestResponse)
        else:
            return(False,requestResponse)




    def sendUpdateFirmwareRequest(self, deviceID, fileData, target=None, fileName=None, timeout=60):

        # Compound target optional argument
        if ( target is None ):
            firmwareTargetArgument = ""
        else:
            firmwareTargetArgument = """ firmware_target="%s" """ % target

        # Compound filename optional argument
        if ( fileName is None ):
            fileNameArgument = ""
        else:
            fileNameArgument = """ filename="%s" """ % fileName

        firmwareRequest = \
        """<sci_request version="1.0">
            <update_firmware%s%s>
                <targets>
                    <device id="%s"/>
                </targets>
                <data>%s</data>
            </update_firmware>
        </sci_request>"""


        firmwareResponse = """<sci_reply version="1.0"><update_firmware><device id="%s"><submitted/></device></update_firmware></sci_reply>"""

        # Encoding file data with base64 algorithm
        fileDataBase64 = base64.encodestring(fileData)

        # Compound full request
        query = firmwareRequest % (firmwareTargetArgument, fileNameArgument, deviceID, fileDataBase64)

        # Compound full response
        expectedResponse = firmwareResponse % deviceID


        # Send SCI request to update firmware on Device
        requestResponse = self.postRequest("sci", data=query, timeout=timeout)

        # Check query was successfully
        if (requestResponse.status_code == 200):

            # Check if the response is the expected
            if ( requestResponse.content == expectedResponse ):
                return(True,requestResponse)
            else:
                return(False,requestResponse)
        else:
            return(False,requestResponse)



    def uploadFileToDevice(self, deviceID, filePath, fileData, offset = None, truncate = None, timeout = 60):

        uploadFileRequest = \
        """<sci_request version="1.0">
            <file_system>
              <targets>
                <device id="%s"/>
              </targets>
              <commands>
                <put_file path="%s" %s>
                  <data>%s</data>
                </put_file>
              </commands>
            </file_system>
          </sci_request>"""


        extraArguments = ""

        if ( offset is not None ):
            extraArguments += "offset = \"%d\" " % offset

        if ( truncate is not None ):
            extraArguments += "truncate = \"%r\" " % truncate



        uploadFileResponse = """<sci_reply version="1.0"><file_system><device id="%s"><commands><put_file/></commands></device></file_system></sci_reply>"""

        # Encoding file data with base64 algorithm
        fileDataBase64 = base64.encodestring(fileData)

        # Generate the query
        query = uploadFileRequest % (deviceID, filePath, extraArguments, fileDataBase64)

        # Generate the expected response
        expectedResponse = uploadFileResponse % deviceID

        # Send SCI request to reboot the Device
        requestResponse = self.postRequest("sci", data=query, timeout=timeout)

        # Check query was successfully
        if (requestResponse.status_code == 200):

            # Check if the response is the expected
            if ( requestResponse.content == expectedResponse ):
                return(True,requestResponse)
            else:
                return(False,requestResponse)
        else:
            return(False,requestResponse)




    def downloadFileFromDevice(self, deviceID, filePath, fileSize = None, offset = None, timeout = 60):

        downloadFileRequest = \
        """<sci_request version="1.0">
            <file_system>
              <targets>
                <device id="%s"/>
              </targets>
              <commands>
                <get_file path="%s" %s/>
              </commands>
            </file_system>
          </sci_request>"""

        extraArguments = ""

        if ( offset is not None ):
            extraArguments += "offset = \"%d\" " % offset

        if ( fileSize is not None ):
            extraArguments += "length = \"%d\" " % fileSize


        # Generate the query
        query = downloadFileRequest % (deviceID, filePath, extraArguments)

        # Send SCI request to reboot the Device
        requestResponse = self.postRequest("sci", data=query, timeout=timeout)

        # Check query was successfully
        if (requestResponse.status_code == 200):

            try:
                # Get File content
                fileDataBase64 = requestResponse.resource["sci_reply"]["file_system"]["device"]["commands"]["get_file"]["data"]

                if( fileDataBase64 is not None ):
                    # Decoding file data with base64 algorithm
                    fileData = base64.decodestring(fileDataBase64)
                else:
                    fileData = ""

                return(True,fileData,requestResponse)

            except KeyError,e:
                return(False,requestResponse,requestResponse)
        else:
            return(False,requestResponse,requestResponse)



    def downloadFileFromServer(self, filePath, timeout = 60):

        #* Get resource path
        resourcePath = "FileData/~/%s" % filePath

        #* Send the request to obtain it
        requestResponse = self.getRequest(resourcePath, timeout=timeout)

        # Check query was successfully
        if (requestResponse.status_code == 200):
            return(True,requestResponse.content,requestResponse)
        else:
            return(False,requestResponse,requestResponse)


    def downloadFileHistoryFromServer(self, filePath, timeout = 60):

        #* Get resource path
        resourcePath = "FileDataHistory/~/%s" % filePath

        #* Send the request to obtain it
        requestResponse = self.getRequest(resourcePath, timeout=timeout, params={'embed' : 'true', 'orderby' : 'fdLastModifiedDate'} )

        # Check query was successfully
        if (requestResponse.status_code != 200):
            return(False,requestResponse,requestResponse)

        #* Get FileDataHistory for this File
        try:
            numFileDataHistory = requestResponse.resource["result"]["resultSize"]
            listFileDataHistory = requestResponse.resource["result"]["FileDataHistory"]
        except KeyError,e:
            # Return an error
            return(False,requestResponse,requestResponse)

        #* Obtain the FileDataHistory node
        if ( numFileDataHistory == "1" ):
            # Obtain the node for the file
            fileDataHistoryNode = listFileDataHistory
        else:
            #* Obtain the last modified in the FileDataHistory list
            fileDataHistoryNode = listFileDataHistory[-1]


        #* Obtain the filecontent in base64
        fileDataBase64 = fileDataHistoryNode["fdData"]

        if ( fileDataBase64 is None ):
            # We have an empty fileData
            fileData = ""
        else:
            # Decoding file data with base64 algorithm
            fileData = base64.decodestring(fileDataBase64)

        return(True,fileData,requestResponse)



    def removeFileFromDevice(self, deviceID, filePath):

        removeFileRequest = \
        """<sci_request version="1.0">
            <file_system>
              <targets>
                <device id="%s"/>
              </targets>
              <commands>
                <rm path="%s"/>
              </commands>
            </file_system>
          </sci_request>"""


        # Generate the query
        query = removeFileRequest % (deviceID, filePath)

        # Send SCI request to reboot the Device
        requestResponse = self.postRequest("sci", data=query, timeout=60)

        # Check query was successfully
        if (requestResponse.status_code == 200):
            return(True,requestResponse)
        else:
            return(False,requestResponse)




    def removeFileFromServer(self, filePath):

        #* Get resource path
        resourcePath = "FileData/~/%s" % filePath

        #* Send the request to obtain it
        requestResponse = self.deleteRequest(resourcePath, timeout=60)

        # Check query was successfully
        if (requestResponse.status_code == 200):
            return(True,requestResponse)
        else:
            return(False,requestResponse)



    def removeFileHistoryFromServer(self, filePath):

        #* Get resource path
        resourcePath = "FileDataHistory/~/%s" % filePath

        #* Send the request to obtain it
        requestResponse = self.deleteRequest(resourcePath, timeout=60)

        # Check query was successfully
        if (requestResponse.status_code == 200):
            return(True,requestResponse)
        else:
            return(False,requestResponse)



    def listFilesytemFromDevice(self, deviceID, path, hashAlgorithm = None, timeout = 60):

        listFilesystemRequest = \
        """<sci_request version="1.0">
            <file_system>
              <targets>
                <device id="%s"/>
              </targets>
              <commands>
                <ls path="%s" %s/>
              </commands>
            </file_system>
          </sci_request>"""

        extraArguments = ""
        Exception(hashAlgorithm)
        if ( hashAlgorithm is not None ):
            Exception("eee")
            extraArguments += "hash=\"%s\" " % (hashAlgorithm)


        # Generate the query
        query = listFilesystemRequest % (deviceID, path, extraArguments)

        # Send SCI request to reboot the Device
        requestResponse = self.postRequest("sci", data=query, timeout=timeout)

        # Check query was successfully
        if (requestResponse.status_code == 200):
            return(True,requestResponse)
        else:
            return(False,requestResponse)





    def sendDeviceRequest(self, deviceID, target, payload, format="text"):

        # Request and Response without reply
        deviceRequest = \
        """<sci_request version="1.0">
        <data_service>
            <targets>
              <device id="%s"/>
            </targets>
            <requests>
              <device_request format="%s" target_name="%s">%s</device_request>
            </requests>
        </data_service>
        </sci_request>"""


        # Generate the query
        query = deviceRequest % (deviceID, format, target, payload)


        # Send SCI request to reboot the Device
        requestResponse = self.postRequest("sci", data=query, timeout=60)

        # Check query was successfully
        if (requestResponse.status_code == 200):
            return(True,requestResponse)
        else:
            return(False,requestResponse)



    def getDataPoints(self, deviceID, dataStream, size=None, order="ascending", timeline="server"):

        #* Get resource path
        resourcePath = "DataPoint/%s/%s?timeline=%s&order=%s" % (deviceID,dataStream,timeline,order)

        if ( size is not None ):
            resourcePath += "&size=%s" % (size)


        #* Send the request to obtain it
        requestResponse = self.getRequest(resourcePath, timeout=60)

        #* Check query was successfully
        if (requestResponse.status_code != 200):
            return(False,requestResponse,requestResponse)

        #* Create DataPoint list
        dataPoints = []

        #* Get DataPoints for this DataStream
        try:
            numDataPoints = requestResponse.resource["result"]["resultSize"]
            listDataPoints = requestResponse.resource["result"]["DataPoint"]
        except KeyError,e:
            # Return an empty list
            return(True,dataPoints,requestResponse)


        if ( numDataPoints == "1" ):
            # Create a new DataPoint object
            newDataPoint = DataPoint( dataPointStructure=listDataPoints )

            # Add each DataPoint object to the list
            dataPoints.append(newDataPoint)
        else:
            #* Create a DataPoint objects list
            for eachDataPoint in listDataPoints:

                # Create a new DataPoint object
                newDataPoint = DataPoint( dataPointStructure=eachDataPoint )

                # Add each DataPoint object to the list
                dataPoints.append(newDataPoint)

        return(True,dataPoints,requestResponse)



    def getDataStream(self, deviceID, dataStream):

        #* Get resource path
        resourcePath = "DataStream/%s/%s" % (deviceID,dataStream)

        #* Send the request to obtain it
        requestResponse = self.getRequest(resourcePath, timeout=60)

        #* Check query was successfully
        if (requestResponse.status_code != 200):
            return(False,requestResponse,requestResponse)

        #* Create DataStream list
        dataStreams = []

        #* Get DataStreams for this path
        try:
            numDataStream = requestResponse.resource["result"]["resultSize"]
            listDataStream = requestResponse.resource["result"]["DataStream"]
        except KeyError,e:
            # Return an empty list
            return(True,dataStreams,requestResponse)


        if ( numDataStream == "1" ):
            # Create a new DataStream object
            newDataStream = DataStream( dataStreamStructure=listDataStream )

            # Add each DataStream object to the list
            dataStreams.append(newDataStream)
        else:
            #* Create a DataStream objects list
            for eachDataStream in listDataStream:

                # Create a new DataStream object
                newDataStream = DataStream( dataStreamStructure=eachDataStream )

                # Add each DataStream object to the list
                dataStreams.append(newDataStream)

        return(True,dataStreams,requestResponse)



    def removeDataStream(self, deviceID, dataStream):

        #* Get resource path
        resourcePath = "DataStream/%s/%s" % (deviceID,dataStream)


        #* Send the request to obtain it
        requestResponse = self.deleteRequest(resourcePath, timeout=60)

        #* Check query was successfully
        if (requestResponse.status_code == 200):
            return(True,requestResponse)
        else:
            return(False,requestResponse)



class DataPoint(object):
    "Class to manage a DataPoint"

    id = None
    cstId = None
    streamId = None
    timestamp = None
    timestampISO = None
    serverTimestamp = None
    serverTimestampISO = None
    data = None
    description = None
    quality = None
    location = None

    def __init__(self, dataPointStructure = None):

        # Initialize vars
        if ( dataPointStructure is None ):
            self.id = None
            self.cstId = None
            self.streamId = None
            self.timestamp = None
            self.timestampISO = None
            self.serverTimestamp = None
            self.serverTimestampISO = None
            self.data = None
            self.description = None
            self.quality = None
            self.location = None
        else:
            # Set the values from dict structure
            if( dataPointStructure.has_key("id") ):
                self.id = dataPointStructure["id"]
            if( dataPointStructure.has_key("cstId") ):
                self.cstId = dataPointStructure["cstId"]
            if( dataPointStructure.has_key("streamId") ):
                self.streamId = dataPointStructure["streamId"]
            if( dataPointStructure.has_key("timestamp") ):
                self.timestamp = dataPointStructure["timestamp"]
            if( dataPointStructure.has_key("timestampISO") ):
                self.timestampISO = dataPointStructure["timestampISO"]
            if( dataPointStructure.has_key("serverTimestamp") ):
                self.serverTimestamp = dataPointStructure["serverTimestamp"]
            if( dataPointStructure.has_key("serverTimestampISO") ):
                self.serverTimestampISO = dataPointStructure["serverTimestampISO"]
            if( dataPointStructure.has_key("data") ):
                # Check if it is a Binary DataPoint
                if ("#text" in dataPointStructure["data"]):
                    self.data = dataPointStructure["data"]["#text"]
                else:
                    self.data = dataPointStructure["data"]
            if( dataPointStructure.has_key("description") ):
                self.description = dataPointStructure["description"]
            if( dataPointStructure.has_key("quality") ):
                self.quality = dataPointStructure["quality"]
            if( dataPointStructure.has_key("location") ):
                self.location = dataPointStructure["location"]

    def __str__(self):
        message = "{Datapoint:"
        if( self.id is not None ):
            message += "  {id: '%s'}," % self.id
        if( self.cstId is not None ):
            message += "  {cstId: '%s'}," % self.cstId
        if( self.streamId is not None ):
            message += "  {streamId: '%s'}," % self.streamId
        if( self.timestamp is not None ):
            message += "  {timestamp: '%s'}," % self.timestamp
        if( self.timestampISO is not None ):
            message += "  {timestampISO: '%s'}," % self.timestampISO
        if( self.serverTimestamp is not None ):
            message += "  {serverTimestamp: '%s'}," % self.serverTimestamp
        if( self.serverTimestampISO is not None ):
            message += "  {serverTimestampISO: '%s'}," % self.serverTimestampISO
        if( self.data is not None ):
            message += "  {data: '%s'}," % self.data
        if( self.description is not None ):
            message += "  {description: '%s'}," % self.description
        if( self.quality is not None ):
            message += "  {quality: '%s'}," % self.quality
        if( self.location is not None ):
            message += "  {location: '%s'}," % self.location

        message +="}"
        return message

class DataStream(object):
    "Class to manage a DataStream"

    cstId = None
    streamId = None
    dataType = None
    forwardTo = None
    currentValue = None
    description = None
    units = None
    dataTtl = None
    rollupTtl = None


    def __init__(self, dataStreamStructure = None):

        # Initialize vars
        if ( dataStreamStructure is None ):
            self.cstId = None
            self.streamId = None
            self.dataType = None
            self.forwardTo = None
            self.currentValue = None
            self.description = None
            self.units = None
            self.dataTtl = None
            self.rollupTtl = None
        else:
            # Set the values from dict structure
            if( dataStreamStructure.has_key("cstId") ):
                self.cstId = dataStreamStructure["cstId"]
            if( dataStreamStructure.has_key("streamId") ):
                self.streamId = dataStreamStructure["streamId"]
            if( dataStreamStructure.has_key("dataType") ):
                self.dataType = dataStreamStructure["dataType"]
            if( dataStreamStructure.has_key("forwardTo") ):
                self.forwardTo = dataStreamStructure["forwardTo"]
            if( dataStreamStructure.has_key("currentValue") ):
                # Create a new DataPoint object
                newDataPoint = DataPoint( dataPointStructure=dataStreamStructure["currentValue"] )
                # Save a DataPoint Object
                self.currentValue = newDataPoint
            if( dataStreamStructure.has_key("description") ):
                self.description = dataStreamStructure["description"]
            if( dataStreamStructure.has_key("units") ):
                self.units = dataStreamStructure["units"]
            if( dataStreamStructure.has_key("dataTtl") ):
                self.dataTtl = dataStreamStructure["dataTtl"]
            if( dataStreamStructure.has_key("rollupTtl") ):
                self.rollupTtl = dataStreamStructure["rollupTtl"]
