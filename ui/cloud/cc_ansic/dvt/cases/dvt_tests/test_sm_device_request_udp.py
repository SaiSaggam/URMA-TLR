#import time
import ic_testcase
#import datetime
#import re
#import os
from base64 import b64decode

import xml.dom.minidom
from xml.dom.minidom import getDOMImplementation
impl = getDOMImplementation()

from ..utils import getText

class DeviceRequestUdpTestCase(ic_testcase.UDPTestCase):
       
    def test_sm_user_msg_udp(self):
        """ User msg with reply for expected target """
        self.send_sm_udp_request("user_msg ", '<user_msg path="myTarget">My device request data</user_msg>', True, "My device response data")
        
    def test_sm_user_msg_noresp_udp(self):
        """ User msg with no reply """
        self.send_sm_udp_request("user_msg no reply", '<user_msg path="myTarget">My device request data</user_msg>', False, "")

    def test_sm_user_msg_nulltarget_udp(self):
        """ User msg with reply for empty target """
        self.send_sm_udp_request("user_msg empty target", '<user_msg>My device request data</user_msg>', True, "Empty target in SM device request")

    def test_sm_user_msg_badtarget_udp(self):
        """ User msg with reply for unexpected target """
        self.send_sm_udp_request("user_msg bad target", '<user_msg path="badTarget">My device request data</user_msg>', True, "Unexpected target in SM device request")


    def send_sm_udp_request (self, test_name, command, reply_needed, response):

        my_sm_udp_request_with_reply = \
            """<sci_request version="1.0">
              <send_message>
                <targets>
                  <device id="%s"/>
                </targets>
                <sm_udp retries="3" retryTimeout="5">
                  %s
                </sm_udp>
              </send_message>
            </sci_request>""" % (self.device_id, command)

        my_sm_udp_request_no_reply = \
            """<sci_request version="1.0">
              <send_message reply="none">
                <targets>
                  <device id="%s"/>
                </targets>
                <sm_udp retries="3" retryTimeout="5">
                  %s
                </sm_udp>
              </send_message>
            </sci_request>""" % (self.device_id, command)

        if reply_needed == True:
            my_sm_udp_request = my_sm_udp_request_with_reply
        else:
            my_sm_udp_request = my_sm_udp_request_no_reply

        self.log.info("Sending SM UDP command \"%s\" to server for device id  %s." % (test_name, self.device_id))

        self.log.info("Sending SCI Request:\n%s\n" % my_sm_udp_request)

        # Send device request
        sm_udp_response = self.session.post('http://%s/ws/sci' % self.hostname, data=my_sm_udp_request).content

        # Parse request response 
        self.log.info("Got Response: %s" % sm_udp_response)
        dom = xml.dom.minidom.parseString(sm_udp_response)
        if reply_needed == True:
            sm_response = dom.getElementsByTagName("sm_udp")
        else:
            sm_response = dom.getElementsByTagName("send_message")

        if len(sm_response) == 0:
            self.fail("Unexpected response from device: %s" % sm_udp_response)

        if reply_needed == True and len(sm_response) != 0:
            data = b64decode(getText(dom.getElementsByTagName("user_msg")[0]))
            self.log.info("Response data: %s" % data)
            self.assertEqual(data, response,
                "returned response (%s) is not (%s)" % (data, response))

if __name__ == '__main__':
    unittest.main() 
