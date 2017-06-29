# -*- coding: utf-8 -*-
import time
import ic_testcase
from base64 import encodestring

import xml.dom.minidom
from xml.dom.minidom import getDOMImplementation
impl = getDOMImplementation()

from ..utils import getText, DeviceConnectionMonitor, update_firmware

target = 0

FIRMWARE_QUERY_REQUEST = \
"""<sci_request version="1.0">
<query_firmware_targets>
    <targets>
    <device id="%s"/>
    </targets>
</query_firmware_targets>
</sci_request>"""

expected_firmware_version = '1.0.0.0'
firmware_targets = [('Binary Image' , '.*\.[bB][iI][nN]', '1.0.0.0'),
                    ('Library Image', '.*\.a'           , '0.0.1.0')]
firmware_files   = ['dvt/cases/test_files/firmware.bin',
                    'dvt/cases/test_files/firmware.a']
class FirmwareTestCase(ic_testcase.TestCase):
    monitor=None

    def setUp(self):
        ic_testcase.TestCase.setUp(self)
        # Optimization, reuse the DeviceConnectionMonitor to avoid creating
        # multiple sessions over and over.
        if FirmwareTestCase.monitor is None:
            FirmwareTestCase.monitor = DeviceConnectionMonitor(self.push_client, self.dev_id)
            FirmwareTestCase.monitor.start()
        self.monitor = FirmwareTestCase.monitor

    @classmethod
    def tearDownClass(cls):
        if FirmwareTestCase.monitor is not None:
            FirmwareTestCase.monitor.stop()
        ic_testcase.TestCase.tearDownClass()


    def test_check_firmware_version_match(self):

        """ Verifies that firmware version returned from device matches
        firmware version in configuration.
        """

        self.log.info("Beginning Test to Confirm Firmware Version.")
        # Retrieve Firmware Version
        self.log.info("Retrieving Firmware Version from %s." % self.device_id)
        response = self.rest_session.get_first('DeviceCore',
                        params={'condition' : "devConnectwareId='%s'" % self.device_id})
        self.assertEqual(200, response.status_code)
        new_device_core = response.resource
        # Ensure firmware version is correct
        self.assertEqual(new_device_core.dpFirmwareLevelDesc,
                        expected_firmware_version,
                        "Firmware Version (%s) does not match expected (%s)" %
                        (new_device_core.dpFirmwareLevelDesc,
                        expected_firmware_version))

    def test_confirm_firmware_targets(self):

        """ Verifies that the firmware targets returned match the firmware
        targets in the configuration.
        """

        self.log.info("Beginning Test to Validate Firmware Targets.")
        self.log.info("Sending firmware target query to %s." % self.device_id)

        # Send firmware target query request
        firmware_targets_xml = self.session.post('http://%s/ws/sci' % self.hostname,
            data=FIRMWARE_QUERY_REQUEST % self.device_id).content

        # Parse request response for firmware targets
        dom = xml.dom.minidom.parseString(firmware_targets_xml)
        targets = dom.getElementsByTagName("target")

        self.log.info("Determining if the number of targets matches targets in configuration.")
        self.assertEqual(len(targets), len(firmware_targets),
            "number of returned firmware targets (%i) does not match number of targets given (%i)"
            % (len(targets), len(firmware_targets)))

        for i in range(len(targets)):
            target = int(targets[i].getAttribute("number"))
            parse_firmware_target = (getText(targets[i].getElementsByTagName("name")[0]),
                                    getText(targets[i].getElementsByTagName("pattern")[0]),
                                    getText(targets[i].getElementsByTagName("version")[0]))

            self.log.info("firmware target %i name: %s" % (target, parse_firmware_target[0]))
            self.log.info("firmware target %i pattern: %s" % (target, parse_firmware_target[1]))
            self.log.info("firmware target %i version: %s" % (target, parse_firmware_target[2]))
            self.log.info("Determining if firmware target %s matches given target" % target)
            self.assertEqual(parse_firmware_target, firmware_targets[target],
                            "Firmware Target %s does not match given target %s" %
                            (parse_firmware_target, firmware_targets[target]))


    def test_update_firmware_bytarget_multitargets(self):

        """ Updates firmware by target and verifies that the firmware
        is submitted and that the device disconnects and reconnects
        """

        self.log.info("Beginning Test to Update Firmware by Target.")

        self.log.info("Sending firmware target query to %s." % self.device_id)

        # Determine if device has firmware targets
        firmware_targets_xml = self.session.post('http://%s/ws/sci' % self.hostname,
            data=FIRMWARE_QUERY_REQUEST % self.device_id).content
        dom = xml.dom.minidom.parseString(firmware_targets_xml)
        targets = dom.getElementsByTagName("target")

        self.log.info("Determining if the device has defined firmware targets.")
        self.assertNotEqual(0, len(targets),
                "No targets exist on device %s" % self.device_id)

        # Send request to update firmware
        self.log.info("Sending request to update firmware.")
        response = update_firmware(self.session, self.hostname, self.device_id,
            firmware_files[target], "%d" % target)

        # Print response
        self.log.info("response: \n%s" % response)

        # Determine if Firmware was submitted
        submitted = response.find("submitted")
        self.assertNotEqual(-1, submitted, "Firmware was not submitted.")

        self.log.info("Waiting for iDigi to disconnect device.")
        self.monitor.wait_for_disconnect(30)
        self.log.info("Device disconnected.")

        self.log.info("Waiting for Device to reconnect.")
        self.monitor.wait_for_connect(30)
        self.log.info("Device connected.")


    def test_update_firmware_nonexisting_target(self):

        """ Sends a firmware upgrade to a non-existing target and verifies
        that an error message is returned.
        """

        self.log.info("Beginning Test to Update Non-Existing Firmware Target.")
        # Check time the device was last connected
        last_connected = self.device_core.dpLastConnectTime

        # Send firmware target query
        self.log.info("Sending firmware target query to %s." % self.device_id)

        firmware_targets_xml = self.session.post('http://%s/ws/sci' % self.hostname,
            data=FIRMWARE_QUERY_REQUEST % self.device_id).content

        # Determine number of firmware targets
        self.log.info("Determining number of firmware targets.")
        dom = xml.dom.minidom.parseString(firmware_targets_xml)
        targets = dom.getElementsByTagName("target")

        # Choose non-existing target for upgrade
        upgrade_target = len(targets)

        #Send upgrade request
        self.log.info("Sending request to update firmware on target %d." % upgrade_target)
        response = update_firmware(self.session, self.hostname, self.device_id,
            firmware_files[target], "%d" % upgrade_target)

        self.log.info("response: \n%s" % response)

        # Verify that firmware was not updated
        # should not find "submitted" in response
        self.log.info("Verify that the firmware was not updated.")
        submitted = response.find("submitted")
        self.assertEqual(-1, submitted,
                "Firmware was successfully submitted to non-existing target.")

        # error message should include "Invalid Target"
        invalid_response = response.find("Invalid Target")
        self.assertNotEqual(-1, invalid_response,
                "Device returned incorrect response to invalid firmware update.")

if __name__ == '__main__':

    unittest.main()
