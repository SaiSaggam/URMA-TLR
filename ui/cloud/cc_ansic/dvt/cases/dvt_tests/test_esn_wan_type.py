import time
import ic_testcase
import datetime
import re
import os
from base64 import b64encode

import xml.dom.minidom
from xml.dom.minidom import getDOMImplementation
impl = getDOMImplementation()

# from ..utils import clean_slate
from ..utils import getText, clean_slate

esn_wan_type = "1"

impl = getDOMImplementation()
class ESNWanTypeTestCase(ic_testcase.TestCase):  

    def test_esn_target(self):
        """ Verifies that the [wan_type] device request for ESN wan type. """
        self.send_device_request("wan_type", esn_wan_type)

    def send_device_request (self, target, expected_wan_type):

        my_target_device_request = \
            """<sci_request version="1.0">
              <data_service>
                <targets>
                  <device id="%s"/>
                </targets>
                <requests>
                <device_request target_name="%s"></device_request>
                </requests>
              </data_service>
            </sci_request>""" % (self.device_id, target)

        self.log.info("Beginning device request. target[%s]" %target)
        self.log.info("Sending device request for \"%s\" target_name to server for device id  %s." % (target, self.device_id))

        # Send device request
        device_request_response = self.session.post('http://%s/ws/sci' % self.hostname, 
                            data=my_target_device_request).content

        # Parse request response 
        self.log.info("Got Response: %s" % device_request_response)
        dom = xml.dom.minidom.parseString(device_request_response)
        device_response = dom.getElementsByTagName("device_request")

        if len(device_response) == 0:
            self.fail("Unexpected response from device: %s" % device_request_response)

        # Validate target name
        self.log.info("Determining if the target_name is \"%s\"." %target)
        target_name = device_response[0].getAttribute('target_name')
        self.assertEqual(target_name, target, 
                         "returned target (%s) is not (%s)" 
                         % (target_name, target))

        # Validate status
        self.log.info("Determining if status is success.")
        status = device_response[0].getAttribute("status")
        if target == "not handle":
            self.assertEqual(status, '1', "returned error status(%s)" %status)
        else:
            self.assertEqual(status, '0', "returned error status(%s)" %status)

            # Validate response data
            self.log.info("Determining if response data is expected \"%s\"." %target)

            wan_type = getText(device_response[0].getElementsByTagName('type')[0])
            wan_id = getText(device_response[0].getElementsByTagName('id')[0])

            if len(wan_type) > 0:
                self.log.info("WAN type %s WAN ID %s" % (wan_type, wan_id))

                this_id = self.device_id
                wan_id = wan_id.replace("-", "")
                this_id = this_id.replace("-", "")[24:]

                self.log.info("expected WAN id %s WAN ID %s" % (wan_id, this_id))

                self.assertEqual(wan_type, expected_wan_type, "returned unexpected wan type (%s) expected type %s" % (wan_type, expected_wan_type))
                self.assertEqual(this_id, wan_id, "returned unexpected wan id (%s) expected type %s" % (this_id, wan_id))

            else:
                self.fail("No WAN Type is respons: %s" %device_response)

if __name__ == '__main__': 
    unittest.main() 
