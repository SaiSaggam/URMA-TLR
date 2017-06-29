import time
import ic_testcase
#import datetime
#import re
#import os
import xml.dom.minidom
from base64 import b64encode
from xml.dom.minidom import getDOMImplementation

from ..utils import clean_slate

impl = getDOMImplementation()

filedata = 'FileData/~/'
file_name = 'test/sm_data.txt'
file_name1 = 'test/sm_data_noresp.txt'

class SmUdpSendDataTestCase(ic_testcase.UDPTestCase):

    def test_sm_udp_send_data(self):
    
        """ This routine gets the sent file from the cloud and   
        verifies that the correct data is pushed """
        
        self.log.info("Begin sm_send_data test.")
       
        # Create paths to files.
        expected_content = 'iDigi sm_send_data sample data\n'
        file_location = filedata + self.device_id + '/' + file_name
        file_location1 = filedata + self.device_id + '/' + file_name1
        self.log.info("Attempting to Retrieve data from File Locations: %s, %s" % (file_location, file_location1))

        # Check for existing file and its contents.
        try:
            file_content = self.session.get('https://%s/ws/%s' % (self.hostname, file_location)).content
        except Exception, e:
            self.fail('Failed to Get Device Contents.  Reason: %s' % e)
        
        # Verify file's contents
        if file_content != expected_content:
           self.fail("Received content: %s does not match expected: %s" % (file_content, expected_content))

        self.log.info("File %s: %s" % (file_name, file_content))
        # Check for existing file and its contents.
        try:
            file_content = self.session.get('https://%s/ws/%s' % (self.hostname, file_location1)).content
        except Exception, e:
            self.fail('Failed to Get Device Contents.  Reason: %s' % e)

        self.log.info("File %s: %s" % (file_name1, file_content))
        
        # Verify file's contents
        if file_content != expected_content:
           self.fail("Received content: %s does not match expected: %s" % (file_content, expected_content))

    def tearDown(self):
        self.log.info("Performing cleanup.")
        file_name = 'test/sm_data.txt'
        file_location = filedata + self.device_id + '/' + file_name
        file_location1 = filedata + self.device_id + '/' + file_name1
        self.rest_session.delete(file_location)
        self.rest_session.delete(file_location1)

if __name__ == '__main__':
    unittest.main() 
