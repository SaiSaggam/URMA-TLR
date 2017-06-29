#***************************************************************************
# Copyright (c) 2012 Digi International Inc.,
# All rights not expressly granted are reserved.
# 
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this file,
# You can obtain one at http://mozilla.org/MPL/2.0/.
# 
# Digi International Inc. 11001 Bren Road East, Minnetonka, MN 55343
#
# ***************************************************************************
# device_request.py
# Send SCI device_request to server. 
# -------------------------------------------------
# Usage: device_request.py <username> <password> <device_id>
# -------------------------------------------------
import httplib
import base64
import sys

from base64 import b64decode

import xml.dom.minidom
from xml.dom.minidom import getDOMImplementation

# impl = getDOMImplementation()

def getText(elem):
    rc = []
    for node in elem.childNodes:
        if node.nodeType == node.TEXT_NODE:
            rc.append(node.data)
    return str(''.join(rc))
    
def Usage():
    print 'Usage: initial_action.py <username> <password> <device_id>\n'
   
def PostMessage(username, password, device_id):
    # create HTTP basic authentication string, this consists of
    # "username:password" base64 encoded
    auth = base64.encodestring("%s:%s"%(username,password))[:-1]
    
    # device request message to send to server
    message = """<sci_request version="1.0">
        <send_message>
            <targets>
                <device id="%s"/>
            </targets>
            <sm_udp retries="3" retryTimeout="5">
            <ping/>
            </sm_udp>
        </send_message>
    </sci_request>
    """%(device_id)
 
    # to what URL to send the request with a given HTTP method
    webservice = httplib.HTTP("login.etherios.com",80)
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
    dom = xml.dom.minidom.parseString(response_body)
    
    if statuscode == 200:
        print '\nResponse:'
        print response_body
        
        ping_response = dom.getElementsByTagName("ping");
        if len(ping_response) > 0:
            print 'Ping success'
        else:
            print 'No ping response'
    else:
        print '\nError: %d %s' %(statuscode, statusmessage)
    
    webservice.close()


def main(argv):
    #process arguments
    count = len(argv);
    if count != 3:
        Usage()
    else:
        PostMessage(argv[0], argv[1], argv[2])


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
    
