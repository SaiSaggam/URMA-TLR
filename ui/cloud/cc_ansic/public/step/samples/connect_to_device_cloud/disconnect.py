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
# disconnect.py
# Send disconnect SCI operation to disconnect Device Cloud.
# -------------------------------------------------
# Usage: disconnect.py <Username> <Password> <Device ID> [<Device Cloud URL>]
# -------------------------------------------------

import httplib
import base64
import sys

def Usage():
    print 'Usage: disconnect.py <Username> <Password> <Device ID> [<Device Cloud URL>]'
    print '    Causes a device to momentarily disconnect from Device Cloud.'
    print '    where:' 
    print '        <Username> is the Device Cloud account Username to which your device is'
    print '                   connected.'
    print '        <Password> is the account password'
    print '        <Device ID> is the device to disconnect.' 
    print '        [<Device Cloud URL>] is an optional Device Cloud URL.  The default URL is' 
    print '                   login.etherios.com.' 
    print '' 
    print '    Note:'
    print '        <Device ID> format can either be:'
    print '            Long: 00000000-00000000-00049DFF-FFAABBCC.' 
    print '            or short: 00049DFF-FFAABBCC\n'
    print '    Example Usage:' 
    print '        python ./disconnect.py myaccount mypassword 00049DFF-FFAABBCC'
    print '            Momentarily disconnects 00000000-00000000-00049DFF-FFAABBCC (in user account'
    print '            myaccount) from login.etherios.com.\n'
    print '        python ./disconnect.py myukaccount myukpassword 00049DFF-FFAABBCC login.etherios.co.uk'
    print '            Momentarily disconnects 00000000-00000000-00049DFF-FFAABBCC (in user account'
    print '            myukaccount) from login.etherios.co.uk.\n'


def PostMessage(username, password, device_id, cloud_url):
    # create HTTP basic authentication string, this consists of
    # "username:password" base64 encoded
    auth = base64.encodestring("%s:%s"%(username,password))[:-1]
    
    # build disconnect message sent to Device Cloud
    message = """<sci_request version="1.0">
        <disconnect>
            <targets>
                <device id="%s"/>
            </targets>
        </disconnect>
    </sci_request>
    """%(device_id)
    
    # to what URL to send the request with a given HTTP method
    webservice = httplib.HTTP(cloud_url,80)
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
    if statuscode == 200:
        print '%s successfully disconnected from %s' %(device_id, cloud_url)
    else:
        print '\nERROR: %d %s\n' %(statuscode, statusmessage)
        print response_body
        
    webservice.close()
            
def main(argv):
    #process arguments
    count = len(argv);
    if (count < 3) or (count > 4):
        Usage()
    else:
        if count > 3:
            cloud_url = argv[3]
        else:
            cloud_url = "login.etherios.com"
        PostMessage(argv[0], argv[1], argv[2], cloud_url)

if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))

