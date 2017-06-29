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
# EnableUDP_Battery.py
# Enable "Battery Operated Mode" option for UDP transport for a device-id in Device Cloud using web Services.
# -------------------------------------------------------------------------------------------------------------------
# Usage: EnableUDP_Battery.py <Username> <Password> <Device Cloud URL> <Device ID> <EnableUDP_Battery(true or false)>
# -------------------------------------------------------------------------------------------------------------------
import httplib
import base64
import sys
import time
import socket
#import easy to use xml parser called minidom:
from xml.dom.minidom import parseString

def Usage():
    print 'Usage: EnableUDP_Battery.py <Username> <Password> <Device Cloud URL> <Device ID> <EnableUDP_Battery(true or false)>'
    print '    Enable "Battery Operated Mode" option for UDP transport for a device-id in Device Cloud using web Services'
    print '    where:'
    print '        <Username> is the Device Cloud account Username to which your device is'
    print '                   registered.'
    print '        <Password> is the account password'
    print '        <Device Cloud URL> is the Device Cloud URL.'
    print '        <Device ID> is the Device ID in format: 00000000-00000000-B06002FF-FFD33499'
    print '        <EnableUDP_Battery> true or false'



def modifyDeviceInCloudServer(username, password, cloud_url, deviceID, EnableUDP):
    # create HTTP basic authentication string, this consists of
    # "username:password" base64 encoded
    auth = base64.encodestring("%s:%s" % (username, password))[:-1]

    message = '''<?xml version="1.0" ?>
                <DeviceCore>
                    <devConnectwareId>%s</devConnectwareId>
                    <dpSmBatteryOperated>%s</dpSmBatteryOperated>
                </DeviceCore>'''

    message = message % (deviceID, EnableUDP)

    # to what URL to send the request with a given HTTP method
    webservice = httplib.HTTPConnection(cloud_url, 80, timeout=10)
    webservice.putrequest("PUT", "/ws/DeviceCore")

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
    print '\nQuery to enable/disable Battery Operrated mode fon device...'
    if response.status == 200:
        print '   Correct Response: %d %s\n' % (response.status, response.reason)
        return True
    else:
        print '   Error: %d %s\n%s\n' %(response.status, response.reason, response.msg)
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
        Enable = argv[4]

        result = modifyDeviceInCloudServer(username, password, cloud_url, deviceID, Enable)


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
