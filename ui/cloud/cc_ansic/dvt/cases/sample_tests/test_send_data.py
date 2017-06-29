import time
import ic_testcase
import datetime
import re
import os
from base64 import b64encode
from xml.dom.minidom import getDOMImplementation

from ..utils import clean_slate

impl = getDOMImplementation()

filedata = 'FileData/~/'
    
class SendDataTestCase(ic_testcase.TestCase):
    
    def test_send_data(self):
    
        """ This routine gets the sent file from the cloud and   
        verifies that the correct data is pushed """
        
        self.log.info("Begin Send data test.")
       
        # Create paths to files.
        file_name = 'test/test.txt'
        file_location = filedata + self.device_id + '/' + file_name
        self.log.info("Attempting to Retrieve data from File Location: %s" % file_location)
        match = re.match(".*\/(.*)$", file_name)
        if match:
            file_name = match.groups()[0]

        # Check for existing file and its contents.
        try:
            file_content = self.session.get('https://%s/ws/%s' % (self.hostname, file_location)).content
        except Exception, e:
            self.fail('Failed to Get Device Contents.  Reason: %s' % e)
        
        # Verify file's contents
        expected_content = 'Connector data service sample\n'
        if file_content != expected_content:
            # This is a small hack for backwards compability with old send_data sample.
            if not file_content.startswith('Connector data service sample'):
                self.fail("Received content: %s does not match expected: %s" % (file_content, expected_content))

    def tearDown(self):
        self.log.info("Performing cleanup.")
        file_name = 'test/test.txt'
        file_location = filedata + self.device_id + '/' + file_name
        self.rest_session.delete(file_location)

if __name__ == '__main__':
    unittest.main() 
