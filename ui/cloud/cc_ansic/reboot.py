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
# reboot.py
# Send a reboot command to the device.
#
# -------------------------------------------------
# Usage: reboot.py <Device Cloud URL> <username> <password> <device_id>
# -------------------------------------------------
import httplib
import base64
import sys
import time

import xml.dom.minidom
from xml.dom.minidom import getDOMImplementation
impl = getDOMImplementation()

def Usage():
    print 'Usage: reboot.py <Device Cloud URL> <username> <password> <device_id>\n'
    
def getText(elem):
    rc = []
    for node in elem.childNodes:
        if node.nodeType == node.TEXT_NODE:
            rc.append(node.data)
    return str(''.join(rc))

def GetConnectionState(device_cloud, username, password, device_id):
    # create HTTP basic authentication string, this consists of
    # "username:password" base64 encoded
    auth = base64.encodestring("%s:%s"%(username,password))[:-1]
    
    # to what URL to send the request with a given HTTP method
    webservice = httplib.HTTP(device_cloud,80)

    # get the record for the device ID that has the connection state
    webservice.putrequest("GET", "/ws/DeviceCore/?condition=devConnectwareId='%s'"%(device_id))
    
    # add the authorization string into the HTTP header
    webservice.putheader("Authorization", "Basic %s"%auth)
    webservice.endheaders()
    
    # get the response
    statuscode, statusmessage, header = webservice.getreply()
    response_body = webservice.getfile().read()
    
    # check if the status code is success
    if statuscode == 200:
        # Parse response for the "DeviceCore" tag 
        dom = xml.dom.minidom.parseString(response_body)
        device_response = dom.getElementsByTagName("DeviceCore")

        if len(device_response) == 0:
            print("Unexpected response from device: %s" % device_response)
            connection_state = "error"
        else:
            # Parse response for the "dpConnectionStatus" tag
            connection_state =  getText(device_response[0].getElementsByTagName("dpConnectionStatus")[0])
    else:
        print '\nError: %d %s' %(statuscode, statusmessage)
        connection_state = "error"
    
    webservice.close()

    return connection_state
   
def SendReboot(device_cloud, username, password, device_id):
    # create HTTP basic authentication string, this consists of
    # "username:password" base64 encoded
    auth = base64.encodestring("%s:%s"%(username,password))[:-1]
    
    # create reboot message to send to Device Cloud
    message = """<sci_request version="1.0"> 
        <reboot> 
            <targets>
                <device id="%s"/>
            </targets> 
        </reboot>
    </sci_request>
    """%(device_id)
    
    # to what URL to send the request with a given HTTP method
    webservice = httplib.HTTP(device_cloud,80)
    webservice.putrequest("POST", "/ws/sci")
    
    # add the authorization string into the HTTP header
    webservice.putheader("Authorization", "Basic %s"%auth)
    webservice.putheader("Content-type", "text/xml; charset=\"UTF-8\"")
    webservice.putheader("Content-length", "%d" % len(message))
    webservice.endheaders()
    webservice.send(message)
    
    # get the response
    statuscode, statusmessage, header = webservice.getreply()
    response_body = webservice.getfile().read()
    
    # print the output to standard out
    if statuscode != 200: 
        print '\nError: %d %s' %(statuscode, statusmessage)
    
    webservice.close()


def main(argv):
    #process arguments
    count = len(argv);
    if count != 4:
        Usage()
    else:
        # force reboots
        # print "Sending reboot"
        SendReboot(argv[0], argv[1], argv[2], argv[3])
        connection_status = '0'

        # Wait to be reconnected
        #while connection_status == '0':
            #time.sleep(.25)
            #connection_status = GetConnectionState(argv[0], argv[1], argv[2], argv[3])
            #print "dpConnectionStatus = \"%s\"" % connection_status

if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
