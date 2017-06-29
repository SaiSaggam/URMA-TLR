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

from base64 import b64encode, b64decode, encodestring

import xml.dom.minidom
from xml.dom.minidom import getDOMImplementation
impl = getDOMImplementation()

from ..utils import getText
from ..utils import DeviceConnectionMonitor, device_is_connected

my_test_file = "my_test_file.txt"
fw_target_num = 0

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
    <update_firmware firmware_target="%d">
        <targets>
            <device id="%s"/>
        </targets>
        <data>%s</data>
    </update_firmware>
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


    def test_stacksize_with_fw(self):
        """ Sends a firmware update """

        self.log.info("**** Stack Size DVT: Firmware Service")

        # Send firmware target query
        self.log.info("Sending firmware target query to %s." % self.device_id)

        # Find firmware targets
        firmware_targets_xml = self.session.post('http://%s/ws/sci' % self.hostname, 
            data=FIRMWARE_QUERY_REQUEST % self.device_id).content
        #self.log.info("TARGETS %s." % firmware_targets_xml)

        dom = xml.dom.minidom.parseString(firmware_targets_xml)
        targets = dom.getElementsByTagName("target")

        if len(targets) == 0:
            self.log.info("Service not available")
            return

        # Encode firmware for transmittal
        firmware_file = 'dvt/cases/test_files/firmware_malloc.txt'
        self.log.info("Target %d %s." % (fw_target_num, firmware_file))
 
        f = open(firmware_file, 'rb')
        data = f.read()
        data_value = encodestring(data)
        f.close()


        if device_is_connected(self) == False:
            self.log.info("Waiting for device to connect before start testing")
            self.monitor.wait_for_connect(30)
        self.log.info("Device is connected. Start testing")

        # Send update request
        request = (FIRMWARE_DATA_REQUEST % (fw_target_num, self.device_id, data_value))

        self.log.info("Sending request to update firmware.")
        response = self.session.post('http://%s/ws/sci' % self.hostname, data=request).content
        self.log.info("Response to:\n%s" % response)

        self.log.info("Waiting for iDigi to disconnect device.")
        self.monitor.wait_for_disconnect(30)
        self.log.info("Device disconnected.")

        self.log.info("Waiting for Device to reconnect.")
        self.monitor.wait_for_connect(30)
        self.log.info("Device connected.")


if __name__ == '__main__':
    unittest.main()
