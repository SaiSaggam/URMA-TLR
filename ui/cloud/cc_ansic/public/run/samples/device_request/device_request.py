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
# device_request.py
# Send SCI device_request to Device Cloud. 
# -------------------------------------------------
# Usage: device_request.py <Username> <Password> <Device ID> [<Device Cloud URL>]
# -------------------------------------------------
import httplib
import base64
import sys

def Usage():
    print 'Usage: device_request.py <Username> <Password> <Device ID> <Target> <Request Data> [<Device Cloud URL>]'
    print '    Sends an SCI Data Service-Device Request to <Device ID> with the'
    print '    device_request target_name set to \"myTarget\" and the Device Request data'
    print '    set to \"My device request data\".'
    print '    where:' 
    print '        <Username> is the Device Cloud account Username to which your device is'
    print '                   connected.'
    print '        <Password> is the account password'
    print '        <Device ID> is the device to receive the Device Request.' 
    print '        <Target> is the Device\'s target that will handle the Request Data.' 
    print '        <Request Data> is the data to be passed to the Device\'s target.' 
    print '        [<Device Cloud URL>] is an optional Device Cloud URL.  The default URL is' 
    print '                   login.etherios.com.' 
    print '' 
    print '    Note:'
    print '        <Device ID> format can either be:'
    print '            Long: 00000000-00000000-00049DFF-FFAABBCC.' 
    print '            or short: 00049DFF-FFAABBCC\n'
    print '    Example Usage:' 
    print '        python ./device_request.py myaccount mypassword 00049DFF-FFAABBCC'
    print '            Sends an SCI Data Service-Device Request to 00000000-00000000-00049DFF-FFAABBCC '
    print '            (in user account myaccount) through login.etherios.com.\n'
    print '        python ./device_request.py myaccount mypassword 00049DFF-FFAABBCC login.etherios.co.uk'
    print '            Sends an SCI Data Service-Device Request to 00000000-00000000-00049DFF-FFAABBCC '
    print '            (in user account myaccount) through login.etherios.co.uk.\n'
   
def PostMessage(username, password, device_id, target, state, cloud_url):
    # create HTTP basic authentication string, this consists of
    # "username:password" base64 encoded
    auth = base64.encodestring("%s:%s"%(username,password))[:-1]
    
    # device request message to send to Device Cloud
    message = """<sci_request version="1.0">
        <data_service>
            <targets>
                <device id="%s"/>
            </targets>
            <requests>
            <device_request target_name="%s">%s</device_request>
            </requests>
        </data_service>
    </sci_request>
    """%(device_id, target, state)
    
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
        print '\nResponse:'
        print response_body
    else:
        print '\nError: %d %s' %(statuscode, statusmessage)
    
    webservice.close()


def main(argv):
    #process arguments
    count = len(argv);
    if (count < 5) or (count > 6):
        Usage()
    else:
        if count > 5:
            cloud_url = argv[5]
        else:
            cloud_url = "login.etherios.com"
        PostMessage(argv[0], argv[1], argv[2], argv[3], argv[4], cloud_url)


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
