import time
import ic_testcase
import logging
import datetime
import json
from base64 import b64encode, b64decode
import xml.dom.minidom
from xml.dom.minidom import getDOMImplementation
impl = getDOMImplementation()
from threading import Event
from ..utils import clean_slate, update_firmware, getText, get_and_verify

firmware_file = 'dvt/cases/test_files/pattern.txt'

class KeepAliveTestCase(ic_testcase.TestCase):
    """ Downloads the firmware and make sure device sends fw and rx keepalive periodically """

    def test_firmware_and_rx_keepalive(self):
        # send request to update firmware
        target = 0
        error_message = "Download Checksum"
        self.log.info("Sending request to update firmware.")
        response = update_firmware(self.session, self.hostname, self.device_id, 
                        firmware_file, "%d" % target)
            
        # print response
        self.log.info("response: \n%s" % response)
            
        self.log.info("Determining if correct error response was returned.")
        
        # Parse response to verify an error was returned.
        dom = xml.dom.minidom.parseString(response)
        error_exists = dom.getElementsByTagName("error")
        self.assertTrue(error_exists, "Response is not an error message.")
        
        # Verify that correct error message was returned
        correct_error = response.find(error_message)
        self.assertNotEqual(-1, correct_error,
                "The expected error message (%s) was not returned."
                % error_message)

    def test_tx_keep_alive(self):        
        # Create paths to delete the file.
        file_location = 'FileData/~/%s/keepalive.txt' % self.device_id

        self.log.info("delete file %s" % file_location)
        clean_slate(self.rest_session, file_location)

        # Create content that are expected from the reboot result file.
        expected_content = "keepalive DVT success\n"

        # get and verify correct content is pushed.
        get_and_verify(self, self.push_client, self.device_id, file_location, expected_content, wait_time=300)

if __name__ == '__main__':
    unittest.main()
