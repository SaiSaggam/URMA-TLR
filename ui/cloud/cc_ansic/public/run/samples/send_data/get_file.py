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
# get_file.py
# Get a file from Device Cloud storage
# -------------------------------------------------
# Usage: get_file.py <Username> <Password> <Device ID> [<Device Cloud URL>]
# -------------------------------------------------

import httplib
import base64
import sys
import re

def Usage():
    print 'Usage: get_file.py <Username> <Password> <Device ID> [<Device Cloud URL>]'
    print '    Pulls test/test.txt from a user account and displays the file\'s contents.'
    print '    Where' 
    print '        <Username> is the Device Cloud account Username to which your device is connected.'
    print '        <Password> is the account password'
    print '        <Device ID> is the device\'s ID.' 
    print '        [<Device Cloud URL>] is an optional Device Cloud URL.  The default URL is login.etherios.com.' 
    print '' 
    print '    Note:'
    print '        <Device ID> format can either be:'
    print '            Long: 00000000-00000000-00049DFF-FFAABBCC.' 
    print '            or short: 00049DFF-FFAABBCC\n'
    print '    Example Usage:' 
    print '        python ./get_file.py myaccount mypassword 00049DFF-FFAABBCC'
    print '            Pulls test/test.txt from user account myaccount for device'
    print '            00000000-00000000-00049DFF-FFAABBCC on login.etherios.com.\n'
    print '        python ./get_file.py myukaccount myukpassword 00049DFF-FFAABBCC login.etherios.co.uk'
    print '            Pulls test/test.txt from user account myukaccount for device'
    print '            00000000-00000000-00049DFF-FFAABBCC on login.etherios.co.uk.\n'

def GetMessage(username, password, device_id, cloud_url):
    # create HTTP basic authentication string, this consists of
    # "username:password" base64 encoded
    auth = base64.encodestring("%s:%s"%(username,password))[:-1]

    # device request message to send to Device Cloud
    path = """/ws/FileData/~/%s/test/test.txt"""%(device_id)

    # to what URL to send the request with a given HTTP method
    webservice = httplib.HTTP(cloud_url,80)
    webservice.putrequest("GET", path)

    # add the authorization string into the HTTP header
    webservice.putheader("Authorization", "Basic %s"%auth)
    webservice.putheader("Accept", "text/html; charset=\"UTF-8\"")
    #webservice.putheader("Content-length", "%d" % len(message))
    webservice.endheaders()
    #webservice.send(message)

    # get the response
    statuscode, statusmessage, header = webservice.getreply()
    response_body = webservice.getfile().read()

    # print the output to standard out
    if statuscode == 200:
        print response_body
    else:
        print '\nError: %d %s' %(statuscode, statusmessage)

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

        if len(argv[2]) == len("12345678-12345678"):
            device_id = "00000000-00000000-" + argv[2]
        else:
            device_id = argv[2]

        if re.match( "([0-9A-Fa-f]{8}-){3}[0-9A-Fa-f]{8}", device_id) == None:
            print 'Error: Invalid device id [%s]' %argv[2]
        else:
            GetMessage(argv[0], argv[1], device_id, cloud_url)

if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))

