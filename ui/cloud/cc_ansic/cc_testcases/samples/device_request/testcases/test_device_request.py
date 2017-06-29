import time
import cc_testcase
import datetime
import re
import os
from base64 import b64encode

import xml.dom.minidom
from xml.dom.minidom import getDOMImplementation

class SendDeviceRequestTestCase(cc_testcase.TestCase):

    def sendDeviceRequest(self, device_id, target_name, payload):
        my_target_device_request = \
            """            <sci_request version="1.0">
              <data_service>
                <targets>
                  <device id="%s"/>
                </targets>
                <requests>
                <device_request target_name="%s">%s</device_request>
                </requests>
              </data_service>
            </sci_request>""" % (device_id, target_name, payload)
        self.log.info("Sending SCI Request:\n%s\n" % my_target_device_request)
        response = self.cloudHandler.postRequest("sci", data = my_target_device_request)
        # Check query was successfully
        if (response.status_code != 200):
            self.fail("Unexpected response from the Cloud")
        # Show response in xml format
        self.log.info("Got Response:\n%s" % xml.dom.minidom.parseString(response.content).toprettyxml(indent="  ") )
        # Get element 'device_request'
        element = response.resource["sci_reply"]["data_service"]["device"]["requests"]["device_request"]

        # Return xml response in dict format
        return element
      
    def test_device_request(self):
    
        """ Verifies that the device request returned match the expected 
        response.
        """
        my_target_name = "myTarget"
        my_payload = "My device request data"
        my_target_response = "My device response data"

        self.log.info("Beginning Test for device request.")
        self.log.info("Sending device request for \"%s\" target_name to server for device id  %s." % (my_target_name, self.device_id))
        
        element = self.sendDeviceRequest(self.device_id, my_target_name, my_payload)
        
        # Validate target name
        self.log.info("Determining if the target_name is \"%s\"." % my_target_name)
        if (not element.has_key("@target_name")):
            self.fail("Response does not have key 'target_name'")
        target_name = element["@target_name"]
        self.assertEqual(target_name, my_target_name, 
            "returned target (%s) is not (%s)" 
            % (target_name, my_target_name))

        self.log.info("Determining if status is success.")
        status = element["@status"]
        self.assertEqual(status, '0', "returned status (%s) is not success status" % status)
        
        self.log.info("Determining if response data is expected \"%s\"." % my_target_response)
        
        data = element["#text"]
        self.assertEqual(data, my_target_response,
            "returned target (%s) is not (%s)" 
            % (data, my_target_response))

    def test_invalid_target_device_request(self):
    
        """ Verifies that the device request returned match the expected 
        response.
        """
        my_target_name = "invalidTarget"
        my_payload = "My device request data"


        self.log.info("Beginning Test for device request.")
        element = self.sendDeviceRequest(self.device_id, my_target_name, my_payload)
        
        self.log.info("Determining if \"%s\" target is cancelled." % my_target_name)
        status = element["@status"]
        response = element["#text"]
        
        cancelled_response = "Target is not supported"

        self.assertEqual(response, cancelled_response,
            "returned target (%s) is not (%s)" 
            % (response, cancelled_response))
        
        self.assertEqual(status, "1",
            "status (%s) is not (%s)" 
            % (status, "1"))

if __name__ == '__main__':
    unittest.main() 
