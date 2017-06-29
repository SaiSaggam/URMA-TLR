import time
import cc_testcase
import datetime
import re
import os
from base64 import b64encode
from xml.dom.minidom import getDOMImplementation

class SendDataTestCase(cc_testcase.TestCase):

    def test_send_data(self):

        """ This routine gets the sent file from the cloud and
        verifies that the correct data is pushed """

        self.log.info("Begin Send data test.")

        # Create paths to files.
        file_name = 'test/test.txt'
        file_location = 'FileData/~/' + self.device_id + '/' + file_name
        self.log.info("Attempting to Retrieve data from File Location: %s" % file_location)
        match = re.match(".*\/(.*)$", file_name)
        if match:
            file_name = match.groups()[0]

        # Send device request & get plain text
        requestResponse = self.cloudHandler.getRequest(file_location)
        file_content = requestResponse.content

        self.log.info("file_content requestResponse: %s" % requestResponse)
        self.log.info("file_content: \n%s" % file_content)

        expected_content = 'Connector data service sample\n'

        self.log.info("expected_content: \n%s" % expected_content)

        self.assertEqual(expected_content, file_content,
                        "File's contents do not match what is expected")
        requestResponse = self.cloudHandler.deleteRequest(file_location)
        
if __name__ == '__main__':
    unittest.main()
