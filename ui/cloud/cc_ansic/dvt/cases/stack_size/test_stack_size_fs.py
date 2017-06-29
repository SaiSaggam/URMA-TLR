import xpath
import time
import ic_testcase
import datetime
import re
import os
import fileinput
import random
import base64
import tempfile
import shutil

from base64 import b64encode, b64decode, encodestring

import xml.dom.minidom
from xml.dom.minidom import getDOMImplementation
impl = getDOMImplementation()

from ..utils import getText
from ..utils import DeviceConnectionMonitor, device_is_connected

TERMINATE_TEST_FILE = "terminate.file"
my_test_file = "my_test_file.txt"

FILE_PUT_GET_REQUEST = \
"""<sci_request version="1.0">
  <file_system>
    <targets>
      <device id="%s"/>
    </targets>
    <commands>
    <put_file path="%s" offset = "0" truncate = "true">
      <data>%s</data>
    </put_file>
    <get_file path="%s" offset = "0" length = "4294967294"/>
    </commands>
  </file_system>
</sci_request>""" 

FILE_LS_REQUEST = \
"""<sci_request version="1.0">
  <file_system>
    <targets>
      <device id="%s"/>
    </targets>
    <commands>
    <ls path="%s" hash="any"/>
    </commands>
  </file_system>
</sci_request>"""

FILE_RM_REQUEST = \
"""<sci_request version="1.0">
  <file_system>
    <targets>
      <device id="%s"/>
    </targets>
    <commands>
    <rm path="%s"/>
    </commands>
  </file_system>
</sci_request>"""

RCI_QUERY_STATE = \
"""<sci_request version="1.0"> 
  <send_message cache="false"> 
    <targets> 
      <device id="%s"/> 
    </targets> 
    <rci_request version="1.1"> 
      <query_state/>
    </rci_request>
  </send_message>
</sci_request>"""

class StackSizeTestCase(ic_testcase.TestCase):
    monitor=None

    def setUp(self):
        ic_testcase.TestCase.setUp(self)
        # Optimization, reuse the DeviceConnectionMonitor to avoid creating
        # multiple sessions over and over.
        if StackSizeTestCase.monitor is None:
            StackSizeTestCase.monitor = DeviceConnectionMonitor(self.push_client, self.dev_id)
            StackSizeTestCase.monitor.start()
        self.monitor = StackSizeTestCase.monitor

    @classmethod
    def tearDownClass(cls):
        if StackSizeTestCase.monitor is not None:
            StackSizeTestCase.monitor.stop()
        ic_testcase.TestCase.tearDownClass()


    def test_stacksize_with_begin_rci(self):

        """ Sends query_setting. 
        """
        self.log.info("**** Stack Size FS Test:  start RCI query_state")
        # Get the current value

        rci_request = (RCI_QUERY_STATE % self.device_id);

        # Send RCI request
        rci_response = self.session.post('http://%s/ws/sci' % self.hostname, data=rci_request).content

        self.log.info('Response from query state: %s' %rci_response)

        # Parse request response 
        dom = xml.dom.minidom.parseString(rci_response)
        rci_error_response = dom.getElementsByTagName('error')

        if len(rci_error_response) != 0:
            self.log.info("Request: %s" % rci_request)
            self.log.info("Response: %s" % rci_response)
            self.fail("Got error response: '%s'" % rci_response)

    def test_stacksize_with_fs(self):

        """ Sends file get command. 
        """
        my_dir = tempfile.mkdtemp(suffix='_stacksizefs')
        my_file_path  = my_dir + '/' + my_test_file
        my_ls_path   = my_dir

        file_data = self.get_random_word(2048);
        put_data = base64.encodestring(file_data)[:-1]

        self.log.info("**** Stack Size DVT: File System")
        self.log.info("Sending file put and file get commands for \"%s\" to server for device id  %s." % (my_file_path, self.device_id))

        request = (FILE_PUT_GET_REQUEST % (self.device_id, my_file_path, put_data, my_file_path))
        # Send device request
        file_get_response = self.session.post('http://%s/ws/sci' % self.hostname, data=request).content
        #self.log.info("%s" % file_get_response)
        if file_get_response.find('error id="2107"') != -1:
            self.log.info("Service not available.")
            shutil.rmtree(my_dir)
            return

        # Parse request response 
        self.log.info(file_get_response)
        dom = xml.dom.minidom.parseString(file_get_response)
        get_data = dom.getElementsByTagName("get_file")

        if len(get_data) == 0:
            self.fail("Response didn't contain \"get_file\" element: %s" % file_get_response)

        data =  b64decode(getText(get_data[0].getElementsByTagName("data")[0]))

        # Print file data
        #self.log.info("Received:")
        #self.log.info("File Data: \"%s\"." % data)

        infile = open(my_file_path, "r")
        in_text = infile.read()
        infile.close()

        self.log.info("!!! data len: \"%d\", in len: \"%d\"" %  (len(data), len(in_text)))

        self.assertEqual(in_text, data,
            "get file error") 

        request = (FILE_LS_REQUEST % (self.device_id, my_ls_path))

        self.log.info("Sending file ls command for \"%s\" to server for device id  %s." % (my_ls_path, self.device_id))
        file_ls_response = self.session.post('http://%s/ws/sci' % self.hostname, data=request).content
        self.log.info("%s" % file_ls_response)

        request = (FILE_RM_REQUEST % (self.device_id, my_file_path))
        self.log.info("Sending file rm command for \"%s\" to server for device id  %s." % (my_file_path, self.device_id))
        file_rm_response = self.session.post('http://%s/ws/sci' % self.hostname, data=request).content

        shutil.rmtree(my_dir)


        if device_is_connected(self) == False:
            self.log.info("Waiting for device to connect before start testing")
            self.monitor.wait_for_connect(30)
        self.log.info("Device is connected. Start testing")

        request = (FILE_PUT_GET_REQUEST % (self.device_id, "terminate.test", put_data, my_file_path))
        # Send device request
        file_get_response = self.session.post('http://%s/ws/sci' % self.hostname, data=request).content

        self.log.info("Waiting for iDigi to disconnect device.")
        self.monitor.wait_for_disconnect(30)
        self.log.info("Device disconnected.")

        self.log.info("Waiting for Device to reconnect.")
        self.monitor.wait_for_connect(30)
        self.log.info("Device connected.")


    def get_random_word(self, wordLen):
        word = ''
        for i in range(wordLen):
            word += random.choice('ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789')
        return word
        
if __name__ == '__main__':
    unittest.main()
