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
# file_system.py
# Send SCI file_system to Device Cloud. 
# -------------------------------------------------
# Usage: file_system.py <Username> <Password> <Device ID> [<Device Cloud URL>]
# -------------------------------------------------
import httplib
import base64
import sys

expected_content = "Connector file system sample\n"

test_file = "test_file.txt"
get_path  = test_file
put_path  = test_file
ls_path   = test_file

def Usage():
    print 'Usage: file_system.py <Username> <Password> <Device ID> [<Device Cloud URL>]'
    print '    Puts file \"test_file.txt\" in the <Device ID> file system and'
    print '    then gets the file back.'
    print '    where:' 
    print '        <Username> is the Device Cloud account Username to which your device is'
    print '                   connected.'
    print '        <Password> is the account password'
    print '        <Device ID> is the device to query.' 
    print '        [<Device Cloud URL>] is an optional Device Cloud URL.  The default URL is' 
    print '                   login.etherios.com.' 
    print '' 
    print '    Note:'
    print '        <Device ID> format can either be:'
    print '            Long: 00000000-00000000-00049DFF-FFAABBCC.'
    print '            or short: 00049DFF-FFAABBCC\n'
    print '    Example Usage:' 
    print '        python ./file_system.py myaccount mypassword 00049DFF-FFAABBCC'
    print '            Transfers file \"test_file.txt\" with 00000000-00000000-00049DFF-FFAABBCC '
    print '            (in user account myaccount) through login.etherios.com.\n'
    print '        python ./file_system.py myaccount mypassword 00049DFF-FFAABBCC login.etherios.co.uk'
    print '            Transfers file \"test_file.txt\" with 00000000-00000000-00049DFF-FFAABBCC '
    print '            (in user account myaccount) through login.etherios.co.uk.\n'
   
def PostMessage(username, password, device_id, cloud_url):
    # create HTTP basic authentication string, this consists of
    # "username:password" base64 encoded
    auth = base64.encodestring("%s:%s"%(username,password))[:-1]

    put_data = base64.encodestring(expected_content)[:-1]
    
    # file system message to send to Device Cloud
    message = """<sci_request version="1.0">
        <file_system>
            <targets>
                <device id="%s"/>
            </targets>
            <commands>
                <put_file path="%s" offset = "0" truncate = "true">
                  <data>%s</data>
                </put_file>
                <get_file path="%s" offset = "0" length = "4294967294"/>
                <ls path="%s"/>
            </commands>
        </file_system>
    </sci_request>
    """ % (device_id, put_path, put_data, get_path, ls_path)

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

        data_start = response_body.find('<data>')
        data_end = response_body.find('</data>')

        if data_start != -1 and data_end != -1:
            data = response_body[data_start + len('<data>'):data_end]
            print base64.b64decode(data)
    else:
        print '\nError: %d %s' %(statuscode, statusmessage)
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
    
