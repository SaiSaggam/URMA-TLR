import time
import ic_testcase
import datetime
import re
import os
from base64 import b64decode

import xml.dom.minidom
from xml.dom.minidom import getDOMImplementation
impl = getDOMImplementation()

# from ..utils import clean_slate
from ..utils import getText, clean_slate

impl = getDOMImplementation()

test_file_name = './dvt/cases/test_files/random.txt'
#test_file_name = '../test_files/random.txt'

impl = getDOMImplementation()

def verify_the_file_on_server (instance, file_path, file_expected):

   instance.log.info("Determining the file and content of %s." %file_path)
   if file_expected:
      f = open(test_file_name, 'rb')
      expected_content = f.read()
      f.close()

      file_content = instance.session.get('http://%s/ws/%s' % (instance.hostname, file_path)).content
      instance.assertEqual(expected_content, file_content,
                       "File's contents do not match what is expected")
      instance.rest_session.delete(file_path)
   else:
      # Check that file was not created
      instance.assertEquals(404, instance.rest_session.get(file_path).status_code)

class SmUdpMultipartDvtTestCase(ic_testcase.UDPTestCase):

    def test_sm_udp_multipart_request(self):
        """ Verifies that the multipart user request with path is processed correctly. """
        f = open(test_file_name, 'rb')
        file_content = f.read()
        f.close()
        payload = '<user_msg path="myTarget">' + file_content + '</user_msg>'
        self.send_sm_udp_request("user_msg", payload, os.path.getsize(test_file_name))

    def test_sm_udp_multipart_cli(self):
        """ Verifies that the multipart CLI to the device processed correctly. """
        self.send_sm_udp_request("command", '<command> get file </command>', os.path.getsize(test_file_name))

    def test_sm_udp_multipart_cli_limit_1p(self):
        """ Verifies that the multipart CLI, with max 1 packet response, to the device processed correctly. """
        self.send_sm_udp_request("command", '<command maxResponseSize="1"> get 1 packet </command>', 1450)

    def test_sm_udp_multipart_cli_limit_2p(self):
        """ Verifies that the multipart CLI, with max 2 packet response, to the device processed correctly. """
        self.send_sm_udp_request("command", '<command maxResponseSize="2"> get 2 packet </command>', 2898)

    def test_sm_udp_multipart_verify_send_data(self):
        time.sleep(10) #Wait a little
        file_path = 'FileData/~/' + self.device_id + '/test/sm_udp_mp0.txt'
        verify_the_file_on_server(self, file_path, True)

    def send_sm_udp_request (self, test_name, command, size_to_read):

        my_sm_udp_request_with_reply = \
            """<sci_request version="1.0">
              <send_message>
                <targets>
                  <device id="%s"/>
                </targets>
                <sm_udp retries="5" retryTimeout="10">
                  %s
                </sm_udp>
              </send_message>
            </sci_request>""" % (self.device_id, command)

        my_sm_udp_request = my_sm_udp_request_with_reply

        self.log.info("Sending SM UDP command \"%s\" to server for device id  %s." % (test_name, self.device_id))

        self.log.info("Sending SCI Request:\n%s\n" % my_sm_udp_request)

        # Send device request
        sm_udp_response = self.session.post('http://%s/ws/sci' % self.hostname, data=my_sm_udp_request).content

        self.log.info("Received SCI Response:\n%s\n" % sm_udp_response)

        # Parse request response 
        dom = xml.dom.minidom.parseString(sm_udp_response)
        sm_response = dom.getElementsByTagName("sm_udp")
        if len(sm_response) == 0:
            self.fail("Unexpected response from device: %s" % sm_udp_response)

        sm_payload = getText(sm_response[0].getElementsByTagName(test_name)[0])
        received_data = b64decode(sm_payload)
        f = open(test_file_name, 'rb')
        expected_data = f.read(size_to_read)
        f.close()

        self.assertEqual(expected_data, received_data, "Received data does not match what is expected:\n Expected Data:%s\nReceived Data:%s\n" % (repr(expected_data),repr(received_data)))

if __name__ == '__main__':
    unittest.main() 
