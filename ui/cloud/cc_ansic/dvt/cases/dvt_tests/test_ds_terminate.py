import time
import ic_testcase

from base64 import encodestring

import xml.dom.minidom
from xml.dom.minidom import getDOMImplementation
impl = getDOMImplementation()

from ..utils import clean_slate, getText, update_firmware, get_and_verify

filedata = 'FileData/~/'

TERMINATE_TEST_FILE = "terminate_test.txt"

FIRMWARE_QUERY_REQUEST = \
"""<sci_request version="1.0">
  <query_firmware_targets>
    <targets>
      <device id="%s"/>
    </targets>
  </query_firmware_targets>
</sci_request>"""
    
FIRMWARE_DATA_REQUEST= \
"""<sci_request version="1.1">
    <update_firmware synchronous="false" firmware_target="%d">
        <targets>
            <device id="%s"/>
        </targets>
        <data>%s</data>
    </update_firmware>
</sci_request>"""

target = 0
firmware_file = 'dvt/cases/test_files/pattern.txt'

def send_device_request(self, target_name, target_content):

    """ Verifies that the device request returned match the expected 
    response.
    """
    my_target_device_request = \
        """<sci_request version="1.0">
          <data_service>
            <targets>
              <device id="%s"/>
            </targets>
            <requests>
            <device_request target_name="%s">%s</device_request>
            </requests>
          </data_service>
        </sci_request>""" % (self.device_id, target_name, target_content)

    self.log.info("Sending \"%s\" device request" % target_name)
    
    # Send device request
    device_request_response = self.session.post('http://%s/ws/sci' % self.hostname, 
        data=my_target_device_request).content
        
    self.log.info("Response: %s" % device_request_response)

    # Parse request response 
    dom = xml.dom.minidom.parseString(device_request_response)
    device_response = dom.getElementsByTagName("device_request")

    # Validate target name
    my_target_name = device_response[0].getAttribute('target_name')
#    if my_target_name != target_name:
#        self.log.info("Response: %s" % device_request_response)

    self.assertEqual(my_target_name, target_name, 
                    "returned target (%s) is not (%s)" 
                    % (my_target_name, target_name))

    # Validate status 
    self.log.info("Determining if status is success (0).")
    status = device_response[0].getAttribute("status")
    self.log.info("status = \"%s\" device request" % status)
    if status == '0':
        return 0
    else:
        self.log.info("Error \"%s\" device request" % target_name)
        return -1

def delete_file(self, file_name):

    # Create paths to delete the file.
    file_location = filedata + self.device_id + '/' + file_name

    self.log.info("delete file %s" % file_location)
    # If the file exists, delete it.
    clean_slate(self.rest_session, file_location)


class TerminateTestCase(ic_testcase.TestCase):
       
    def test_terminate_in_application(self):
    
        """ Sends device request to application to call connector_initiate_terminate
        """
        
        self.log.info("***** Terminate in application thread *****")

        # delete the file that sample is going to send for
        # the reboot result.
        delete_file(self, TERMINATE_TEST_FILE)

        def send_data():
            # Send device request to request terminate the sample
            status = send_device_request(self, 'request_terminate_in_application', '')
            self.assertTrue(status == 0, "Unable to send device request")
                  
        # Wait for terminate result file.
        # Create content that are expected from the reboot result file.
        expected_content = "terminate_ok"
        datetime_created = []

        # Create path to file for push.
        file_push_location = filedata + self.device_id + '/' + TERMINATE_TEST_FILE
    
        # get and verify correct content is pushed.
        get_and_verify(self, self.push_client, self.device_id, 
                       file_push_location, expected_content,
                       trigger_function=send_data)
               
    def test_terminate_in_put_request(self):
    
        """ Sends device request to application to call connector_initiate_terminate
        """
        
        self.log.info("***** Terminate in callback *****")

        # delete the file that sample is going to send for
        # the reboot result.
        delete_file(self, TERMINATE_TEST_FILE)

        def send_data():
            # Send device request to request terminate the sample
            status = send_device_request(self, 'request_terminate_in_callback', '')
            self.assertTrue(status == 0, "Unable to send device request")
                   
        # Wait for terminate result file.
        # Create content that are expected from the reboot result file.
        expected_content = "terminate_ok"
        datetime_created = []

        # Create path to file for push.
        file_push_location = filedata + self.device_id + '/' + TERMINATE_TEST_FILE
    
        # get and verify correct content is pushed.
        get_and_verify(self, self.push_client, self.device_id, 
                       file_push_location, expected_content,
                       trigger_function=send_data)
        

    def test_terminate_in_firmware(self):
    
        """ Sends a firmware update to teminate iik """
    
        self.log.info("***** Updating Firmware to terminate iDigi connector *****")

        # delete the file that sample is going to send for
        # the reboot result.
        delete_file(self, TERMINATE_TEST_FILE)

        # Send firmware target query
        self.log.info("Sending firmware target query to %s." % self.device_id)
               
        # Find firmware targets
        firmware_targets_xml = self.session.post('http://%s/ws/sci' % self.hostname, 
            data=FIRMWARE_QUERY_REQUEST % self.device_id).content
    
        dom = xml.dom.minidom.parseString(firmware_targets_xml)
        targets = dom.getElementsByTagName("target")
    
        self.log.info("Determining if the device has defined firmware targets.")
        self.assertNotEqual(0, len(targets), 
                "No targets exist on device %s" % self.device_id)

        # Encode firmware for transmittal
        f = open(firmware_file, 'rb')
        data = f.read()
        data_value = encodestring(data)
        f.close()

        def update_firmware():
            # Send update asynchronous update request
            request = (FIRMWARE_DATA_REQUEST % (target, self.device_id, data_value))

            self.log.info("Sending request to update firmware.")
            response = self.session.post('http://%s/ws/sci' % self.hostname, 
                            data=request).content
            self.log.info("Response to:\n%s" % response)
   
        # Wait for terminate result file.
        # Create content that are expected from the reboot result file.
        expected_content = "terminate_ok"
        datetime_created = []

        # Create path to file for push.
        file_push_location = filedata + self.device_id + '/' + TERMINATE_TEST_FILE
    
        # get and verify correct content is pushed.
        get_and_verify(self, self.push_client, self.device_id, 
                       file_push_location, expected_content,
                       trigger_function=update_firmware)

 
if __name__ == '__main__':
    unittest.main()
