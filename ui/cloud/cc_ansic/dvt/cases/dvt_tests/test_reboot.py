# -*- coding: iso-8859-1 -*-
import time
import ic_testcase

from ..utils import DeviceConnectionMonitor, device_is_connected

DISCONNECT_REQUEST = \
"""<sci_request version="1.0">
    <disconnect>
        <targets>
            <device id="%s"/>
        </targets>
    </disconnect>
</sci_request>"""

REBOOT_REQUEST = \
"""<sci_request version="1.0">
  <reboot>
    <targets>
      <device id="%s"/>
    </targets>
  </reboot>
</sci_request>"""

class ConnectionTestCase(ic_testcase.TestCase):
    monitor=None

    def setUp(self):
        ic_testcase.TestCase.setUp(self)
        # Optimization, reuse the DeviceConnectionMonitor to avoid creating
        # multiple sessions over and over.
        if ConnectionTestCase.monitor is None:
            ConnectionTestCase.monitor = DeviceConnectionMonitor(self.push_client, self.dev_id)
            ConnectionTestCase.monitor.start()
        self.monitor = ConnectionTestCase.monitor

    @classmethod
    def tearDownClass(cls):
        if ConnectionTestCase.monitor is not None:
            ConnectionTestCase.monitor.stop()
        ic_testcase.TestCase.tearDownClass()

    def test_reboot(self):

        """ Sends reboot request to given device and verifies that
        the device disconnects and reconnects to an iDigi server.
        """

        self.log.info("***** Beginning Reboot Test *****")

        if device_is_connected(self) == False:
            self.log.info("Waiting for device connected before starting the test")
            self.monitor.wait_for_connect(30)

        self.log.info("Device is connected. Start testing.")
        self.log.info("Sending Reboot to %s." % self.device_id)
        # Create reboot request
        reboot_request = REBOOT_REQUEST % (self.device_id)

        # Send SCI reboot request
        response = self.session.post('http://%s/ws/sci' % self.hostname,
                        data=reboot_request).content
        self.log.info("response to reboot request = %s" % response)

        self.log.info("Waiting for iDigi to disconnect device.")
        self.monitor.wait_for_disconnect(30)
        self.log.info("Device disconnected.")

        self.log.info("Waiting for Device to reconnect.")
        self.monitor.wait_for_connect(30)
        self.log.info("Device connected.")