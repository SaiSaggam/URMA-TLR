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
# update_firmware.py
# Send an image file to do firmware upgrade using update_firmware SCI operation.
# It updates firmware target = 0 with image file name "image.a"
# -------------------------------------------------
# Usage: query_firmware.py <Username> <Password> <Device ID> [<Device Cloud URL>]
# -------------------------------------------------

import httplib
import base64
import sys

def Usage():
    print 'Usage: update_firmware.py <Username> <Password> <Device ID> <image path> [<Device Cloud URL>]'
    print '    Performs a firmware download operation to target 0 on <Device ID> using a local'
    print '    file \"image.a\".'
    print '    where:' 
    print '        <Username> is the Device Cloud account Username to which your device is'
    print '                   connected.'
    print '        <Password> is the account password'
    print '        <Device ID> is the device to download.'
    print '        <image path> is where the image is' 
    print '        [<Device Cloud URL>] is an optional Device Cloud URL.  The default URL is' 
    print '                   login.etherios.com.' 
    print '' 
    print '    Note:'
    print '        The file \"image.a\" must be created locally.\n'
    print '        <Device ID> format can either be:'
    print '            Long: 00000000-00000000-00049DFF-FFAABBCC.' 
    print '            or short: 00049DFF-FFAABBCC\n'
    print '    Example Usage:' 
    print '        python ./update_firmware.py myaccount mypassword 00049DFF-FFAABBCC'
    print '            Performs a firmware download of \"image.a\" to 00000000-00000000-00049DFF-FFAABBCC '
    print '            (in user account myaccount) through login.etherios.com.\n'
    print '        python ./update_firmware.py myaccount mypassword 00049DFF-FFAABBCC login.etherios.co.uk'
    print '            Performs a firmware download of \"image.a\" to 00000000-00000000-00049DFF-FFAABBCC '
    print '            (in user account myaccount) through login.etherios.co.uk.\n'


def PostMessage(username, password, device_id, image_file, cloud_url):
    # create HTTP basic authentication string, this consists of
    # "username:password" base64 encoded
    auth = base64.encodestring("%s:%s"%(username,password))[:-1]
    
    # open download file and encoded the data
    try: 
        fileHandle = open(image_file, 'rb')
    except IOError:
        print 'Usage: update_firmware.py <Username> <Password> <Device ID> [<Device Cloud URL>]'
        print '    Image not found\n'
        return -1
        

    # build firmware download message sent to Device Cloud
    message = """<sci_request version="1.0">
        <update_firmware firmware_target="0" filename="%s">
            <targets>
                <device id="%s"/>
            </targets>
            <data>%s</data>
        </update_firmware>
    </sci_request>
    """%(image_file, device_id, base64.encodestring(fileHandle.read()))
    
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
        print response_body
        
    webservice.close()


def main(argv):
    #process arguments
    count = len(argv);
    if (count < 4) or (count > 5):
        Usage()
    else:
        if count > 4:
            cloud_url = argv[4]
        else:
            cloud_url = "login.etherios.com"
        PostMessage(argv[0], argv[1], argv[2], argv[3], cloud_url)

if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))


