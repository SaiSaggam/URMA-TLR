#!/usr/bin/python
#
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
# ProvisionSMS.py
# Configures the Device Phone Number where to send SMSs on a registered device-id in Device Cloud.
# ------------------------------------------------------------------------------------------
# Usage: ProvisionSMS.py <Username> <Password> <Device Cloud URL> <Device ID> <Device Phone Number>
# ------------------------------------------------------------------------------------------
import httplib
import base64
import sys
import time
import socket
import xml.dom.minidom

def Usage():
    print 'Usage: ProvisionSMS.py <Username> <Password> <Device Cloud URL> <Device ID> <Phone Number>'
    print '    Add a new device in an account'
    print '    where:'
    print '        <Username> is the Device Cloud account Username to which your device is'
    print '                   connected.'
    print '        <Password> is the account password'
    print '        <Device Cloud URL> is the Device Cloud URL.'
    print '        <Device ID> is the Device ID in format: 00000000-00000000-B06002FF-FFD33499'
    print '        <Device Phone number> Phone number'

def getText(elem):
    rc = []
    for node in elem.childNodes:
        if node.nodeType == node.TEXT_NODE:
            rc.append(node.data)
    return str(''.join(rc))

# Add a new entry in /ws/NetworkInterface for the device-id with the phone number
# Better not to call it directly, it'll be called from addDevicePhoneNumber() if
# no current entry is found.
def addDevicePhoneNumber(username, password, cloud_url, deviceID, phoneNumber):

    message = ''
    req = ''

    # create HTTP basic authentication string, this consists of
    # "username:password" base64 encoded
    auth = base64.encodestring("%s:%s" % (username, password))[:-1]

    message = '<?xml version="1.0" ?>'

    req = "/ws/DeviceCore/?condition=devConnectwareId='%s'"
    req = req % (deviceID)

    # to what URL to send the request with a given HTTP method
    webservice = httplib.HTTPConnection(cloud_url, 80, timeout=10)
    webservice.putrequest("GET", req)

    # add the authorization string into the HTTP header
    webservice.putheader("Authorization", "Basic %s"%auth)
    webservice.putheader("Content-type", "text/xml; charset=\"UTF-8\"")
    webservice.putheader("Content-length", "%d" % len(message))
    webservice.endheaders()
    webservice.send(message)


    # get the HTTPResponse
    response = webservice.getresponse() #  HTTPConnection.getresponse()

    responseBody = response.read()

    webservice.close()

    ## print the output to standard out
    print '\nQuery to get devId from the device...'
    if response.status == 200:
        print("   Correct Response: %d %s\n" % (response.status, response.reason))
    else:
        print("   Error: %d %s\n%s\n" %(response.status, response.reason, response.msg))
        return False

    #Parse <devId> from above
    devId = '0'

    dom = xml.dom.minidom.parseString(responseBody)

    nodelist_devId = dom.getElementsByTagName("devId")
    element_devId = nodelist_devId.item(0)
    if element_devId != None:
        print("element_devId: %s\n" % element_devId.toxml())
      
        devId = getText(element_devId)
        print("devId: %s\n" % devId)

    if devId == '0':
        print("Didn't devId\n")
        return False

    webservice = httplib.HTTPConnection(cloud_url, 80, timeout=10)

    message = '''<?xml version="1.0" ?>
                <NetworkInterface>
                    <devId>%s</devId>
                    <niInterfaceType>0</niInterfaceType>
                    <niTerminated>false</niTerminated>
                    <niPhone>%s</niPhone>
                    <niActivePhone>true</niActivePhone>
                </NetworkInterface>'''

    message = message % (devId, phoneNumber)

    req = "/ws/NetworkInterface"

    webservice.putrequest("POST", req)

    # add the authorization string into the HTTP header
    webservice.putheader("Authorization", "Basic %s"%auth)
    webservice.putheader("Content-type", "text/xml; charset=\"UTF-8\"")
    webservice.putheader("Content-length", "%d" % len(message))
    webservice.endheaders()
    webservice.send(message)


    # get the HTTPResponse
    response = webservice.getresponse() #  HTTPConnection.getresponse()

    responseBody = response.read()

    webservice.close()

    ## print the output to standard out
    print("\nQuery to add niPhone on device into the account...")
    if response.status == 201:
        print("   Correct Response: %d %s\n" % (response.status, response.reason))
        return True
    else:
        print("   Error: %d %s\n%s\n" %(response.status, response.reason, response.msg))
        return False

# Updates entry in /ws/NetworkInterface for the device-id with the phone number
# Calls addDevicePhoneNumber() if no current entry is found.
def updateDevicePhoneNumber(username, password, cloud_url, deviceID, phoneNumber):

    message = ''
    req = ''

    # create HTTP basic authentication string, this consists of
    # "username:password" base64 encoded
    auth = base64.encodestring("%s:%s" % (username, password))[:-1]

    message = '<?xml version="1.0" ?>'

    req = "/ws/DeviceInterface/?condition=devConnectwareId='%s'"
    req = req % (deviceID)

    # to what URL to send the request with a given HTTP method
    webservice = httplib.HTTPConnection(cloud_url, 80, timeout=10)
    webservice.putrequest("GET", req)

    # add the authorization string into the HTTP header
    webservice.putheader("Authorization", "Basic %s"%auth)
    webservice.putheader("Content-type", "text/xml; charset=\"UTF-8\"")
    webservice.putheader("Content-length", "%d" % len(message))
    webservice.endheaders()
    webservice.send(message)


    # get the HTTPResponse
    response = webservice.getresponse() #  HTTPConnection.getresponse()

    responseBody = response.read()

    webservice.close()

    ## print the output to standard out
    print '\nQuery to get niId from the device...'
    if response.status == 200:
        print("   Correct Response: %d %s\n" % (response.status, response.reason))
    else:
        print("   Error: %d %s\n%s\n" %(response.status, response.reason, response.msg))
        return False

    #Parse <niId> from above
    niId = '0'

    dom = xml.dom.minidom.parseString(responseBody)

    nodelist_niId = dom.getElementsByTagName("niId")
    element_niId = nodelist_niId.item(0)
    if element_niId != None:
        niId = getText(element_niId)

    if niId == '0':
        #If niId do not exist, go through devId
        return addDevicePhoneNumber(username, password, cloud_url, deviceID, phoneNumber)         

    webservice = httplib.HTTPConnection(cloud_url, 80, timeout=10)

    message = '''<?xml version="1.0" ?>
                <NetworkInterface>
                    <niTerminated>false</niTerminated>
                    <niPhone>%s</niPhone>
                    <niActivePhone>true</niActivePhone>
                </NetworkInterface>'''

    message = message % (phoneNumber)

    req = "/ws/NetworkInterface/%s/0"
    req = req % (niId)

    webservice.putrequest("PUT", req)

    # add the authorization string into the HTTP header
    webservice.putheader("Authorization", "Basic %s"%auth)
    webservice.putheader("Content-type", "text/xml; charset=\"UTF-8\"")
    webservice.putheader("Content-length", "%d" % len(message))
    webservice.endheaders()
    webservice.send(message)


    # get the HTTPResponse
    response = webservice.getresponse() #  HTTPConnection.getresponse()

    responseBody = response.read()

    webservice.close()

    ## print the output to standard out
    print("\nQuery to update niPhone on device into the account...")
    if response.status == 200:
        print("   Correct Response: %d %s\n" % (response.status, response.reason))
        return True
    else:
        print("   Error: %d %s\n%s\n" %(response.status, response.reason, response.msg))
        return False

def main(argv):
    #process arguments
    count = len(argv);
    if (count < 5):
        Usage()
    else:
        # Get parameters from command line
        username = argv[0]
        password = argv[1]
        cloud_url = argv[2]
        deviceID = argv[3]
        phoneNumber = argv[4]


        # update/add Battery Operated status in Device Cloud
        result = updateDevicePhoneNumber(username, password, cloud_url, deviceID, phoneNumber)


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
